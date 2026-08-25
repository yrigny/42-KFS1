#ifndef VGA_H
#define VGA_H

#include "libk.h"

void vga_init(void);
void vga_clear(void);
void vga_putchar(char c);
void vga_putstr(const char *str);

#endif
