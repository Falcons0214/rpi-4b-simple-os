#include "utils.h"

void buf_init(char *buf, uint32_t size) {
    for (uint32_t i = 0; i < size; i ++)
        buf[i] = '\0';
}

void memcpy(char *dest, char *src, uint32_t size) {
    for (uint32_t i = 0; i < size; i ++)
        dest[i] = src[i];
}