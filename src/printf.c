#include "libk.h"
#include "vga.h"

void	fprint_uint(uint32_t n, int base, const char *digits)
{
	if (n >= (uint32_t)base)
		fprint_uint(n / base, base, digits);
	vga_putchar(digits[n % base]);
}

void	fprint_int(int n)
{
	if (n < 0) {
		vga_putchar('-');
		fprint_uint((uint32_t)(-n), 10, "0123456789");
	} else {
		fprint_uint((uint32_t)n, 10, "0123456789");
	}
}

void	kvprintf(const char *fstr, va_list ap)
{
	for (const char *ptr = fstr; *ptr != '\0'; ptr++) {
		if (*ptr != '%') {
			vga_putchar(*ptr);
			continue;
		}
		ptr++;	// Step to the next character after '%'
		switch (*ptr) {
			case 'c':	// Get the argument as int and cast to char
				vga_putchar((char)va_arg(ap, int));
				break;
			case 's':	// Handle null strings gracefully
				const char *s = va_arg(ap, const char *);
				vga_print(s ? s : "(null)");
				break;
			case 'd':	// Same as 'i' for signed decimal integers
			case 'i':	// Get the argument as int32_t and print it
				fprint_int(va_arg(ap, int32_t));
				break;
			case 'u':	// Get the argument as uint32_t and print it in base 10
				fprint_uint(va_arg(ap, uint32_t), 10, "0123456789");
				break;
			case 'x':	// Get the argument as uint32_t and print it in base 16 (lowercase)
				fprint_uint(va_arg(ap, uint32_t), 16, "0123456789abcdef");
				break;
			case 'X':	// Get the argument as uint32_t and print it in base 16 (uppercase)
				fprint_uint(va_arg(ap, uint32_t), 16, "0123456789ABCDEF");
				break;
			case 'p':	// Same as 'x' but with prefix "0x" for pointers
				const void *p = va_arg(ap, const void *);
				if (p) {
					vga_print("0x");
					fprint_uint((uint32_t)p, 16, "0123456789abcdef");
				} else	// handle null pointer gracefully
					vga_print("(nil)");
				break;
			case '%':	// Print a literal '%' character
				vga_putchar('%');
				break;
			case '\0':	// Format error: '%' at the end of the string with no specifier
				return;
			default:	// Handle unknown format specifiers by printing them literally
				vga_putchar('%');
				vga_putchar(*ptr);
				break;
		}
	}
}

void	kprintf(const char *fstr, ...)
{
	va_list	ap;	// A pointer to the variable arguments

	va_start(ap, fstr);	// Initialize ap to the address of the first argument after fstr
	kvprintf(fstr, ap);	// Call the function that handles the variable arguments
	va_end(ap);			// Clean up the variable argument list
}
