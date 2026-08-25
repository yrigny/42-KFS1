#ifndef LIBK_H
#define LIBK_H

typedef unsigned int	size_t;
typedef unsigned char	uint8_t;
typedef unsigned short	uint16_t;

void	*memcpy(void *dest, const void *src, size_t n);
void	*memmove(void *dest, const void *src, size_t n);
void	*memset(void *s, int c, size_t n);
size_t	strlen(const char *s);
int		strcmp(const char *s1, const char *s2);
int		strncmp(const char *s1, const char *s2, size_t n);

#endif
