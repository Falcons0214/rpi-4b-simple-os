.section .text
.global switch_el2_to_el1
switch_el2_to_el1:
    msr sp_el1, x0    // Set EL1 stack pointer
    mov x0, (1 << 31) // EL1 uses aarch64
    msr hcr_el2, x0
    mov x0, 0x1c5     
    msr spsr_el2, x0
    msr elr_el2, lr
    eret              // return to EL1

.section .text
.global switch_el1_to_el0
switch_el1_to_el0:
    msr sp_el0, x0
    mov x0, 0x180
    msr spsr_el1, x0
    msr elr_el1, lr
    eret
// 0b0001-1000-0000

.global _read_excp_number_by_elr1
_read_excp_number_by_elr1:
    mrs x0, elr_el1
    sub x0, x0, #8
    //ldr x1, [x0]
    //mov x0, x1
    ret

.global _read_esr_el1
_read_esr_el1:
    mrs x0, esr_el1
    ret


.global _set_cntfrq_el0
_set_cntfrq_el0:
    msr cntfrq_el0, x0
    ret

.global _get_cbar_el1
_get_cbar_el1:
    mrs x0, s3_1_c15_c3_0
    ret