/*
 * BjConfiguration.h
 *
 *  Created on: 14 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_
#define LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_

#include <stdint.h>


//-------------------------------------------------------------------------------//
// UNIT TESTING
//-------------------------------------------------------------------------------//
//#define UNIT_TESTING
//#define DEBUG
//-------------------------------------------------------------------------------//
// DEBUG LOG
//-------------------------------------------------------------------------------//
#define SEND_DEBUG_BJ_PKG 0

//---------------------------------------------------------------------------------------------------
// ARDUINO ON BOARD
//---------------------------------------------------------------------------------------------------
const uint8_t BOARD_ALIVE_LED_PIN_O = 13;

//---------------------------------------------------------------------------------------------------
// LED LIGHTS
//---------------------------------------------------------------------------------------------------
const uint8_t NUM_OF_CUPS  = 1u;
const uint8_t LEDS_PER_CUP = 1u;

// WS2812 Data Pin
const uint8_t WS2812_DATA_PIN_O = 6;
//---------------------------------------------------------------------------------------------------
// UART INTERFACE
//---------------------------------------------------------------------------------------------------
const uint32_t UART_BAUD_RATE = 9600;

//---------------------------------------------------------------------------------------------------
// UNIT CONTROLLER
//---------------------------------------------------------------------------------------------------
const uint8_t ARDUINO_CTRL_ID = 1;

//---------------------------------------------------------------------------------------------------
// SUCCESS / FAILURE Defines
//---------------------------------------------------------------------------------------------------
const int16_t BJ_SUCCESS = 0;
const int16_t BJ_FAILURE = -1;

#endif /* LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_ */