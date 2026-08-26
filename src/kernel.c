#include "vga.h"
#include "libk.h"

void kernel_main(void)
{
    vga_init();

    // Test basic printing
    vga_print("42\n");

    // Test printing a long string to check newline handling
    vga_print("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd\n");

    // Test printing with different colors and scrolling
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
    for (int i = 0; i < 25; i++) {
        vga_print("scroll line\n");
    }

    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_print("done.\n");

    while (1) {
        __asm__ volatile ("hlt");
    }
}
