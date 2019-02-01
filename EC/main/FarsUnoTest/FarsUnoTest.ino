
//--------------------------------------------------------------------------------//
//  Name:
//  Date: 04-10-16
//  Author: Mark Ulletved Povlsen
//
//  Description:
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES
//-------------------------------------------------------------------------------//

// System Files
#include "stdint.h"

// Arduino Libraries
#include "LightController.h"
#include "UARTMessageController.h"
#include "Logger.h"
#include "BjDataPackageDefines.h"
#include "BjConfiguration.h"
#include "libraries/TimerOne/TimerOne.h"
#include "UnitController.h"

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
static LightController* ptrLightCtrl;
static UARTMessageController* ptrUartMsgCtrl;
static UnitController* ptrUnitCtrl;

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

    pinMode(BOARD_ALIVE_LED_PIN_O, OUTPUT);

    // Initialize ALIVE LED timer
    Timer1.initialize(ALIVE_LED_ISR_FREQ_US);
    Timer1.attachInterrupt(aliveLedISR);

    // Enable serial printout
//    Serial.begin(UART_BAUD_RATE);

    // Setup the controllers
    ptrLightCtrl = new LightController(WS2812_DATA_PIN_O);

    // Unit controller
    ptrUartMsgCtrl = new UARTMessageController(UART_BAUD_RATE);
    ptrUnitCtrl = new UnitController(*ptrUartMsgCtrl);
//
//    // Add the controllers
    ptrUnitCtrl->addController(*ptrLightCtrl, TargetModule::E_MODULE_LIGHT);

    BJBP_LOG_INFO("Controller setup");
//
//    if(initVal)
//    {
//        BJBP_LOG_INFO("Arduino successfully setup\n");
//    }
//    else
//    {
//        BJBP_LOG_ERR("Arduino setup failed");
//    }
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

