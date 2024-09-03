.global switch_el2_to_el1
.global switch_el1_to_el0

switch_el2_to_el1:
    msr sp_el1, x0    // Set EL1 stack pointer
    mov x0, (1 << 31) // EL1 uses aarch64
    msr hcr_el2, x0
    mov x0, 0x1c5     
    msr spsr_el2, x0
    msr elr_el2, lr
    eret              // return to EL1

switch_el1_to_el0:
    msr sp_el0, x0
    mov x0, 0x1c0
    msr spsr_el1, x0
    msr elr_el1, lr
    eret