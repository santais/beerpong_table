/*
 * BjConfiguration.h
 *
 *  Created on: 14 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_
#define LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_

#include <stdint.h>
#include <math.h>

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
// SENSORS
//---------------------------------------------------------------------------------------------------
const uint8_t NUM_OF_SHIFT_REG          = 3u;
const uint8_t BITS_PER_SHIFT_REG        = 8u;
const uint8_t SN74HC165_CLK_PIN_O       = 12;
const uint8_t SN74HC165_CLK_INH_PIN_O   = 9;
const uint8_t SN74HC165_SDLD_PIN_O      = 8;
const uint8_t SN74HC165_QH_PIN          = 11;

//---------------------------------------------------------------------------------------------------
// LED LIGHTS
//---------------------------------------------------------------------------------------------------
const uint8_t NUM_OF_CUPS  = 20u;
const uint8_t LEDS_PER_CUP = 16u;
const uint8_t SHIFT_REG_REQUIRED = ceil((double) NUM_OF_CUPS / BITS_PER_SHIFT_REG);
const uint8_t SENSOR_READINGS_REQUIRED  = SHIFT_REG_REQUIRED * BITS_PER_SHIFT_REG;


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

//---------------------------------------------------------------------------------------------------
// EDGE LIGHT CONTROLLER
//---------------------------------------------------------------------------------------------------
const uint8_t I2C_PWM_MOD_ADDR = 0x40;
const uint16_t I2C_PWM_FREQ_HZ = 1000;
const uint8_t NUM_EDGE_LEDS    = 2; // 1 on each side of the table

//---------------------------------------------------------------------------------------------------
// SUCCESS / FAILURE Defines
//---------------------------------------------------------------------------------------------------
const int16_t BJ_SUCCESS = 0;
const int16_t BJ_FAILURE = -1;

#endif /* LIBRARIES_COMMONTYPES_SRC_BJCONFIGURATION_H_ */
