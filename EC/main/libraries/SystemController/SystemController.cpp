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
#include "SystemController.h"
#include "NetworkController.h"
#include "Logger.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define defSET_REQUEST_BUFFER_SIZE ((uint8_t) 1)

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//
static eControllerState_t eActiveControllerState;

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
static boolean vCheckValidSetRequest(uint8_t ui8ControlState);
static char* getControllerStateAsString(eControllerState_t state);

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
int16_t SystemController_Init(void)
{
	int16_t i16Return = 1;

	eActiveControllerState = E_STATE_AUTOMATIC;

	return i16Return;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void SystemController_SetControllerState(eControllerState_t eControllerState)
{
	eActiveControllerState = eControllerState;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
eControllerState_t SystemController_GetControllerState(void)
{
	return eActiveControllerState;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void SystemController_ReceiveDataPacket(stPackage* package)
{
	static uint8_t ui8TransmitBuffer = 0;

	uint8_t ui8BufIdx = 0;
	eHeaderModuleOption_t eHeaderOption = (eHeaderModuleOption_t) package->header.bitHeader.module;
	if(package->header.bitHeader.module == E_GET_REQUEST)
	{
		// Create a transfer package
		stPackage transferPackage;

		if(!NetworkComm_CreatePackage(&transferPackage, E_TARGET_CONTROLLER_REQUEST, eHeaderOption, 
			defSET_REQUEST_BUFFER_SIZE, (uint8_t*) &eActiveControllerState, package->ipAddr))
		{
			BJBP_LOG_ERR("Unable to create package in SystemController");
		}
		else
		{
			NetworkComm_TransmitPacket(&transferPackage);
		}	
	}	
	else if(package->header.bitHeader.module == E_SET_REQUEST)
	{
		if(package->payloadSize > 1)
		{
			BJBP_LOG_ERR("SC - Buffer size is larger than 1");
		}
		else
		{
			if(vCheckValidSetRequest(package->payload[0]) == true)
			{
				eActiveControllerState = (eControllerState_t) package->payload[0];
				BJBP_LOG_INFO(getControllerStateAsString(eActiveControllerState));

				// Notify all
				package->header.bitHeader.module = E_NOTIFY_ALL_REQUEST;
				NetworkComm_TransmitPacket(package);
			}
		}
	}
	else
	{
		BJBP_LOG_ERR("SC - INVALID HEADER MODULE REQUEST");
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
boolean vCheckValidSetRequest(uint8_t ui8ControlState)
{
	boolean bReturn = true;

	if(ui8ControlState > (uint8_t) E_STATE_SEQUENCE)
	{
		BJBP_LOG_ERR("SC - INVALID SET REQUEST OF SIZE: %u\n", ui8ControlState);
		bReturn = false;
	}

	return bReturn;
}

static char* getControllerStateAsString(eControllerState_t state)
{
	static char buf[255] = {};

	switch(state)
	{
		case E_STATE_IDLE:
		snprintf(&buf[0], 255, "%s", "IDLE STATE\n");
		break;
		case E_STATE_MANUAL:
		snprintf(&buf[0], 255, "%s", "MANUAL STATE\n");
		break;
		case E_STATE_AUTOMATIC:
		snprintf(&buf[0], 255, "%s", "AUTOMATIC STATE\n");
		break;
		case E_STATE_SEQUENCE:
		snprintf(&buf[0], 255, "%s", "SEQUENCE STATE\n");
		break;
		default:
		snprintf(&buf[0], 255, "%s", "UNKNOWN STATE\n");
		break;
	}

	return &buf[0];
}