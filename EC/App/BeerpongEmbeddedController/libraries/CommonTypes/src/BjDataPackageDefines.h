 	/*
 * BjDataPackageDefines.h
 *
 *  Created on: 11 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGEDEFINES_H_
#define LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGEDEFINES_H_

#include <stdint.h>

namespace BjDataPackageDefines
{

//-------------------------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------------------------//
const uint8_t PACKAGE_HEADER_SIZE     = 13;
const uint16_t MAX_PAYLOAD_SIZE       = 255;
const uint16_t MAX_PACKAGE_SIZE       = (uint16_t) PACKAGE_HEADER_SIZE + (uint16_t) MAX_PAYLOAD_SIZE; // 8 bit + the additional fields in the protocol
const uint8_t PING_PAYLOAD_SIZE       = 1;

// Constant values used for the package FSM
const uint8_t SOF_VAL            = 0x5C;
const uint8_t EOF_VAL            = 0xC5;
const uint8_t HEADER_SIZE_BYTES  = 1;
const uint8_t PAYLOAD_SIZE_BYTES = 2;
const uint8_t EOF_SIZE           = 2;
const uint8_t HEADER_TARGET_MASK = 0xF0;
const uint8_t HEADER_MODULE_MASK = 0x0F;
const uint8_t HEADER_SHIFT_BITS  = 4;


//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//
enum class TargetModule
{
    E_NONE                      = 0x00,     /// Unknown target
    E_MODULE_LIGHT              = 0x01,     /// Light controller request
    E_MODULE_SYSTEM             = 0x02,     /// System controller request
    E_MODULE_SENSOR             = 0x03,     /// Sensor controller request
    E_MODULE_EDGE_LIGHT         = 0x04,     /// Edge light controller request

    E_MODULE_PING               = 0x05,     /// Ping request
    E_MODULE_LOG                = 0x06      /// Log request
};

enum class RestRequest
{
    E_NONE_REQUEST              = 0x00,     /// Default init request
    E_GET_REQUEST               = 0x01,     /// Get request
    E_PUT_REQUEST               = 0x02,     /// Set request
    E_POST_REQUEST              = 0x03,     /// Post request
    E_DELETE_REQUEST            = 0x04,     /// Delete request

    E_RESPOND_REQUEST           = 0x05,     /// Answer of a request
    E_NOTIFY_REQUEST            = 0x06,     /// Notification request
    E_PING_REQUEST              = 0x07      /// Ping Request
};

enum class PackageField
{
    E_SOF                       = 0x00,     /// Start of File
    E_CRC_CHECKSUM              = 0x01,     /// CRC Checksum
    E_REST_REQUEST              = 0x05,     /// REST request
    E_MODULE                    = 0x06,     /// Target module
    E_IP_ADDR_SRC               = 0x07,     /// Source IP Address
    E_PAYLOAD_SIZE              = 0x0B,     /// Number of bytes in the payload
    E_PAYLOAD                   = 0x0C,     /// Payload
    E_EOF                                   /// End of file
};

};


#endif /* LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGEDEFINES_H_ */
