#include "io.h"

void main() {
    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !\n");
    simple_shell();
    return;
}