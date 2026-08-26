MAGICNB     equ 0x1BADB002
MBFLAGS     equ 0x00000003
CHECKSUM    equ - (MAGICNB + MBFLAGS)


; ===== multiboot1 header，GRUB use this to recognize bootable kernel =====
section .multiboot
align 4                         ; align by 4 bytes
    dd MAGICNB                  ; magic number for kernel recognition
    dd MBFLAGS                  ; justify layout (Bit 0) + provide memory map (Bit 1)
    dd CHECKSUM                 ; checksum：magic+flags+checksum = 0

; ===== Kernel's stack =====
section .bss
align 16
stack_bottom:
    resb 16384                  ; 16KB stack
stack_top:

; ===== Kernel code entrypoint =====
section .text
global _start
extern kernel_main

_start:
    mov esp, stack_top          ; point to stack
    call kernel_main            ; call C code
    cli                         ; clear interrupt
.hang:
    hlt                         ; hang the CPU
    jmp .hang
