#include "PCA9685RPi.h"

/****************************/
/*    Private Variables     */
/****************************/
int m_fd;

/**
 * @brief Setup the PCA9865
 *
 * @param i2cAddress    Hex Address of te I2C connection
 * @param frequency     Frequency of the PCA9865
 * @return
 */
int PCA9685Setup(const uint8_t i2cAddress, uint16_t freq)
{
    // Setup the I2C Port
#ifdef RPI
    m_fd = wiringPiI2CSetup(i2cAddress);
    // Read current settings and clear restart bit
    int settings = wiringPiI2CReadReg8(m_fd, MODE1) & MODE1_SETUP_MASK;
    // Enable auto increment
    settings |= MODE1_AR_MASK;

    // Write to the register
    wiringPiI2CWriteReg8(m_fd, MODE1, settings);

    // Settup the frequency
    PCA9685SetFreq(freq);
#endif

    return m_fd;
}

/**
 * @brief Reset PCA9685 to default settings.
 */
void PCA9685Reset()
{
    // Restart the PCA9685
#ifdef RPI
    int mode1 = wiringPiI2CReadReg8(m_fd, MODE1) & MODE1_RESTART_MASK;

    if(mode1)
    {
        int settings = mode & MODE1_WAKE_MASK;      // Clear SLEEP bit
        wiringPiI2CReadReg8(m_fd, MODE1, settings);
        usleep(500);                                // Sleep for 500 microseconds
        settings |= MODE1_RESTART_MASK;
        wiringPiI2CReadReg8(m_fd, MODE1, settings);
    }
#endif
}

/**
 * @brief Set the frequency of the PCA9685
 *
 * @param freq  Frequency to be set. Range 24 - 1526 Hz
 */
uint16_t PCA9685SetFreq(uint16_t freq)
{
    uint16_t frequency = 0;

    if(freq > FREQ_RESOLUTION)
    {
        frequency = FREQ_RESOLUTION;
    }
    else
    {
        frequency = freq;
    }

    uint16_t prescale = round(CLOCK_FREQ / (FREQ_RESOLUTION * frequency)) - 1;

#ifdef RPI
    // Set the settings byte
    int settings = wiringPiI2CReadReg8(m_fd, MODE1) & MODE1_SETUP_MASK;
    int sleep = settings | MODE1_SLEEP_MASK;        // Set bit 5 SLEEP
    int wake  = settings & MODE1_WAKE_MASK;         // Clear bit 5 SLEEP
    int restart = settings | MODE1_RESTART_MASK;    // Set bit 7 RESTART

    // Set device to sleep
    wiringPiI2CWriteReg8(m_fd, MODE1, sleep);
    // Set PWM
    wiringPiI2CWriteReg8(m_fd, PRE_SCALE, prescale);
    // Wake up the PCA9685
    wiringPiI2CWriteReg8(m_fd, MODE1, wake);

    // Wait 1ms to for oscillator to stabilize and then restart it
    usleep(500);
    wiringPiI2CWriteReg8(m_fd, MODE1, restart);
#endif
    return prescale;
}

/**
 * @brief Set the PWM Duty Cycle of a specific pin
 *
 * @param LEDPin        Pin to set the Duty Cycle on. Range 0 - 15
 * @param duty_cycle    Duty cycle. Range 0 - 100 %
 */
void PCA9685SetPWMDC(uint8_t LEDPin, uint8_t dutyCycle)
{
    // Calculate max and minimum duty cycle
    uint8_t m_dutyCycle = dutyCycle;
    if(dutyCycle > 100)
    {
        m_dutyCycle = 100;
    }

    // Get lower and upper values
    uint16_t onTime  = (FREQ_RESOLUTION * dutyCycle) / 100;
    uint16_t offTime = FREQ_RESOLUTION - onTime;

    PCA9685SetPWM(LEDPin, onTime, offTime);
}

/**
 * @brief Set the PWM of a specific pin
 *
 * @param LEDPin        Pin to set the pwm for. Range 0 - 15
 * @param onTime        On time of the LED
 * @param offTime       Off time of the LED
 */
