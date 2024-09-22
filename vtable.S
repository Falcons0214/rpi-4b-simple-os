#include "asm.h"

.macro save_all
    sub sp, sp, #REG_FILE_SIZE
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]
    str x30, [sp, 16 * 15]
.endm

.macro load_all
    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    ldr x30, [sp, 16 * 15]
    add sp, sp, #REG_FILE_SIZE
.endm

.macro	ventry	label
.align	7
b	\label
.endm

incomplete_excep_handler:

.extern exception_entry 
exception_handler:
    save_all
    bl exception_entry
    load_all
    eret

/*
 * !! When irq_entry returns, we need to check whether we restored
 * the context of the process we interrupted or if we need to load
 * the context of the next process and return to it.
 */ 
.extern irq_entry
irq_handler:
    save_all
    add x0, sp, #REG_FILE_SIZE
    bl irq_entry
    load_all
    eret

.extern serr_entry
serr_handler:
    save_all
    bl serr_entry
    load_all
    eret

.section .excp_vector, "axw"
.align	11
exception_vector_table:
    // Current EL with SP0
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler

    // Current EL with SPx
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler

    // Lower EL using AArch64
    ventry exception_handler
    ventry irq_handler
    ventry incomplete_excep_handler
    ventry serr_handler

    // Lower EL using AArch32
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler
    ventry incomplete_excep_handler

.extern _excp_vector_start
.global vector_table_init
vector_table_init:
    ldr x0, =_excp_vector_start
    msr vbar_el1, x0
    ret
