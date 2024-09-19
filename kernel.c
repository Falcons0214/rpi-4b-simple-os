#include "io.h"
#include "utils.h"
#include "sys_utils.h"
#include "vtable.h"

static int intids[256];

void gic_init() {
    mn_uart_write_txt("GIC_INIT Start\n");

    unsigned int gicd_typer, ver, ITLnumber, val;

    // Check GIC Version
    ver = mmio_read32(GICC_IIDR);
    mn_uart_write_txt("GIC Version: ");
    mn_uart_write_hex((ver & ARCH_VER_MASK) >> 16);
    mn_uart_write_txt("\n");

    // Check GIC mm base
    unsigned long gic_base = _get_cbar_el1();
    mn_uart_write_txt("GIC mm base: ");
    mn_uart_write_hex(gic_base);
    mn_uart_write_txt("\n");

    // Get ITLnumber 
    gicd_typer = mmio_read32(GICD_TYPER);
    ITLnumber = (gicd_typer & GICD_TYPER_ITLN_MASK) + 1;
    mn_uart_write_txt("GIC ITLNumber: ");
    mn_uart_write_hex(ITLnumber);
    mn_uart_write_txt("\nGIC Interrupt Line: ");
    mn_uart_write_dec(GET_INTR_NUM(ITLnumber));
    mn_uart_write_txt("\n");

    /*
     * Clear GICD_ISENABLE & GICD_ISPEND
     */
    for (int i = 0; i < ITLnumber; i ++) {
        mmio_write32((GICD_ISENABLER0 + i * 4), 0x00000000);
        mmio_write32((GICD_ICPENDR0 + i * 4), 0xffffffff);
    }

    // Disable interrupt forwarding from Distribution to CPU.
    mmio_write32(GICD_CTLR, 0x00000000);
    
    // int intids[8];
    unsigned int index = 0;
    intids[index ++] = 30;  // PNS Timer
    intids[index ++] = 125; // AUX IRQ SPI
    // intids[index ++] = 58;
    // intids[index ++] = 111;
    
    /*
     * Enable INTIDs by GICD_ISENABLERn and check is it enable.
     * Select all interrupt to group 0 (IRQ or FIQ).
     */
    unsigned long base, mask;
    for (int i = 0; i < index; i ++) {
        mask = 1;
        base = GET_ISENABLE_N_BASE(intids[i]);
        mask <<= GET_ISENABLE_OFFSET(intids[i]);
        mmio_set32(base, mask);

        base = mmio_read32(GET_ISENABLE_N_BASE(intids[i]));
        if ((base & mask)) {
            mn_uart_write_txt("INTID: ");
            mn_uart_write_dec(intids[i]);
            mn_uart_write_txt(" enable\n");
        }

        /*
         * We can ignore this part, because all interrupt after reset will
         * belong to group 0, and if we need modify group we should move
         * to EL3 access, it will ignore all access except you in Secure EL.
         */
        // mask = 1;
        // base = GET_IGROUPR_N_BASE(intids[i]);
        // mask <<= GET_IGROUPR_OFFSET(intids[i]);
        // mmio_set32(base, mask);
    }

    /*
     * Setting target processor for interrupts.
     * 0 ~ 7 are read-only for SGIs & PPIs.
     * BCM2711 AUX ID 96(VC base) + 29(AUX offset)
     * Set AUX ID to CPU 0.
     */
    unsigned long gicd_itar;
    for (int i = 0; i < index; i ++) {
        if (intids[i] < 31)
            continue;
        gicd_itar = GET_ITARGETSR_N_BASE(intids[i]);
        val = (0x0000001 << (GET_ITARGETSR_OFFSET(intids[i]) * 8));
        mmio_set32(gicd_itar, val);

        mn_uart_write_hex(gicd_itar);
        mn_uart_write_txt("\n");    
        mn_uart_write_hex(mmio_read32(gicd_itar));
        mn_uart_write_txt("\n");    
    }

    /*
     * Interrupts priority setting.
     * Fisrt, we need check how many bits are support by processor.
     * Because the byte is RAZ/WI, so we can write 0xff to it and read
     * then check the value is same as we write or less than.
     */
    unsigned int pri_bits, prev_pri;
    prev_pri = mmio_read32(GICD_IPRIORITY0);
    mmio_write32(GICD_IPRIORITY0, 0x000000ff);
    pri_bits = (0x000000ff & mmio_read32(GICD_IPRIORITY0));
    mmio_write32(GICD_IPRIORITY0, prev_pri);

    mn_uart_write_txt("GIC Priority Bits: ");
    mn_uart_write_hex(pri_bits);
    mn_uart_write_txt("\n");

    unsigned long gicd_pri;
    for (int i = 0; i < index; i ++) {
        gicd_pri = GET_IPRIORITY_N_BASE(intids[i]);
        val = (0xe0 << (GET_IPRIORITY_OFFSET(intids[i]) * 8));
        mmio_set32(gicd_pri, val);

        val = mmio_read32(GET_IPRIORITY_N_BASE(intids[i]));
        val >>= (GET_IPRIORITY_OFFSET(intids[i]) * 8);
        val >>= 4;
        mn_uart_write_dec(intids[i]);
        mn_uart_write_txt(" priority is ");
        mn_uart_write_dec(val);
        mn_uart_write_txt("\n");
    }

    /*
     * Trigger mode setting.
     */
    for (int i = 0; i < index; i ++) {
        mmio_set32(GET_ICFG_N_BASE(intids[i]), (0x2) << (GET_ICFG_OFFSET(intids[i])));
    }

    /*
     * CPU Interface Setting.
     * 0b-0010-0110-0001
     * Driven by GIC on group 0, and disable group 1.
     * Separates the priority drop and interrupt deactivation operations
     */
    mmio_set32(GICC_CTLR, 0x00000261);

    /*
     * Binary Point.
     */
    mmio_set32(GICC_BPR, 0x00000003);

    /*
     * Set the threshold of priority.
     */
    mmio_set32(GICC_PMR, 0x000000f0);

    /*
     * Enable group 0 interrupt forwarding from Distribution to CPU.
     */
    mmio_set32(GICD_CTLR, 0x00000001);

    mn_uart_write_txt("GIC_INIT End\n");
    return; 
}

