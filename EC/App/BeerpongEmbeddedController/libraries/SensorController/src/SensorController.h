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
#include "IRestController.h"
#include "BjConfiguration.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
namespace Controller
{

const uint8_t NUM_OF_SENSORS = NUM_OF_CUPS; //(BITS_PER_SHIFT_REG * NUM_OF_SHIFT_REG <- Actual possible sensor input;

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//
enum class CupSide
{
	E_RED_SIDE 	= 0x01,
	E_BLUE_SIDE = 0x02
};

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// CLASS
//-------------------------------------------------------------------------------//
class SensorController : public IRestController
{
public:
    SensorController(uint8_t clkPin, uint8_t clkInhPin, uint8_t sdLdPin,
            uint8_t serPin);
    virtual ~SensorController();

    virtual int read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);
    virtual int write(uint8_t* ptrPayload, uint8_t payloadSize) {return 0;}

    //uint8_t readNumOfHighBits(CupSide eCupSide);

#ifdef UNIT_TESTING
    uint8_t testSensorReadings[NUM_OF_SENSORS];

public:
#else
private:
#endif
    uint8_t m_numOfShiftReg;

    //uint8_t getNumOfHighBits(uint8_t ui8Data);
    bool readRawBits(uint8_t* payload);
};

};

#endif /* SENSORCONTROLLER_H_ */
