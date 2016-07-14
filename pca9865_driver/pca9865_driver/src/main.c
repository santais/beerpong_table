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

void displayPCA9685Struct()
{
    printf("\n=====================================\n");
    PCA9685* pca9685 = getActivePCA9685Struct();

    if(pca9685 != NULL)
    {
        printf("I2C Address: %i\n", pca9685->i2cAddress);
        printf("Frequency: %i\n", pca9685->frequency);
        printf("File Descriptor: %i\n", pca9685->fileDescriptor);

        // Print pins and values
        PinOutputList* current = pca9685->pinOutputList;
        while(current != NULL)
        {
            printf("LED Pin %i is value of %i\n", current->pinOutput.pin, current->pinOutput.value);
            current = current->next;
        }
    }
    printf("=====================================\n");
}

void testProgram()
{
    // Turn all off
    PCA9685AllLEDsOn();
    sleep(1);

    PCA9685AllLEDsOff();
    sleep(1);

    // Turn each one individually
    PCA9685LEDOn(0);
    sleep(1);

    PCA9685LEDOff(0);
    PCA9685LEDOn(1);
    sleep(1);

    PCA9685LEDOff(1);
    PCA9685LEDOn(2);
    sleep(1);

    PCA9685LEDOff(2);
    sleep(1);

    // Try different colour combinations
    PCA9685SetPWMDC(0, 50);
    PCA9685SetPWMDC(1, 10);
    PCA9685SetPWMDC(2, 100);
    sleep(1);

    PCA9685SetPWMDC(0, 0);
    PCA9685SetPWMDC(1, 100);
    PCA9685SetPWMDC(2, 100);
    sleep(0.9);

    PCA9685SetPWMDC(0, 30);
    PCA9685SetPWMDC(1, 30);
    PCA9685SetPWMDC(2, 70);
    sleep(0.8);

    PCA9685SetPWMDC(0, 70);
    PCA9685SetPWMDC(1, 10);
    PCA9685SetPWMDC(2, 70);
    sleep(0.7);

    PCA9685SetPWMDC(0, 10);
    PCA9685SetPWMDC(1, 10);
    PCA9685SetPWMDC(2, 100);
    sleep(0.6);

    PCA9685SetPWMDC(0, 100);
    PCA9685SetPWMDC(1, 10);
    PCA9685SetPWMDC(2, 75);
    sleep(0.5);

}

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

    // Setup the PCA9685
    // Default Address 0x40 and frequency 500
    if(argc == 2)
    {
        printf("I2C Address is: %i\n", atoi(argv[1]));
        PCA9685Setup(atoi(argv[1]), 500);
    }
    else if(argc == 3)
    {
        printf("I2C Address is: %i and frequency: %i\n", atoi(argv[1]), atoi(argv[2]));
        PCA9685Setup(atoi(argv[1]), atoi(argv[2]));
    }
    else
    {
        PCA9685Setup(1000, 500);
    }

    for(;;)
    {
        printf("Please select either of the following functions: \n");
        printf("1: Set PWM  2: Get PWM  3: Set LED Full ON  4: Set LED Full OFF  5: Set All LEDs Full ON  6: Set All LEDs Full Off  7: Set Frequency  8: Reset"
               "  9: Get FD  A: Get PCA9685 Module  B: Run Test Program\n");
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

        case '9':
            printf("File Descriptor value is: %i\n", getActivePCA9685Struct()->fileDescriptor);
            break;

        case 'A':
            displayPCA9685Struct();
            break;

        case 'B':
            testProgram();
            break;

        default:
            printf("Undefined input!\n");
            break;
        }

        sleep(1);
    }

    return 0;
}
