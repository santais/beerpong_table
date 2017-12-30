
//--------------------------------------------------------------------------------//
//	Name:
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//
//	Description:
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES
//-------------------------------------------------------------------------------//

// System Files
#include "stdint.h"

// Arduino Libraries
#include <TimerOne.h>

// User Libraries
#include "sn74hc165.h"
#include "SensorController.h"
#include "LightController.h"
#include "SystemController.h"
#include "UARTMessageController.h"
#include "Logger.h"
#include "BjDataPackageDefines.h"
#include "BjConfiguration.h"
#include "UnitController.h"
#include "EdgeLightController.h"

using namespace Controller;

//-------------------------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------------------------//
const uint32_t HZ_TO_US_MULTIPLIER = 1000000;
const uint32_t ALIVE_LED_ISR_FREQ_HZ = 1;
const uint32_t ALIVE_LED_ISR_FREQ_US = (uint32_t) (((double) (1/ALIVE_LED_ISR_FREQ_HZ)) * HZ_TO_US_MULTIPLIER);

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STATIC VARIABLES
//-------------------------------------------------------------------------------//
LightController* ptrLightCtrl;
SystemController* ptrSystemCtrl;
SensorController* ptrSensorCtrl;
EdgeLightController* ptrEdgeLightCtrl;
UARTMessageController* ptrUartMsgCtrl;
UnitController* ptrUnitCtrl;

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

void aliveLedISR(void)
{
    static uint8_t ui8Shift = 0;

    if(ui8Shift == 0)
    {
        digitalWrite(BOARD_ALIVE_LED_PIN_O, HIGH);
        ui8Shift = 1;
    }
    else if(ui8Shift == 1)
    {
        digitalWrite(BOARD_ALIVE_LED_PIN_O, LOW);
        ui8Shift = 0;
    }
}

/*********************************************************************************/
// Description:
//
// @param
// @return
/*********************************************************************************/
void setup()
{
    bool initVal = true;

    pinMode(BOARD_ALIVE_LED_PIN_O, OUTPUT);

    // Initialize ALIVE LED timer
    Timer1.initialize(ALIVE_LED_ISR_FREQ_US);
    Timer1.attachInterrupt(aliveLedISR);

    pinMode(7, INPUT);

    // Enable serial printout
    Serial.begin(115200);

    // Setup the controllers
    ptrLightCtrl = new LightController(WS2812_DATA_PIN_O);
    ptrEdgeLightCtrl = new EdgeLightController(I2C_PWM_MOD_ADDR, I2C_PWM_FREQ_HZ);
    ptrSystemCtrl = new SystemController();
    ptrSensorCtrl = new SensorController(SN74HC165_CLK_PIN_O, SN74HC165_CLK_INH_PIN_O, SN74HC165_SDLD_PIN_O, SN74HC165_SER_PIN_I);
    ptrUartMsgCtrl = new UARTMessageController(UART_BAUD_RATE);
    ptrUnitCtrl = new UnitController(*ptrUartMsgCtrl);

    // Add the controllers
    initVal = ptrUnitCtrl->addController(*ptrLightCtrl, TargetModule::E_MODULE_LIGHT);
    initVal = ptrUnitCtrl->addController(*ptrEdgeLightCtrl, TargetModule::E_MODULE_EDGE_LIGHT);
    initVal &= ptrUnitCtrl->addController(*ptrSystemCtrl, TargetModule::E_MODULE_SYSTEM);
    initVal &= ptrUnitCtrl->addController(*ptrSensorCtrl, TargetModule::E_MODULE_SENSOR);

    if(initVal)
    {
        BJBP_LOG_INFO("Arduino successfully setup\n");
    }
    else
    {
        BJBP_LOG_ERR("Arduino setup failed");
    }
}

/*********************************************************************************/
// Description:
//
// @param
// @return
/*********************************************************************************/
void loop()
{
    ptrUnitCtrl->run();

    delay(10);
}

