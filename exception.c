#include "exception.h"
#include "io.h"
#include "sys_utils.h"
#include "utils.h"

void exception_entry(void) {
    unsigned long value;

    mn_uart_write_txt("Exception Start !\n");
    value = _read_esr_el1();
    mn_uart_write_txt("Exception Number: ");
    mn_uart_write_hex(value & 0xfc000000);

    mn_uart_write_txt("\nException End !\n");
    return;
}

void core_timer_hander() {
    unsigned long sec = 2;
    // If writing buffer is fill the that will be ignore.
    uart_async_write_txt("time up !\n");
    _core_timer_set_exp((sec * _get_cntfrq_el0()));
    return;
}

extern char msg_buf[MSG_LEN];
extern unsigned int msg_cur, msg_tail;

void mini_uart_handler() {
    unsigned short state = (mmio_read32(AUX_MU_IIR_REG) & 0x00000006) >> 1;

    switch(state) {
        case 1:
            /*
             * Transmit.
             */
            while (1) {
                mmio_write32(AUX_MU_IO_REG, msg_buf[msg_cur]);
                msg_buf[msg_cur ++] = '\0';
                msg_cur %= MSG_LEN;
                state = mmio_read32(AUX_MU_STAT_REG) & 0x00000020;
                if (state || msg_buf[msg_cur] == '\0') {
                    /*
                     * Transmit FIFO is full right now or read the `\0`.
                     */
                    break;
                }
            }
            break;
        case 2:
            /*
             * Receive.
             */
            break;
        default:
            break;
    }

    return;
}

void irq_entry(void) {
    unsigned int intid;
    intid = mmio_read32(GICC_IAR) & GICC_IAR_INTID_MASK;

    switch(intid) {
        case 30:
            core_timer_hander();
            break;
        case 125:
            mini_uart_handler();
            break;
        default:
            break;
    }
    mmio_set32(GICC_EOIR, intid);
    mmio_set32(GICC_DIR, intid);
    return;
}

void serr_entry(void) {
    mn_uart_write_txt("Not support\n");
}