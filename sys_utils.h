#include "io.h"

#define ESR1_EC_MASK 0xfc000000
#define ESR1_EC(value) \
    ((value & ESR1_EC_MASK) >> 2)

#define ESR1_ECXP_NUM_MASK 0x0000ffff
#define ESR1_EXCP_NUM(value) \
    (value & ESR1_ECXP_NUM_MASK)


extern int switch_el2_to_el1(unsigned long el1_stack_addr);

extern int switch_el1_to_el0(void *user_task, unsigned long el0_stack_addr);

extern unsigned int _read_excp_number_by_elr1(void);
extern unsigned int _read_esr_el1(void);

extern unsigned long _get_cntfrq_el0(void);
extern void _set_cntfrq_el0(unsigned int freq);
extern void _core_timer_init(void);
extern void _core_timer_set_exp(unsigned long sec);

extern unsigned long _get_sp_el0(void);
extern unsigned long _get_elr_el1(void);
extern unsigned int _get_spsr_el1(void);
extern void _set_sp_el0(unsigned long stack_ptr);
extern void _set_elr_el1(unsigned long elr_el1);
extern void _set_spsr_el1(unsigned int spsr_el1);

/*
 * Holds the physical base address of the memory-mapped 
 * GIC CPU interface registers.
 */
extern unsigned long _get_cbar_el1(void);

extern unsigned long _get_sp(void);

extern void enable_irq(void);
extern void disable_irq(void);