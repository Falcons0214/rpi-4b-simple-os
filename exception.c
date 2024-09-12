#include "exception.h"
#include "io.h"
#include "sys_utils.h"
#include "utils.h"

void exception_entry(void) {
    unsigned long value;

    mn_uart_write_txt("Exception Start !\n");
    value = _read_esr_el1();
    mn_uart_write_txt("Exception Number: ");
    mn_uart_write_hex(value & 0xfc000000);

    mn_uart_write_txt("\nException End !\n");
    return;
}

void core_timer_hander() {
    unsigned long sec = 1;
    mn_uart_write_txt("time up !\n");
    _core_timer_set_exp((sec * _get_cntfrq_el0()) >> 4);
    return;
}

void irq_entry(void) {
    unsigned int intid;

    mn_uart_write_txt("IRQ Start !\n");
    
    intid = mmio_read32(GICC_IAR) & GICC_IAR_INTID_MASK;
    mn_uart_write_txt("IRQ INTID: ");
    mn_uart_write_dec(intid);
    mn_uart_write_txt("\n");

    switch(intid) {
        case 30:
            core_timer_hander();
            break;
        default:
            mn_uart_write_txt("IRQ unsupport!\n");
    }

    mmio_set32(GICC_EOIR, intid);
    mmio_set32(GICC_DIR, intid);

    mn_uart_write_txt("IRQ End!\n");
    return;
}

void serr_entry(void) {
    mn_uart_write_txt("Not support\n");
}