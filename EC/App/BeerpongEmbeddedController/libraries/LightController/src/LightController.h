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
#include "NetworkController.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define defNUM_OF_WS2812_PER_CUP ((uint8_t) 16)

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

/// Structure of the value of each RGB LED 
typedef struct 
{
	uint8_t ui8RedValue;
	uint8_t ui8GreenValue;
	uint8_t ui8BlueValue;
} stLedValues_t;

/// Structure of each WS2812 RGB LED
typedef struct 
{
	uint8_t ui8Id;

	stLedValues_t stLedValues;
} stWs2812Led_t;

/// Structure of each Beerpong cup containg the 
/// WS2812 LEDs
typedef struct 
{
	// Id of the cup
	uint8_t ui8Id;

	// WS2812 elements on WS2812 ring.
	stWs2812Led_t stWs2812Leds[defNUM_OF_WS2812_PER_CUP];
} stCupLights_t;



//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
int16_t 	   LightController_Init(uint8_t ui8DataPin);
void		   LightController_SetCupLed(uint8_t ui8CupId, uint8_t ui8Ws2812Id, stLedValues_t *stLedValues);
stLedValues_t  LightController_GetCupLed(uint8_t ui8CupId, uint8_t ui8Ws2812Id);
stCupLights_t* LightController_GetCupLightPtr(uint8_t ui8CupId);
void 		   LightController_ReceiveDataPacket(stPackage* package);

#endif /* LIGHTCONTROLLER_H_ */
