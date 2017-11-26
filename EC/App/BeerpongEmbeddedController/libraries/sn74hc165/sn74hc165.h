
//------------------------------------------------------------//
//	Name: sn74hc595.h
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: SN74HC165 8 bit Shift Register driver designed
//  			 for Raspberry Pi. Uses wiringPi for GPIO drive
//
//
//-----------------------------------------------------------//

#ifndef SN74HC165_H_
#define SN74HC165_H_

//------------------------------------------------------------
// INCLUDES 
//------------------------------------------------------------
#include <stdint.h>

//------------------------------------------------------------
// FUNCTIONS
//------------------------------------------------------------
int8_t SN74HC165_Init(const uint8_t ui8ClkPin, const uint8_t ui8ClkInhPin, 
	const uint8_t ui8ShLdPin, const uint8_t ui8SerPin);
void   SN74HC165_Read(uint8_t *uip8Buffer, const uint8_t ui8BufSize);

#endif /* SN74HC165_H_ */


