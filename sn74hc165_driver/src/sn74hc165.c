
#include "sn74hc165.h"

#define SECONDS_TO_MICROSECONDS_MULTIPLIER 1000000

#define DEFAULT_FREQUENCY_HZ            1000000
#define DEFAULT_FREQUENCY_PERIOD_MUS    (1/DEFAULT_FREQUENCY_HZ) * SECONDS_TO_MICROSECONDS_MULTIPLIER

/****************************/
/*    Private Variables     */
/****************************/
static SN74HC165* m_SN74HC165 = NULL;

/**
 * @brief
 *
 * @param clkPin                CLK Pin
 * @param sclkEnPin             CLK INH Pin
 * @param latchPin              SH/LD Pin
 * @param dataPin               Data Pin
 * @param numOfShiftRegister    Number of shift registers
 *
 * @return 1 upon success. -1 otherwise
 */
int SN74HC165Setup(const uint8_t clkPin, const uint8_t clkEnPin,
                   const uint8_t latchPin, const uint8_t dataPin, const uint8_t numOfShiftRegisters)
{
    if(m_SN74HC165 == NULL)
    {
        m_SN74HC165 = (SN74HC165*) malloc(sizeof(SN74HC165));
        
        if(m_SN74HC165 == NULL)
        {
            printf("Out of memory at initiating SN74HC165 struct %s\n", strerror(errno));
            return -1;
        }
    }
    
    // Initialize pins
    m_SN74HC165->clkPin              = clkPin;
    m_SN74HC165->clkEnPin            = clkEnPin;
    m_SN74HC165->latchPin            = latchPin;
    m_SN74HC165->dataPin             = dataPin;
    m_SN74HC165->numOfShiftRegisters = numOfShiftRegisters;
    
    // Setup the pinmodes
#ifdef ARM
    pinMode(clkPin, OUTPUT);
    pinMode(clkEnPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, INPUT);
    
    // Set initial values
    digitalWrite(clkPin, LOW);
    digitalWrite(clkEnPin, HIGH);
    digitalWrite(latchPin, HIGH);
#endif
 
    return 1;
}


/**
 * @brief Read the data from the 165. The number of bytes returned depends on numOfShiftRegisters registered
 *
 * @return Read Data
 */
uint8_t* SN74HC165Read()
{
    if(m_SN74HC165 != NULL)
    {
        printf ("SN74HC165 not setup. Call SN74HC165Setup first %s\n", strerror (errno));
        return NULL;
    }
    
    // Initialize data array
    uint8_t* data = (uint8_t*) malloc(sizeof(uint8_t) * m_SN74HC165->numOfShiftRegisters);
    
    return data;
}

/**
 * @brief Returns the SN75HC595 structure
 *
 * @return
 */
SN74HC165 getSN74HC165fStruct()
{
    return *m_SN74HC165;
}

/**
 * @brief Returns the active frequency
 */
int getFrequency()
{
    return (int) DEFAULT_FREQUENCY_HZ;
}