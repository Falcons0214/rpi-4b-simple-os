#include "user.h"
#include "io.h"

void init_task(void) {
    mn_uart_write_txt("USER: Start !\n");

    unsigned int x = 0xabcdabcd;
    unsigned int size = sizeof(x);
    char *ptr = (char*)&x;
    
    for (int i = 0; i < size; i ++) {
        mn_uart_write_hex(*(ptr + i));
        mn_uart_write_txt("\n");
    }

    mn_uart_write_txt("USER: Exit\n");
    while (1) {
        mn_uart_write_txt("Task 1\n");
    }
    return;
}

void task2() {
    while (1) {
        mn_uart_write_txt("Task 2\n");
    }
}

// void task3() {
//     while (1) {
//         mn_uart_write_txt("Task 3\n");
//     }
// }