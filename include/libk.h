#ifndef LIBK_H
#define LIBK_H

/* Replace stdint.h type definitions */
typedef unsigned int		size_t;
typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef signed int			int32_t;

/* Replace stdarg.h type definitions */
typedef __builtin_va_list	va_list;
#define va_start(ap, param)	__builtin_va_start(ap, param)
#define va_end(ap)			__builtin_va_end(ap)
#define va_arg(ap, type)	__builtin_va_arg(ap, type)

#define KERN_EMERG	"\x01"
#define KERN_ERR	"\x02"
#define KERN_WARN	"\x03"
#define KERN_INFO	"\x04"
#define KERN_DEBUG	"\x05"

/* Memory and string functions */
void	*memcpy(void *dest, const void *src, size_t n);
void	*memmove(void *dest, const void *src, size_t n);
void	*memset(void *s, int c, size_t n);
size_t	strlen(const char *s);
int		strcmp(const char *s1, const char *s2);
int		strncmp(const char *s1, const char *s2, size_t n);

/* Printf /Printk functions */
void	kvprintf(const char *fstr, va_list ap);
void	kprintf(const char *fstr, ...);
void	kprintk(const char *fstr, ...);

#endif
