#include "vga.h"
#include "libk.h"
#include "keyboard.h"

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

	// Test kprintf with various format specifiers
	kprintf("dec:  %d  neg: %d\n", 1234, -42);
	kprintf("uint: %u\n", 4000000000u);
	kprintf("hex:  %x\n", 0xdeadbeef);
	kprintf("HEX:  %X\n", 0xDEADBEEF);
	kprintf("char: %c\n", 'K');
	kprintf("string: %s\n", "Hello, world!");
	kprintf("null string: %s\n", (const char *)0);
	kprintf("pointer: %p\n", (void*)0x12345678);
	kprintf("null pointer: %p\n", (void*)0);
	kprintf("percent: 100%%\n");

	while (1) {
		// __asm__ volatile ("hlt");
		char c = keyboard_getc();
		vga_putchar(c);
	}
}
