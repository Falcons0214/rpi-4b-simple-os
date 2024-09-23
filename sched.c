#include <stddef.h>
#include "sched.h"
#include "mm.h"
#include "user.h"
#include "sys_utils.h"
#include "fork.h"
#include "io.h"

/*
 * This pointer will always point to the process task structure
 * that is currently occupying the processor.
 */
struct task_struct *current;

/*
 * The map record each user task structre that is runing now.
 */
struct task_struct *task_map[NR_TASKS];

/*
 * Number of user tasks that exist now.
 */
int nr_tasks;

void sched_init(unsigned long init_task) {
    nr_tasks = 0;
	copy_process(init_task, 0);
	current = task_map[0];

    for (int i = 1; i < NR_TASKS; i ++)
        task_map[i] = NULL;
}

void preempt_disable(void)
{
	current->preempt_count ++;
}

void preempt_enable(void)
{
	current->preempt_count --;
}

void switch_to(struct task_struct *next, unsigned long reg_stack_base) 
{
	if (current == next) 
		return;
	/*
     * If the timer triggers a wake-up for the scheduler, 
     * we need to store the registers of the user process 
     * interrupted by the timer in the user task block.
	 *
	 * After store the registers, next is cover the original
	 * region by the next user task registers file.
     */
	struct task_struct *prev = current;
	struct cpu_context *prev_cnx, *cur_cnx;
	current = next;

	/*
	 * First we need copy the prev process state from 
	 * `reg_stack_base` + #REG_FILE_SIZE to its task block.
	 *
	 * Include General purpose registers & spsr_el1 & elr_rl1.
	 */
	prev_cnx = &prev->cpu_context;
	unsigned long *base = (unsigned long*)reg_stack_base;
	for (int i = 0; i < 29; i ++)
		prev_cnx->general_regs[i] = *(base - i);
	prev_cnx->fp = *(base - 29);
	prev_cnx->lr = *(base - 30);
	prev_cnx->spsr_el1 = _get_spsr_el1();
	prev_cnx->elr_el1 = _get_elr_el1();
	prev_cnx->sp_el0 = _get_sp_el0();

	/*
	 * Second we put new task register file & update 
	 * spsr_el1 & elr_el1, so when we return from irq_entry
	 * the `load_all` macro will puts new task. 
	 */
	cur_cnx = &current->cpu_context;
	for (int i = 0; i < 29; i ++)
		*(base - i) = cur_cnx->general_regs[i];
	*(base - 29) = cur_cnx->fp;
	*(base - 30) = cur_cnx->lr;
	_set_spsr_el1(cur_cnx->spsr_el1);
	_set_elr_el1(cur_cnx->elr_el1);
	_set_sp_el0(cur_cnx->sp_el0);
}

/*
 * This scheduler is copy from Linux Kernel 0.0.1
 */
void _schedule(unsigned long reg_stack_base) {
	preempt_disable();

	int next, c;
	struct task_struct *p;
	while (1) {
		c = -1;
		next = 0;
		for (int i = 0; i < NR_TASKS; i ++){
			p = task_map[i];
			if (p && p->state == TASK_RUNNING && p->counter > c) {
				c = p->counter;
				next = i;
			}
		}
		if (c) {
			break;
		}
		for (int i = 0; i < NR_TASKS; i ++) {
			p = task_map[i];
			if (p) {
				p->counter = (p->counter >> 1) + p->priority;
			}
		}
	}

	switch_to(task_map[next], reg_stack_base);
	preempt_enable();
}

void timer_tick(unsigned long reg_stack_base) {
	-- current->counter;
	if (current->counter > 0 || current->preempt_count > 0) {
		return;
	}
	current->counter = 0;
	enable_irq();
	_schedule(reg_stack_base);
	disable_irq();
}