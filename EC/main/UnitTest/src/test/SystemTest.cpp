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
#include "SystemController.h"

using namespace BjDataPackageDefines;
using namespace Controller;

class SystemControllerFixture : public ::testing::Test
{

public:
    BjDataPackage m_bjDataPackage;
    SystemController m_systemCtrl;

protected:
    virtual void SetUp()
    {
        // Default values of the package
        m_bjDataPackage.setRestRequest(RestRequest::E_GET_REQUEST);
        m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_SYSTEM);
        m_bjDataPackage.setIpAddr(0);
    }

    virtual void TearDown()
    {

    }

};

TEST_F(SystemControllerFixture, ValidateNullInput)
{
    uint8_t payload[100] = {};
    uint8_t payloadBytesRead = 10;

    // Get the payload
    EXPECT_EQ(-1, m_systemCtrl.read(NULL, &payloadBytesRead));;
    EXPECT_EQ(0, m_systemCtrl.read(payload, &payloadBytesRead));
}

TEST_F(SystemControllerFixture, InvalidInput)
{
    uint8_t payload = 4;
    uint8_t payloadSize = 1;

    // Set the payload
    EXPECT_EQ(-1, m_systemCtrl.write(&payload, payloadSize));
}

TEST_F(SystemControllerFixture, ValidInput)
{
    uint8_t payload = 3;
    uint8_t payloadSize = 1;

    // Set the payload
    EXPECT_EQ(0, m_systemCtrl.write(&payload, payloadSize));
}

TEST_F(SystemControllerFixture, ValidatePutAndGet)
{
    uint8_t payload = (uint8_t) ControllerState::E_STATE_AUTOMATIC;
    uint8_t payloadSize = 1;
    uint8_t payloadBytesRead = 0;

    // Set the payload
    EXPECT_EQ(0, m_systemCtrl.write(&payload, payloadSize));

    // Get the payload
    EXPECT_EQ(0, m_systemCtrl.read(&payload, &payloadBytesRead));
    EXPECT_EQ(1, payloadBytesRead);
    EXPECT_EQ(ControllerState::E_STATE_AUTOMATIC, (ControllerState) payload);
}
