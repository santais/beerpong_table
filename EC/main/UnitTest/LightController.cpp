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
const uint8_t LED_RGB_MAX_VAL           = 255u;
const uint8_t RGB_BUF_START_INDEX       = 1u;

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
    for(Ws2812Led led : m_WsLeds)
    {
        led.setId(cupIdItr);
        led.setRgbValues(LED_RGB_MAX_VAL, 0, 0);
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
{
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip = new Adafruit_NeoPixel(LEDS_PER_CUP * NUM_OF_CUPS, dataPin, NEO_GRB + NEO_KHZ800);
    m_ptrNeoPixelStrip->begin();
#endif

    // Initialize all the cups
    for(unsigned int i = 0; i < NUM_OF_CUPS; i++)
    {
        m_cupLights[i] = CupLight(i);
    }
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
int LightController::handleGet(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int retVal = -1;
    uint8_t cupId = 0;

    if((ptrBuffer != NULL) && (ptrBytesWritten != NULL))
    {
        retVal = 0;
        cupId = ptrBuffer[0];

        // Check the validity of the cup ID
        if(cupId > (NUM_OF_CUPS - 1))
        {
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Invalid cup ID %i", cupId);
#endif
            retVal = -1;
        }
        else
        {
            // Set the RGB values starting from index 1
            Ws2812Led* ptrWsLed = m_cupLights[cupId].getWsRef(retVal);
            ptrBuffer[RGB_BUF_START_INDEX] = ptrWsLed->getRedVal();
            ptrBuffer[RGB_BUF_START_INDEX + 1] = ptrWsLed->getGreenVal();
            ptrBuffer[RGB_BUF_START_INDEX + 2] = ptrWsLed->getBlueVal();

            // Set the bytes written to the size of all thre RGB values and the index
            *ptrBytesWritten = RGB_WS_LED_SIZE;
        }
    }
    else
    {
#ifndef UNIT_TESTING
        BJBP_LOG_ERR("Invalid buffer input");
#endif
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
int LightController::handlePut(uint8_t* ptrPayload, uint8_t payloadSize)
{
    int retVal = 0;
    uint8_t cupId = 0;
    uint8_t numOfLeds = payloadSize / RGB_WS_LED_SIZE;

    // Retreive the values from the buffer
    if(((payloadSize % RGB_WS_LED_SIZE) > 0) || (payloadSize == 0))
    {
#ifndef UNIT_TESTING
        BJBP_LOG_ERR("Payloadsize exceeds maximum LEDs");
#endif
        retVal = -1;
    }
    else
    {
        for(int i = 0; i < numOfLeds; i++)
        {
            // Get the RGB Reference
            // NOTE: Only 1 color is allowed per WS right now.
            cupId = ptrPayload[(i * RGB_WS_LED_SIZE)];

            for(unsigned int j = 0; j < LEDS_PER_CUP; j++)
            {
                Ws2812Led* ptrWsLed = m_cupLights[cupId].getWsRef(j);

                ptrWsLed->setRgbValues(ptrPayload[(i * RGB_WS_LED_SIZE) + 1], ptrPayload[(i * RGB_WS_LED_SIZE) + 2],
                        ptrPayload[(i * RGB_WS_LED_SIZE) + 3]);

                // Set the physical value of the neo pixel led
                setNeoPixelLight(ptrWsLed);
            }

            // DEBUG
#ifndef UNIT_TESTING
            BJBP_LOG_INFO("Cup %u values R[%u] G[%u] B[%u] \n",
                cupId, ptrPayload[(i * RGB_WS_LED_SIZE) + 1], ptrPayload[(i * RGB_WS_LED_SIZE) + 2],
                ptrPayload[(RGB_WS_LED_SIZE) + 3]);
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
void LightController::setNeoPixelLight(Ws2812Led* wsLed)
{
    // Set the start ring buffer. This is seen as an array of lights starting frmo cup 0.
    uint16_t wsLedStartIdx = wsLed->getId() * LEDS_PER_CUP;

    // Set neo pixel lights
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip->setPixelColor(wsLedStartIdx, wsLed->getRedVal(), wsLed->getGreenVal(), wsLed->getBlueVal());
#endif
}

};

