#include "io.h"
#include "utils.h"
#include "sys_utils.h"
#include "vtable.h"

void gic_init() {
    mn_uart_write_txt("GIC_INIT Start\n");

    unsigned int gicd_typer, ver, gicd_ctl, ITLnumber, \
                 gicc_ctl, val;

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

    // Disable interrupt forwarding from Distribution to CPU.
    gicd_ctl = mmio_read32(GICD_CTLR);
    gicd_ctl = 0x00000000;
    mmio_write32(GICD_CTLR, gicd_ctl);

    /*
     * Enable INTIDs by GICD_ISENABLERn.
     * Select all interrupt to group 0 (IRQ or FIQ).
     */
    for (int i = 0; i < ITLnumber; i ++) {
        mmio_write32(GICD_ISENABLER0 + 4 * i, 0xffffffff);
        mmio_write32(GICD_IGROUPR0 + 4 * i, 0x00000000);
    }

    /*
     * Setting target processor for interrupts.
     * 0 ~ 7 are read-only for PPIs.
     * BCM2711 AUX ID 96(VC base) + 29(AUX offset)
     * Set AUX ID to CPU 0.
     */
    int intids[4], index = 0;
    unsigned long gicd_itar;

    intids[index] = 125; // AUX IRQ SPI;
    intids[index ++] = 30;  // PNS timer IRQ PPI;
    intids[index ++] = -1;

    for (int i = 0; intids[i] != -1; i ++) {
        gicd_itar = GET_ITARGETSR_N_BASE(intids[i]);
        val = (0x01 << (GET_ITARGETSR_OFFSET(intids[i]) * 8));
        mmio_set32(gicd_itar, val);
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
    for (int i = 0; intids[i] != -1; i ++) {
        gicd_pri = GET_IPRIORITY_N_BASE(intids[i]);
        val = (0x00 << (GET_IPRIORITY_OFFSET(intids[i]) * 8));
        mmio_set32(gicd_pri, val);
    }

    /*
     * CPU Interface Setting.
     * 0b-0010-0110-0001
     * Driven by GIC on group 0, and disable group 1.
     * Separates the priority drop and interrupt deactivation operations
     */
    gicc_ctl = 0x00000261;
    mmio_write32(GICC_CTLR, gicc_ctl);

    /*
     * Enable group 0 interrupt forwarding from Distribution to CPU.
     */
    gicd_ctl = mmio_read32(GICD_CTLR);
    gicd_ctl = 0x01;
    mmio_write32(GICD_CTLR, gicd_ctl);

    mn_uart_write_txt("GIC_INIT End\n");
    return; 
}

void sys_timer_init() {


}

void user_task1(void) {
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


void main() {
    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !\n");

    show_curEL();

    mn_uart_write_txt("Switch From EL2 to El1 !\n");
    switch_el2_to_el1(0x10000000);
    /********** Below in EL1 **********/

    //sys_timer_init();

    show_curEL();

    unsigned long vector_base = vector_table_init();
    mn_uart_write_hex(vector_base);
    mn_uart_write_txt("\n");

    gic_init();

    mn_uart_write_txt("\nSwitch From EL1 to El0 !\n");
    switch_el1_to_el0(0x11000000);
    /********** Below in EL0 **********/

    // user_task1();
    while(1);

    return;
}