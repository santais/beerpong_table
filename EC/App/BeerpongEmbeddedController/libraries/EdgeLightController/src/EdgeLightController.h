//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef EDGELIGHTCONTROLLER_H_
#define EDGELIGHTCONTROLLER_H_


//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include <stdint.h>

#include "IRestController.h"
#include "BjDataPackageDefines.h"
#include "BjConfiguration.h"
#ifndef UNIT_TESTING
#include "Adafruit_NeoPixel.h"
#include "Adafruit_PWMServoDriver.h"
#endif

namespace Controller
{

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//

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

class Ws2812Led;

class EdgeLightController : public IRestController
{
public:
    EdgeLightController(uint8_t i2cAddr, float freq);
    virtual ~EdgeLightController();

    virtual int read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);
    virtual int write(uint8_t* ptrPayload, uint8_t payloadSize);
    virtual int handlePost(uint8_t* ptrPayload, uint8_t payloadSize) {return 0;}
    virtual int handleDelete(uint8_t* ptrPayload, uint8_t payloadSize) {return 0;}

#ifndef UNIT_TESTING
private:
#else
public:
#endif

    uint16_t calcPwmOnTime(uint8_t rgbVal);
    int16_t setRgbLight(Ws2812Led* ptrRgbLight, uint16_t* rgbPwmBuffer);
    int16_t setPwm(uint8_t channelID, uint16_t* pwmValues, uint8_t numOfPwms);
    int16_t getSingleRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);
    int16_t getAllRgbVal(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);

#ifndef UNIT_TESTING
    Adafruit_PWMServoDriver m_pwmDriver;
#endif

    Ws2812Led* m_rgbLeds[NUM_EDGE_LEDS];

};

};

#endif /* EDGELIGHTCONTROLLER_H_ */
