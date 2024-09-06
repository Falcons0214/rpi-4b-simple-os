#include "io.h"
#include "utils.h"
#include "sys_utils.h"
#include "vtable.h"

// void user_task1(void) {
//     mn_uart_write_txt("USER: Start\n");

//     unsigned int x = 0xabcdabcd;
//     unsigned int size = sizeof(x);
//     char *ptr = (char*)&x;
    
//     for (int i = 0; i < size; i ++) {
//         mn_uart_write_hex(*(ptr + i));
//         mn_uart_write_txt("\n");
//     }

//     mn_uart_write_txt("USER: Exit\n");
//     return;
// }

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

    switch_el2_to_el1(0x10000000);

    show_curEL();
    mn_uart_write_txt("Switch From EL2 to El1 !\n");

    unsigned long vector_base = vector_table_init();
    mn_uart_write_hex(vector_base);

    switch_el1_to_el0(0x11000000);
    // show_curEL();
    mn_uart_write_txt("\nSwitch From EL1 to El0 !\n");

    // Enter in EL0.
    user_task1();
    // Exit from EL0.

    while(1);

    return;
}