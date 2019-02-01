//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
// System
#include <math.h>

#include "EdgeLightController.h"
#include "LightController.h"
#ifndef UNIT_TESTING
#include "Arduino.h"
#include "Logger.h"
#else
#include <stddef.h>
#define min(a,b) ((a)<(b)?(a):(b))
#endif

namespace Controller
{

//-------------------------------------------------------------------------------//
// CONSTANTS
//-------------------------------------------------------------------------------//
const uint8_t RGB_PWM_LED_SIZE           = 3;
const uint8_t RGB_PWM_LED_PKG_SIZE       = RGB_PWM_LED_SIZE + 1;
const uint16_t PWM_MAX_RESOLUTION        = 4096;

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//



/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::LightController
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return:
/**************************************************************/
EdgeLightController::EdgeLightController(uint8_t i2cAddr, float freq)
#ifndef UNIT_TESTING
    : m_pwmDriver(Adafruit_PWMServoDriver(i2cAddr))
#endif
{
    // Initialize the RGB leds
    for(int i = 0; i < NUM_EDGE_LEDS; i++)
    {
        // Set ID to i and initial color to red.
        m_rgbLeds[i] = new Ws2812Led(i, 255, 0, 0);
    }

#ifndef UNIT_TESTING
    // Set the frequency
    m_pwmDriver.setPWMFreq(freq);

    // Begin the drive
    m_pwmDriver.begin();
#endif
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::~LightController
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
EdgeLightController::~EdgeLightController()
{

}

/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::handleGet
// Description: TODO
// Inputs: uint8_t*, uint8_t*
// Output: TODO
// Return: int
/**************************************************************/
int EdgeLightController::read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int retVal = BJ_SUCCESS;

    if((ptrBuffer == NULL) || (ptrBytesWritten == NULL))
    {
#ifndef UNIT_TESTING
        BJBP_LOG_ERR("NULL input buffer\n");
#endif
        retVal = BJ_FAILURE;
    }

    // Check if it's all values or single request
    if(retVal == BJ_SUCCESS)
    {
        if(ptrBuffer[RGB_ID_BUF_POS] == RGB_GET_ALL_VALUES_REQ)
        {
            retVal = getAllRgbVal(ptrBuffer, ptrBytesWritten);
        }
        else
        {
            retVal = getSingleRgbVal(ptrBuffer, ptrBytesWritten);
        }
    }

    return retVal;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::handlePut
// Description: Package format: (0: idx, 1: RED LED, 2: GREEN LED, 3: BLUE LED).
//            : Each ID includes 4 PWM channels on the PCA 9685 driver.
// Inputs: uint8_t*, uint8_t
// Output: TODO
// Return: fhdrint
/**************************************************************/
int EdgeLightController::write(uint8_t* ptrPayload, uint8_t payloadSize)
{
    int retVal = BJ_SUCCESS;
    uint8_t numOfLeds = 0;
    uint16_t baseIdx = 0;
    uint16_t rgbPwmVals[RGB_PWM_LED_SIZE] = {0};
    uint8_t channelId = 0;

    // Validate null pointer
    if(ptrPayload == NULL)
    {
        retVal = BJ_FAILURE;
    }

    // Validate payload size
    if((payloadSize % RGB_PWM_LED_PKG_SIZE) > 0)
    {
        retVal = BJ_FAILURE;
    }

    // Set the rgb lights
    if(retVal == BJ_SUCCESS)
    {
        // Calculate num of LEDS
        numOfLeds = payloadSize / RGB_PWM_LED_PKG_SIZE;

        for(int i = 0; i < numOfLeds; i++)
        {
            // Calculate start base index and channel id
            baseIdx = i * RGB_PWM_LED_PKG_SIZE;
            channelId = ptrPayload[baseIdx];


            // Validate the channel
            if(channelId > NUM_EDGE_LEDS)
            {
                retVal = BJ_FAILURE;
#ifndef UNIT_TESTING
                BJBP_LOG_ERR("Invalid channel ID request: %i\n", ptrPayload[baseIdx]);
#endif
                break;
            }

            // Set the RGB light values
            m_rgbLeds[channelId]->setRgbValues(ptrPayload[baseIdx + RGB_RED_BUF_POS],
                    ptrPayload[baseIdx + RGB_GREEN_BUF_POS], ptrPayload[baseIdx + RGB_BLUE_BUF_POS]);

            retVal = setRgbLight(m_rgbLeds[channelId], &rgbPwmVals[0]);

            // Validate pwm setting. If successful, set the actual pwm values
            if(retVal == BJ_SUCCESS)
            {
                retVal = setPwm(channelId, rgbPwmVals, RGB_PWM_LED_SIZE);
            }

        }
    }

    return retVal;
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: calcPwmOnTime
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return: uint16_t
/**************************************************************/
uint16_t EdgeLightController::calcPwmOnTime(uint8_t rgbVal)
{
    // Calculate the value
    uint16_t pwmRes = (uint16_t) floor(((float) rgbVal / (float) UINT8_MAX) * (float) PWM_MAX_RESOLUTION);

    // Ensure that the maximum PWM value is not exceeded
    return(min(pwmRes, PWM_MAX_RESOLUTION));
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: EdgeLightController::setRgbLight
// Description: TODO
// Inputs: Ws2812Led*
// Output: TODO
// Return: int16_t
/**************************************************************/
int16_t EdgeLightController::setRgbLight(Ws2812Led* ptrRgbLight,  uint16_t* rgbPwmBuffer)
{
    int16_t retVal = BJ_FAILURE;

    if((ptrRgbLight != NULL) && (rgbPwmBuffer != NULL))
    {
        retVal = BJ_SUCCESS;

        // Calculate the pwm frequency
        rgbPwmBuffer[RGB_RED_BUF_POS - 1]   = calcPwmOnTime(ptrRgbLight->getRedVal());
        rgbPwmBuffer[RGB_GREEN_BUF_POS - 1] = calcPwmOnTime(ptrRgbLight->getGreenVal());
        rgbPwmBuffer[RGB_BLUE_BUF_POS - 1]  = calcPwmOnTime(ptrRgbLight->getBlueVal());
    }

    return retVal;
}


/**************************************************************/
// Date: 30 Dec 2017
// Function: EdgeLightController::setPwm
// Description: TODO
// Inputs: uint8_t, uint16_t*, uint8_t
// Output: TODO
// Return: fhdrint16_t
/**************************************************************/
int16_t EdgeLightController::setPwm(uint8_t channelID, uint16_t* pwmValues, uint8_t numOfPwms)
{
    int16_t retVal = BJ_SUCCESS;
    uint16_t pwmOffTime = 0;

    // Validate input
    for(int i = 0; i < numOfPwms; i++)
    {
        if(pwmValues[i] > PWM_MAX_RESOLUTION)
        {
            retVal = BJ_FAILURE;
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("pwmValue exceeds max pwm val: %i\n", pwmValues[i]);
#endif
        }
    }

    // Set the pwm values
    if(retVal == BJ_SUCCESS)
    {
        for(unsigned int i = 0; i < numOfPwms; i++)
        {
            pwmOffTime = PWM_MAX_RESOLUTION - pwmValues[i];
#ifndef UNIT_TESTING
            m_pwmDriver.setPWM(channelID + i, pwmValues[i], pwmOffTime);
#endif
        }
    }

    return retVal;
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: EdgeLightController::getSingleRgbVal
// Description: TODO
// Inputs: uint8_t*, uint8_t*
// Output: TODO
// Return: fdrint16_t
/**************************************************************/
int16_t EdgeLightController::getSingleRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    uint16_t retVal = BJ_SUCCESS;

    // Set the channel ID
    uint8_t channelID = ptrBuffer[RGB_ID_BUF_POS];

    // Validate channel ID request
    if(channelID > NUM_EDGE_LEDS)
    {
        retVal = BJ_FAILURE;
    }
    else
    {
        Ws2812Led* ptrWsRgbLed = m_rgbLeds[channelID];

        // Set the buffer values
        ptrBuffer[RGB_RED_BUF_POS]   = ptrWsRgbLed->getRedVal();
        ptrBuffer[RGB_GREEN_BUF_POS] = ptrWsRgbLed->getGreenVal();
        ptrBuffer[RGB_BLUE_BUF_POS]  = ptrWsRgbLed->getBlueVal();
    }

    if(retVal == BJ_SUCCESS)
    {
        *ptrBytesWritten = RGB_PWM_LED_PKG_SIZE;
    }

    return retVal;
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: EdgeLightController::getAllRgbVal
// Description: TODO
// Inputs: uint8_t*, uint8_t*
// Output: TODO
// Return: int16_t
/**************************************************************/
int16_t EdgeLightController::getAllRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    uint16_t retVal = BJ_SUCCESS;
    Ws2812Led* ptrWsRgbLed = NULL;
    uint8_t baseIdx = 0;

    // Get all the values
    for(int i = 0; i < NUM_EDGE_LEDS; i++)
    {
        baseIdx = i * RGB_WS_LED_SIZE;

        // Get Ws2812 Reference
        ptrWsRgbLed = m_rgbLeds[i];

        // Set the buffer values
        ptrBuffer[baseIdx]                     = ptrWsRgbLed->getId();
        ptrBuffer[baseIdx + RGB_RED_BUF_POS]   = ptrWsRgbLed->getRedVal();
        ptrBuffer[baseIdx + RGB_GREEN_BUF_POS] = ptrWsRgbLed->getGreenVal();
        ptrBuffer[baseIdx + RGB_BLUE_BUF_POS]  = ptrWsRgbLed->getBlueVal();
    }

    if(retVal == BJ_SUCCESS)
    {
        *ptrBytesWritten = RGB_PWM_LED_PKG_SIZE * NUM_EDGE_LEDS;
    }
    return retVal;
}

};

