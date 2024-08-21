#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

/*--------------- Base Define ---------------*/
#define REG_SIZE 4
#define SYS_CLK_MU_UART 500000000

#define PERIPHERAL_BASE 0xFE000000
#define AUX_BASE  PERIPHERAL_BASE + 0x215000
#define GPIO_BASE PERIPHERAL_BASE + 0x200000

/*--------------- mmio interface ---------------*/
void mmio_set32(unsigned long reg, uint32_t value);
void mmio_clear32(unsigned long reg, uint32_t value);
void mmio_write32(unsigned long reg, uint32_t value);
uint32_t mmio_read32(unsigned long reg);

/*--------------- Aux Define ---------------*/
/*
 * AUX registers
 */
#define AUX_IRQ     AUX_BASE
#define AUX_ENABLES AUX_BASE + 0x04

/*
 * Mini Uart registers
 */
#define AUX_MU_IO_REG   AUX_BASE + 0x40
#define AUX_MU_IER_REG  AUX_BASE + 0x44
#define AUX_MU_IIR_REG  AUX_BASE + 0x48
#define AUX_MU_LCR_REG  AUX_BASE + 0x4c
#define AUX_MU_MCR_REG  AUX_BASE + 0x50
#define AUX_MU_LSR_REG  AUX_BASE + 0x54
#define AUX_MU_MSR_REG  AUX_BASE + 0x58
#define AUX_MU_SCRATCH  AUX_BASE + 0x5c
#define AUX_MU_CNTL_REG AUX_BASE + 0x60
#define AUX_MU_STAT_REG AUX_BASE + 0x64
#define AUX_MU_BAUD_REG AUX_BASE + 0x68

#define AUX_MU_IIR_FIFI_CLR 0x00000006

#define AUX_MU_IER_ERI 0x00000002
#define AUX_MU_IER_ETI 0x00000001

#define AUX_MU_CNTL_TE 0x00000002
#define AUX_MU_CNTL_RE 0x00000001

#define AUX_MU_IO_MASK      0x000000FF
#define AUX_MU_IRQ_MASK     0x00000001
#define AUX_MU_ENA_MASK     0x00000001

#define AUX_MU_IER_ERI_MASK 0x00000002
#define AUX_MU_IER_ETI_MASK 0x00000001


/*--------------- mini uart interface ---------------*/
#define BADU_RATE(baud) ((SYS_CLK_MU_UART / (baud << 3)) - 1)

void mn_uart_init();
uint32_t mn_uart_read_ch();
void mn_uart_write_ch(char ch);
void mn_uart_write_txt(char *buf);
void mn_uart_write_dec(unsigned long value);
void mn_uart_write_hex(unsigned long value);

void mn_ctl_regs_dump();

void simple_shell();

/*--------------- GPIO Define ---------------*/

#define GPIO_MAX_NUM 58

/*
 * GPIO (Alternate function)
 */
#define GPFSEL0 GPIO_BASE
#define GPFSEL1 GPIO_BASE + 0x04
#define GPFSEL2 GPIO_BASE + 0x08
#define GPFSEL3 GPIO_BASE + 0x0c
#define GPFSEL4 GPIO_BASE + 0x10
#define GPFSEL5 GPIO_BASE + 0x14

#define GPFSEL_14_F5_TX 0b010
#define GPFSEL_15_F5_RX 0b010

/*
 * GPIO (Pin Output Set)
 */
#define GPSET0 GPIO_BASE + 0x1c
#define GPSET1 GPIO_BASE + 0x20

/*
 * GPIO (Pin Output Clear)
 */
#define GPCLR0 GPIO_BASE + 0x28
#define GPCLR1 GPIO_BASE + 0x2c

/*
 * GPIO (Pin Level)
 */
#define GPLEV0 GPIO_BASE + 0x34
#define GPLEV1 GPIO_BASE + 0x38

/*
 * GPIO (Pin Event Detect Status)
 */
#define GPEDS0 GPIO_BASE + 0x40
#define GPEDS1 GPIO_BASE + 0x44

/*
 * GPIO (Pin Rising Edge Detect Enable)
 */
#define GPREN0 GPIO_BASE + 0x4c
#define GPREN1 GPIO_BASE + 0x50

/*
 * GPIO (Pin Falling Edge Detect Enable)
 */
#define GPFEN0 GPIO_BASE + 0x58
#define GPFEN1 GPIO_BASE + 0x5c

/*
 * GPIO (Pin High Detect Enable)
 */
#define GPHEN0 GPIO_BASE + 0x64
#define GPHEN1 GPIO_BASE + 0x68

/*
 * GPIO (Pin Low Detect Enable)
 */
#define GPLEN0 GPIO_BASE + 0x70
#define GPLEN1 GPIO_BASE + 0x74

/*
 * GPIO (Pin Async. Rising Edge Detect 0)
 */
#define GPAREN0 GPIO_BASE + 0x7c
#define GPAREN1 GPIO_BASE + 0x80

/*
 * GPIO (Pin Async. Falling Edge Detect)
 */
#define GPAFEN0 GPIO_BASE + 0x88
#define GPAFEN1 GPIO_BASE + 0x8c

/*
 * GPIO (Pull-up / Pull-down Register)
 */
#define GPIO_PUP_PDN_CNTRL_REG0 GPIO_BASE + 0xe4
#define GPIO_PUP_PDN_CNTRL_REG1 GPIO_BASE + 0xe8
#define GPIO_PUP_PDN_CNTRL_REG2 GPIO_BASE + 0xec
#define GPIO_PUP_PDN_CNTRL_REG3 GPIO_BASE + 0xf0

#define GPPPCT_14_PD 10
#define GPPPCT_15_PD 10

/*--------------- GPIO Interface ---------------*/
uint32_t gpio_set(uint32_t pin_number, uint32_t value,\
                  uint32_t base, uint32_t size);

#endif