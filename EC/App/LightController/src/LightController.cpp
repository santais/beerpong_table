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
#include "LightController.h"
#include "NetworkComm.h"
#include <Adafruit_NeoPixel.h>
#include "Logger.h"
//#include "Ws2812Driver.h"

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define NUM_OF_CUPS    ((uint8_t) 22)
#define LEDS_PER_CUP   ((uint8_t) 16)

#define defLED_RGB_MAX_VAL ((uint8_t) 255)

#define defSINGLE_SET_BUF_SIZE_BYTES ((uint8_t) 4)
#define defALL_SET_BUF_SIZE_BYTES    ((uint8_t) (ui32TmpDataUM_OF_CUPS * 3))
//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

// Table to hold all lights
static stCupLights_t stCupLightArray[NUM_OF_CUPS];

// Strip object
static Adafruit_NeoPixel* neoPixelStrip;

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//
void vInitStWs2812Leds(stWs2812Led_t *stWs2812Led, uint8_t ui8NumOfWsLeds);
void vInitLedValues(stLedValues_t *stLedValues);
void vSetOutputLedValues(void);

void vHandlePostRequest(stPackage* package);

void vHandleSingleGetRequest(uint8_t *pui8Buffer, uint8_t ui8BufSize, uint32_t ui32IpAddr);
void vHandleAllGetRequest(uint8_t *pui8Buffer, uint8_t ui8BufSize, uint32_t ui32IpAddr);

void vSetWs2812Leds(uint8_t ui8CupId, stLedValues_t *stLedValues);