void PCA9685SetPWM(uint8_t LEDPin, uint16_t onTime, uint16_t offTime)
{
    if(LEDPin < 16)
    {
        uint8_t LEDRegister = LED0_ON_L + (LED_NEXT_MASK * LEDPin);
#ifdef RPI
        // Calculate the on and off time and write to the registers
        uint8_t LEDRegisterVal = onTime & LED_L_MASK;               // LED_ON_L
        wiringPiI2CWriteReg8(m_fd, LEDRegister, LEDRegisterVal);
        LEDRegisterVal = onTime >> LED_H_SHIFT_MASK;                // LED_ON_H
        wiringPiI2CWriteReg8(m_fd, LEDRegister + 1, LEDRegisterVal);

        LEDRegisterVal = offTime & LED_L_MASK;                      // LED_OFF_L
        wiringPiI2CWriteReg8(m_fd, LEDRegister + 2, LEDRegisterVal);
        LEDRegisterVal = offTime & LED_H_SHIFT_MASK;                // LED_OFF_H
        wiringPiI2CWriteReg8(m_fd, LEDRegister + 3, LEDRegisterVal);
#endif
    }
}

/**
 * @brief Retrieve the PWM for a specific pin
 *
 * @param LEDPin        Pin to get the PWM signal from. Range 0 - 15
 *
 * @return
 */
uint16_t PCA9685GetPWM(uint8_t LEDPin)
{
    uint8_t LEDRegister = LED0_ON_L + (LED_NEXT_MASK * LEDPin);
    uint16_t pwmValue = 0;
#ifdef RPI
    // First retriev LED_ON_H
    uint8_t registerValue = wiringPiI2CReadReg8(m_fd, LEDRegister + 1);
    pwmValue = registerValue & 0xF;         // Only get the 3. byte
    pwmValue <<= LED_H_SHIFT_MASK;          // Shift 8 bytes to make room for LED_ON_L

    // Retrieve LED_ON_L
    registerValue = wiringPiI2CReadReg8(m_fd, LEDRegister);
    pwmValue |= registerValue;
#endif

    return pwmValue;
}

/**
 * @brief Turn LED pin fully on
 *
 * @param LEDPin        Pin to turn all on. Range 0 - 15
 */
void PCA9685LEDOn(uint8_t LEDPin)
{
    if(LEDPin < 16)
    {
        uint8_t LEDRegister = LED0_ON_L + (LED_NEXT_MASK * LEDPin);
#ifdef RPI
        // Read current settings from LED_ON_H
        int settings = wiringPiI2CReadReg8(m_fd, LEDRegister + 1);
        settings |= LED_FULL_ON_OFF_MASK;       // Set full ON mask

        // Write to register
        wiringPiI2CWriteReg8(m_fd, LEDRegister, settings);

        // Ensure that the LED_OFF_H bit 4 is set low aswell
        settings = wiringPiI2CReadReg8(m_fd, LEDRegister + 3);
        settings &= LED_FULL_NEGATED;
        wiringPiI2CWriteReg8(m_fd, LEDRegister, settings);
#endif
    }
}

/**
 * @brief Turn LED pin fully off
 *
 * @param LEDPin        Pin to tunr all off. Range 0 - 15
 */
void PCA9685LEDOff(uint8_t LEDPin)
{
    if(LEDPin < 16)
    {
        uint8_t LEDRegister = LED0_ON_L + (LED_NEXT_MASK * LEDPin);
#ifdef PRPI
        // Read current settings from LED_OFF_H
        int settings = wiringPiI2CReadReg8(m_fd, LEDRegister + 3);
        settings |= LED_FULL_ON_OFF_MASK;

        // Write to register
        wiringPiI2CWriteReg8(m_fd, LEDRegister, settings);
#endif
    }
}

/**
 * @brief Turns on all 16 LEDs output to max 100% duty cycle
 */
void PCA9685AllLEDsOn()
{
#ifdef RPI
    // Read current settings
    int settings = wiringPiI2CReadReg8(m_fd, ALL_LED_ON_H);
    settings |= LED_FULL_ON_OFF_MASK;

    // Write to register
    wiringPiI2CWriteReg8(m_fd, ALL_LED_ON_H, settings);
#endif
}

/**
 * @brief Turn off all 16 LEDs out to min 0% duty cycle.
 */
void PCA9685AllLEDsOff()
{
#ifdef RPI
    // Read current settings
    int settings = wiringPiI2CReadReg8(m_fd, ALL_LED_OFF_H);
    settings |= LED_FULL_ON_OFF_MASK;

    // Write to register
    wiringPiI2CWriteReg8(m_fd, ALL_LED_OFF_H, settings);
#endif
}

/**
 * @brief Get the file descriptor for the I2C port
 *
 * @return
 */
int getFileDescriptor()
{
    return m_fd;
}


