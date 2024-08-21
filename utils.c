#include "utils.h"

void buf_init(char *buf, uint32_t size) {
    for (int i = 0; i < size; i ++)
        buf[i] = '\0';
}