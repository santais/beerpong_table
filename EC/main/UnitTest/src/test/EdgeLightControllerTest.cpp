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
#include "EdgeLightController.h"
#include "LightController.h"

using namespace BjDataPackageDefines;
using namespace Controller;

class EdgeLightControllerFixture : public ::testing::Test
{

public:
    BjDataPackage m_bjDataPackage;
    EdgeLightController m_edgeLightCtrl;

    EdgeLightControllerFixture() : m_edgeLightCtrl(0x40, 1000)
    {

    }

protected:
    virtual void SetUp()
    {
        // Default values of the package
        m_bjDataPackage.setRestRequest(RestRequest::E_GET_REQUEST);
        m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_EDGE_LIGHT);
        m_bjDataPackage.setIpAddr(0);
    }

    virtual void TearDown()
    {

    }
};

TEST_F(EdgeLightControllerFixture, validateNullInput)
{
    uint8_t payload[100] = {};
    uint8_t payloadBytesRead = 10;

    // Get the payload
    EXPECT_EQ(-1, m_edgeLightCtrl.read(NULL, &payloadBytesRead));
    EXPECT_EQ(0, m_edgeLightCtrl.read(payload, &payloadBytesRead));
}

TEST_F(EdgeLightControllerFixture, calcPwmOnTimeMin)
{
    uint8_t rgbVal = 0;

    EXPECT_EQ(m_edgeLightCtrl.calcPwmOnTime(rgbVal), 0);
}

TEST_F(EdgeLightControllerFixture, calcPwmOnTimeMax)
{
    uint8_t rgbVal = 255;

    EXPECT_EQ(m_edgeLightCtrl.calcPwmOnTime(rgbVal), 4096);
}

TEST_F(EdgeLightControllerFixture, calcPwmOnTime156)
{
    uint8_t rgbVal = 156;

    EXPECT_EQ(m_edgeLightCtrl.calcPwmOnTime(rgbVal), 2505);
}

TEST_F(EdgeLightControllerFixture, setbRgbLightNullInput)
{
    uint16_t pwmValuesBuffer[3] = {};
    Ws2812Led led(0, 0, 0, 0);

    EXPECT_EQ(m_edgeLightCtrl.setRgbLight(NULL, pwmValuesBuffer), BJ_FAILURE);
    EXPECT_EQ(m_edgeLightCtrl.setRgbLight(&led, NULL), BJ_FAILURE);
    EXPECT_EQ(m_edgeLightCtrl.setRgbLight(NULL, NULL), BJ_FAILURE);
}

TEST_F(EdgeLightControllerFixture, setbRgbLightValidInput)
{
    uint16_t pwmValuesBuffer[3] = {};
    Ws2812Led led(0, 255, 120, 50);

    EXPECT_EQ(m_edgeLightCtrl.setRgbLight(&led, pwmValuesBuffer), BJ_SUCCESS);
    EXPECT_EQ(pwmValuesBuffer[0], 4096);
    EXPECT_EQ(pwmValuesBuffer[1], 1927);
    EXPECT_EQ(pwmValuesBuffer[2], 803);
}

TEST_F(EdgeLightControllerFixture, SetandGetSingle)
{
    uint8_t payloadZero[4] = {0, 100, 200, 123};
    uint8_t payloadOne[4] = {1, 55, 120, 241};
    uint8_t bytesRead = 0;

    EXPECT_EQ(m_edgeLightCtrl.write(payloadZero, 4), BJ_SUCCESS);

    // Get the ID again
    EXPECT_EQ(m_edgeLightCtrl.read(payloadZero, &bytesRead), BJ_SUCCESS);
    EXPECT_EQ(bytesRead, 4);

    // Check individual values
    EXPECT_EQ(payloadZero[1], 100);
    EXPECT_EQ(payloadZero[2], 200);
    EXPECT_EQ(payloadZero[3], 123);

    EXPECT_EQ(m_edgeLightCtrl.write(payloadOne, 4), BJ_SUCCESS);

    // Get the ID again
    EXPECT_EQ(m_edgeLightCtrl.read(payloadOne, &bytesRead), BJ_SUCCESS);
    EXPECT_EQ(bytesRead, 4);

    // Check individual values
    EXPECT_EQ(payloadOne[1], 55);
    EXPECT_EQ(payloadOne[2], 120);
    EXPECT_EQ(payloadOne[3], 241);
}

TEST_F(EdgeLightControllerFixture, SetAndGetInvalidId)
{
    uint8_t payload[4] = {0, 100, 200, 123};
    uint8_t bytesRead = 0;

    EXPECT_EQ(m_edgeLightCtrl.write(payload, 4), BJ_SUCCESS);

    // Set invalid id
    payload[0] = 3;

    // Get the ID again
    EXPECT_EQ(m_edgeLightCtrl.read(payload, &bytesRead), BJ_FAILURE);
    EXPECT_EQ(bytesRead, 0);
}

TEST_F(EdgeLightControllerFixture, SetAndGetAll)
{
    uint8_t payload[RGB_WS_LED_SIZE * NUM_EDGE_LEDS] = {0};
    uint8_t tmpPayload[4] = {};
    uint8_t bytesRead = 0;
    uint8_t baseIdx = 0;

    for(int i = 0; i < NUM_EDGE_LEDS; i++)
    {
        // Set the ID
        tmpPayload[0] = i;

        // Set random RGB values
        tmpPayload[1] = i + 1;
        tmpPayload[2] = i + 5;
        tmpPayload[3] = i + 6;

        EXPECT_EQ(m_edgeLightCtrl.write(tmpPayload, 4), BJ_SUCCESS);

    }

    // Get all the values
    payload[0] = RGB_GET_ALL_VALUES_REQ;
    EXPECT_EQ(m_edgeLightCtrl.read(&payload[0], &bytesRead), BJ_SUCCESS);
    EXPECT_EQ(bytesRead, NUM_EDGE_LEDS * RGB_WS_LED_SIZE);

    for(int i = 0; i < NUM_EDGE_LEDS; i++)
    {
        baseIdx = i * RGB_WS_LED_SIZE;

        EXPECT_EQ(payload[baseIdx], i);
        EXPECT_EQ(payload[baseIdx + RGB_RED_BUF_POS],   i + 1);
        EXPECT_EQ(payload[baseIdx + RGB_GREEN_BUF_POS], i + 5);
        EXPECT_EQ(payload[baseIdx + RGB_BLUE_BUF_POS],  i + 6);


    }
}
