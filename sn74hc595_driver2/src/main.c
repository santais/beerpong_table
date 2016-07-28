#include <stdio.h>
#include "sn74hc595.h"

#define NUM_OF_SHIFT_REGISTERS 1
#define MAX_CHAR_SIZE 100

#define SR_CLK_PIN       15
#define RCLK_PIN         16
#define SER_PIN          1


int main(int argc, char *argv[])
{
    // Clear printf buffer immediately
    setbuf(stdout, NULL);

    static uint8_t buffer[MAX_CHAR_SIZE] = {};


#ifdef ARM
    printf("This is RPI\n");
   // Setup wiringPi
    wiringPiSetup();

#else
    printf("This is x86!\n");
#endif
        // Setup the SN75HC595 Module
        if(SN74HC595Setup(SR_CLK_PIN, RCLK_PIN, SER_PIN, NUM_OF_SHIFT_REGISTERS) < 0)
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

        if(val > 255) 
        {
            printf("Value exceeds 255! Setting value to 255\n");
            val = 255;
        }

        printf("Value is %i\n", val);
        buffer[0] = (uint8_t) val;
	printf("Buffer value 0 is: %i\n", buffer[0]);

        printf("Return value of write: %i\n", SN74HC595Write(buffer));
    }

    return 0;
}
