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

/** WS2812LED **/

Ws2812Led::Ws2812Led() : m_id(0), RgbLeds(0, 0, 0)
{

}

Ws2812Led::Ws2812Led(uint8_t id, uint8_t redVal, uint8_t greenVal, uint8_t blueVal) :
    m_id(id), RgbLeds(redVal, greenVal, blueVal)
{}

uint8_t Ws2812Led::getId()
{
    return m_id;
}

void Ws2812Led::setId(int id)
{
    m_id = id;
}

void Ws2812Led::setRgbValues(uint8_t redVal, uint8_t greenVal, uint8_t blueVal)
{
    m_redval = redVal;
    m_greenVal = greenVal;
    m_blueVal = blueVal;
}

/** CupLight **/

CupLight::CupLight() : m_id(0)
{

}

CupLight::CupLight(uint8_t id) : m_id(id)
{
    uint8_t cupIdItr = 0;

    for(Ws2812Led& led : m_WsLeds)
    {
        led.setId(cupIdItr);
        led.setRgbValues(0, LED_RGB_MAX_VAL, 0);
        cupIdItr++;
    }
}

void CupLight::setId(uint8_t id)
{
    m_id = id;
}

uint8_t CupLight::getId()
{
    return m_id;
}

Ws2812Led* CupLight::getWsRef(uint8_t id)
{
    Ws2812Led* ptrWsLed = NULL;

    if(id < LEDS_PER_CUP)
    {
        ptrWsLed = &(m_WsLeds[id]);
    }

    return ptrWsLed;
}

/** LightController **/

LightController::LightController(uint8_t dataPin)
#ifndef UNIT_TESTING
: m_ptrNeoPixelStrip(NULL)
#endif
{
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip = new Adafruit_NeoPixel((LEDS_PER_CUP * NUM_OF_CUPS), dataPin, NEO_GRB + NEO_KHZ800);
    m_ptrNeoPixelStrip->begin();
#endif

    for(unsigned int i = 0; i < NUM_OF_CUPS; i++)
    {
        m_cupLights[i] = CupLight(i);

        for(unsigned int j = 0; j < LEDS_PER_CUP; j++)
        {
            setNeoPixelLight(m_cupLights[i].getWsRef(j), i);
        }
    }

#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip->show();
#endif
}

LightController::~LightController()
{

}

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
        cupId = ptrBuffer[RGB_ID_BUF_POS];

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

int LightController::write(uint8_t* ptrPayload, uint8_t payloadSize)
{
    int retVal = BJ_SUCCESS;
    uint8_t cupId = 0;
    uint8_t numOfLeds = payloadSize / RGB_WS_LED_SIZE;
    uint8_t baseIdx = 0;

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

                    setNeoPixelLight(ptrWsLed, m_cupLights[cupId].getId());
                }
            }

#ifdef DEBUG
            BJBP_LOG_INFO("Cup %u values R[%u] G[%u] B[%u] \n",
                cupId, ptrPayload[baseIdx + RGB_RED_BUF_POS], ptrPayload[baseIdx + RGB_GREEN_BUF_POS],
                ptrPayload[baseIdx + RGB_BLUE_BUF_POS]);
#endif
        }

#ifndef UNIT_TESTING
        m_ptrNeoPixelStrip->show();
#endif
    }

    return retVal;
}

void LightController::runCupRemovedSequence(uint8_t cupId)
{
  Ws2812Led wsLed(0, 0, 0, 255);

  for (int wsLedIdx = 0; wsLedIdx < LEDS_PER_CUP; wsLedIdx++) {
      wsLed.setId(wsLedIdx);
      setNeoPixelLight(&wsLed, cupId);
      m_ptrNeoPixelStrip->show();
      delay(50);
  }

  /*for (int cycle = 0; cycle < 2; cycle++) {
    for (int ledIdx = 0; ledIdx < 3; ledIdx++) {
        for (uint16_t pixelIdx = 0; pixelIdx < LEDS_PER_CUP; pixelIdx = pixelIdx + 3) {
            m_ptrNeoPixelStrip->setPixelColor(startLedIdx + ledIdx, wheel( (startLedIdx + cycle) % 255));    //turn every third pixel on
        }

      m_ptrNeoPixelStrip->show();

      delay(100);

      for (uint16_t pixelIdx = 0; pixelIdx < m_ptrNeoPixelStrip->numPixels(); pixelIdx = pixelIdx + 3) {
        m_ptrNeoPixelStrip->setPixelColor(startLedIdx + ledIdx, 0);
      }
    }
  }*/
}

