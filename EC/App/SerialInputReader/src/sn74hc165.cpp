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
#include <stdio.h>      // Printf
#include <stdint.h>     // data types

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//
#define defFREQ_MICRO_SECS   ((uint32_t) 1) // 1 MHz.

#define defSHIFT_REG_BIT_RES ((uint8_t) 8)
#define defSHIFT_MSB         ((uint8_t) 0x80)

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
    uint8_t ui8Ser;     // data Pin
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

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return 1 on success, -1 on failure
/*********************************************************************************/
int8_t SN74HC165_Init(const uint8_t ui8ClkPin, const uint8_t ui8ClkInhPin, 
    const uint8_t ui8ShLdPin, const uint8_t ui8SerPin)
{
    uint8_t ui8Return = 1;
    
    // Set the Data Pins
    stShiftRegPins.ui8Clk    = ui8ClkPin;
    stShiftRegPins.ui8ClkInh = ui8ClkInhPin;
    stShiftRegPins.ui8SdLd   = ui8ShLdPin;
    stShiftRegPins.ui8Ser    = ui8SerPin;

    // Set the pin Modes
    pinMode(stShiftRegPins.ui8Clk, OUTPUT);
    pinMode(stShiftRegPins.ui8ClkInh, OUTPUT);
    pinMode(stShiftRegPins.ui8SdLd, OUTPUT);
    pinMode(stShiftRegPins.ui8Ser, INPUT);

    // Reset all pinmode states
    digitalWrite(stShiftRegPins.ui8Clk, LOW);
    digitalWrite(stShiftRegPins.ui8ClkInh, LOW);
    digitalWrite(stShiftRegPins.ui8SdLd, LOW);

    return ui8Return;
}

/*********************************************************************************/
// Description: 
//                            
// @param 
// @return 1 on success, -1 on failure
/*********************************************************************************/
void SN74HC165_Read(uint8_t *uip8Buffer, uint8_t ui8BufSize)
{
    uint8_t ui8ShiftRegIdx = 0;
    int8_t i8Bits = 0;
    uint8_t ui8Data = 0;    

    // Set the latch bit low, so no the memory register is cleared
/*
    digitalWrite(stShiftRegPins.ui8SdLd, LOW);
    digitalWrite(stShiftRegPins.ui8ClkInh, HIGH);
    delayMicroseconds(defFREQ_MICRO_SECS);
*/

    digitalWrite(stShiftRegPins.ui8ClkInh, HIGH);
    digitalWrite(stShiftRegPins.ui8SdLd, LOW);
    delayMicroseconds(defFREQ_MICRO_SECS);
    digitalWrite(stShiftRegPins.ui8SdLd, HIGH);
    digitalWrite(stShiftRegPins.ui8ClkInh, LOW);

    // Shift 8 bits at a time to the shift register
    for(ui8ShiftRegIdx = 0; ui8ShiftRegIdx < ui8BufSize; ui8ShiftRegIdx++)
    {
        // Clear byte
        uip8Buffer[ui8ShiftRegIdx] = 0;
        ui8Data = 0;

        // Shift the bits in one bit a time. Every rising clock of SrClk
        // will shift the next bit in
        for(i8Bits = 0; i8Bits < defSHIFT_REG_BIT_RES; i8Bits++)
        {
            ui8Data = digitalRead(stShiftRegPins.ui8Ser);
            
            uip8Buffer[ui8ShiftRegIdx] |= (ui8Data << ((defSHIFT_REG_BIT_RES - 1) - i8Bits));

            //uip8Buffer[ui8ShiftRegIdx] |= (digitalRead(stShiftRegPins.ui8Ser));

            // Pulse Clock to 
            vPulsePin(stShiftRegPins.ui8Clk, defFREQ_MICRO_SECS);
        }

        //Serial.printf("Read a byte %x\n", uip8Buffer[ui8ShiftRegIdx]);
    }

    digitalWrite(stShiftRegPins.ui8ClkInh, HIGH);

    // Set the latch pin high so the memory register is stored.
    //digitalWrite(stShiftRegPins.ui8SdLd, LOW);
    //digitalWrite(stShiftRegPins.ui8ClkInh, HIGH);

    //Serial.println("done read");
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

