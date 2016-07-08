#include <stdio.h>
#include "PCA9685RPi.h"

int main(int argc, char *argv[])
{
    printf("Hello Worlds!\n");

#ifdef RPI
    printf("This is RPI\n");
#else
    printf("This is x86!\n");
#endif
    PCA9685Setup(0, 0);

    return 0;
}
