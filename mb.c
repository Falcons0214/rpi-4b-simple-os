#include "io.h"
#include "utils.h"

volatile static uint32_t __attribute__((aligned(16))) mbox[16];

int mb_request_a_tag(uint32_t channel, uint32_t tag, \
                     uint32_t tag_size, uint32_t *buf) {
    uint32_t re = (((long)(mbox) & ~0xf) | (channel & 0xf));

    buf_init((char*)mbox, sizeof(uint32_t) * 16);
    mbox[0] = sizeof(uint32_t) * 16;
    mbox[1] = 0x0;
    mbox[2] = tag;
    mbox[3] = tag_size;
    mbox[4] = 0x0;
    mbox[5] = 0x0;
    mbox[6] = 0x00010003;
    mbox[7] = 8;

    while (mmio_read32(MAILBOX_STAT) & MAILBOX_FULL);
    mmio_write32(MAILBOX_WRITE, re);

    while (1) {
        while (mmio_read32(MAILBOX_STAT) & 0x40000000);
        if (re == mmio_read32(MAILBOX_READ))
            break;
    }

    memcpy(buf, mbox, sizeof(uint32_t) * 16);
    return 1;
}