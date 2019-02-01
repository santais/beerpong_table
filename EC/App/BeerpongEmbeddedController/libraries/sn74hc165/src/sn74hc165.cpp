//--------------------------------------------------------------------------------//
//  Name: sn74hc595.c
//  Date: 04-10-16
//  Author: Mark Ulletved Povlsen
//  
//  Description: SN74HC595 8 bit Shift Register driver designed for Raspberry Pi. 
//               Uses wiringPi for GPIO drive
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include "sn74hc165.h"  
#include "Arduino.h"    // pinMode and digitalWrite
#include "BjConfiguration.h"
#include <stdio.h>      // Printf
#include <stdint.h>     // data types

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define PULSE_WIDTH_USEC     ((uint32_t) 15) // 1 MHz.

#define SHIFT_REG_BIT_RES ((uint8_t) 8)
#define SHIFT_MSB         ((uint8_t) 0x80)

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//
static struct 
{
    uint8_t ui8Clk;     // Clock Pin
    uint8_t ui8ClkInh;  // Clock INH pin
    uint8_t ui8SdLd;    // Latch Pin
    uint8_t ui8Qh;      // data Pin
} stShiftRegPins;

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// FUNCTIONS PROTOTYPES
//-------------------------------------------------------------------------------//
static void vPulsePin(uint8_t ui8Pin, uint32_t ui32DelayUs);

//-------------------------------------------------------------------------------//
// FUNCTIONS
//-------------------------------------------------------------------------------//

int8_t SN74HC165_Init(const uint8_t ui8ClkPin, const uint8_t ui8ClkInhPin, 
    const uint8_t ui8ShLdPin, const uint8_t ui8QhPin)
{
    uint8_t ui8Return = 1;
    
    // Set the Data Pins
    stShiftRegPins.ui8Clk    = ui8ClkPin;
    stShiftRegPins.ui8ClkInh = ui8ClkInhPin;
    stShiftRegPins.ui8SdLd   = ui8ShLdPin;
    stShiftRegPins.ui8Qh     = ui8QhPin;

    // Set the pin Modes
    pinMode(stShiftRegPins.ui8Clk, OUTPUT);
    pinMode(stShiftRegPins.ui8ClkInh, OUTPUT);
    pinMode(stShiftRegPins.ui8SdLd, OUTPUT);
    pinMode(stShiftRegPins.ui8Qh, INPUT);

    // Reset all pinmode states
    digitalWrite(stShiftRegPins.ui8Clk, LOW);
    digitalWrite(stShiftRegPins.ui8ClkInh, LOW);
    digitalWrite(stShiftRegPins.ui8SdLd, LOW);

    return ui8Return;
}

void SN74HC165_Read(uint8_t *uip8Buffer, uint8_t ui8BufSize)
{
    uint8_t ui8ShiftRegIdx = 0;
    int8_t i8Bits = 0;
    uint8_t ui8Data = 0;    

    // Set the latch bit low, so no the memory register is cleared
    digitalWrite(stShiftRegPins.ui8SdLd, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(stShiftRegPins.ui8SdLd, HIGH);

    // Shift 8 bits at a time to the shift register
    for(ui8ShiftRegIdx = 0; ui8ShiftRegIdx < ui8BufSize; ui8ShiftRegIdx++)
    {
        // Clear byte
        uip8Buffer[ui8ShiftRegIdx] = 0;
        ui8Data = 0;

        // Shift the bits in one bit a time. Every rising clock of SrClk
        // will shift the next bit in
        for(i8Bits = 0; i8Bits < SHIFT_REG_BIT_RES; i8Bits++)
        {
            ui8Data = digitalRead(stShiftRegPins.ui8Qh);
            
#ifdef DEBUG
            Serial.print(ui8Data);
#endif

            uip8Buffer[ui8ShiftRegIdx] |= (ui8Data << ((SHIFT_REG_BIT_RES - 1) - i8Bits));

            // Pulse Clock to 
            vPulsePin(stShiftRegPins.ui8Clk, PULSE_WIDTH_USEC);
        }

    }

#ifdef DEBUG
    Serial.println();
#endif

}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return 1 on success, -1 on failure
/*********************************************************************************/
void vPulsePin(uint8_t ui8Pin, uint32_t ui32DelayUs)
{
    digitalWrite(ui8Pin, HIGH);
    delayMicroseconds(ui32DelayUs);
    digitalWrite(ui8Pin, LOW);
    delayMicroseconds(ui32DelayUs);
}

