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
/*    for(int i = 0; i < MAX_NUM_CONTROLLERS; i++)
    {
        m_controllerObjs[i] = NULL;
    }*/
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
void UnitController::run()
{
    checkInputData();
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
      //  if(m_controllerObjs[i] == NULL)
      //  {
            // Add the new controller
            ControllerObj newControllerObj(restController, module);
            m_controllerObjs[i] = newControllerObj;

#ifndef UNIT_TESTING
            BJBP_LOG_INFO("Added controller target ID %i and i %i and pointer %p \n", m_controllerObjs[i].getTargetModule(), i,
                    &m_controllerObjs[i]);
#endif

            retVal = true;
            break;
        //}
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
      //  if(m_controllerObjs[i] != NULL)
       // {
            if(targetModule == m_controllerObjs[i].getTargetModule())
            {
              //  Serial.println("FTM");
                ptrRestCtrl = m_controllerObjs[i].getRestCtrlRef();
                break;
            }
       // }
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
            ptrRestCtrl->read(bjDataPkg.getPayload(NULL), &bytesWritten);

            if(bytesWritten < 1)
            {
#ifndef UNIT_TESTING
                BJBP_LOG_ERR("Failed to GET data");
#endif
                retVal = false;
            }
            break;
        case RestRequest::E_PUT_REQUEST:
            ptrRestCtrl->handlePut(bjDataPkg.getPayload(NULL), bjDataPkg.getPayloadSize());
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

    while(m_ptrMessageCtrl->dataAvailable()) {
        m_tmpPkgBuffer[dataBytesRead] = m_ptrMessageCtrl->readByte();
        dataBytesRead += 1;
        delay(10);
    }

    // Check if the data was read successfully. If so, create a BJ package for further processing
    if(dataBytesRead > 0)
    {
#endif
      //  BJBP_LOG_INFO("Read %i number of bytes \n", dataBytesRead);

        BjDataPackage bjDataPackage;
        bjDataPackage.setPkgFromRawPayload(&m_tmpPkgBuffer[0], dataBytesRead);

        // Send the package to the corresponding controller
        retVal = sendPkgContentToController(bjDataPackage);
#ifndef UNIT_TESTING
    }
#endif
    return retVal;
}

};
