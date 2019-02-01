//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//  PROTOCOL:
//
//	    1 Byte         4 bytes            1 Bytes              4 bytes            1 Bytes     	   2^(8) max      1 byte
//  |    SOF    |     32-bit CRC     | HEADER OPTIONS    |     IP ADDR     |    PAYLOAD SIZE    |    PAYLOAD     |    EOF    |
//
// HEADER OPTIONS: Lower 4 bits: Header Module Option. 
// 				   Upper 4 bits: Header Module Target.
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES
//-------------------------------------------------------------------------------//
// System includes
#include <string.h>

#include "UnitController.h"
#include "SensorController.h"
#include "SystemController.h"
#include "LightController.h"

#ifndef UNIT_TESTING
#include "Logger.h"
#else
#include <math.h>
#include <string.h>
#include <iostream>
#endif

using namespace BjDataPackageDefines;

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
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

UnitController::UnitController(IMessageController& messageCtrl) : m_ptrMessageCtrl(&messageCtrl)
{
    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        m_controllerObjs[i] = NULL;
    }

    memset(m_sensorValueShiftReg, 0x0, NUM_OF_SENSORS);
    memset(m_sensorBinaryValues, 0x0, SENSOR_READINGS_REQUIRED);
}

UnitController::~UnitController()
{

}

bool UnitController::run(bool initRun)
{
    bool retVal = readAndSetSensorInput();
    if(retVal || initRun)
    {
        retVal = handleSensorUpdate(m_sensorValueShiftReg, NUM_OF_CUPS, initRun);
    }

    retVal &= checkInputData();

    return retVal;
}

bool UnitController::addController(IRestController& restController, TargetModule module)
{
    bool retVal = false;

    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        if(m_controllerObjs[i] == NULL)
        {
            m_controllerObjs[i] = new ControllerObj(restController, module);

#ifdef DEBUG
            BJBP_LOG_INFO("Added controller ID %i\n", module);
#endif

            retVal = true;
            break;
        }
    }

    return retVal;
}


IRestController* UnitController::getRestCtrlRef(TargetModule targetModule)
{
    IRestController* ptrRestCtrl = NULL;

    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        if(m_controllerObjs[i] != NULL)
        {
            if(targetModule == m_controllerObjs[i]->getTargetModule())
            {
                ptrRestCtrl = m_controllerObjs[i]->getRestCtrlRef();
                break;
            }
        }
    }

    return ptrRestCtrl;
}

bool UnitController::sendPkgContentToController(BjDataPackage& bjDataPkg)
{
    bool retVal = false;
    IRestController* ptrRestCtrl = NULL;
    uint8_t bytesWritten = 0;

    ptrRestCtrl = getRestCtrlRef(bjDataPkg.getTargetModule());

    if(ptrRestCtrl != NULL)
    {
        retVal = true;
        RestRequest restReqest = bjDataPkg.getRestRequest();
        switch(restReqest)
        {
        case RestRequest::E_GET_REQUEST:
            ptrRestCtrl->read(bjDataPkg.getPayload(NULL), &bytesWritten);

            if(bytesWritten < 1)
            {
#ifdef DEBUG
                BJBP_LOG_ERR("Failed to GET data");
#endif
                retVal = false;
            }
            break;
        case RestRequest::E_PUT_REQUEST:
            ptrRestCtrl->write(bjDataPkg.getPayload(NULL), bjDataPkg.getRawPayloadSize());
            break;
        default:
#ifdef DEBUG
            BJBP_LOG_ERR("Unsupported REST command");
#endif
            retVal = false;
            break;
        }
    }

    return retVal;

}

void UnitController::returnPingRequest(BjDataPackage& bjDataPkg)
{
    uint8_t ptrPayload = ARDUINO_CTRL_ID;
    bjDataPkg.setPayload(&ptrPayload, 1);

    if(m_ptrMessageCtrl->write(bjDataPkg.getRawPayload(), bjDataPkg.getRawPayloadSize()) < (int) bjDataPkg.getRawPayloadSize())
    {
#ifdef DEBUG
        BJBP_LOG_ERR("Failed to return ping request");
#endif
    }
}

bool UnitController::checkInputData()
{
    bool retVal = true;
    uint16_t dataBytesRead = 0;

    // Check if there is any data to read
#ifndef UNIT_TESTING
    dataBytesRead = m_ptrMessageCtrl->dataAvailable();
    if(dataBytesRead)
    {
        dataBytesRead = m_ptrMessageCtrl->readBytes(&m_tmpPkgBuffer[0], dataBytesRead);
    }

    if(dataBytesRead)
    {
#endif
        BjDataPackage bjDataPackage;
        bjDataPackage.setPkgFromRawPayload(m_tmpPkgBuffer, dataBytesRead);

        retVal = sendPkgContentToController(bjDataPackage);
#ifndef UNIT_TESTING
    }
#endif
    return retVal;
}

