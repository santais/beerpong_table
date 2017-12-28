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
#include "LightController.h"

using namespace BjDataPackageDefines;
using namespace Controller;

class LightControllerFixture : public ::testing::Test
{

public:
    BjDataPackage m_bjDataPackage;
    LightController m_lightCtrl;

    LightControllerFixture() : m_lightCtrl(0)
    {

    }

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

// TESTS:
// 4.Set and Get expcted

TEST_F(LightControllerFixture, ValidateNullInput)
{
    uint8_t payload[100] = {};
    uint8_t payloadBytesRead = 10;

    // Get the payload
    EXPECT_EQ(-1, m_lightCtrl.handleGet(NULL, &payloadBytesRead));
    EXPECT_EQ(-1, m_lightCtrl.handleGet(payload, NULL));
    EXPECT_EQ(0, m_lightCtrl.handleGet(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, CupIdTooLarge)
{
    uint8_t payload[4] = {NUM_OF_CUPS, 0, 0, 0};
    uint8_t payloadBytesRead = 0;

    EXPECT_EQ(-1, m_lightCtrl.handleGet(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, CupIdMaxBoundary)
{
    uint8_t payload[4] = {NUM_OF_CUPS - 1, 0, 0, 0};
    uint8_t payloadBytesRead = 0;

    EXPECT_EQ(0, m_lightCtrl.handleGet(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, InvalidPutModulusSize)
{
    uint8_t payload[3] = {NUM_OF_CUPS - 1, 0, 0};
    uint8_t payloadSize = 3;

    EXPECT_EQ(-1, m_lightCtrl.handlePut(payload, payloadSize));
    EXPECT_EQ(-1, m_lightCtrl.handlePut(payload, payloadSize - 1));
    EXPECT_EQ(-1, m_lightCtrl.handlePut(payload, payloadSize - 2));
    EXPECT_EQ(-1, m_lightCtrl.handlePut(payload, payloadSize - 3));

}

TEST_F(LightControllerFixture, SetAndGetTest)
{
    uint8_t cupIdPayloadZero[4] = {0, 200, 100, 50};
    uint8_t cupIdPayloadTen[4] = {10, 34, 36, 231};
    uint8_t payloadSize = 4;
    uint8_t payloadBytesRead = 0;
    uint8_t payload[4] = {0, 0, 0, 0};

    // Set the payload of light 0 and 10
    EXPECT_EQ(0, m_lightCtrl.handlePut(cupIdPayloadZero, payloadSize));
    EXPECT_EQ(0, m_lightCtrl.handlePut(cupIdPayloadTen, payloadSize));

    // Get the payload of cup 0 and validate
    EXPECT_EQ(0, m_lightCtrl.handleGet(payload, &payloadBytesRead));
    EXPECT_EQ(4, payloadBytesRead);

    // Validate the contents for cup 0
    for(int i = 0; i < payloadSize; i++)
    {
        EXPECT_EQ(cupIdPayloadZero[i], payload[i]);
    }

    // Get the payload of cup 10 and validate
    payload[0] = 10;
    EXPECT_EQ(0, m_lightCtrl.handleGet(payload, &payloadBytesRead));
    EXPECT_EQ(4, payloadBytesRead);

    // Validate the contents for cup 10
    for(int i = 0; i < payloadSize; i++)
    {
        EXPECT_EQ(cupIdPayloadTen[i], payload[i]);
    }
}


