/*
 * BjDataPackage.h
 *
 *  Created on: 11 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGE_H_
#define LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGE_H_

#include <stdint.h>

#include <BjDataPackageDefines.h>

using namespace BjDataPackageDefines;

class BjDataPackage
{
public:
    BjDataPackage();
    virtual ~BjDataPackage();

    // Individual get/setters
    void setRestRequest(RestRequest request) { m_restRequest = request; }
    RestRequest getRestRequest() { return m_restRequest; }

    void setTargetModule(TargetModule module) { m_targetModule = module; }
    TargetModule getTargetModule() { return m_targetModule; }

    void setIpAddr(uint32_t ipAddr) { m_ipAddr = ipAddr; }
    uint32_t getIpAddr() { return m_ipAddr; }

    bool setPayload(uint8_t* payload, uint16_t payloadSize);
    uint8_t* getPayload(uint16_t* ptrPayloadSize);

    bool setPkgFromRawPayload(uint8_t* payload, uint16_t payloadSize);

    // Raw get/setters
    uint8_t* getRawPayload(void);
    uint16_t getRawPayloadSize(void);

private:
    void setRawPayload(void);

    // Data variables
    RestRequest m_restRequest;
    TargetModule m_targetModule;
    uint32_t m_ipAddr;
    uint8_t m_payloadSize;
    uint8_t m_payload[MAX_PAYLOAD_SIZE];

    uint8_t m_rawPayload[MAX_PACKAGE_SIZE];
    uint16_t m_rawPayloadSize;
};

#endif /* LIBRARIES_CONTROLLERINTERFACE_SRC_BJDATAPACKAGE_H_ */
