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
#include <string.h>
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

/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::UnitController
// Description: TODO
// Inputs: IMessageController&
// Output: TODO
// Return:
/**************************************************************/
UnitController::UnitController(IMessageController& messageCtrl) : m_ptrMessageCtrl(&messageCtrl)
{
    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        m_controllerObjs[i] = NULL;
    }

    memset(m_sensorValues, 0x0, NUM_OF_SENSORS);
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: fhdUnitController::~UnitController
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
UnitController::~UnitController()
{

}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::run
// Description: TODO
// Inputs:
// Output: TODO
// Return: void
/**************************************************************/
bool UnitController::run()
{
    bool retVal = true;

    // Check for any updates of the Sensor controller
    retVal = checkSensorInput();

    // Check for any new data input
    retVal &= checkInputData();

    return retVal;
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::addController
// Description: TODO
// Inputs: IRestController&, TargetModule
// Output: TODO
// Return: bool
/**************************************************************/
bool UnitController::addController(IRestController& restController, TargetModule module)
{
    bool retVal = false;

    // Find an available slot
    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        if(m_controllerObjs[i] == NULL)
        {
            // Add the new controller
            m_controllerObjs[i] = new ControllerObj(restController, module);

#ifndef UNIT_TESTING
            BJBP_LOG_INFO("Added controller ID %i\n", module);
#endif

            retVal = true;
            break;
        }
    }

    return retVal;
}


/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::getRestCtrlRef
// Description: TODO
// Inputs: TargetModule
// Output: TODO
// Return: IRestController*
/**************************************************************/
IRestController* UnitController::getRestCtrlRef(TargetModule targetModule)
{
    IRestController* ptrRestCtrl = NULL;

    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        if(m_controllerObjs[i] != NULL)
        {
            // Check if the target module is the same
            if(targetModule == m_controllerObjs[i]->getTargetModule())
            {
                ptrRestCtrl = m_controllerObjs[i]->getRestCtrlRef();
                break;
            }
        }
    }

    return ptrRestCtrl;
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::sendPkgContentToController
// Description: TODO
// Inputs: BjDataPackage&
// Output: TODO
// Return: bool
/**************************************************************/
bool UnitController::sendPkgContentToController(BjDataPackage& bjDataPkg)
{
    bool retVal = false;
    IRestController* ptrRestCtrl = NULL;
    uint8_t bytesWritten = 0;

    ptrRestCtrl = getRestCtrlRef(bjDataPkg.getTargetModule());

    // check what REST command that was instantiated
    if(ptrRestCtrl != NULL)
    {
        retVal = true;
        RestRequest restReqest = bjDataPkg.getRestRequest();
        switch(restReqest)
        {
        case RestRequest::E_GET_REQUEST:
            ptrRestCtrl->handleGet(bjDataPkg.getPayload(NULL), &bytesWritten);

            if(bytesWritten < 1)
            {
#ifndef UNIT_TESTING
                BJBP_LOG_ERR("Failed to GET data");
#endif
            }
            break;
        case RestRequest::E_PUT_REQUEST:
            ptrRestCtrl->handlePut(bjDataPkg.getPayload(NULL), bjDataPkg.getRawPayloadSize());
            break;
        default:
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Unsupported REST command");
#endif
            retVal = false;
            break;
        }
    }

    return retVal;

}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UnitController::returnPingRequest
// Description: TODO
// Inputs: BjDataPackage&
// Output: TODO
// Return: void
/**************************************************************/
void UnitController::returnPingRequest(BjDataPackage& bjDataPkg)
{
    uint8_t ptrPayload = ARDUINO_CTRL_ID;
    bjDataPkg.setPayload(&ptrPayload, 1);

    if(m_ptrMessageCtrl->write(bjDataPkg.getRawPayload(), bjDataPkg.getRawPayloadSize()) < (int) bjDataPkg.getRawPayloadSize())
    {
#ifndef UNIT_TESTING
        BJBP_LOG_ERR("Failed to return ping request");
#endif
    }
}

/**************************************************************/
// Date: 27 Dec 2017
// Function: UnitController::checkInputData
// Description: TODO
// Inputs:
// Output: TODO
// Return: bool
/**************************************************************/
bool UnitController::checkInputData()
{
    bool retVal = true;
    uint16_t dataBytesRead = 0;

    // Check if there is any data to read
#ifndef UNIT_TESTING
    dataBytesRead = m_ptrMessageCtrl->dataAvailable();
    if(dataBytesRead)
    {
        // Retrieve the data
        dataBytesRead = m_ptrMessageCtrl->readBytes(&m_tmpPkgBuffer[0], dataBytesRead);
    }

    // Check if the data was read successfully. If so, create a BJ package for further processing
    if(dataBytesRead)
    {
#endif
        BjDataPackage bjDataPackage;
        bjDataPackage.setPkgFromRawPayload(m_tmpPkgBuffer, dataBytesRead);

        // Send the package to the corresponding controller
        retVal = sendPkgContentToController(bjDataPackage);
#ifndef UNIT_TESTING
    }
#endif
    return retVal;
}

