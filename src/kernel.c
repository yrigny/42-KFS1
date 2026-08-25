#include "vga.h"
#include "libk.h"

void kernel_main(void)
{
    vga_init();
    vga_putstr("42");

    if (strcmp("42", "42") == 0)
        vga_putstr("\nString comparison successful!");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
