#include "io.h"
#include "utils.h"
#include "sys_utils.h"
#include "vtable.h"
#include "gic.h"
#include "timer.h"
#include "user.h"
#include "sched.h"
#include "mm.h"

void main() {
    mn_uart_init();
    mn_uart_write_txt("\033[2J\033[H");
    mn_uart_write_txt("Hello World !\n");

    show_curEL();
    mn_uart_write_txt("Switch From EL2 to El1 !\n");
    switch_el2_to_el1(0x10000000);
    
    unsigned long vector_base = vector_table_init();
    gic_init();
    core_timer_init();
    mm_init();
    sched_init();

    show_curEL();
    mn_uart_write_txt("\nSwitch From EL1 to El0 !\n");
    // core_timer_set_expir_time((unsigned long)1);

    /*
     * We will switch to init task.
     * Fisrt arg: Task address, this will be assign to elr_el1.
     * Secind arg: Task stack address, this will be user task stack.
     */
    switch_el1_to_el0(init_task, (unsigned long)task_map[0]);

    for (int i = 0, ret; i < 10; i ++) {
        ret = uart_async_write_txt("Async eifhoei---===fa oefjaoif j oiafjoeiafo !\n");
    }

    return;
}