bool UnitController::readAndSetSensorInput()
{
    bool retVal = false;
    bool sensorUpdate = false;

    IRestController* ptrSensorCtrl = getRestCtrlRef(TargetModule::E_MODULE_SENSOR);
    if(ptrSensorCtrl != NULL)
    {
        uint8_t tmpSensorReadings[NUM_OF_SHIFT_REG] = {};
        uint8_t bytesWritten = 0;
        if(ptrSensorCtrl->read(tmpSensorReadings, &bytesWritten) < 0)
        {
#ifdef DEBUG
            BJBP_LOG_ERR("Failed to get sensor readings");
#endif
        }
        else if(bytesWritten < NUM_OF_SHIFT_REG)
        {
#ifdef DEBUG
            BJBP_LOG_ERR("Incorrect number of sensors read: %i", bytesWritten);
#endif
        }
        else
        {
            for(int i = 0; i < NUM_OF_SHIFT_REG; i++)
            {
  //              Serial.println(m_sensorValueShiftReg[i]);
   //             delay(1000);
                if(m_sensorValueShiftReg[i] != tmpSensorReadings[i])
                {
                    sensorUpdate = true;
                }
            }

            if(sensorUpdate)
            {
#ifdef DEBUG
                BJBP_LOG_INFO("New sensor readings! \n");
#endif
                retVal = true;
                memcpy(m_sensorValueShiftReg, tmpSensorReadings, NUM_OF_SHIFT_REG);
            }
        }
    }
    else
    {
#ifdef DEBUG
      BJBP_LOG_ERR("Sensor controller not found");
#endif
        retVal = false;
    }

    return retVal;
}

bool UnitController::handleSensorUpdate(uint8_t* sensorReadings, uint8_t numOfSensors, bool initRun)
{
    bool retVal = true;

    IRestController* ptrSystemCtrl = getRestCtrlRef(TargetModule::E_MODULE_SYSTEM);
    if(ptrSystemCtrl != NULL)
    {
        uint8_t systemState = 0;

        if(ptrSystemCtrl->read(&systemState, NULL) < 0)
        {
#ifdef DEBUG
            BJBP_LOG_ERR("Failed to get system state");
#endif
            retVal = false;
        }
        else
        {
            if(static_cast<ControllerState>(systemState) == ControllerState::E_STATE_AUTOMATIC)
            {
               retVal = setLightsFromSensorVals(sensorReadings, numOfSensors, initRun);
            }
        }
    }
    else
    {
#ifdef DEBUG
        BJBP_LOG_ERR("System Controller not found");
#endif
        retVal = false;
    }

    return retVal;
}

bool UnitController::setLightsFromSensorVals(uint8_t* sensorReadings, uint8_t numOfSensors, bool initRun)
{
    bool retVal = true;
    uint8_t baseIdx = 0;

    IRestController* ptrLightCtrl = getRestCtrlRef(TargetModule::E_MODULE_LIGHT);
    if((ptrLightCtrl != NULL) && (numOfSensors == NUM_OF_CUPS))
    {
        static char sensorBinaryValues[SENSOR_READINGS_REQUIRED] = {};
        convertByteToBinary(sensorReadings, sensorBinaryValues, SHIFT_REG_REQUIRED);


        Serial.print("Sensor member: ");
        for(int cupId = 0; cupId < numOfSensors; cupId++)
        {
            Serial.print((int) m_sensorBinaryValues[cupId]);
        }

        Serial.println();

        Serial.print("Sensor new   : ");
        for(int cupId = 0; cupId < numOfSensors; cupId++)
        {
            Serial.print((int) sensorBinaryValues[cupId]);
        }

        Serial.println();

        delay(100);

        for(int cupId = 0; cupId < numOfSensors; cupId++)
        {
            Ws2812Led wsLed;

            baseIdx = cupId * RGB_WS_LED_SIZE;
            wsLed.setId(cupId);

            if((sensorBinaryValues[cupId] != m_sensorBinaryValues[cupId]) || (initRun))
            {
                if(sensorBinaryValues[cupId])
                {
                    static_cast<LightController*>(ptrLightCtrl)->runCupRemovedSequence(cupId);
                    wsLed.setRgbValues(0, LED_RGB_MAX_VAL, 0);
                }
                else
                {
                    wsLed.setRgbValues(LED_RGB_MAX_VAL, 0, 0);
                }

                m_tmpLightCtrlBuffer[baseIdx] = cupId;
                m_tmpLightCtrlBuffer[baseIdx + RGB_RED_BUF_POS] = wsLed.getRedVal();
                m_tmpLightCtrlBuffer[baseIdx + RGB_GREEN_BUF_POS] = wsLed.getGreenVal();
                m_tmpLightCtrlBuffer[baseIdx + RGB_BLUE_BUF_POS] = wsLed.getBlueVal();
            }
        }

        if(ptrLightCtrl->write(m_tmpLightCtrlBuffer, numOfSensors * RGB_WS_LED_SIZE) == BJ_FAILURE)
        {
    #ifdef DEBUG
        BJBP_LOG_ERR("Failed to transmit new sensor values to the light controller\n");
    #endif
            retVal = false;
        }

        memcpy(m_sensorBinaryValues, sensorBinaryValues, SENSOR_READINGS_REQUIRED);
    }
    else
    {
#ifdef DEBUG
      BJBP_LOG_ERR("Light Controller not found or sensors out of bound %i\n", numOfSensors);
#endif
        retVal = false;
    }

    return retVal;
}

void UnitController::convertByteToBinary(uint8_t* inputBytes, char* outputBinaries, uint8_t byteSize)
{
    for(int i = 0; i < byteSize; i++)
    {
        uint8_t startIndex = i * BITS_PER_SHIFT_REG;
        for(int j = 0; j < BITS_PER_SHIFT_REG; j++)
        {
            outputBinaries[startIndex + j] = ((inputBytes[i] & (1 << j)) != 0);
        }
    }
}

};
