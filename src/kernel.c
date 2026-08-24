/*
 * 0xB8000 是显卡"文字模式缓冲区"的物理地址，80列x25行。
 * 每个格子 2 字节：低字节=ASCII字符，高字节=颜色。
 * 直接写这块内存，屏幕立刻显示，不需要调用任何"打印函数"。
 */
void kernel_main(void)
{
    volatile unsigned short *vga = (unsigned short *)0xB8000;
    const char *str = "42";
    unsigned char color = 0x0F; /* 背景黑(0) + 前景白(F) */

    for (int i = 0; str[i] != '\0'; i++) {
        vga[i] = (unsigned short)str[i] | ((unsigned short)color << 8);
    }

    while (1) {
        __asm__ volatile ("hlt"); /* CPU 停下省电，别空转 */
    }
}