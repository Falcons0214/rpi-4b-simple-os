SECTIONS
{
    . = 0x80000;     /* Kernel load address for AArch64 */
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    .excp_vector : {
        KEEP(vtable.o (.excp_vector))
    }
    _end = .;

   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }

   _excp_vector_start = ADDR(.excp_vector);
}
__bss_size = (__bss_end - __bss_start)>>3;
