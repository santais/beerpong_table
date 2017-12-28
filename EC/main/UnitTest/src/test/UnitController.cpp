/*
 * SystemUnitTest.cpp
 *
 *  Created on: 16 Dec 2017
 *      Author: markpovlsen
 */

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"

#include <string.h>
#include <vector>

#include "BjDataPackage.h"
#include "UnitController.h"
#include "IMessageController.h"
#include "SystemController.h"
#include "BjConfiguration.h"

using namespace BjDataPackageDefines;
using namespace Controller;

class StubMsgCtrl : public IMessageController
{
public:
    StubMsgCtrl() {};
    virtual bool dataAvailable() {return true;}
    virtual int readByte() {return 0;}
    virtual int readBytes(unsigned char* buffer, unsigned int bytesToRead) {return 0;}
    virtual int write(unsigned char* payload, unsigned int payloadSize) {return 0;}
};

class UnitControllerFixture : public ::testing::Test
{

public:
    StubMsgCtrl m_stubMsgCtrl;
    UnitController m_unitCtrl;
    BjDataPackage m_bjDataPackage;

    UnitControllerFixture() : m_stubMsgCtrl(), m_unitCtrl(m_stubMsgCtrl)
    {

    }

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

TEST_F(UnitControllerFixture, AddSingleController)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));
}

TEST_F(UnitControllerFixture, AddAllControllers)
{
    std::vector<SystemController> systemCtrl(m_unitCtrl.MAX_NUM_CONTROLLERS);

    for(int i = 0; i < m_unitCtrl.MAX_NUM_CONTROLLERS; i++)
    {
        EXPECT_TRUE(m_unitCtrl.addController(systemCtrl[i], TargetModule::E_MODULE_SYSTEM));
    }

    EXPECT_FALSE(m_unitCtrl.addController(systemCtrl[0], TargetModule::E_MODULE_SYSTEM));
}

TEST_F(UnitControllerFixture, ValidateSendCtrlPkg)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    EXPECT_TRUE(m_unitCtrl.sendPkgContentToController(m_bjDataPackage));
}

TEST_F(UnitControllerFixture, InvalidSendCtrlPkg)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_LIGHT);
    EXPECT_FALSE(m_unitCtrl.sendPkgContentToController(m_bjDataPackage));
}

TEST_F(UnitControllerFixture, InvalidSendCtrlPkgCommand)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    m_bjDataPackage.setRestRequest(RestRequest::E_NOTIFY_REQUEST);
    EXPECT_FALSE(m_unitCtrl.sendPkgContentToController(m_bjDataPackage));
}

TEST_F(UnitControllerFixture, RunInvalidRestRequest)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    m_bjDataPackage.setTargetModule(TargetModule::E_MODULE_LIGHT);
    EXPECT_FALSE(m_unitCtrl.run());
}

TEST_F(UnitControllerFixture, ValidateValidSensorCtrlCheck)
{
    SystemController systemCtrl;
    SensorController sensorCtrl(0, 0, 0, 0);

    EXPECT_TRUE(m_unitCtrl.addController(sensorCtrl, TargetModule::E_MODULE_SENSOR));
    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    EXPECT_TRUE(m_unitCtrl.checkSensorInput());
}

TEST_F(UnitControllerFixture, ValidateInvalidSensorCtrlCheck)
{
    SystemController systemCtrl;

    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));

    EXPECT_FALSE(m_unitCtrl.checkSensorInput());
}

TEST_F(UnitControllerFixture, ValidateLightSettings)
{
    SystemController systemCtrl;
    SensorController sensorCtrl(0, 0, 0, 0);
    LightController lightCtrl(0);

    EXPECT_TRUE(m_unitCtrl.addController(sensorCtrl, TargetModule::E_MODULE_SENSOR));
    EXPECT_TRUE(m_unitCtrl.addController(systemCtrl, TargetModule::E_MODULE_SYSTEM));
    EXPECT_TRUE(m_unitCtrl.addController(lightCtrl, TargetModule::E_MODULE_LIGHT));

    // Change the sensor readings to trigger a light set
    sensorCtrl.testSensorReadings[0] = 1;

    EXPECT_TRUE(m_unitCtrl.checkSensorInput());
}
