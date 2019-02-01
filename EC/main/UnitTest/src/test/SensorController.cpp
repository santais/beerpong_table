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
#include "SensorController.h"

using namespace BjDataPackageDefines;
using namespace Controller;

class SensorControllerFixture : public ::testing::Test
{

public:
    BjDataPackage m_bjDataPackage;


protected:
    virtual void SetUp()
    {

        // Default values of the package
        m_bjDataPackage.setRestRequest(RestRequest::E_GET_REQUEST);
        m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_SENSOR);
        m_bjDataPackage.setIpAddr(0);
    }

    virtual void TearDown()
    {

    }


};

TEST_F(SensorControllerFixture, CheckOnSensorChange)
{
    SensorController m_sensorCtrl(0, 0, 0, 0);

    // Read the sensor values
    uint8_t sensorValues[NUM_OF_SENSORS] = {};
    uint8_t newSensorValues[NUM_OF_SENSORS] = {};
    uint8_t bytesWritten = 0;

    m_sensorCtrl.read(sensorValues, &bytesWritten);

    // Set the sensor values
    m_sensorCtrl.testSensorReadings[0] = 1;

    m_sensorCtrl.read(newSensorValues, &bytesWritten);

    EXPECT_FALSE(0 == std::memcmp(sensorValues, newSensorValues, sizeof(sensorValues)));
}

TEST_F(SensorControllerFixture, CheckSensorPattern)
{
    SensorController m_sensorCtrl(0, 0, 0, 0);
    uint8_t bytesWritten = 0;

    uint8_t sensorPattern[NUM_OF_SENSORS] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
    uint8_t sensorReading[NUM_OF_SENSORS] = {};

    std::copy(std::begin(sensorPattern), std::end(sensorPattern), std::begin(m_sensorCtrl.testSensorReadings));

    m_sensorCtrl.read(sensorReading, &bytesWritten);

    EXPECT_TRUE(0 == std::memcmp(sensorPattern, sensorReading, sizeof(sensorPattern)));
}

TEST_F(SensorControllerFixture, CheckBytesWritten)
{
    SensorController m_sensorCtrl(0, 0, 0, 0);

    uint8_t sensorValues[NUM_OF_SENSORS] = {};
    uint8_t bytesWritten = 0;
    m_sensorCtrl.read(sensorValues, &bytesWritten);

    EXPECT_EQ(NUM_OF_SENSORS, bytesWritten);
}


