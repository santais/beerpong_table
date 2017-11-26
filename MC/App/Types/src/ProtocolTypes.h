/*
 * ProtocolTypes.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _PROTOCOLTYPES_H_
#define _PROTOCOLTYPES_H_

/*
 * \brief Max data package size
 */
#define DATA_PACKAGE_SIZE 255u

/*
 * \brief IPv4 Address length
 */
#define IPv4_ADDRESS_LEN 4u

/**
 * \brief Size of data protocol up to data payload
 */
#define PROTOCOL_INITIAL_SIZE 7u

/**
 * \brief Minimum protocl size
 */
#define PROTOCOL_MINIMUM_SIZE (PROTOCOL_INITIAL_SIZE + 2u)

/**
 * \brief Mask to get the module
 */
#define PROTOCOL_MODULE_MASK 0x0F

/**
 * \brief Mask to get the target
 */
#define PROTOCOL_TARGET_MASK 0xF0


/**
 * Protocol Header
 */
typedef enum
{
    E_SOF   = 0x5C,
    E_EOF   = 0xC5,

    E_PING_TARGET       = 0x41,
    E_PING_PAYLOAD_SIZE = 0x01,
    E_PING_PKG_SIZE     = 0x09,
    E_PING_VAL          = 0x04,

    E_TARGET_BITSHIFT   = 0x04,
    E_TARGET_MASK       = 0xF0,

    E_MODULE_MASK       = 0x0F
} eProtocolHeaders_t;

/**
 * \brief Protocol Option
 */
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

/**
 * \brief Data package format
 */
struct DataPackage_t
{
	uint8_t sof;
	uint8_t header;
	uint8_t ip[IPv4_ADDRESS_LEN];
	uint8_t payloadSize;
	uint8_t payload[DATA_PACKAGE_SIZE];
	uint8_t eof;
};



#endif /* _PROTOCOLTYPES_H_ */
