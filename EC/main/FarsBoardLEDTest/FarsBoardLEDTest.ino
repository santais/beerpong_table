
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
//LightController* ptrLightCtrl;
//UARTMessageController* ptrUartMsgCtrl;
//UnitController* ptrUnitCtrl;

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
        digitalWrite(13, HIGH);
        ui8Shift = 1;
    }
    else if(ui8Shift == 1)
    {
        digitalWrite(13, LOW);
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
    pinMode(13, OUTPUT);

//    // Initialize ALIVE LED timer
//    Timer1.initialize(ALIVE_LED_ISR_FREQ_US);
//    Timer1.attachInterrupt(aliveLedISR);
//
//    // Enable serial printout
//    Serial.begin(UART_BAUD_RATE);

    // Setup the controllers
//    ptrLightCtrl = new LightController(WS2812_DATA_PIN_O);

//    // Unit controller
//    ptrUartMsgCtrl = new UARTMessageController(UART_BAUD_RATE);
//    ptrUnitCtrl = new UnitController(*ptrUartMsgCtrl);

    // Add the controllers
//    initVal = ptrUnitCtrl->addController(*ptrLightCtrl, TargetModule::E_MODULE_LIGHT);

}

/*********************************************************************************/
// Description:
//
// @param
// @return
/*********************************************************************************/
void loop()
{
//    ptrUnitCtrl->run();
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
    delay(10);
}


