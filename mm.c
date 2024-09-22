#include "mm.h"

static unsigned char mem_map[PAGING_PAGES];

void mm_init() {
    for (int i = 0; i < PAGING_PAGES; i ++)
        mem_map[i] = 0;
}

unsigned long get_free_page() {
    for (int i = 0; i < PAGING_PAGES; i ++) {
        if (!mem_map[i]) {
            mem_map[i] = 1;
            return LOW_MEMORY + (PAGE_SIZE * i);
        }
    }
    
    return 0;
}

void free_page(unsigned long page) {
    mem_map[(page - LOW_MEMORY) / PAGE_SIZE] = 0;
}