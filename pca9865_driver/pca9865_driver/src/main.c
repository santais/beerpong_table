#include <stdio.h>
#include "PCA9685RPi.h"

#define MAX_CHAR_SIZE 5

#define ASCII_TO_INT_OFFSET 48

int convertCharArrayToInt(char input[MAX_CHAR_SIZE])
{
    int result;

    printf("Low input is: %c\n", input[3]);// - ASCII_TO_INT_OFFSET);

    return( (int)((input[0] - ASCII_TO_INT_OFFSET) * 1000) + ((input[1] - ASCII_TO_INT_OFFSET) * 100) +
            ((input[2] - ASCII_TO_INT_OFFSET) * 10) + ((input[3] - ASCII_TO_INT_OFFSET)));
}


void setPwm()
{
    printf("\n=====================================\n");
    printf("setPwm()\n");
    char *p;
    char inputChar[MAX_CHAR_SIZE] = {};

    printf("Type input pin number (0 - 15)\n");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);
    int pin = strtol(inputChar, &p, 10);

    printf("Type PWM duty cycle (0 - 100)\n");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);

    int pwm = strtol(inputChar, &p, 10);
    if(inputChar[0] != '\n' && (*p == '\n' || *p == '\0'))
    {
        printf("Pin is %i and PWM is %i\n", pin, pwm);

        PCA9685SetPWMDC((uint8_t) pin, (uint16_t) pwm);
    }
    printf("=====================================\n");
}

void getPwm()
{
    printf("\n=====================================\n");
    printf("getPwm()\n");
    char *p;
    char inputChar[MAX_CHAR_SIZE] = {};

    printf("Type input pin number (0 - 15): ");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);
    int pin = strtol(inputChar, &p, 10);

    if(inputChar[0] != '\n' && (*p == '\n' || *p == '\0'))
    {
        printf("Pwm of pin %i is: %i\n", pin, PCA9685GetPWM((uint8_t) pin));
    }

    printf("=====================================\n");
}

void setLEDFullOn()
{
    printf("\n=====================================\n");
    printf("setLEDFullOn()\n");
    char *p;
    char inputChar[MAX_CHAR_SIZE] = {};

    printf("Type input pin number (0 - 15): ");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);
    int pin = strtol(inputChar, &p, 10);

    if(inputChar[0] != '\n' && (*p == '\n' || *p == '\0'))
    {
        PCA9685LEDOn((uint8_t) pin);
    }
    printf("=====================================\n");
}


void setLEDFullOff()
{
    printf("\n=====================================\n");
    printf("setLEDFullOff()\n");
    char *p;
    char inputChar[MAX_CHAR_SIZE] = {};

    printf("Type input pin number (0 - 15): ");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);
    int pin = strtol(inputChar, &p, 10);

    if(inputChar[0] != '\n' && (*p == '\n' || *p == '\0'))
    {
        PCA9685LEDOff((uint8_t) pin);
    }
    printf("=====================================\n");
}

void setAllLEDFullOn()
{
    printf("\n=====================================\n");
    printf("setAllLEDFullOn()\n");
    PCA9685AllLEDsOn();
    printf("=====================================\n");
}

void setAllLEDFullOff()
{
    printf("\n=====================================\n");
    printf("setLEDFullOff()\n");
    PCA9685AllLEDsOff();
    printf("=====================================\n");
}

void setFrequency()
{
    printf("\n=====================================\n");
    printf("setFrequency()\n");
    char *p;
    char inputChar[MAX_CHAR_SIZE] = {};

    printf("Type frequency pin number (24 - 1526): ");
    fgets(inputChar, MAX_CHAR_SIZE, stdin);
    int pin = strtol(inputChar, &p, 10);

    if(inputChar[0] != '\n' && (*p == '\n' || *p == '\0'))
    {
        printf("Prescale value is: %i\n", PCA9685SetFreq((uint16_t) pin));
    }
    printf("=====================================\n");
}

void reset()
{
    printf("\n=====================================\n");
    printf("reset()\n");
    PCA9685Reset();
    printf("=====================================\n");
}

int main(int argc, char *argv[])
{
    // Clear printf buffer immediately
    setbuf(stdout, NULL);

    static char inputChar[MAX_CHAR_SIZE] = {};

#ifdef RPI
    printf("This is RPI\n");
#else
    printf("This is x86!\n");
#endif

    for(;;)
    {
        printf("Please select either of the following functions: \n");
        printf("1: Set PWM  2: Get PWM  3: Set LED Full ON  4: Set LED Full OFF  5: Set All LEDs Full ON  6: Set All LEDs Full Off  7: Set Frequency  8: Reset\n");
        printf("Type input: ");
        fgets(inputChar, MAX_CHAR_SIZE, stdin);
        printf("\n");

        switch(inputChar[0])
        {
        case '1':
            setPwm();
            break;

        case '2':
            getPwm();
            break;

        case '3':
            setLEDFullOn();
            break;

        case '4':
            setLEDFullOff();
            break;

        case '5':
            setAllLEDFullOn();
            break;

        case '6':
            setAllLEDFullOff();
            break;

        case '7':
            setFrequency();
            break;

        case '8':
            reset();
            break;

        default:
            printf("Undefined input!\n");
            break;
        }

        sleep(1);
    }

    return 0;
}
