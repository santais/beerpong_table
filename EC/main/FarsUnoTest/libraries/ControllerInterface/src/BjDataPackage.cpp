/*
 * BjDataPackage.cpp
 *
 *  Created on: 11 Dec 2017
 *      Author: markpovlsen
 */

#include <string.h>

#include "BjDataPackage.h"
#include "Arduino.h"
#include "Logger.h"

using namespace BjDataPackageDefines;

 /**************************************************************/
// Date: 13 Dec 2017
// Function: BjDataPackage::BjDataPackage
// Description:
// Inputs: uint8_t*
// Output: TODO
// Return:
/**************************************************************/
BjDataPackage::BjDataPackage() : m_restRequest(RestRequest::E_NONE_REQUEST),
        m_targetModule(TargetModule::E_NONE), m_ipAddr(0), m_payloadSize(0),
        m_rawPayloadSize(0)
{
    memset(m_payload, 0x0, MAX_PACKAGE_SIZE);
    memset(m_rawPayload, 0x0, MAX_PACKAGE_SIZE);
}

bool BjDataPackage::setPayload(uint8_t* payload, uint16_t payloadSize)
{
    bool retVal = false;

    if((payload != NULL) && (payloadSize < MAX_PAYLOAD_SIZE))
    {
        retVal = true;
        m_payloadSize = payloadSize;
        memcpy(m_payload, payload, m_payloadSize);
    }

    return retVal;

}

uint8_t* BjDataPackage::getPayload(uint16_t* ptrPayloadSize)
{
    if(ptrPayloadSize != NULL)
    {
        *ptrPayloadSize = m_payloadSize;
    }

    return &m_payload[0];
}

bool BjDataPackage::setPkgFromRawPayload(uint8_t* payload, uint16_t payloadSize)
{
    bool retVal = false;

    // Validate the payload size
    if((payload != NULL) && (payloadSize >= (PACKAGE_HEADER_SIZE)))
    {
        retVal = true;

        // Set the checksum
        // TODO Implement checksum

        // Rest and Target Module
        m_restRequest = static_cast<RestRequest>(payload[(uint8_t) PackageField::E_REST_REQUEST]);
        m_targetModule = static_cast<TargetModule>(payload[(uint8_t) PackageField::E_MODULE]);

        // IP Address
        m_ipAddr = (static_cast<uint32_t>(payload[(uint8_t) PackageField::E_IP_ADDR_SRC]) << 24);
        m_ipAddr |= (static_cast<uint32_t>(payload[(uint8_t) PackageField::E_IP_ADDR_SRC + 1]) << 16);
        m_ipAddr |= (static_cast<uint32_t>(payload[(uint8_t) PackageField::E_IP_ADDR_SRC + 2]) << 8);
        m_ipAddr |= static_cast<uint32_t>(payload[(uint8_t) PackageField::E_IP_ADDR_SRC + 3]);

        // Payloadsize and payload
        m_payloadSize = payload[(uint8_t) PackageField::E_PAYLOAD_SIZE];
        memcpy(m_payload, &payload[(uint8_t) PackageField::E_PAYLOAD], m_payloadSize);
    }

    return retVal;
}

BjDataPackage::~BjDataPackage()
{

}

uint16_t BjDataPackage::getPayloadSize(void) {
    return m_payloadSize;
}

 uint8_t* BjDataPackage::getRawPayload(void)
{
     // Set the raw payload
     setRawPayload();

     // Return the set payload
     return &m_rawPayload[0];
}

uint16_t BjDataPackage::getRawPayloadSize(void)
{
    // Set the raw payload
    setRawPayload();

    // Return the newly set payload size
    return(m_rawPayloadSize);
}

 void BjDataPackage::setRawPayload(void)
 {
     uint16_t ui16BufItr = 0;

     // Set the header values
     m_rawPayload[ui16BufItr++] = SOF_VAL;

     // Skip the CRC for now
     // TODO ADD CRC CHECK
    // m_rawPayload[ui16BufItr] = 0;
    // ui16BufItr += 4;

     // Rest and Target Module
     m_rawPayload[ui16BufItr++] = (uint8_t) m_restRequest;
     m_rawPayload[ui16BufItr++] = (uint8_t) m_targetModule;

     // IP Address
     m_rawPayload[ui16BufItr++] = (uint8_t) ((m_ipAddr & 0xFF000000) >> 24);
     m_rawPayload[ui16BufItr++] = (uint8_t) ((m_ipAddr & 0x00FF0000) >> 16);
     m_rawPayload[ui16BufItr++] = (uint8_t) ((m_ipAddr & 0x0000FF00) >> 8);
     m_rawPayload[ui16BufItr++] = (uint8_t) ((m_ipAddr & 0x000000FF));

     // PayloadSize and Payload
     m_rawPayload[ui16BufItr++] = m_payloadSize;
     memcpy(&m_rawPayload[ui16BufItr], &m_payload[0], m_payloadSize);
     ui16BufItr += m_payloadSize;

     // EOF
     m_rawPayload[ui16BufItr] = EOF_VAL;

     // Set the raw payloadsize
     m_rawPayloadSize = m_payloadSize + PACKAGE_HEADER_SIZE;
 }
