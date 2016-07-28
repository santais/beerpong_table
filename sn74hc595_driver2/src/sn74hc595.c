
#include "sn74hc595.h"

#define SHIFT_REGISTER_BIT_SIZE 8

#define SECONDS_TO_MICROSECONDS_MULTIPLIER 1000000
#define DEFAULT_FREQUENCY_HZ            1000000
#define DEFAULT_FREQUENCY_PERIOD_MUS    (1/DEFAULT_FREQUENCY_HZ) * SECONDS_TO_MICROSECONDS_MULTIPLIER
/****************************/
/*    Private Variables     */
/****************************/
static SN74HC595* m_activeSN74HC595 = NULL;


/****************************/
/*    Private Functions     */
/****************************/



/**
 * @brief
 *
 * @param clkPin                CLK Pin
 * @param sclkEnPin             CLK INH Pin
 * @param latchPin              SH/LD Pin
 * @param dataPin               Data Pin
 * @param numOfShiftRegister    Number of shift registers
 *
 * @return fileDescriptor
 */
int SN74HC595Setup(const uint8_t clkPin, const uint8_t clkEnPin,
                   const uint8_t dataPin, const uint8_t numOfShiftRegisters)
{
	if(m_activeSN74HC595 == NULL)
	{
		m_activeSN74HC595 = (SN74HC595*) malloc(sizeof(SN74HC595));
		if(m_activeSN74HC595 == NULL)
		{
			printf ("Not enough memory to initiate SN74HC595 structure: %s\n", strerror (errno));
			return -1;
		}
	}

	m_activeSN74HC595->clkPin   		   = clkPin;
	m_activeSN74HC595->clkEnPin 		   = clkEnPin;
	m_activeSN74HC595->dataPin  		   = dataPin;
	m_activeSN74HC595->numOfShiftRegisters = numOfShiftRegisters;

#ifdef ARM

	pinMode(clkPin, OUTPUT);
	pinMode(clkEnPin, OUTPUT);
	pinMode(dataPin, OUTPUT);

	// Set initial values
	digitalWrite(clkPin, LOW);
	digitalWrite(clkEnPin, LOW);
	digitalWrite(dataPin, LOW);

#endif

	return 1;

}

/**
 * @brief Read the data from the 165. The number of bytes returned depends on numOfShiftRegisters registered
 *
 * @return Read Data
 */
int SN74HC595Write(uint8_t* data)
{
    if(m_activeSN74HC595 == NULL)
    {
        printf ("SN74HC595 not setup. Call SN74HC165Setup first %s\n", strerror (errno));
        return -1;
    }
#ifdef ARM
    // Set ŔCLK low while loading data into the register
    digitalWrite(m_activeSN74HC595->clkEnPin, LOW);
    digitalWrite(m_activeSN74HC595->clkPin, LOW);

    for(size_t i = 0; i < m_activeSN74HC595->numOfShiftRegisters; i++)
    {
    	for(uint8_t bits = SHIFT_REGISTER_BIT_SIZE - 1; bits >= 0; --bits)
    	{
    		digitalWrite(m_activeSN74HC595->dataPin, data[i] & (1 << bits));

    		digitalWrite(m_activeSN74HC595->clkPin, HIGH);
    		usleep(DEFAULT_FREQUENCY_PERIOD_MUS);
    		digitalWrite(m_activeSN74HC595->clkPin, LOW);
    		usleep(DEFAULT_FREQUENCY_PERIOD_MUS);

    	}
    }

    // Set the latch pin high to store the data
    digitalWrite(m_activeSN74HC595->clkEnPin, HIGH);
#endif
    return 1;
}

/**
 * @brief Returns the SN74HC595 structure
 *
 * @return 
 */
SN74HC595* getSN74HC595Struct()
{
	return m_activeSN74HC595;
}