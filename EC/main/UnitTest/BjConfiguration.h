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
#define UNIT_TESTING

//-------------------------------------------------------------------------------//
// DEBUG LOG
//-------------------------------------------------------------------------------/
#define SEND_DEBUG_BJ_PKG 0

//---------------------------------------------------------------------------------------------------
// ARDUINO ON BOARD
//---------------------------------------------------------------------------------------------------
const uint8_t BOARD_ALIVE_LED_PIN_O = 13;

//---------------------------------------------------------------------------------------------------
// SENSORS
//---------------------------------------------------------------------------------------------------
const uint8_t NUM_OF_SHIFT_REG          = 4u;
const uint8_t BITS_PER_SHIFT_REG        = 8u;
const uint8_t SN74HC165_CLK_PIN_O       = 12;
const uint8_t SN74HC165_CLK_INH_PIN_O   = 9;
const uint8_t SN74HC165_SDLD_PIN_O      = 8;
const uint8_t SN74HC165_SER_PIN_I       = 11;

//---------------------------------------------------------------------------------------------------
// LED LIGHTS
//---------------------------------------------------------------------------------------------------
const uint8_t NUM_OF_CUPS  = 22u;
const uint8_t LEDS_PER_CUP = 16u;

// WS2812 Data Pin
const uint8_t WS2812_DATA_PIN_O = 6;
//---------------------------------------------------------------------------------------------------
// UART INTERFACE
//---------------------------------------------------------------------------------------------------
const uint32_t UART_BAUD_RATE = 115200;

//---------------------------------------------------------------------------------------------------
// UNIT CONTROLLER
//---------------------------------------------------------------------------------------------------
const uint8_t ARDUINO_CTRL_ID = 1;

#endif /* LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_ */
