//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//  PROTOCOL:
//
//	    1 Byte          1 Bytes              4 bytes           1 Bytes     	    2^(8) max      1 byte
//  |    SOF    |   HEADER OPTIONS    |     IP ADDR     |    PAYLOAD SIZE    |    PAYLOAD     |    EOF    | 
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

// Arduino includes
#include "Arduino.h"
#include "NetworkController.h"
#include "SensorController.h"
#include "SystemController.h"
#include "LightController.h"
#include "Logger.h"


//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//

// Arduino Controller ID
#define ARDUINO_CTRL_ID ((uint8_t) 0x01)

// Protocl defines
#define defSOF     ((uint8_t) 0x5C)
#define defEOF	   ((uint8_t) 0xC5)

#define defHEADER_SIZE_BYTES  ((uint8_t) 1)
#define defPAYLOAD_SIZE_BYTES ((uint8_t) 2)
#define defEOF_SIZE			  ((uint8_t) 1)
#define defHEADER_TARGET_MASK ((uint8_t) 0xF0)
#define defHEADER_MODULE_MASK ((uint8_t) 0x0F)
#define defHEADER_SHIFT_BITS  ((uint8_t) 4)

#define DEFAULT_BAUD_RATE	((uint32_t) 115200)


//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

// Protocol Buffer
static uint8_t aui8ProtocolBuffer[defMAX_BUFFER_SIZE];

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
void vProcessProtocolBuffer(uint8_t *pui8Buffer, uint32_t ui32BufSize);
void vSendBufferToTarget(stPackage* package);

