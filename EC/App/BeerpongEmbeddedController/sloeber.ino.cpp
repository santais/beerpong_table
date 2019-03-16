#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2019-03-16 09:27:50

#include "Arduino.h"
#include "stdint.h"
#include <TimerOne.h>
#include "sn74hc165.h"
#include "SensorController.h"
#include "LightController.h"
#include "SystemController.h"
#include "UARTMessageController.h"
#include "Logger.h"
#include "BjDataPackageDefines.h"
#include "BjConfiguration.h"
#include "UnitController.h"
#include "EdgeLightController.h"

void aliveLedISR(void) ;
void setup() ;
void loop() ;
void testSerialInput() ;
void printSensorData(uint8_t* sensorData, uint8_t numOfSensors) ;

#include "BeerpongEmbeddedController.ino"


#endif
