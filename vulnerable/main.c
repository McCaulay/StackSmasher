#include <string.h>
#include <stdio.h>

void dummy()
{
    __asm__("jmp %esp\n\t");
}

void removeChar(char *str, char garbage)
{
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

void printHello(char *bar)
{
    removeChar(bar, '\x0A');
    removeChar(bar, '\x0D');
    removeChar(bar, '\x43');

    char c[0x512];
    strcpy(c, bar);
    printf("Hello %s!\n", c);
}

int main(int argc, char **argv)
{
    printHello(argv[1]);
    return 0;
}