void LightController::setNeoPixelLight(Ws2812Led* wsLed, uint8_t cupId)
{
    // Set the start ring buffer. This is seen as an array of lights starting from cup 0.
    uint16_t wsLedStartIdx = (LEDS_PER_CUP * cupId) + wsLed->getId() ;

    // Set neo pixel lights
#ifndef UNIT_TESTING
    m_ptrNeoPixelStrip->setPixelColor(wsLedStartIdx, wsLed->getRedVal(), wsLed->getGreenVal(), wsLed->getBlueVal());
#endif
}

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
        Ws2812Led* ptrWsLed = m_cupLights[cupId].getWsRef(0);
        ptrBuffer[RGB_RED_BUF_POS]   = ptrWsLed->getRedVal();
        ptrBuffer[RGB_GREEN_BUF_POS] = ptrWsLed->getGreenVal();
        ptrBuffer[RGB_BLUE_BUF_POS]  = ptrWsLed->getBlueVal();

        *ptrBytesWritten = RGB_WS_LED_SIZE;
    }

    return retVal;
}

int16_t LightController::getAllRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int16_t retVal = BJ_SUCCESS;
    Ws2812Led* ptrWsLed = NULL;
    uint8_t baseIdx = 0;

    for(int i = 0; i < NUM_OF_CUPS; i++)
    {
        baseIdx = RGB_WS_LED_SIZE * i;

        // TODO: Get the WS light ref. Individual setting is not yet supported.
        ptrWsLed = m_cupLights[i].getWsRef(0);

        ptrBuffer[baseIdx]                     = m_cupLights[i].getId();
        ptrBuffer[RGB_RED_BUF_POS + baseIdx]   = ptrWsLed->getRedVal();
        ptrBuffer[RGB_GREEN_BUF_POS + baseIdx] = ptrWsLed->getGreenVal();
        ptrBuffer[RGB_BLUE_BUF_POS + baseIdx]  = ptrWsLed->getBlueVal();
    }

    *ptrBytesWritten = RGB_WS_LED_SIZE * NUM_OF_CUPS;

    return retVal;
}

void LightController::runLedTestProgram()
{
    setTestProgramColor(0, LED_RGB_MAX_VAL, 0);
    setTestProgramColor(0, 0, LED_RGB_MAX_VAL);
    setTestProgramColor(LED_RGB_MAX_VAL, LED_RGB_MAX_VAL, LED_RGB_MAX_VAL);

    rainbowCycle(5, 500);
    setTestProgramColor(0, LED_RGB_MAX_VAL, 0);
}

void LightController::setTestProgramColor(uint8_t redColor, uint8_t greenColor, uint8_t blueColor)
{
    for(unsigned int wsLedId = 0; wsLedId < LEDS_PER_CUP; wsLedId++)
    {
        for(unsigned int cupId = 0; cupId < NUM_OF_CUPS; cupId++)
        {
            Ws2812Led wsLed(wsLedId, redColor, greenColor, blueColor);
            setNeoPixelLight(&wsLed, cupId);
        }

        m_ptrNeoPixelStrip->show();

        delay(100);
    }

}


// Slightly different, this makes the rainbow equally distributed throughout
void LightController::rainbowCycle(uint8_t wait, uint16_t cycles) {
  uint16_t i, j;

  for(j = 0; j < cycles; j++) {
    for(i = 0; i < m_ptrNeoPixelStrip->numPixels(); i++) {
        m_ptrNeoPixelStrip->setPixelColor(i, wheel(((i * 256 / m_ptrNeoPixelStrip->numPixels()) + j) & 255));
    }

    m_ptrNeoPixelStrip->show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t LightController::wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return m_ptrNeoPixelStrip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return m_ptrNeoPixelStrip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return m_ptrNeoPixelStrip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


};

