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
    SensorController m_sensorCtrl;

    SensorControllerFixture() : m_sensorCtrl(0, 0, 0, 0)
    {

    }

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



