
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
#include <sn74hc165.h>
#include <NetworkController.h>
#include <SensorController.h>
#include <LightController.h>
#include <Logger.h>

//-------------------------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------------------------//
#define defNETWORK_SSID	((char*) "oblat61saldo98farce")
#define defNETWORK_PW	((char*) "Mpnetwork2014!")

#define defWIFI_SERVER_PORT ((uint16_t) 80)

#define defHZ_TO_US_MULTIPLIER   ((uint32_t) 1000000)
#define defALIVE_LED_ISR_FREQ_HZ ((uint32_t) 1)
#define defALIVE_LED_ISR_FREQ_US ((uint32_t) (((double) (1/defALIVE_LED_ISR_FREQ_HZ)) * defHZ_TO_US_MULTIPLIER))

// Shift Register defines
#define defSN74HC165_CLK_PIN_O     ((uint8_t) 12)
#define defSN74HC165_CLK_INH_PIN_O ((uint8_t) 9)
#define defSN74HC165_SDLD_PIN_O    ((uint8_t) 8)
#define defSN74HC165_SER_PIN_I     ((uint8_t) 11)

// WS2812 Data Pin
#define defWS2812_DATA_PIN_O       ((uint8_t) 6)

#define defMULTIPLEX_FREQ_HZ       ((uint8_t) 50)

#define defBOARD_ALIVE_LED_PIN_O         ((uint8_t) 13)
#define defBOARD_WIFI_ALIVE_LED_PIN_O    ((uint8_t) 9)

#define MAIN_TASK_DELAY_MS         ((uint8_t) 10)
//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STATIC VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

void vAliveLedISR(void)
{
  static uint8_t ui8Shift = 0;

  if(ui8Shift == 0)
  {
    digitalWrite(defBOARD_ALIVE_LED_PIN_O, HIGH);
    digitalWrite(defBOARD_WIFI_ALIVE_LED_PIN_O, HIGH);
    ui8Shift = 1;
  }
  else if(ui8Shift == 1)
  {
    digitalWrite(defBOARD_ALIVE_LED_PIN_O, LOW);
    digitalWrite(defBOARD_WIFI_ALIVE_LED_PIN_O, LOW);
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
	int16_t i16Return = -1;
  stShiftRegPins_t stShiftRegPins;

  // Protocol setup
  NetworkComm_Init();

  pinMode(defBOARD_ALIVE_LED_PIN_O, OUTPUT);
  pinMode(defBOARD_WIFI_ALIVE_LED_PIN_O, OUTPUT);

  // Initialize ALIVE LED timer
  Timer1.initialize(defALIVE_LED_ISR_FREQ_US);
  Timer1.attachInterrupt(vAliveLedISR);

  // Sensor Controller setup
  stShiftRegPins.ui8ClkPin    = defSN74HC165_CLK_PIN_O;
  stShiftRegPins.ui8ClkInhPin = defSN74HC165_CLK_INH_PIN_O;
  stShiftRegPins.ui8SdLdPin   = defSN74HC165_SDLD_PIN_O;
  stShiftRegPins.ui8SerPin    = defSN74HC165_SER_PIN_I;
  SensorController_Init(&stShiftRegPins);

  // LightController Init
  LightController_Init(defWS2812_DATA_PIN_O);

  BJBP_LOG_INFO("Arduino successfully setup\n");
}

/*********************************************************************************/
// Description:
//
// @param
// @return
/*********************************************************************************/
void loop()
{
  NetworkComm_Run();

  delay(10);
}

