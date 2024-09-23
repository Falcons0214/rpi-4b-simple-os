#include "mm.h"

#define THREAD_SIZE				PAGE_SIZE
#define USER_STACK_SIZE         (THREAD_SIZE - sizeof(struct task_structure))
#define NR_TASKS				(1 << 4) 

#define FIRST_TASK task_map[0]
#define LAST_TASK task_map[NR_TASKS-1]

#define TASK_RUNNING				0

extern struct task_struct *current;
extern struct task_struct *task_map[NR_TASKS];
extern int nr_tasks;

struct cpu_context {
    unsigned long general_regs[29]; // x0 ~ x28 
    unsigned long fp;               // x29
    unsigned long lr;               // x30
    unsigned long sp_el0;
    unsigned long elr_el1;
    unsigned int spsr_el1;
};

struct task_struct {
    struct cpu_context cpu_context;
    long state;
    long priority;
    long counter;
    long preempt_count;
};

void sched_init(unsigned long init_task);
void timer_tick(unsigned long reg_stack_base);
void preempt_disable(void);
void preempt_enable(void);
void cpu_switch_to(unsigned long prev, unsigned long next);