/*
 * Core Timer:
 *
 * 1. CNTFRQ_EL0: For frequency (50MHz)
 * 2. CNTP_CTL_EL0
 * 3. CNTP_CVAL_EL0
 * 4. CNTPCT_EL0
 */
void core_timer_set_freq() {
    /*
     * RPI_b default timer freq is 54MHZ.
     */
    // _set_cntfrq_el0(50000000);
}
void core_timer_init() {
    _core_timer_init();
}
void core_timer_set_expir_time(unsigned long sec) {
    _core_timer_set_exp(sec * _get_cntfrq_el0());
}

void task1(void) {
    mn_uart_write_txt("USER: Start !\n");

    unsigned int x = 0xabcdabcd;
    unsigned int size = sizeof(x);
    char *ptr = (char*)&x;
    
    for (int i = 0; i < size; i ++) {
        if (i == 2) {
            asm (
                "svc 0xcfcf" : :
            );
        }
        mn_uart_write_hex(*(ptr + i));
        mn_uart_write_txt("\n");
    }

    mn_uart_write_txt("USER: Exit\n");
    while (1);
    return;
}

// void task2() {
//     while (1) {
//         mn_uart_write_txt("Task 2\n");
//     }
// }

// void task3() {
//     while (1) {
//         mn_uart_write_txt("Task 3\n");
//     }
// }

void main() {
    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !\n");

    unsigned long vector_base = vector_table_init();
    mn_uart_write_hex(vector_base);
    mn_uart_write_txt("\n");

    gic_init();
    // core_timer_init();
    
    isit_uart_intr_en();
    
    show_curEL();

    mn_uart_write_txt("Switch From EL2 to El1 !\n");
    switch_el2_to_el1(0x10000000);
    /********** Below in EL1 **********/

    show_curEL();
    // int (*tasks[4])(void);
    // tasks[0] = task2;
    // tasks[1] = task3;
    // tasks[2] = NULL;
    // mmio_set32(GET_ISACTIVE_N_BASE(125), (0x1 <<  GET_ISACTIVE_OFFSET(125)));

    mn_uart_write_txt("\nSwitch From EL1 to El0 !\n");
    
    // core_timer_set_expir_time((unsigned long)1);

    switch_el1_to_el0(0x11000000);
    /********** Below in EL0 **********/

    // task1();
    for (int i = 0, ret; i < 10; i ++) {
        ret = uart_async_write_txt("Async eifhoei---===fa oefjaoif j oiafjoeiafo !\n");
    }

    while(1);

    return;
}