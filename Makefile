NAME		= kernel.bin
ISO			= kfs1.iso

CC			= gcc
ASM			= nasm
LD			= ld

CFLAGS		= -m32 -Wall -Wextra -Werror \
				-fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
ASMFLAGS	= -f elf32
LDFLAGS 	= -m elf_i386 -T linker.ld

SRC_ASM		= boot/boot.asm
SRC_C		= src/kernel.c
OBJ			= boot.o kernel.o

all: $(NAME)

$(NAME): $(OBJ) linker.ld
	$(LD) $(LDFLAGS) -o $(NAME) $(OBJ)

boot.o: $(SRC_ASM)
	$(ASM) $(ASMFLAGS) -o $@ $<

kernel.o: $(SRC_C)
	$(CC) $(CFLAGS) -c -o $@ $<

iso: $(NAME)
	mkdir -p isodir/boot/grub
	cp $(NAME) isodir/boot/$(NAME)
	printf 'set timeout=0\nset default=0\nmenuentry "KFS-1" {\n\tmultiboot /boot/$(NAME)\n}\n' \
		> isodir/boot/grub/grub.cfg
	podman run --rm \
		-v $(PWD)/isodir:/isodir:ro \
		-v $(PWD):/output \
		docker.io/library/debian:bookworm-slim \
		bash -c "apt-get update && apt-get install -y grub-pc-bin xorriso 2>/dev/null | tail -3 && \
				grub-mkrescue -d /usr/lib/grub/i386-pc -o /output/$(ISO) /isodir"

run: $(NAME)
	qemu-system-i386 -kernel $(NAME)

run-iso: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) $(ISO)
	rm -rf isodir

re: fclean all

.PHONY: all clean fclean re run run-iso iso
