#include "vga.h"

#define VGA_WIDTH	80
#define VGA_HEIGHT	25
#define VGA_MEMORY	(uint16_t *)0xB8000

/*
 * VGA registers for I/O port access:
 * 0x3D4 - Control register
 * 0x3D5 - Data register
 * 16 bits are needed to store the cursor position, so two registers are used:
 * 0x0E - Cursor high byte
 * 0x0F - Cursor low byte
 */
#define VGA_CTRL_REG	0x3D4	// which register to write to (0x0E or 0x0F)
#define VGA_DATA_REG	0x3D5	// data to write to the register
#define VGA_CURSOR_HI	0x0E	// store the high 8 bits of the cursor position
#define VGA_CURSOR_LO	0x0F	// store the low 8 bits of the cursor position

/* Static variables to keep track of the current cursor position */
static size_t vga_row;
static size_t vga_col;
static uint8_t vga_color;

/* 
 * Writes a byte to the specified I/O port.
 * "outb" is an inline assembly function that uses the x86 "out" instruction to write a byte to the specified I/O port.
 * "volatile" keyword prevents the compiler from optimizing this function.
 * "a" constraint specifies that the value to be written is in the eax register.
 * "Nd": "d" constraint specifies that the port number is in the dx register. "N" constraint allows the compiler to use an immediate value if the number < 256.
 */
static inline void	outb(uint16_t port, uint8_t val)
{
	__asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

/* Creates a VGA entry for a character and color */
static uint16_t	vga_entry(char c, uint8_t color)
{
	return (uint16_t)(unsigned char)c | ((uint16_t)color << 8);
}

/*
 * Scrolls the VGA buffer up by one row.
 */
static void	vga_scroll(void)
{
	uint16_t	*vga = VGA_MEMORY;
	uint16_t	blank = vga_entry(' ', vga_color);

	// Use memmove to shift the contents of the VGA buffer up by one row
	memmove(vga, vga + VGA_WIDTH, sizeof(uint16_t) * VGA_WIDTH * (VGA_HEIGHT - 1));

	// Clear the last row by filling it with spaces
	for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga[i] = blank;

	// Set the cursor to the first column of the last row
	vga_row = VGA_HEIGHT - 1;
	vga_col = 0;
}

/* Compose a 8-bit color value for VGA entry with 4-bit for foreground and 4-bit for background */
uint8_t	vga_make_color(vga_color_t fg, vga_color_t bg)
{
	return (uint8_t)(fg | (bg << 4));
}

/* Sets the color for the VGA buffer */
void	vga_set_color(vga_color_t fg, vga_color_t bg)
{
	vga_color = vga_make_color(fg, bg);
}

/* Initializes the VGA buffer */
void	vga_init(void)
{
	vga_color = vga_make_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	vga_clear();
}

/* Clears the VGA buffer */
void	vga_clear(void)
{
	uint16_t	*vga = VGA_MEMORY;
	uint16_t	blank = vga_entry(' ', vga_color);

	for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++)
		vga[i] = blank;
	vga_row = 0;
	vga_col = 0;
	vga_update_cursor();
}

/* Prints a character to the VGA buffer */
void	vga_putchar(char c)
{
	uint16_t	*vga = VGA_MEMORY;

	if (c == '\n') {
		vga_row++;
		vga_col = 0;
	} else if (c == '\t') {
		vga_col = (vga_col + 8) & ~(size_t)7;
	} else {
		vga[vga_row * VGA_WIDTH + vga_col] = vga_entry(c, vga_color);
		vga_col++;
	}

	//If the cursor goes beyond the width of the screen, move to the next line
	if (vga_col >= VGA_WIDTH) {
		vga_col = 0;
		vga_row++;
	}
	// If the cursor goes beyond the height of the screen, scroll the screen up
	if (vga_row >= VGA_HEIGHT) {
		vga_scroll();
	}

	vga_update_cursor();
}

/* Prints a string to the VGA buffer */
void	vga_print(const char *str)
{
	while (*str) {
		vga_putchar(*str++);
	}
}

/* Updates the cursor position on the screen */
void	vga_update_cursor(void)
{
	uint16_t	pos = vga_row * VGA_WIDTH + vga_col;
	outb(VGA_CTRL_REG, VGA_CURSOR_HI);
	outb(VGA_DATA_REG, (uint8_t)(pos >> 8));
	outb(VGA_CTRL_REG, VGA_CURSOR_LO);
	outb(VGA_DATA_REG, (uint8_t)(pos & 0xFF));
}
