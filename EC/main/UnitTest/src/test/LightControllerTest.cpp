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
    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.read(NULL, &payloadBytesRead));
    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.read(payload, NULL));
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.read(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, CupIdTooLarge)
{
    uint8_t payload[4] = {NUM_OF_CUPS, 0, 0, 0};
    uint8_t payloadBytesRead = 0;

    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.read(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, CupIdMaxBoundary)
{
    uint8_t payload[4] = {NUM_OF_CUPS - 1, 0, 0, 0};
    uint8_t payloadBytesRead = 0;

    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.read(payload, &payloadBytesRead));
}

TEST_F(LightControllerFixture, InvalidPutModulusSize)
{
    uint8_t payload[3] = {NUM_OF_CUPS - 1, 0, 0};
    uint8_t payloadSize = 3;

    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.write(payload, payloadSize));
    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.write(payload, payloadSize - 1));
    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.write(payload, payloadSize - 2));
    EXPECT_EQ(BJ_FAILURE, m_lightCtrl.write(payload, payloadSize - 3));

}

TEST_F(LightControllerFixture, SetAndGetTest)
{
    uint8_t cupIdPayloadOne[4] = {1, 200, 100, 50};
    uint8_t cupIdPayloadTen[4] = {10, 34, 36, 231};
    uint8_t payloadSize = 4;
    uint8_t payloadBytesRead = 0;
    uint8_t payload[4] = {1, 0, 0, 0};

    // Set the payload of light 0 and 10
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.write(cupIdPayloadOne, payloadSize));
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.write(cupIdPayloadTen, payloadSize));

    // Get the payload of cup 0 and validate
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.read(payload, &payloadBytesRead));
    EXPECT_EQ(4, payloadBytesRead);

    // Validate the contents for cup 0
    for(int i = 0; i < payloadSize; i++)
    {
        EXPECT_EQ(cupIdPayloadOne[i], payload[i]);
    }

    // Get the payload of cup 10 and validate
    payload[0] = 10;
    EXPECT_EQ(0, m_lightCtrl.read(payload, &payloadBytesRead));
    EXPECT_EQ(4, payloadBytesRead);

    // Validate the contents for cup 10
    for(int i = 0; i < payloadSize; i++)
    {
        EXPECT_EQ(cupIdPayloadTen[i], payload[i]);
    }
}

TEST_F(LightControllerFixture, SetAndGetIndexZero)
{
    uint8_t cupIdPayload[4] = {0, 200, 100, 50};
    uint8_t payloadSize = 4;
    uint8_t payloadBytesRead = 0;
    uint8_t payload[4] = {0, 0, 0, 0};

    // Set the payload of light 0
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.write(cupIdPayload, payloadSize));

    // Get the payload
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.read(payload, &payloadBytesRead));
    EXPECT_TRUE(0 == std::memcmp(cupIdPayload, payload, sizeof(payload)));

}

TEST_F(LightControllerFixture, SetAndGetAll)
{
    uint8_t cupIdPayload[4] = {0, 0, 0, 0};
    uint8_t cupIdGetAll[NUM_OF_CUPS * RGB_WS_LED_SIZE] = {0};
    uint8_t payloadRead = 0;
    uint8_t baseIdx = 0;
    const uint8_t expectedReadAllCupsPayload = 80;

    // Put the values of all cups
    for(int i = 0; i < NUM_OF_CUPS; i++)
    {
        // Set CUP ID
        cupIdPayload[0] = i;

        // Set random light vals
        cupIdPayload[1] = i + 5;
        cupIdPayload[2] = i + 6;
        cupIdPayload[3] = i + 7;

        EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.write(cupIdPayload, 4));
    }


    // Get all the values of all cups
    cupIdGetAll[0] = RGB_GET_ALL_VALUES_REQ;
    EXPECT_EQ(BJ_SUCCESS, m_lightCtrl.read(cupIdGetAll, &payloadRead));
    EXPECT_EQ(expectedReadAllCupsPayload, payloadRead);

    // Verify that the get values are correct
    for(int i = 0; i < NUM_OF_CUPS; i++)
    {
        baseIdx = i * RGB_WS_LED_SIZE;

        EXPECT_EQ(cupIdGetAll[baseIdx], i);
        EXPECT_EQ(cupIdGetAll[baseIdx + RGB_RED_BUF_POS],   i + 5);
        EXPECT_EQ(cupIdGetAll[baseIdx + RGB_GREEN_BUF_POS], i + 6);
        EXPECT_EQ(cupIdGetAll[baseIdx + RGB_BLUE_BUF_POS],  i + 7);
    }
}