//void vWriteLedValusToOutput(void);
//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
int16_t LightController_Init(uint8_t ui8DataPin)
{
	uint16_t i16Return = 1;

	// Initialize the CupLightsArray
	uint8_t ui8CupLightIdx = 0;
	for(ui8CupLightIdx = 0; ui8CupLightIdx < NUM_OF_CUPS; ui8CupLightIdx++)
	{
		stCupLightArray[ui8CupLightIdx].ui8Id = ui8CupLightIdx;

		// Init the WS2812 LEDS. 
		// TODO: Only the first WS2812 is supported now. Therefore only [0] is inserted.
		vInitStWs2812Leds(&stCupLightArray[ui8CupLightIdx].stWs2812Leds[0], defNUM_OF_WS2812_PER_CUP);
	}

	// Init the WS2812Driver
	neoPixelStrip = new Adafruit_NeoPixel(LEDS_PER_CUP * NUM_OF_CUPS, ui8DataPin, NEO_GRB + NEO_KHZ800);
	neoPixelStrip->begin();

	return i16Return;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void LightController_SetCupLed(uint8_t ui8CupId, uint8_t ui8Ws2812Id, stLedValues_t *stLedValues)
{
	stCupLightArray[ui8CupId].stWs2812Leds[ui8Ws2812Id].stLedValues = *stLedValues;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
stLedValues_t LightController_GetCupLed(uint8_t ui8CupId, uint8_t ui8Ws2812Id)
{
	return(stCupLightArray[ui8CupId].stWs2812Leds[ui8Ws2812Id].stLedValues);
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
stCupLights_t* LightController_GetCupLightPtr(uint8_t ui8CupId)
{
	return(&stCupLightArray[ui8CupId]);
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void LightController_ReceiveDataPacket(stPackage* package)
{
	switch(package->header.bitHeader.module)
	{
		case E_SET_ALL_REQUEST:	// TODO: REMOVE
		case E_SET_REQUEST:
			vHandlePostRequest(package);
		break;
		break;
		case E_GET_REQUEST:
			vHandleSingleGetRequest(&package->payload[0], package->payloadSize, package->ipAddr);
		break;
		case E_GET_ALL_REQUEST:

		break;
		default:
			BJBP_LOG_ERR("LIGHTCONTROLLER - UNKNOWN E HEADER MODULE REQUEST\n");
		break;
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vInitStWs2812Leds(stWs2812Led_t *stWs2812Led, uint8_t ui8NumOfWsLeds)
{
	uint8_t ui8Ws2812LedIdx = 0;

	for(ui8Ws2812LedIdx = 0; ui8Ws2812LedIdx < ui8NumOfWsLeds; ui8Ws2812LedIdx++)
	{
		stWs2812Led[ui8Ws2812LedIdx].ui8Id = ui8Ws2812LedIdx;

		vInitLedValues(&stWs2812Led[ui8Ws2812LedIdx].stLedValues);
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vInitLedValues(stLedValues_t *stLedValues)
{
	// Default value is red
	stLedValues->ui8RedValue   = defLED_RGB_MAX_VAL;
	stLedValues->ui8GreenValue = 0;
	stLedValues->ui8BlueValue  = 0;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vHandlePostRequest(stPackage* package)
{
	uint8_t ui8CupId = 0;
	uint8_t ui8BufItr = 0;
	uint8_t ui8NumOfLeds = package->payloadSize / defSINGLE_SET_BUF_SIZE_BYTES;
	stLedValues_t stLedValues;

	// Retreive the values from the buffer
	if((package->payloadSize % defSINGLE_SET_BUF_SIZE_BYTES) > 0) 
	{
		//Serial.printf("LIGHTCONTROLLER - Buffer size too large in SET SINGLE! Size is: %u\n", ui8BufSize);
	}
	else
	{
		for(ui8BufItr = 0; ui8BufItr < ui8NumOfLeds; ui8BufItr++)
		{
			ui8CupId 			      = package->payload[(ui8BufItr * defSINGLE_SET_BUF_SIZE_BYTES)];
			stLedValues.ui8RedValue   = package->payload[(ui8BufItr * defSINGLE_SET_BUF_SIZE_BYTES) + 1];
			stLedValues.ui8GreenValue = package->payload[(ui8BufItr * defSINGLE_SET_BUF_SIZE_BYTES) + 2];
			stLedValues.ui8BlueValue  = package->payload[(ui8BufItr * defSINGLE_SET_BUF_SIZE_BYTES) + 3];

			// Set the values in the light array
			vSetWs2812Leds(ui8CupId, &stLedValues);

			// DEBUG
			BJBP_LOG_INFO("Cup %u values R[%u] G[%u] B[%u] \n",
			 	ui8CupId, stLedValues.ui8RedValue, stLedValues.ui8GreenValue, 
				stLedValues.ui8BlueValue);

			// Notify all of a new SET 
			package->header.bitHeader.module = E_NOTIFY_ALL_REQUEST;
			NetworkComm_TransmitPacket(package);
		}

		// Set the LED values on the output
		//vWriteLedValusToOutput();
		neoPixelStrip->show();
	
	}
	Serial.println("out");
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vHandleSingleGetRequest(uint8_t *pui8Buffer, uint8_t ui8BufSize, uint32_t ui32IpAddr)
{
	uint8_t ui8TransferBuf[defSINGLE_SET_BUF_SIZE_BYTES];
	uint8_t ui8CupId = 0;
	uint8_t ui8BufItr = 0;

	stLedValues_t stLedValues;

	// Get the cup Id and return a packet with the values
	if(ui8BufSize > 1)
	{
		//Serial.printf("LIGHTCONTROLLER - Buffer size too large in GET SINGLE! Size is: %u\n", ui8BufSize);
	}
	else
	{
		ui8CupId = pui8Buffer[0];
		if(ui8CupId > (NUM_OF_CUPS - 1))
		{
			//Serial.printf("LIGHTCONTROLLER - Invalid cup id of with value: %u\n", ui8CupId);
		}
		else 
		{
			// Trasfer package
			stPackage transferPackage;

			// Tmp buffer
			uint8_t stLedTmpBuffer[defSINGLE_SET_BUF_SIZE_BYTES];
			stLedTmpBuffer[0] = ui8CupId;
			memcpy(&stLedTmpBuffer[1], &stCupLightArray[ui8CupId].stWs2812Leds[0].stLedValues, sizeof(stLedValues_t));

			if(!NetworkComm_CreatePackage(&transferPackage, E_TARGET_LIGHT_REQUEST, E_GET_REQUEST, defSINGLE_SET_BUF_SIZE_BYTES,
				&stLedTmpBuffer[0], ui32IpAddr))
			{
				BJBP_LOG_ERR("Unable to create lightcontroller GET package");
			}
			else
			{
				// Transmit the packet
				NetworkComm_TransmitPacket(&transferPackage);
			}

			/*
			ui8TransferBuf[ui8BufItr++] = ui8CupId;
			ui8TransferBuf[ui8BufItr++] = stLedValues.ui8RedValue;
			ui8TransferBuf[ui8BufItr++] = stLedValues.ui8GreenValue;
			ui8TransferBuf[ui8BufItr]   = stLedValues.ui8BlueValue;*/
		}
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vHandleAllGetRequest(uint8_t *pui8Buffer, uint8_t ui8BufSize, uint32_t ui32IpAddr)
{

}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
void vSetWs2812Leds(uint8_t ui8CupId, stLedValues_t *stLedValues)
{
	uint8_t ui8BufItr = 0;
	uint8_t ui8RingStartIdx = ui8CupId * LEDS_PER_CUP;
	uint8_t ui8RingIdx = 0;

	stWs2812Led_t *stWs2812Leds = &stCupLightArray[ui8CupId].stWs2812Leds[0];

	// Iterate through the lights and set their values
	for(ui8BufItr = 0; ui8BufItr < defNUM_OF_WS2812_PER_CUP; ui8BufItr++)
	{
		ui8RingIdx = ui8RingStartIdx + ui8BufItr;
		stWs2812Leds[ui8BufItr].stLedValues = *stLedValues;

		// Set neo pixel lights
		neoPixelStrip->setPixelColor(ui8RingIdx, stLedValues->ui8RedValue, stLedValues->ui8GreenValue,
			stLedValues->ui8BlueValue);
	}
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return
/*********************************************************************************/
/*void vWriteLedValusToOutput(void)
{	
	uint8_t ui8CupLightItr = 0;
	uint8_t ui8Ws2812LedItr = 0;

	for(ui8CupLightItr = 0; ui8CupLightItr < NUM_OF_CUPS; ui8CupLightItr++)
	{
		for(ui8Ws2812LedItr = 0; ui8Ws2812LedItr < defNUM_OF_WS2812_PER_CUP; ui8Ws2812LedItr++)
		{
			//Ws2812Driver_Write(&stCupLightArray[ui8CupLightItr].stWs2812Leds[ui8Ws2812LedItr].stLedValues);
		}
	}

	// Set the values!
	Ws2812Driver_SetValues();
}*/

