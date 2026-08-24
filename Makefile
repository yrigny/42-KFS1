CC = gcc
ASM = nasm
LD = ld
CFLAGS = -m32 -Wall -Wextra -Werror -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
ASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL = kernel.bin
ISO = kernel.iso

all:  $(KERNEL)

$(KERNEL): boot.o kernel.o
	$(LD) $(LDFLAGS) -o $@ $^

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp kernel.bin isodir/boot/kernel.bin
	cat > isodir/boot/grub/grub.cfg << 'EOF'
	menuentry "KFS-1" {
		multiboot /boot/kernel.bin
	}
	EOF
	grub-mkrescue --compress=xz -o $(ISO) isodir

boot.o: boot/boot.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(KERNEL) $(ISO)
	@-rm -rf isodir

re: clean all
