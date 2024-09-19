#ifndef _IO_H_
#define _IO_H_

#include <stdint.h>

/*--------------- Base Define ---------------*/
#define REG_SIZE 4
#define SYS_CLK_MU_UART 500000000

#define PERIPHERAL_BASE 0xFC000000
#define AUX_BASE  PERIPHERAL_BASE + 0x2215000
#define GPIO_BASE PERIPHERAL_BASE + 0x2200000

#define GICD_BASE PERIPHERAL_BASE + 0x3841000
#define GICC_BASE PERIPHERAL_BASE + 0x3842000

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
#define MSG_LEN 1024
#define BADU_RATE(baud) ((SYS_CLK_MU_UART / (baud << 3)) - 1)

void mn_uart_init();
uint32_t mn_uart_read_ch();
void mn_uart_write_ch(char ch);
void mn_uart_write_txt(char *buf);
void mn_uart_write_dec(unsigned long value);
void mn_uart_write_hex(unsigned long value);

int uart_async_write_txt(char *buf);
uint32_t uart_is_write_ready(void);

void mn_ctl_regs_dump();

void enable_uart_trans_intr();
void enable_uart_rec_intr();
void disable_uart_trans_intr();
void disable_uart_rec_intr();
void isit_uart_intr_en();

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

/*--------------- Mail Box ---------------*/
/*
 * MailBox 0
 */
#define MAILBOX_BASE  PERIPHERAL_BASE + 0x200b880
#define MAILBOX_READ  MAILBOX_BASE
#define MAILBOX_PEEK  MAILBOX_BASE + 0x10
#define MAILBOX_SEND  MAILBOX_BASE + 0x14
#define MAILBOX_STAT  MAILBOX_BASE + 0x18
#define MAILBOX_CONF  MAILBOX_BASE + 0x1c

// Mailbox 1
#define MAILBOX_WRITE MAILBOX_BASE + 0x20

#define MAILBOX_FULL  0x80000000
#define MAILBOX_EMPTY 0x40000000

int mb_request_a_tag(uint32_t channel, uint32_t tag, \
                     uint32_t tag_size, uint32_t *buf);


/*--------------- Watchdog ---------------*/
#define WATCHDOG_BASE PERIPHERAL_BASE + 0x2100000
#define PM_RSTC				0x1c
#define PM_RSTS				0x20
#define PM_WDOG				0x24

#define PM_PASSWORD			0x5a000000
#define PM_WDOG_TIME_SET		0x000fffff
#define PM_RSTC_WRCFG_CLR		0xffffffcf
#define PM_RSTS_HADWRH_SET		0x00000040
#define PM_RSTC_WRCFG_SET		0x00000030
#define PM_RSTC_WRCFG_FULL_RESET	0x00000020
#define PM_RSTC_RESET			0x00000102


/*--------------- GIC ---------------*/

/*
 * For GICD_ISENABLERn, we need check GICD_TYPER.ITLinesNumber.
 * It define the maximum number of interrupts that the GIC supports.
 */
#define GICD_CTLR       GICD_BASE + 0x0000
#define GICD_TYPER      GICD_BASE + 0x0004
#define GICD_IGROUPR0   GICD_BASE + 0x0080
#define GICD_ISENABLER0 GICD_BASE + 0x0100
#define GICD_ICENABLER0 GICD_BASE + 0x0180
#define GICD_ISPENDR0   GICD_BASE + 0x0200
#define GICD_ICPENDR0   GICD_BASE + 0x0280
#define GICD_ISACTIVER0 GICD_BASE + 0x0300
#define GICD_IPRIORITY0 GICD_BASE + 0x0400
#define GICD_ITARGETSR0 GICD_BASE + 0x0800
#define GICD_ICFGR0     GICD_BASE + 0x0c00

#define GICD_TYPER_ITLN_MASK 0x0000001f

#define GET_ISENABLE_N_BASE(INTID) \
    (GICD_ISENABLER0 + ((INTID) / 32) * 4)
#define GET_ISENABLE_OFFSET(INTID) ((INTID) % 32)

#define GET_ISACTIVE_N_BASE(INTID) \
    (GICD_ISACTIVER0 + ((INTID) / 32) * 4)
#define GET_ISACTIVE_OFFSET(INTID) ((INTID) % 32)

#define GET_ICENABLE_N_BASE(INTID) \
    (GICD_ICENABLER0 + ((INTID) / 32) * 4)
#define GET_ICENABLE_OFFSET(INTID) ((INTID) % 32)

#define GET_IGROUPR_N_BASE(INTID) \
    (GICD_IGROUPR0 + ((INTID) / 32) * 4)
#define GET_IGROUPR_OFFSET(INTID) ((INTID) % 32)

#define GET_ITARGETSR_N_BASE(INTID) \
    (GICD_ITARGETSR0 + ((INTID) / 4) * 4)
#define GET_ITARGETSR_OFFSET(INTID) ((INTID) % 4)

#define GET_IPRIORITY_N_BASE(INTID) \
    (GICD_IPRIORITY0 + ((INTID) / 4) * 4)
#define GET_IPRIORITY_OFFSET(INTID) ((INTID) % 4)

#define GET_ISACTIVE_N_BASE(INTID) \
    (GICD_ISACTIVER0 + ((INTID) / 32) * 4)
#define GET_ISACTIVE_OFFSET(INTID) ((INTID) % 32)

#define GET_ISPEND_N_BASE(INTID) \
    (GICD_ISPENDR0 + ((INTID) / 32) * 4)
#define GET_ISPEND_OFFSET(INTID) ((INTID) % 32)

#define GET_ICPEND_N_BASE(INTID) \
    (GICD_ICPENDR0 + ((INTID) / 32) * 4)
#define GET_ICPEND_OFFSET(INTID) ((INTID) % 32)

#define GET_ICFG_N_BASE(INTID) \
    (GICD_ICFGR0 + ((INTID) / 16) * 4)
#define GET_ICFG_OFFSET(INTID) ((INTID) % 16)

    
#define GET_INTR_NUM(ITLnum) (((ITLnum) + 1) * 32)

#define GICC_CTLR GICC_BASE + 0x0000
#define GICC_PMR  GICC_BASE + 0x0004
#define GICC_BPR  GICC_BASE + 0x0008
#define GICC_IAR  GICC_BASE + 0x000c
#define GICC_EOIR GICC_BASE + 0x0010
#define GICC_IIDR GICC_BASE + 0x00fc
#define GICC_DIR  GICC_BASE + 0x1000

#define GICC_CTLR_ENABLE_GRP1 = 0x00000001

#define GICC_IAR_INTID_MASK 0x000003ff

#define ARCH_VER_MASK 0x000f0000

void show_gic_ver(void);
void irq_enable(unsigned int intid);
void irq_enable(unsigned int intid);

#endif