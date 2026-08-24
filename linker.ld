ENTRY(_start)

SECTIONS
{
    . = 1M;                 /* 内核从 1MB 处开始放，x86 上的安全惯例 */

    .text : {
        *(.multiboot)       /* 必须在文件很靠前，GRUB 才扫得到 */
        *(.text)
    }

    .rodata : 
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }
}
