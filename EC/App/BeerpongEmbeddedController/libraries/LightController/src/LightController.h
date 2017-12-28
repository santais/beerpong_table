//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef LIGHTCONTROLLER_H_
#define LIGHTCONTROLLER_H_


//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include <stdint.h>

#include "IRestController.h"
#include "BjDataPackageDefines.h"
#include "BjConfiguration.h"
#ifndef UNIT_TESTING
#include "Adafruit_NeoPixel.h"
#endif

namespace Controller
{

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
// Light Controller RGB
const uint8_t RGB_WS_LED_SIZE           = 4;
const uint8_t RGB_RED_BUF_POS           = 1;
const uint8_t RGB_GREEN_BUF_POS         = 2;
const uint8_t RGB_ID_BUF_POS            = 0;

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// CLASS
//-------------------------------------------------------------------------------//

//
// RgbLeds
//
class RgbLeds
{
public:
    RgbLeds(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);

protected:
    uint8_t m_redval;
    uint8_t m_greenVal;
    uint8_t m_blueVal;
};

//
// WS28LED CLASS
//
class Ws2812Led : public RgbLeds
{
public:
    Ws2812Led();
    Ws2812Led(uint8_t id, uint8_t redVal = 0, uint8_t greenVal = 0, uint8_t blueVal = 0);

    uint8_t getId();
    void setId(int id);
    void setRgbValues(uint8_t redVal, uint8_t greenVal, uint8_t blueVal);
    uint8_t getRedVal() { return m_redval; }
    uint8_t getGreenVal() { return m_greenVal; }
    uint8_t getBlueVal() { return m_blueVal; }

private:
    uint8_t m_id;
};

//
// CupLight Class
//
class CupLight
{
public:
    CupLight();
    CupLight(uint8_t id);

    void setId(uint8_t id);
    Ws2812Led* getWsRef(uint8_t id);

private:
    Ws2812Led m_WsLeds[LEDS_PER_CUP];

    uint8_t m_id;
};

class LightController : public IRestController
{
public:
    LightController(uint8_t dataPin);
    virtual ~LightController();

    virtual int handleGet(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);
    virtual int handlePut(uint8_t* ptrPayload, uint8_t payloadSize);
    virtual int handlePost(uint8_t* ptrPayload, uint8_t payloadSize) {return 0;}
    virtual int handleDelete(uint8_t* ptrPayload, uint8_t payloadSize) {return 0;}
private:
    void setNeoPixelLight(Ws2812Led* wsLed, uint8_t cupId);

#ifndef UNIT_TESTING
    Adafruit_NeoPixel* m_ptrNeoPixelStrip;
#endif
    CupLight m_cupLights[NUM_OF_CUPS];
};

};

#endif /* LIGHTCONTROLLER_H_ */
