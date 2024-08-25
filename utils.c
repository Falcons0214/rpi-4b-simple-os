#include "utils.h"
#include "io.h"

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