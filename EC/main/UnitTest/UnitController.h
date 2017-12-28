//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef NETWORKCOMM_H_
#define NETWORKCOMM_H_

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include <stdint.h>

#include "BjConfiguration.h"
#include "BjDataPackageDefines.h"
#include "IMessageController.h"
#include "BjDataPackage.h"
#include "IRestController.h"
#include "LightController.h"
#include "SensorController.h"

using namespace BjDataPackageDefines;

namespace Controller
{


//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

class UnitController
{
#ifdef UNIT_TESTING
public:
#endif
    static const uint8_t MAX_NUM_CONTROLLERS = 5;

public:
    virtual ~UnitController();
    UnitController(IMessageController& messageCtrl);

    bool run();
    bool addController(IRestController& restController, TargetModule module);

#ifndef UNIT_TESTING
private:
#else
public:
#endif
    // Private map container class
    class ControllerObj
    {
    public:
        ControllerObj(IRestController& restCtrl, TargetModule module) : m_ptrRestCtrl(&restCtrl),
            m_module(module) {}

        TargetModule getTargetModule() {return m_module;}
        IRestController* getRestCtrlRef() {return m_ptrRestCtrl;}

    private:
        IRestController* m_ptrRestCtrl;
        TargetModule m_module;
    };

    // Internal functions
    IRestController* getRestCtrlRef(TargetModule targetModule);
    bool sendPkgContentToController(BjDataPackage& bjDataPkg);
    void returnPingRequest(BjDataPackage& bjDataPkg);
    bool checkInputData();
    bool checkSensorInput();
    bool handleSensorUpdate(uint8_t* sensorReadings, uint8_t numOfSensors);
    bool setLightsFromSensorVals(uint8_t* sensorReadings, uint8_t numOfSensors);

    // Internal member variables
    IMessageController* m_ptrMessageCtrl;
    ControllerObj* m_controllerObjs[MAX_NUM_CONTROLLERS];

    // Temporary data containers
    uint8_t m_tmpPkgBuffer[MAX_PACKAGE_SIZE];
    uint8_t m_tmpLightCtrlBuffer[NUM_OF_CUPS * RGB_WS_LED_SIZE];
    uint8_t m_sensorValues[NUM_OF_SENSORS];
};

};

#endif /* NETWORKCOMM_H_ */
