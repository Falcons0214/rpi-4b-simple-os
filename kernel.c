#include "io.h"

void main() {

    uint32_t tag[16];

    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !\n");
    mb_request_a_tag(8, 0x00010002, 4, tag);
    simple_shell();
    return;
}