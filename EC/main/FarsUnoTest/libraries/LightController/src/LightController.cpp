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
#include "LightController.h"
#ifndef UNIT_TESTING
#include "Arduino.h"
#include "Logger.h"
#else
#include <stddef.h>
#endif

namespace Controller
{

//-------------------------------------------------------------------------------//
// CONSTANTS
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

RgbLeds::RgbLeds(uint8_t redVal, uint8_t greenVal, uint8_t blueVal) :
        m_redval(redVal), m_greenVal(greenVal), m_blueVal(blueVal)
{

}

/**************************************************************/
// Date: 16 Dec 2017
// Function: Ws2812Led::Ws2812Led
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
Ws2812Led::Ws2812Led() : m_id(0), RgbLeds(0, 0, 0)
{

}
/**************************************************************/
// Date: 15 Dec 2017
// Function: Ws2812Led::Ws2812Led
// Description: TODO
// Inputs: uint8_t, uint8_t, uint8_t, uint8_t
// Output: TODO
// Return:
/**************************************************************/
Ws2812Led::Ws2812Led(uint8_t id, uint8_t redVal, uint8_t greenVal, uint8_t blueVal) :
    m_id(id), RgbLeds(redVal, greenVal, blueVal)
{}

/**************************************************************/
// Date: 15 Dec 2017
// Function: Ws2812Led::getId
// Description: TODO
// Inputs:
// Output: TODO
// Return: fhdruint8_t
/**************************************************************/
uint8_t Ws2812Led::getId()
{
    return m_id;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: Ws2812Led::setId
// Description: TODO
// Inputs: int
// Output: TODO
// Return: void
/**************************************************************/
void Ws2812Led::setId(int id)
{
    m_id = id;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: Ws2812Led::setRgbValues
// Description: TODO
// Inputs: uint8_t, uint8_t, uint8_t
// Output: TODO
// Return: void
/**************************************************************/
void Ws2812Led::setRgbValues(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
    m_redval = redVal;
    m_greenVal = greenVal;
    m_blueVal = blueVal;
}


/**************************************************************/
// Date: 15 Dec 2017
// Function: CupLight::CupLight
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
CupLight::CupLight() : m_id(0)
{

}

/**************************************************************/
// Date: 15 Dec 2017
// Function: CupLight::CupLight
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return:
/**************************************************************/
CupLight::CupLight(uint8_t id) : m_id(id)
{
    uint8_t cupIdItr = 0;

    // Iterate through all the lights and initialize them
    for(Ws2812Led& led : m_WsLeds)
    {
        led.setId(cupIdItr);
        led.setRgbValues(0, 0, LED_RGB_MAX_VAL);
        cupIdItr++;
    }
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: CupLight::setId
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return: fhdrvoid
/**************************************************************/
void CupLight::setId(uint8_t id)
{
    m_id = id;
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: CupLight::getId
// Description: TODO
// Inputs:
// Output: TODO
// Return: uint8_t
/**************************************************************/
uint8_t CupLight::getId()
{
    return m_id;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: CupLight::getWsRef
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return: Ws2812Led*
/**************************************************************/
Ws2812Led* CupLight::getWsRef(uint8_t id)
{
    Ws2812Led* ptrWsLed = NULL;

    if(id < LEDS_PER_CUP)
    {
        ptrWsLed = &(m_WsLeds[id]);
    }

    return ptrWsLed;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::LightController
// Description: TODO
// Inputs: uint8_t
// Output: TODO
// Return:
/**************************************************************/
LightController::LightController(uint8_t dataPin)
#ifndef UNIT_TESTING
: m_ptrNeoPixelStrip(NULL)
#endif
{
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip = new Adafruit_NeoPixel((LEDS_PER_CUP * NUM_OF_CUPS), dataPin, NEO_GRB + NEO_KHZ800);
    m_ptrNeoPixelStrip->begin();
#endif

    // Initialize all the cups
    for(unsigned int i = 0; i < NUM_OF_CUPS; i++)
    {
        m_cupLights[i] = CupLight(i);

        // Set the color
        for(unsigned int j = 0; j < LEDS_PER_CUP; j++)
        {
            setNeoPixelLight(m_cupLights[i].getWsRef(j), i);
        }
    }

#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip->show();
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
LightController::~LightController()
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
int LightController::read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int retVal = BJ_SUCCESS;
    uint8_t cupId = 0;

    if((ptrBuffer == NULL) || (ptrBytesWritten == NULL))
    {
#ifdef DEBUG
        BJBP_LOG_ERR("Invalid input buffer or bytes written\n");
#endif
        retVal = BJ_FAILURE;
    }

    if(retVal == BJ_SUCCESS)
    {
        cupId = ptrBuffer[0];

        // Check if it's a get all request or just a single request
        if(cupId == RGB_GET_ALL_VALUES_REQ)
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
// Description: TODO
// Inputs: uint8_t*, uint8_t
// Output: TODO
// Return: fhdrint
/**************************************************************/
int LightController::write(uint8_t* ptrPayload, uint8_t payloadSize)
{
    int retVal = BJ_SUCCESS;
    uint8_t cupId = 0;
    uint8_t numOfLeds = payloadSize / RGB_WS_LED_SIZE;
    uint8_t baseIdx = 0;

   // BJBP_LOG_ERR("received data to Light Controll\r");

    // Retreive the values from the buffer
    if(((payloadSize % RGB_WS_LED_SIZE) > 0) || (payloadSize == 0) || (ptrPayload == NULL))
    {
#ifdef DEBUG
        BJBP_LOG_ERR("Payloadsize exceeds maximum LEDs or input is NULL");
#endif
        retVal = BJ_FAILURE;
    }
    else
    {
        for(int i = 0; i < numOfLeds; i++)
        {
            baseIdx = RGB_WS_LED_SIZE * i;

            // Get the RGB Reference
            // NOTE: Only 1 color is allowed per WS right now.
            cupId = ptrPayload[baseIdx];

            if(cupId == RGB_GET_ALL_VALUES_REQ)
            {
                retVal = BJ_FAILURE;
            }
            else
            {
                for(unsigned int j = 0; j < LEDS_PER_CUP; j++)
                {
                    Ws2812Led* ptrWsLed = m_cupLights[cupId].getWsRef(j);

                    ptrWsLed->setRgbValues(ptrPayload[baseIdx + RGB_RED_BUF_POS], ptrPayload[baseIdx + RGB_GREEN_BUF_POS],
                            ptrPayload[baseIdx + RGB_BLUE_BUF_POS]);

                    // Set the physical value of the neo pixel led
                    setNeoPixelLight(ptrWsLed, m_cupLights[cupId].getId());
                }
            }

            // DEBUG
#ifdef DEBUG
            BJBP_LOG_INFO("Cup %u values R[%u] G[%u] B[%u] \n",
                cupId, ptrPayload[baseIdx + RGB_RED_BUF_POS], ptrPayload[baseIdx + RGB_GREEN_BUF_POS],
                ptrPayload[baseIdx + RGB_BLUE_BUF_POS]);
#endif
        }

        // Set the LED values on the output
#ifndef UNIT_TESTING
        m_ptrNeoPixelStrip->show();
#endif
    }

    return retVal;
}

/**************************************************************/
// Date: 15 Dec 2017
// Function: LightController::setNeoPixelLight
// Description: TODO
// Inputs: Ws2812Led*
// Output: TODO
// Return: void
/**************************************************************/
void LightController::setNeoPixelLight(Ws2812Led* wsLed, uint8_t cupId)
{
    // Set the start ring buffer. This is seen as an array of lights starting from cup 0.
    // Note: CupId of cups are starting from 1 why 1 must be substracted
    uint16_t wsLedStartIdx = (LEDS_PER_CUP * cupId) + wsLed->getId() ;

    // Set neo pixel lights
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip->setPixelColor(wsLedStartIdx, wsLed->getRedVal(), wsLed->getGreenVal(), wsLed->getBlueVal());
#endif
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: LightController::getSingleRgbVal
// Description: TODO
// Inputs: uint8_t*, uint8_t*
// Output: TODO
// Return: int16_t
/**************************************************************/
int16_t LightController::getSingleRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int16_t retVal = BJ_SUCCESS;
    uint8_t cupId = ptrBuffer[RGB_ID_BUF_POS];

    if(cupId > (NUM_OF_CUPS - 1))
    {
        retVal = BJ_FAILURE;
#ifdef DEBUG
        BJBP_LOG_ERR("Invalid cup ID: %i\n", cupId);
#endif
    }
    else
    {
        // Set the RGB values starting from index 1.
        Ws2812Led* ptrWsLed = m_cupLights[cupId].getWsRef(0);
        ptrBuffer[RGB_RED_BUF_POS]   = ptrWsLed->getRedVal();
        ptrBuffer[RGB_GREEN_BUF_POS] = ptrWsLed->getGreenVal();
        ptrBuffer[RGB_BLUE_BUF_POS]  = ptrWsLed->getBlueVal();

        // Set the bytes written to the size of all thre RGB values and the index
        *ptrBytesWritten = RGB_WS_LED_SIZE;
    }

    return retVal;
}

/**************************************************************/
// Date: 30 Dec 2017
// Function: LightController::getAllRgbVal
// Description: TODO
// Inputs: uint8_t*, uint8_t*
// Output: TODO
// Return: int16_t
/**************************************************************/
int16_t LightController::getAllRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int16_t retVal = BJ_SUCCESS;
    Ws2812Led* ptrWsLed = NULL;
    uint8_t baseIdx = 0;

    for(int i = 0; i < NUM_OF_CUPS; i++)
    {
        // Calculate the base index
        baseIdx = RGB_WS_LED_SIZE * i;

        // Get the WS light ref. Individual setting is not yet supported.
        ptrWsLed = m_cupLights[i].getWsRef(0);

        // Set the buffer value
        ptrBuffer[baseIdx]                     = m_cupLights[i].getId();
        ptrBuffer[RGB_RED_BUF_POS + baseIdx]   = ptrWsLed->getRedVal();
        ptrBuffer[RGB_GREEN_BUF_POS + baseIdx] = ptrWsLed->getGreenVal();
        ptrBuffer[RGB_BLUE_BUF_POS + baseIdx]  = ptrWsLed->getBlueVal();
    }

    // Set the bytes written to the size of all thre RGB values and the index
    *ptrBytesWritten = RGB_WS_LED_SIZE * NUM_OF_CUPS;

    return retVal;
}

};

