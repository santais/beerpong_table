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

#include "SystemController.h"
#include "BjDataPackage.h"
#include "BjConfiguration.h"
#ifndef UNIT_TESTING
#include "Arduino.h"
#include "Logger.h"
#else
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#endif
namespace Controller
{

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define defSET_REQUEST_BUFFER_SIZE ((uint8_t) 1)

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//
//static eControllerState_t eActiveControllerState;

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// CLASS
//--------------------------------------------------- ----------------------------//

SystemController::SystemController() : m_ctrlState(ControllerState::E_STATE_AUTOMATIC)
{

}

SystemController::~SystemController()
{

}

int SystemController::read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten)
{
    int retVal = -1;

    if(ptrBuffer != NULL)
    {
        retVal = 0;

        ptrBuffer[0] = (uint8_t) m_ctrlState;
    }

    if(ptrBytesWritten != NULL)
    {
        *ptrBytesWritten = 1;
    }

    return retVal;
}

int SystemController::write(uint8_t* ptrPayload, uint8_t payloadSize)
{
    int retVal = 0;

    if(ptrPayload != NULL)
    {
        if(checkValidSetRequest(ptrPayload[0]))
        {
            m_ctrlState = static_cast<ControllerState>(ptrPayload[0]);
        }
        else
        {
#ifdef DEBUG
            BJBP_LOG_ERR("Invalid Controller State");
#endif
            retVal = -1;
        }
    }
    else
    {
#ifdef DEBUG
        BJBP_LOG_ERR("NULL Ptr");
#endif
        retVal = -1;
    }

    return retVal;
}

bool SystemController::checkValidSetRequest(uint8_t ui8ControlState)
{
    bool bReturn = true;

    if(ui8ControlState > (uint8_t) ControllerState::E_STATE_SEQUENCE)
    {
#ifdef DEBUG
        BJBP_LOG_ERR("SC - INVALID SET REQUEST OF SIZE: %u\n", ui8ControlState);
#endif
        bReturn = false;
    }

    return bReturn;
}

}

