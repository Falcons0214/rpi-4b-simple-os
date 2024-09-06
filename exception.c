#include "exception.h"
#include "io.h"
#include "sys_utils.h"
#include "utils.h"

void exception_entry(void) {
    unsigned int value, sp;

    mn_uart_write_txt("Exception Start !\n");
    // asm volatile ("mov %0, sp" : "=r" (sp));
    // asm volatile ("mrs %0, elr_el1" : "=r" (value));
    // mn_uart_write_hex(value);
    // mn_uart_write_txt("\n");
    // value =  _read_excp_number();
    // mn_uart_write_hex(value);

    value = _read_excp_number_by_elr1();

    // mn_uart_write_txt("Exception Class: ");
    // mn_uart_write_hex(ESR1_EC(value));

    mn_uart_write_txt("\nException Number: ");
    mn_uart_write_hex(((value & 0x001fffe0) >> 5));

    mn_uart_write_txt("\nException End !\n");
    return;
}