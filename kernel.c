#include "io.h"
#include "utils.h"

void main() {
    int x, y, z;
    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !!\n");
    mn_uart_write_hex((unsigned long)&x);
    mn_uart_write_txt("\n");
    mn_uart_write_hex((unsigned long)&y);
    mn_uart_write_txt("\n");
    mn_uart_write_hex((unsigned long)&z);
    // uint32_t tag[16];
    // mb_request_a_tag(8, 0x00010005, 8, tag);
    // print_buf_hex32(tag, 16);
    // simple_shell();
    return;
}