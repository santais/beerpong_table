//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef NETWORKCOMM_H_
#define NETWORKCOMM_H_

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include <stdint.h>


//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define HEADER_ADDITIONEL_BYTES ((uint8_t) 8)

#define defMAX_PAYLOAD_SIZE ((uint8_t) 240)	// 8 bit payload size
#define defMAX_BUFFER_SIZE  (((uint8_t) 240) + HEADER_ADDITIONEL_BYTES)	// 8 bit + the additional fields in the protocol
#define PING_PAYLOAD_SIZE   ((uint8_t) 1)

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//
typedef enum 
{	
	E_NONE						= 0x00,		/// Unknown target
	E_TARGET_LIGHT_REQUEST	    = 0x01,		/// Light controller request
	E_TARGET_CONTROLLER_REQUEST = 0x02,		/// System controller request
	E_TARGET_SCORE_REQUEST		= 0x03,		/// Score controller request

	E_TARGET_PING				= 0x04,		/// Ping request
	E_TARGET_LOG				= 0x05	 	/// Log request
} eHeaderTargetOption_t;

typedef enum
{
	E_DEFAULT_MODULE			= 0x00,

	E_GET_REQUEST 				= 0x01,		/// Get request 
	E_GET_ALL_REQUEST 			= 0x02,		/// Get all data request
	E_SET_REQUEST 				= 0x03,		/// Set reqest
	E_SET_ALL_REQUEST 			= 0x04,		/// Set all data request
	E_NOTIFY_ALL_REQUEST		= 0x05,		/// Notify all user

	E_LOG_MSG					= 0x06		/// Log message form Arduino to RPI
} eHeaderModuleOption_t;

typedef enum
{											
	E_SOF,									/// Start of File
	E_HEADER_OPTION,						/// Target and module
	E_IP_ADDR_SRC,
	E_PAYLOAD_SIZE,							/// Number of bytes in the payload
	E_PAYLOAD,								/// Payload
	E_EOF 									/// End of file
} ePacketFsm_t;

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

typedef struct 
{
	uint8_t module : 4;
	uint8_t target : 4;
} stHeaderBit;

typedef union 
{
	stHeaderBit bitHeader;
	uint8_t ui8Header;
} stHeader;


typedef struct 
{
	stHeader header;
	uint32_t ipAddr;
	uint8_t payloadSize;
	uint8_t payload[defMAX_PAYLOAD_SIZE];
} stPackage;

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//
int16_t NetworkComm_Init(void);
void 	NetworkComm_Run(void);
void    NetworkComm_TransmitPacket(stPackage* package);
bool 	NetworkComm_CreatePackage(stPackage *package, eHeaderTargetOption_t eHeaderTarget, eHeaderModuleOption_t eHeaderModule,
	uint8_t payloadSize, uint8_t* payload, uint32_t ipAddr);


#endif /* NETWORKCOMM_H_ */