#include <stddef.h>
#include "simple_malloc.h"

#define BANK_SIZE 8192 * 2

static unsigned int top = 0;
static unsigned char bank[BANK_SIZE];

void* malloc(unsigned int size) {
    unsigned int ret = top;

    if (top + size >= BANK_SIZE)
        return NULL;

    /*
     * Let size align by 8 bytes.
     */
    size >>= 3;
    size <<= 3;

    top += size;
    return (void*)&bank[ret];
}

