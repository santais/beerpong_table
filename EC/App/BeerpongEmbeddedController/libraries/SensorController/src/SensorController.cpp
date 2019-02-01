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
#include "SensorController.h"
#include "BjConfiguration.h"
#include "BjDataPackage.h"
#ifndef UNIT_TESTING
#include "Arduino.h"
#include "Logger.h"
#include "sn74hc165.h"
#else
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#endif

namespace Controller
{
//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//

#define UPPPER_SENSOR_MASK	 ((uint8_t) 0x03)

#define RED_SHIFT_REG_0  ((uint8_t) 0)
#define RED_SHIFT_REG_1  ((uint8_t) 1)
#define BLUE_SHIFT_REG_0 ((uint8_t) 2)
#define BLUE_SHIFT_REG_1 ((uint8_t) 3)

#define RED_SCORE_VAL_IDX  ((uint8_t) 0)
#define BLUE_SCORE_VAL_IDX ((uint8_t) 1)

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

SensorController::SensorController(uint8_t clkPin, uint8_t clkInhPin, uint8_t sdLdPin,
        uint8_t qhPin) :
        m_numOfShiftReg(NUM_OF_SHIFT_REG)
{
    // Setup the setup the C driver
#ifndef UNIT_TESTING
    SN74HC165_Init(clkPin, clkInhPin, sdLdPin, qhPin);
#else
    memset(testSensorReadings, 0x0, NUM_OF_SENSORS);
#endif
}

SensorController::~SensorController()
{

}

int SensorController::read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int retVal = -1;

    if(ptrBuffer != NULL)
    {
        if(readRawBits(ptrBuffer))
        {
            *ptrBytesWritten = m_numOfShiftReg;
            retVal = 0;
        }
    }

    return retVal;
}


bool SensorController::readRawBits(uint8_t* payload)
{
    bool retVal = true;

    // Read the values
#ifndef UNIT_TESTING
    SN74HC165_Read(payload, m_numOfShiftReg);
#else
    memcpy(payload, testSensorReadings, m_numOfShiftReg * BITS_PER_SHIFT_REG);
#endif


    return retVal;
}

// TODO: Move to the Android side
/*uint8_t SensorController::readNumOfHighBits(CupSide eCupSide)
{
	uint8_t ui8ActiveSensor = 0;

#ifndef UNIT_TESTING
	SN74HC165_Read(m_sensorValues, NUM_OF_SHIFT_REG);
#endif

	if(eCupSide == CupSide::E_RED_SIDE)
	{
		ui8ActiveSensor = getNumOfHighBits(m_sensorValues[RED_SHIFT_REG_0]);
		ui8ActiveSensor += (getNumOfHighBits(m_sensorValues[RED_SHIFT_REG_1]) & UPPPER_SENSOR_MASK);

	}
	else if(eCupSide == CupSide::E_BLUE_SIDE)
	{
		ui8ActiveSensor = getNumOfHighBits(m_sensorValues[BLUE_SHIFT_REG_0]);
		ui8ActiveSensor += (getNumOfHighBits(m_sensorValues[BLUE_SHIFT_REG_1]) & UPPPER_SENSOR_MASK);
	}
	else
	{
#ifndef UNIT_TESTING
		Serial.println("SENSORCONTROLLER - Unknonw cup side");
#endif
	}

	return ui8ActiveSensor;
}
*/

// TODO: Move to the Android side
/*uint8_t SensorController::getNumOfHighBits(uint8_t ui8Data)
{
	uint32_t ui32TmpData = (uint32_t) ui8Data;
   	ui32TmpData = ui32TmpData - ((ui32TmpData >> 1) & 0x55555555);
   	ui32TmpData = (ui32TmpData & 0x33333333) + ((ui32TmpData >> 2) & 0x33333333);
   	ui32TmpData = (ui32TmpData + (ui32TmpData >> 4)) & 0x0f0f0f0f;
   	ui32TmpData = ui32TmpData + (ui32TmpData >> 8);
   	ui32TmpData= ui32TmpData + (ui32TmpData >> 16);
   	return ((uint8_t) ui32TmpData & 0x000000FF);
}*/

};
