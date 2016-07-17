#include <stdio.h>
#include "sn74hc165.h"

#define CLOCK_PIN 		32
#define CLOCK_INH_PIN	36
#define SHLD_PIN		38
#define QH_PIN 			40

#define NUM_OF_SHIFT_REGISTERS 2

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

    if(SN74HC165Setup(CLOCK_PIN, CLOCK_INH_PIN, SHLD_PIN, QH_PIN, NUM_OF_SHIFT_REGISTERS) < 0)
    {
    	printf("Error setting up SN74HC165Setup: %s\n", strerror(errno));
    	return 0;
    }
    
    printf("SN75HC165Setup successfully setup\n");

    uint8_t* data = SN74HC165Read();

    for(size_t i = 0; i < NUM_OF_SHIFT_REGISTERS; i++)
    {
    	printf("Data of shift register: %zu is %u\n", i, data[i]);
    }

    return 0;
}
