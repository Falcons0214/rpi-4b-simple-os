#include <stdint.h>

void memcpy(char *dest, char *src, uint32_t size);
void buf_init(char *buf, uint32_t size);
void print_buf_hex32(uint32_t *buf, uint32_t size);