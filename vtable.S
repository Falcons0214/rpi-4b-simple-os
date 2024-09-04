incomplete_excep_handler:

exception_handler:

.section .excp_vector, "axw"
.align	11
exception_vector_table:
    // Current EL with SP0
    b incomplete_excep_handler
    .align 7 
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7

    // Current EL with SPx
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7

    // Lower EL using AArch64
    b exception_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7

    // Lower EL using AArch32
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7
    b incomplete_excep_handler
    .align 7

.extern _excp_vector_start

.global vector_table_init
vector_table_init:
    ldr x0, =_excp_vector_start
    msr vbar_el1, x0
    ret