#include "timer.h"
#include "sys_utils.h"

/*
 * Core Timer:
 *
 * 1. CNTFRQ_EL0: For frequency (50MHz)
 * 2. CNTP_CTL_EL0
 * 3. CNTP_CVAL_EL0
 * 4. CNTPCT_EL0
 */
void core_timer_set_freq() {
    /*
     * RPI_b default timer freq is 54MHZ.
     */
    // _set_cntfrq_el0(50000000);
}

void core_timer_init() {
    _core_timer_init();
}

void core_timer_set_expir_time(unsigned long sec) {
    _core_timer_set_exp(sec * _get_cntfrq_el0());
}
