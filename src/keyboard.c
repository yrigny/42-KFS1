#include "keyboard.h"

#define KEYBOARD_DATA_PORT	 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Read a byte from an x86 I/O port
static inline uint8_t	inb(uint16_t port) {
	uint8_t ret;
	__asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}

// Translation table for Scan Code Set 1 (Index = Scancode)
static const char	scancode_ascii_map[] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
	'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/',	0, '*', 0, ' '
};

// Poll port 0x64 bit 0 until key data is ready, then read port 0x60
uint8_t	keyboard_read_scancode(void) {
	while (!(inb(KEYBOARD_STATUS_PORT) & 1)) {
		// CPU polling loop: wait until output buffer has data
	}
	return inb(KEYBOARD_DATA_PORT);
}

// Blocks until a valid key press (Make Code) occurs and returns ASCII char
char	keyboard_getc(void) {
	uint8_t scancode;

	while (1) {
		scancode = keyboard_read_scancode();

		// Ignore key release (Break Codes have bit 7 / 0x80 set)
		if (scancode & 0x80) {
			continue;
		}

		// Map valid scancode array bounds
		if (scancode < sizeof(scancode_ascii_map)) {
			char ascii = scancode_ascii_map[scancode];
			if (ascii != 0) {
				return ascii;
			}
		}
	}
}
