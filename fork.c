#include "mm.h"
#include "sched.h"

int copy_process(unsigned long fn, unsigned long arg)
{
	// preempt_disable();

	struct task_struct *p;
	struct cpu_context *p_cnx;
	p = (struct task_struct*)get_free_page();
	if (!p)
		return 1;
	p->priority = 2;
	p->state = TASK_RUNNING;
	p->counter = p->priority;
	p->preempt_count = 1; // disable preemtion until schedule_tail

	p_cnx = &p->cpu_context;
	for (int i = 0; i < 29; i ++)
		p_cnx->general_regs[i] = 0;
	p_cnx->fp = 0;
	p_cnx->lr = 0;
	p_cnx->elr_el1 = fn;
	p_cnx->spsr_el1 = 0;
	p_cnx->sp_el0 = (unsigned long)p + THREAD_SIZE;

	int pid = nr_tasks ++;
	task_map[pid] = p;

	preempt_enable();
	return 0;
}