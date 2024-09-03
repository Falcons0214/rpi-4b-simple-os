#include "utils.h"
#include "io.h"
#include "sys_utils.h"

unsigned int get_current_el(void) {
    unsigned int el;
    asm volatile ("mrs %0, CurrentEL" : "=r"(el));
    return (el >> 2) & 0x3;  // The EL value is stored in bits [3:2], so shift right by 2 bits.
}

void show_curEL(void) {
    unsigned long curEL;
    curEL = get_current_el();
    mn_uart_write_hex(curEL);
    mn_uart_write_txt("\n");
}

void buf_init(char *buf, uint32_t size) {
    for (uint32_t i = 0; i < size; i ++)
        buf[i] = '\0';
}

void memcpy(char *dest, char *src, uint32_t size) {
    for (uint32_t i = 0; i < size; i ++)
        dest[i] = src[i];
}

void print_buf_hex32(uint32_t *buf, uint32_t size) {
    for (int i = 0; i < size; i ++) {
        mn_uart_write_hex(buf[i]);
        mn_uart_write_txt("\n");
    }
    mn_uart_write_txt("\n");
}