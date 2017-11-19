//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef SENSORCONTROLLER_H_
#define SENSORCONTROLLER_H_

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include "../../NetworkController/src/NetworkController.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//
typedef enum 
{
	E_RED_SIDE 	= 0x01,
	E_BLUE_SIDE = 0x02
} eCupSide_t;

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//
typedef struct 
{
	uint8_t ui8ClkPin;
	uint8_t ui8ClkInhPin;
	uint8_t ui8SdLdPin;
	uint8_t ui8SerPin;
} stShiftRegPins_t;

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
int16_t SensorController_Init(stShiftRegPins_t *stShiftRegPins);
uint8_t SensorController_ReadActiveSensor(eCupSide_t eCupSide);
void	SensorController_ReadRawValues(uint8_t *pui8Buffer, uint8_t ui8BufSize);
void 	SensorController_ReceiveDataPacket(stPackage* package);

#endif /* SENSORCONTROLLER_H_ */