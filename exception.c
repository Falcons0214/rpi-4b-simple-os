#include "exception.h"
#include "io.h"
#include "sys_utils.h"
#include "utils.h"

void exception_entry(void) {
    unsigned long value;

    mn_uart_write_txt("Exception Start !\n");
    // mn_uart_write_hex(value);
    // mn_uart_write_txt("\n");
    // value =  _read_excp_number();
    // mn_uart_write_hex(value);

    value = _read_esr_el1();
    mn_uart_write_txt("Exception Number: ");
    mn_uart_write_hex(value & 0xfc000000);

    mn_uart_write_txt("\nException End !\n");
    return;
}

void irq_entry(void) {
    mn_uart_write_txt("Not support\n");
}

void serr_entry(void) {
    mn_uart_write_txt("Not support\n");
}