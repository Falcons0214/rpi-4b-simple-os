#include "io.h"
#include "utils.h"
#include "sys_utils.h"
#include "vtable.h"
#include "gic.h"
#include "timer.h"

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

    gic_init();
    core_timer_init();
    
    show_curEL();

    mn_uart_write_txt("Switch From EL2 to El1 !\n");
    switch_el2_to_el1(0x10000000);
    /********** Below in EL1 **********/

    show_curEL();

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