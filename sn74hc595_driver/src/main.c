#include <stdio.h>
#include "sn74hc595.h"

#define NUM_OF_SHIFT_REGISTERS 1
#define CE_CHANNEL			   0

#define MAX_CHAR_SIZE 		   50

#define ASCII_TO_INT_OFFSET    48



static const SPIClockSpeed CLOCK_SPEED = SPI_SPEED_16_MHZ;

int main(int argc, char *argv[])
{
    // Clear printf buffer immediately
    setbuf(stdout, NULL);

    static char buffer[MAX_CHAR_SIZE] = {};


#ifdef ARM
    printf("This is RPI\n");
#else
    printf("This is x86!\n");
#endif
        // Setup the SN75HC595 Module
        if(SN74HC595Setup(CE_CHANNEL, CLOCK_SPEED, NUM_OF_SHIFT_REGISTERS) < 0)
        {
            printf("Failed to setup sn75hc595\n");
            return 0;
        }

    int val = 0;
    int len = 0;
    while(1)
    {

        printf("Type in the byte to send to the Shift Register (Value between 0 - 255) : ");
        fgets(buffer, MAX_CHAR_SIZE, stdin);

        len = strlen(buffer) - 1;

        for(size_t i = 0; i < len; ++i)
        {
            if(!isdigit(buffer[i]))
            {
                printf("Invalid input");
                return 1;
            }
        }

        val = atoi(buffer);

        printf("Value is %i\n", val);

        printf("Return value of write: %i\n", SN74HC595ReadWrite((uint8_t) val, len));
    }

    return 0;
}
