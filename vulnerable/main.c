#include <string.h>
#include <stdio.h>

// A dummy function to contain the JMP ESP function.
void dummy()
{
    __asm__("jmp %esp\n\t");
}

// Remove all occurances of the given character `garbage` from the input string `str`.
//
// `str` The string to have a character removed.
// `garbage` The character to be removed from the string.
void removeChar(char *str, char garbage)
{
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

// Print hello to the user, followed by their input.
//
// `name` The name provided by the user.
void printHello(char *name)
{
    // Simulate bad characters by removing certain characters from the input.
    removeChar(name, '\x0A');
    removeChar(name, '\x0D');
    removeChar(name, '\x43');

    // Allocate 0x200 bytes for the name
    char c[0x200];

    // Copy the name to the buffer (unsafe, stackoverflow part)
    strcpy(c, name);

    // Print the name
    printf("Hello %s!\n", c);
}

// The entry point to the vulnerable application. Requires 1 argument from the
// user.
//
// `argc` The number of arguments passed from the user.
// `argv`[0] The name to be printed after Hello.
// return The exit status code of this application.
int main(int argc, char **argv)
{
    // Validate an argument has been passed.
    if (argc < 2)
    {
        printf("Usage: %s [name]\n", argv[0]);
        return 1;
    }

    // Print Hello to the given name.
    printHello(argv[1]);
    return 0;
}
