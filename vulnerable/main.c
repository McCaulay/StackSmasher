#include <string.h>
#include <stdio.h>

void dummy()
{
    __asm__("jmp %esp\n\t");
}

void printHello(char *bar)
{
    char c[0x512];
    strcpy(c, bar);
    printf("Hello %s!\n", c);
}

int main(int argc, char **argv)
{
    printHello(argv[1]);
    return 0;
}
