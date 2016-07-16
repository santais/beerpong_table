#include <stdio.h>
#include "sn74hc595.h"

#define NUM_OF_SHIFT_REGISTERS 1
#define CE_CHANNEL			   0

#define MAX_CHAR_SIZE 		   2

#define ASCII_TO_INT_OFFSET    48



static const SPIClockSpeed CLOCK_SPEED = SPI_SPEED_16_MHZ;

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

    // Setup the SN75HC595 Module
    if(SN74HC595Setup(CE_CHANNEL, CLOCK_SPEED, NUM_OF_SHIFT_REGISTERS) < 0)
    {
    	//return 0;
    }

    printf("Type in the byte to send to the Shift Register (Value between 0 - 255) : ");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);

    printf("Value is %c\n", inputChar[0]);

    printf("Return value of write: %i\n", SN74HC595ReadWrite((uint8_t) inputChar[0], 1));

    return 0;
}
