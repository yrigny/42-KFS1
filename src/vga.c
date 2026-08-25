#include "vga.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY (uint16_t *)0xB8000
#define VGA_COLOR_DEFAULT 0x0A
#define VGA_COLOR_WHITE 0x0F
#define VGA_COLOR_RED 0x0C
#define VGA_COLOR_GREEN 0x0A

/* Static variables to keep track of the current cursor position */
static size_t vga_row;
static size_t vga_col;

/* Creates a VGA entry for a character and color */
static uint16_t	vga_entry(char c, uint8_t color)
{
	return (uint16_t)(unsigned char)c | ((uint16_t)color << 8);
}

/* Initializes the VGA buffer */
void	vga_init(void)
{
	vga_clear();
}

/* Clears the VGA buffer */
void	vga_clear(void)
{
	uint16_t	*vga = VGA_MEMORY;

	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga[i] = vga_entry(' ', VGA_COLOR_DEFAULT);
	vga_row = 0;
	vga_col = 0;
}

/* Prints a character to the VGA buffer */
void	vga_putchar(char c)
{
	uint16_t	*vga = VGA_MEMORY;

	if (c == '\n') {
		vga_row++;
		vga_col = 0;
	} else {
		vga[vga_row * VGA_WIDTH + vga_col] = vga_entry(c, VGA_COLOR_DEFAULT);
		vga_col++;
		if (vga_col >= VGA_WIDTH) {
			vga_col = 0;
			vga_row++;
		}
	}

	if (vga_row >= VGA_HEIGHT) {
		vga_clear();
	}
}

/* Prints a string to the VGA buffer */
void	vga_putstr(const char *str)
{
	while (*str) {
		vga_putchar(*str++);
	}
}
