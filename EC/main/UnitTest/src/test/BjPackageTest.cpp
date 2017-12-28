/*
 * SystemUnitTest.cpp
 *
 *  Created on: 16 Dec 2017
 *      Author: markpovlsen
 */

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"

#include <string.h>

#include "BjDataPackage.h"

using namespace BjDataPackageDefines;

class BjDataPackageFixture : public ::testing::Test
{

public:
    BjDataPackage m_bjDataPackage;

protected:
    virtual void SetUp()
    {
        // Default values of the package
        m_bjDataPackage.setRestRequest(RestRequest::E_GET_REQUEST);
        m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_LIGHT);
        m_bjDataPackage.setIpAddr(0);
    }

    virtual void TearDown()
    {

    }

};


TEST_F(BjDataPackageFixture, EmptyPackageSizeTest)
{
    uint16_t expectedPackageSize = PACKAGE_HEADER_SIZE + 2;
    uint8_t payload[2] = {};
    m_bjDataPackage.setPayload(payload, 2);
    EXPECT_EQ(expectedPackageSize, m_bjDataPackage.getRawPayloadSize());
}

TEST_F(BjDataPackageFixture, ExceedMaxPayloadFailure)
{
    uint16_t expectedPackageSize = PACKAGE_HEADER_SIZE + 256;
    uint8_t payload[256] = {};
    EXPECT_FALSE(m_bjDataPackage.setPayload(payload, 256));
}

TEST_F(BjDataPackageFixture, ValidatePayload)
{
    uint16_t expectedPackageSize = PACKAGE_HEADER_SIZE + 200;
    uint8_t payload[200] = {};
    uint8_t* ptrPayload = NULL;
    uint16_t payloadSize = 0;

    // Set the payload
    for(int i = 0; i < 200; i++)
    {
        payload[i] = i;
    }
    m_bjDataPackage.setPayload(payload, 200);

    // Get the payload
    ptrPayload = m_bjDataPackage.getPayload(&payloadSize);

    // Check payload size
    EXPECT_EQ(200, payloadSize);

    // Check the payload
    for(int i = 0; i < 200; i++)
    {
        EXPECT_EQ(i, ptrPayload[i]);
    }
}

TEST_F(BjDataPackageFixture, SetRestTarget)
{
    m_bjDataPackage.setRestRequest(RestRequest::E_PUT_REQUEST);
    EXPECT_EQ(RestRequest::E_PUT_REQUEST, m_bjDataPackage.getRestRequest());
}

TEST_F(BjDataPackageFixture, SetTargetModule)
{
    m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_LOG);
    EXPECT_EQ(TargetModule::E_MODULE_LOG, m_bjDataPackage.getTargetModule());
}

TEST_F(BjDataPackageFixture, SetLargePayloadCheckRawSize)
{
    uint16_t expectedPackagesize = PACKAGE_HEADER_SIZE + 250;
    uint8_t payload[250] = {};
    m_bjDataPackage.setPayload(payload, 250);
    EXPECT_EQ(expectedPackagesize, m_bjDataPackage.getRawPayloadSize());
}

TEST_F(BjDataPackageFixture, ValidateRawPayloadContent)
{
    uint16_t expectedPackagesize = PACKAGE_HEADER_SIZE + 250;
    uint8_t payload[250] = {};
    uint8_t* rawPayload = m_bjDataPackage.getRawPayload();
    uint32_t checksum = 0;
    uint32_t ipAddr = 0;

    m_bjDataPackage.setPayload(payload, 250);
    EXPECT_EQ(expectedPackagesize, m_bjDataPackage.getRawPayloadSize());

    // SOF
    EXPECT_EQ(SOF_VAL, rawPayload[0]);

    // Checksum
    memcpy(&checksum, &rawPayload[1], 4);
    EXPECT_EQ(0, checksum);

    // Rest REquest
    EXPECT_EQ(RestRequest::E_GET_REQUEST, (RestRequest) rawPayload[5]);

    // Target Module
    EXPECT_EQ(TargetModule::E_MODULE_LIGHT, (TargetModule) rawPayload[6]);

    // IP Addr
    memcpy(&ipAddr, &rawPayload[7], 4);
    EXPECT_EQ(ipAddr, 0);

    // Payload size
    EXPECT_EQ(250, rawPayload[11]);

    // EOF
    EXPECT_EQ(EOF_VAL, rawPayload[(expectedPackagesize - 1)]);
}

TEST_F(BjDataPackageFixture, SetRawBjPkgContent)
{
    uint8_t testPayload[PACKAGE_HEADER_SIZE + 1] = {
            SOF_VAL,
            0, 0, 0, 0,
            (uint8_t) RestRequest::E_GET_REQUEST,
            (uint8_t) TargetModule::E_MODULE_SYSTEM,
            1, 2, 3, 4,
            1,
            240,
            EOF_VAL
    };
    uint16_t payloadSize = 0;
    BjDataPackage bjDataPkg;
    bjDataPkg.setPkgFromRawPayload(testPayload, PACKAGE_HEADER_SIZE + 1);
    uint8_t* ptrPayload = bjDataPkg.getPayload(&payloadSize);

    // Rest Request
    EXPECT_EQ(RestRequest::E_GET_REQUEST, bjDataPkg.getRestRequest());

    // Target Module
    EXPECT_EQ(TargetModule::E_MODULE_SYSTEM, bjDataPkg.getTargetModule());

    // IP Addr
    EXPECT_EQ(16909060, bjDataPkg.getIpAddr());

    // Payload
    EXPECT_EQ(240, ptrPayload[0]);

    // Payload size
    EXPECT_EQ(1, payloadSize);
}
