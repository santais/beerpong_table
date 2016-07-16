#include <stdio.h>
#include "sn74hc165.h"

#define NUM_OF_SHIFT_REGISTERS 1

#define MAX_CHAR_SIZE 3

int main(int argc, char *argv[])
{
    // Clear printf buffer immediately
    setbuf(stdout, NULL);

    static char inputChar[MAX_CHAR_SIZE] = {};


#ifdef ARM
    printf("This is RPI\n");
#else
    printf("This is x86!\n");
#endif

    return 0;
}
