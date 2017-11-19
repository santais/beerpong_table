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
#include "Arduino.h"
#include "SensorController.h"
#include "sn74hc165.h"
#include "Logger.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define defNUM_OF_SHIFT_REG		 ((uint8_t) 4)

#define defUPPPER_SENSOR_MASK	 ((uint8_t) 0x03)

#define defRED_SHIFT_REG_0  ((uint8_t) 0)
#define defRED_SHIFT_REG_1  ((uint8_t) 1)
#define defBLUE_SHIFT_REG_0 ((uint8_t) 2)
#define defBLUE_SHIFT_REG_1 ((uint8_t) 3)

#define defRED_SCORE_VAL_IDX  ((uint8_t) 0)
#define defBLUE_SCORE_VAL_IDX ((uint8_t) 1)

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
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
uint8_t vGetNumOfHighBits(uint8_t ui8Data);

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
int16_t SensorController_Init(stShiftRegPins_t *stShiftRegPins)
{
	// Setup the drive
	SN74HC165_Init(stShiftRegPins->ui8ClkPin, stShiftRegPins->ui8ClkInhPin, 
		stShiftRegPins->ui8SdLdPin, stShiftRegPins->ui8SerPin);
}	

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
uint8_t SensorController_ReadActiveSensor(eCupSide_t eCupSide)
{
	static uint8_t aui8ShiftRegBuf[defNUM_OF_SHIFT_REG];
	uint8_t ui8ActiveSensor = 0;

	SN74HC165_Read(aui8ShiftRegBuf, defNUM_OF_SHIFT_REG);

	if(eCupSide == E_RED_SIDE)
	{
		ui8ActiveSensor = vGetNumOfHighBits(aui8ShiftRegBuf[defRED_SHIFT_REG_0]);
		ui8ActiveSensor += (vGetNumOfHighBits(aui8ShiftRegBuf[defRED_SHIFT_REG_1]) & defUPPPER_SENSOR_MASK);

	}
	else if(eCupSide == E_BLUE_SIDE)
	{
		ui8ActiveSensor = vGetNumOfHighBits(aui8ShiftRegBuf[defBLUE_SHIFT_REG_0]);
		ui8ActiveSensor += (vGetNumOfHighBits(aui8ShiftRegBuf[defBLUE_SHIFT_REG_1]) & defUPPPER_SENSOR_MASK);
	}
	else
	{
		Serial.println("SENSORCONTROLLER - Unknonw cup side");
	}

	return ui8ActiveSensor;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void SensorController_ReadRawValues(uint8_t *pui8Buffer, uint8_t ui8BufSize)
{
	if(ui8BufSize <= defNUM_OF_SHIFT_REG)
	{
		SN74HC165_Read(pui8Buffer, ui8BufSize);
	}
	else
	{
		Serial.printf("SENSORCONTROLLER - Buffer size exceeds maximum size %u\n", defNUM_OF_SHIFT_REG);
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void SensorController_ReceiveDataPacket(stPackage* package)
{
	static uint8_t ui8CupSensorValues[defNUM_OF_SHIFT_REG / 2];

	if(package->header.bitHeader.module == (eHeaderModuleOption_t) E_GET_REQUEST)
	{
		// Create a new package to transmit
		stPackage transferPackage;

		ui8CupSensorValues[defRED_SCORE_VAL_IDX] = SensorController_ReadActiveSensor((eCupSide_t) E_RED_SIDE);
		ui8CupSensorValues[defBLUE_SCORE_VAL_IDX] = SensorController_ReadActiveSensor((eCupSide_t) E_BLUE_SIDE);

		// Transmit the packet
		if(!NetworkComm_CreatePackage(&transferPackage, E_TARGET_SCORE_REQUEST, E_GET_REQUEST, defNUM_OF_SHIFT_REG / 2,
			&ui8CupSensorValues[0], package->ipAddr))
		{
			BJBP_LOG_ERR("Unable to create package in SensorController");
		}
		else
		{
			NetworkComm_TransmitPacket(&transferPackage);
		}
	}
	else
	{
		Serial.println("SENSORCONTROLLER - Invalid header module");
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
uint8_t vGetNumOfHighBits(uint8_t ui8Data)
{
	uint32_t ui32TmpData = (uint32_t) ui8Data;
   	ui32TmpData = ui32TmpData - ((ui32TmpData >> 1) & 0x55555555);
   	ui32TmpData = (ui32TmpData & 0x33333333) + ((ui32TmpData >> 2) & 0x33333333);
   	ui32TmpData = (ui32TmpData + (ui32TmpData >> 4)) & 0x0f0f0f0f;
   	ui32TmpData = ui32TmpData + (ui32TmpData >> 8);
   	ui32TmpData= ui32TmpData + (ui32TmpData >> 16);
   	return ((uint8_t) ui32TmpData & 0x000000FF);
}