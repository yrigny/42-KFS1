ENTRY(_start)

/* Declare 3 program headers with different permissions */
PHDRS
{
    text   PT_LOAD FLAGS(5); /* 5 = R(4)+X(1) */
    rodata PT_LOAD FLAGS(4); /* 4 = R */
    data   PT_LOAD FLAGS(6); /* 6 = R(4)+W(2) */
}

SECTIONS
{
    . = 1M;                 /* Place Kernel at 1MB address */

    .text :
    {
        *(.multiboot)       /* GRUB scan the first 8KB for multiboot header */
        *(.text)
    } :text

    .rodata : 
    {
        *(.rodata)
    } :rodata

    .data :
    {
        *(.data)
    } :data

    .bss :
    {
        *(COMMON)
        *(.bss)
    } :data
}