void vReturnPingRequest(stPackage* package);	
/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
int16_t NetworkComm_Init(void)
{
	uint16_t ui16Return = 1;

	Serial.begin(DEFAULT_BAUD_RATE);

	return ui16Return;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
void NetworkComm_Run(void)
{
	uint32_t ui32MsgRecBufCnt = 0; 
	int  ui8BufferSize = 0;
	static int i = 0;

	// Check if there is any input to read
	ui8BufferSize = Serial.available();
	if(ui8BufferSize > 0)
	{
		// Retrieve the data	
		ui32MsgRecBufCnt = Serial.readBytes(&aui8ProtocolBuffer[0], ui8BufferSize);

		// Process the data. Check for any buffer overflow
		if(ui32MsgRecBufCnt > 0)
		{
			vProcessProtocolBuffer(&aui8ProtocolBuffer[0], ui32MsgRecBufCnt);
		}	
		ui8BufferSize = 0;
	}
}


/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
void NetworkComm_TransmitPacket(stPackage* package)
{
	uint8_t ui8ProtocolIdx = 0;
	uint8_t ui8PayloadIdx = 0;

	// Check if the Serial is available
	if(Serial)
	{
		// Clear the buffer
		memset(aui8ProtocolBuffer, 0x00, defMAX_BUFFER_SIZE);

		// SOF and Headers
		aui8ProtocolBuffer[ui8ProtocolIdx++] = defSOF;
		aui8ProtocolBuffer[ui8ProtocolIdx++] = package->header.ui8Header;

		// IP Addr
		aui8ProtocolBuffer[ui8ProtocolIdx++] = (uint8_t) ((package->ipAddr & 0xFF000000) >> 24);
		aui8ProtocolBuffer[ui8ProtocolIdx++] = (uint8_t) ((package->ipAddr & 0x00FF0000) >> 16);
		aui8ProtocolBuffer[ui8ProtocolIdx++] = (uint8_t) ((package->ipAddr & 0x0000FF00) >> 8);
		aui8ProtocolBuffer[ui8ProtocolIdx++] = (uint8_t) ((package->ipAddr & 0x000000FF));

		// Payload size
		aui8ProtocolBuffer[ui8ProtocolIdx++] = package->payloadSize;

		// Payload
		memcpy(&aui8ProtocolBuffer[ui8ProtocolIdx], &package->payload[0], package->payloadSize);
		ui8ProtocolIdx += package->payloadSize;

		// EOF
		aui8ProtocolBuffer[ui8ProtocolIdx] = defEOF;

		// Write to target
		Serial.write(aui8ProtocolBuffer, package->payloadSize + HEADER_ADDITIONEL_BYTES);
		Serial.flush();
		delay(100);
	}
	else
	{
		//Serial.println("NETWORKCOMM - Failed to trasnmit packet. Serial not available");
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
bool NetworkComm_CreatePackage(stPackage *package, eHeaderTargetOption_t eHeaderTarget, eHeaderModuleOption_t eHeaderModule,
	uint8_t payloadSize, uint8_t* payload, uint32_t ipAddr)
{
	if(payload == NULL || package == NULL) 
	{
		BJBP_LOG_ERR("Package creator err. Payload or pkg NULL");
		return false;
	}

	package->header.bitHeader.target = eHeaderTarget;
	package->header.bitHeader.module = eHeaderModule;
	package->ipAddr = ipAddr;
	package->payloadSize = payloadSize;

	// Copy payload
	memcpy(package->payload, &payload[0], payloadSize);

	return true;

}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
void vProcessProtocolBuffer(uint8_t *pui8Buffer, uint32_t ui32BufSize)
{
	// Packet Finite State Machine
	static ePacketFsm_t ePacketFsm = (ePacketFsm_t) E_SOF;
	static eHeaderTargetOption_t eHeaderTarget = (eHeaderTargetOption_t) E_NONE;
	static eHeaderModuleOption_t eHeaderModule = (eHeaderModuleOption_t) E_DEFAULT_MODULE;
	static uint8_t aui8PayloadBuffer[defMAX_PAYLOAD_SIZE / 2] = {}; 
	static uint32_t ui32IpAddr = 0;


	uint32_t ui32BufIdx = 0;
	uint16_t ui8PayloadIdx = 0;
	uint16_t ui8PayloadSize = 0;


	if(pui8Buffer[ui32BufIdx++] != defSOF) 
	{
		BJBP_LOG_ERR("NETWORKCOMM - NO SOF\n");
	}
	else 
	{
		// Create the package
		stPackage package;
		// HEADER
		package.header.bitHeader.target = ((pui8Buffer[ui32BufIdx] & defHEADER_TARGET_MASK)) >> defHEADER_SHIFT_BITS;
		package.header.bitHeader.module = pui8Buffer[ui32BufIdx++] & defHEADER_MODULE_MASK;
		//eHeaderTarget = (eHeaderTargetOption_t) ((pui8Buffer[ui32BufIdx] & defHEADER_TARGET_MASK) >> defHEADER_SHIFT_BITS);
		//eHeaderModule = (eHeaderModuleOption_t) (pui8Buffer[ui32BufIdx++] & defHEADER_MODULE_MASK);

		// IP ADDR
		package.ipAddr = (uint32_t) pui8Buffer[ui32BufIdx++] << 24;
		package.ipAddr |= (uint32_t)pui8Buffer[ui32BufIdx++] << 16;
		package.ipAddr |= (uint32_t)pui8Buffer[ui32BufIdx++] << 8;
		package.ipAddr |= (uint32_t)pui8Buffer[ui32BufIdx++];

		//ui32IpAddr = (uint32_t) pui8Buffer[ui32BufIdx++] << 24;
		//ui32IpAddr |= (uint32_t)pui8Buffer[ui32BufIdx++] << 16;
		//ui32IpAddr |= (uint32_t)pui8Buffer[ui32BufIdx++] << 8;
		//ui32IpAddr |= (uint32_t)pui8Buffer[ui32BufIdx++];

		// PAYLOAD SIZE
		package.payloadSize = pui8Buffer[ui32BufIdx++];
		//ui8PayloadSize = pui8Buffer[ui32BufIdx++];


		// PAYLOAD
		memset(package.payload, 0x00, defMAX_PAYLOAD_SIZE);
		memcpy(package.payload, &pui8Buffer[ui32BufIdx], package.payloadSize);
		ui32BufIdx += package.payloadSize;
		//for(ui8PayloadIdx = 0; ui8PayloadIdx < ui8PayloadSize; ui8PayloadIdx++)
		//{
		//	aui8PayloadBuffer[ui8PayloadIdx] = pui8Buffer[ui32BufIdx++];
		//}

		// EOF
		if(pui8Buffer[ui32BufIdx] != defEOF) 
		{
			BJBP_LOG_ERR("NETWORKCOMM - NO EOF\n");
		}
		else 
		{
			vSendBufferToTarget(&package);	
		}

	}

}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
void vSendBufferToTarget(stPackage* package)
{
	uint8_t ui8BufIdx = 0;

	switch(package->header.bitHeader.target)
	{
		case E_TARGET_LIGHT_REQUEST:
			BJBP_LOG_INFO("E_TARGET_LIGHT\n");
			LightController_ReceiveDataPacket(package);
		break;
		case E_TARGET_SCORE_REQUEST:
			BJBP_LOG_INFO("E_TARGET_SCORE\n");
			SensorController_ReceiveDataPacket(package);
		break;
		case E_TARGET_CONTROLLER_REQUEST:
			BJBP_LOG_INFO("E_TARGET_CONTROLLER\n");
			SystemController_ReceiveDataPacket(package);
		break;
		case E_TARGET_PING:
			vReturnPingRequest(package);
			BJBP_LOG_INFO("E_TARGET_PING\n");
		break;
		default:
			BJBP_LOG_ERR("Unknown header target\n");
		break;	
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/ 
void vReturnPingRequest(stPackage* package)
{
	package->payload[0] = ARDUINO_CTRL_ID;

	NetworkComm_TransmitPacket(package);
}
