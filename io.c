#include "io.h"
#include "utils.h"
#include "sys_utils.h"

#define MAX_UART_BUF_LEN 64

char msg_buf[MSG_LEN];
unsigned int msg_cur, msg_tail;

void irq_enable(unsigned int intid) {
    mmio_set32(GET_ISENABLE_N_BASE(intid), (0x1 << GET_ISENABLE_OFFSET(intid)));
}

void irq_disable(unsigned int intid) {
    mmio_set32(GET_ICENABLE_N_BASE(intid), (0x1 << GET_ICENABLE_OFFSET(intid)));
}

void mmio_set32(unsigned long reg, uint32_t value) {
    *((volatile uint32_t*)reg) |= value;
}

void mmio_write32(unsigned long reg, uint32_t value) {
    *((volatile uint32_t*)reg) = value;
}

uint32_t mmio_read32(unsigned long reg) {
    return *((volatile uint32_t*)reg);
}

void mmio_clear32(unsigned long reg, uint32_t value) {
    *((volatile uint32_t*)reg) &= value;
}

uint32_t gpio_set(uint32_t pin_number, uint32_t value,\
                  uint32_t base, uint32_t vsize) {
    uint32_t vlimit = (1 << vsize) - 1;

    if (pin_number >= GPIO_MAX_NUM || value > vlimit)
        return 0;

    uint32_t items = REG_SIZE * 8 / vsize;
    uint32_t reg_index = pin_number / items ;
    unsigned long reg_addr = base + (REG_SIZE * reg_index);
    value <<= pin_number % items * vsize;
    mmio_set32(reg_addr, value);
    return 1;
}

void mn_uart_init() {
    for (int i = 0; i < MSG_LEN; i ++)
        msg_buf[i] = '\0';
    msg_cur = msg_tail = 0;
    /*
     * Set func5 for gpio 14, 15.  
     */
    gpio_set(14, GPFSEL_14_F5_TX, GPFSEL0, 3);
    gpio_set(15, GPFSEL_15_F5_RX, GPFSEL0, 3);

    /*
     * Clear gpio 14, 15
     */
    gpio_set(14, 1, GPCLR0, 1);
    gpio_set(15, 1, GPCLR0, 1);

    /*
     * Set pull dowm resistor for gpio 14, 15.
     */
    gpio_set(14, GPPPCT_14_PD, GPIO_PUP_PDN_CNTRL_REG0, 2);
    gpio_set(15, GPPPCT_15_PD, GPIO_PUP_PDN_CNTRL_REG0, 2);

    mmio_set32(AUX_ENABLES, 0x00000001);
    mmio_set32(AUX_MU_LCR_REG, 0x00000001);
    // mmio_clear32(AUX_MU_LCR_REG, ~(0x00000080));

    /*
     * Clear receive * transmit FIFO
     */
    mmio_set32(AUX_MU_IIR_REG, AUX_MU_IIR_FIFI_CLR);

    /*
     * Enable mini uart receiver & transmitter.
     */
    mmio_set32(AUX_MU_CNTL_REG, AUX_MU_CNTL_TE);
    mmio_set32(AUX_MU_CNTL_REG, AUX_MU_CNTL_RE);

    enable_uart_rec_intr();
    enable_uart_trans_intr();

    mmio_write32(AUX_MU_BAUD_REG, BADU_RATE(115200));
}

void isit_uart_intr_en() {
    unsigned int val = mmio_read32(AUX_MU_IER_REG);
    mn_uart_write_txt("mini uart IER state: ");
    mn_uart_write_hex(val);
    mn_uart_write_txt("\n");
}

void enable_uart_trans_intr() {
    /*
     * Enable mini uart transmit interrupt.
     */
    mmio_set32(AUX_MU_IER_REG, 0x00000001);
}

void enable_uart_rec_intr() {
    /*
     * Enable mini uart receive interrupt.
     */
    mmio_set32(AUX_MU_IER_REG, 0x00000002);
}

void disable_uart_trans_intr() {
    /*
     * Disnable mini uart transmit interrupt.
     */
    mmio_clear32(AUX_MU_IER_REG, ~AUX_MU_IER_ETI);
}

void disable_uart_rec_intr() {
    /*
     * Disnable mini uart receive interrupt.
     */
    mmio_clear32(AUX_MU_IER_REG, ~AUX_MU_IER_ERI);
}

uint32_t uart_is_write_ready() {
    return mmio_read32(AUX_MU_LSR_REG) & 0x20;
}

uint32_t uart_is_read_ready() {
    return mmio_read32(AUX_MU_LSR_REG) & 0x01;
}

void uart_write_blocking(char ch) {
    while (!uart_is_write_ready());
    mmio_write32(AUX_MU_IO_REG, (uint32_t)ch);
}

uint32_t uart_read_blocking() {
    while (!uart_is_read_ready());
    return mmio_read32(AUX_MU_IO_REG);
}