/**************************************************************/
// Date: 27 Dec 2017
// Function: UnitController::checkSensorInput
// Description: TODO
// Inputs:
// Output: TODO
// Return: bool
/**************************************************************/
bool UnitController::checkSensorInput()
{
    bool retVal = false;
    bool sensorUpdate = false;
    uint8_t newSensorRead = false;

    IRestController* ptrSensorCtrl = getRestCtrlRef(TargetModule::E_MODULE_SENSOR);
    if(ptrSensorCtrl != NULL)
    {
        uint8_t tmpSensorReadings[NUM_OF_SENSORS] = {};
        uint8_t bytesWritten = 0;
        if(ptrSensorCtrl->handleGet(tmpSensorReadings, &bytesWritten) < 0)
        {
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Failed to get sensor readings");
#endif
        }
        else if(bytesWritten < NUM_OF_SENSORS)
        {
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Incorrect number of sensors read: %i", bytesWritten);
#endif
        }
        else
        {
            retVal = true;

            // Compare the readings with the latest values
            for(int i = 0; i < NUM_OF_SENSORS; i++)
            {
                if(m_sensorValues[i] != tmpSensorReadings[i])
                {
                    sensorUpdate = true;
                    break;
                }
            }

            // Handle the sensor update and copy the newly read sensor data into the stored datas
            if(sensorUpdate)
            {
#ifndef UNIT_TESTING
                BJBP_LOG_INFO("New sensor readings! \n");
#endif

                memcpy(m_sensorValues, tmpSensorReadings, NUM_OF_SENSORS);
                retVal = handleSensorUpdate(m_sensorValues, NUM_OF_CUPS - 1);
            }
        }
    }
    else
    {
#ifndef UNIT_TESTING
      BJBP_LOG_ERR("Sensor controller not found");
#endif
        retVal = false;
    }

    return retVal;
}

/**************************************************************/
// Date: 27 Dec 2017
// Function: UnitController::handleSensorUpdate
// Description: TODO
// Inputs:
// Output: TODO
// Return: bool
/**************************************************************/
bool UnitController::handleSensorUpdate(uint8_t* sensorReadings, uint8_t numOfSensors)
{
    bool retVal = true;

    // Check the state of the system controller
    IRestController* ptrSystemCtrl = getRestCtrlRef(TargetModule::E_MODULE_SYSTEM);
    if(ptrSystemCtrl != NULL)
    {
        uint8_t systemState = 0;

        // Get the system state
        if(ptrSystemCtrl->handleGet(&systemState, NULL) < 0)
        {
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Failed to get system state");
#endif
            retVal = false;
        }
        else
        {
            if(static_cast<ControllerState>(systemState) == ControllerState::E_STATE_AUTOMATIC)
            {
               retVal = setLightsFromSensorVals(sensorReadings, numOfSensors);
            }
        }
    }
    else
    {
#ifndef UNIT_TESTING
        BJBP_LOG_ERR("System Controller not found");
#endif
        retVal = false;
    }

    return retVal;
}

/**************************************************************/
// Date: 27 Dec 2017
// Function: UnitController::setLightsFromSensorVals
// Description: TODO
// Inputs: uint8_t*, uint8_t
// Output: TODO
// Return: fhdrbool
/**************************************************************/
bool UnitController::setLightsFromSensorVals(uint8_t* sensorReadings, uint8_t numOfSensors)
{
    bool retVal = true;
    uint8_t baseIdx = 0;

    // Get the Light controller reference if it exists
    IRestController* ptrLightCtrl = getRestCtrlRef(TargetModule::E_MODULE_LIGHT);
    if((ptrLightCtrl != NULL) && (numOfSensors < NUM_OF_CUPS))
    {
        Ws2812Led wsLed;

        // Iterate through all the sensor readings
        for(int i = 0; i < numOfSensors; i++)
        {
            baseIdx = i * RGB_WS_LED_SIZE;
            wsLed.setId(i);

            // Check if the sensor is high or low
            if(sensorReadings[i])
            {
                // Set red MAX value
                wsLed.setRgbValues(255, 0, 0);
            }
            else
            {
                // Set green MAX value
                wsLed.setRgbValues(0, 255, 0);
            }

            // Set the temporary light payload. Idx of light start from index 1 and upwards
            m_tmpLightCtrlBuffer[baseIdx]                     = i + 1;
            m_tmpLightCtrlBuffer[baseIdx + RGB_RED_BUF_POS]   = wsLed.getRedVal();
            m_tmpLightCtrlBuffer[baseIdx + RGB_GREEN_BUF_POS] = wsLed.getGreenVal();
            m_tmpLightCtrlBuffer[baseIdx + RGB_BLUE_BUF_POS]  = wsLed.getBlueVal();
        }

        // Transmit the data to the light controller
        if(ptrLightCtrl->handlePut(m_tmpLightCtrlBuffer, numOfSensors * RGB_WS_LED_SIZE) == BJ_FAILURE)
        {
#ifndef UNIT_TESTING
            BJBP_LOG_ERR("Failed to transmit new sensor values to the light controller\n");
#endif
            retVal = false;
        }
    }
    else
    {
#ifndef UNIT_TESTING
      BJBP_LOG_ERR("Light Controller not found or sensors out of bound %i\n", numOfSensors);
#endif
        retVal = false;
    }

    return retVal;
}

};