int async_uart_buf_check(int len) {
    if ((msg_tail == msg_cur && msg_buf[msg_tail]) || \
        (msg_tail == msg_cur && len > MSG_LEN))
        return 0;
    if (msg_tail == msg_cur && !msg_buf[msg_tail])
        return 1;
    else if (msg_tail > msg_cur)
        return (MSG_LEN - msg_tail + msg_cur + 1 >= len) ? 1 : 0;
    else if (msg_tail < msg_cur)
        return (msg_cur - msg_tail >= len) ? 1 : 0;
    return 0;
}

// Async
int uart_async_write_txt(char *buf) {
    /*
     * If writing Buffer is fill finction will return 0.
     */
    if(!async_uart_buf_check(strlen(buf)))
        return 0;
    for (int i = 0; buf[i]; i ++) {
        if (buf[i] == '\n') {
            msg_buf[msg_tail ++] = '\r';
            msg_tail %= MSG_LEN;
        }
        msg_buf[msg_tail ++] = buf[i];
        msg_tail %= MSG_LEN;
    }
    return 1;
}

// Async
int uart_read_nonblocking(char *buf) {
    return 1;
}

uint32_t mn_uart_read_ch() {
    return uart_read_blocking();
}

void mn_uart_write_ch(char ch) {
    uart_write_blocking(ch);
}

void mn_uart_write_txt(char *buf) {
    while (*buf) {
        if (*buf == '\n')           
            uart_write_blocking('\r');
        uart_write_blocking(*buf++);
    }
}

void mn_uart_write_dec(unsigned long value) {
    char temp[MAX_UART_BUF_LEN];
    int index = 0;

    for (; value;) {
        temp[index++] = (value % 10) + '0';
        value /= 10;
    }

    for (int i = index - 1; i >= 0; i--)
        uart_write_blocking(temp[i]);
}

void mn_uart_write_hex(unsigned long value) {
    char temp[MAX_UART_BUF_LEN];
    unsigned long mask = 0x0000000F, tmp;
    int index = 0;

    for (; value;) {
        tmp = value & mask;
        if (tmp >= 0 && tmp <= 9) {
            temp[index++] = '0' + tmp;
        } else {
            temp[index++] = 'A' + tmp - 10;
        }
        value >>= 4;
    }

    mn_uart_write_txt("0x");
    for (int i = index - 1; i >= 0; i--)
        uart_write_blocking(temp[i]);
}

// void mn_ctl_regs_dump() {
//     uint32_t reg = 0x0;

//     reg = mmio_read32(AUX_IRQ);
//     mn_uart_write_hex(reg & 0x00000001);
//     mn_uart_write_txt("\n");

//     reg = mmio_read32(AUX_ENABLES);
//     mn_uart_write_hex(reg & 0x00000001);
//     mn_uart_write_txt("\n");

//     reg = mmio_read32(AUX_MU_IER_REG);
//     mn_uart_write_hex(reg & AUX_MU_IER_ERI_MASK);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & AUX_MU_IER_ETI_MASK);
//     mn_uart_write_txt("\n");

//     reg = mmio_read32(AUX_MU_IIR_REG);
//     mn_uart_write_hex(reg & 0x00000006);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000001);
//     mn_uart_write_txt("\n");

//     reg = mmio_read32(AUX_MU_LCR_REG);
//     mn_uart_write_hex(reg & 0x00000040);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000020);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000001);
//     mn_uart_write_txt("\n");

//     reg = mmio_read32(AUX_MU_LSR_REG);
//     mn_uart_write_hex(reg & 0x00000040);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000020);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000002);
//     mn_uart_write_txt("\n");
//     mn_uart_write_hex(reg & 0x00000001);
//     mn_uart_write_txt("\n");
// }

void reboot() {
    mmio_write32(WATCHDOG_BASE + PM_RSTC, PM_PASSWORD | 0x20);
    mmio_write32(WATCHDOG_BASE + PM_WDOG, PM_PASSWORD | 10);
}

void do_command(char *buf) {
    if (buf[0] == '1')
        reboot();
    else
        mn_uart_write_txt("unknow\n");
}

void simple_shell() {
    char buf[32];
    uint32_t index, reg;

    while (1) {
        index = 0;
        buf_init(buf, 32);
        mn_uart_write_txt(": ");
        
        while (1) {
            reg = mn_uart_read_ch();
            if ((reg & AUX_MU_IO_MASK) == '\r') {
                mn_uart_write_txt("\n");
                buf[index + 1] = '\0';
                break;
            }
            mn_uart_write_ch((char)reg);
            buf[index ++] = (char)reg;
        }
        do_command(buf);
        // mn_uart_write_txt(buf);
        // mn_uart_write_txt("\n");
    }
}

void show_gic_ver() {
    uint32_t ver;
    ver = mmio_read32(GICC_IIDR);

    mn_uart_write_txt("GIC Version: ");
    // mn_uart_write_hex((ver & ARCH_VER_MASK) >> 16);
    mn_uart_write_hex(ver);
    mn_uart_write_txt("\n");
    return;
}