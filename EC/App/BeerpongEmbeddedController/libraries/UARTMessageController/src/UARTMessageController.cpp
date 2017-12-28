/*
 * UARTMessageController.cpp
 *
 *  Created on: 16 Dec 2017
 *      Author: markpovlsen
 */

#include "UARTMessageController.h"
#include "Arduino.h"

namespace Controller
{

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::UARTMessageController
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
UARTMessageController::UARTMessageController(int32_t baudRate)
{
    Serial.begin(baudRate);
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::~UARTMessageController
// Description: TODO
// Inputs:
// Output: TODO
// Return:
/**************************************************************/
UARTMessageController::~UARTMessageController()
{
    // TODO Auto-generated destructor stub
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::dataAvailable
// Description: TODO
// Inputs:
// Output: TODO
// Return: bool
/**************************************************************/
bool UARTMessageController::dataAvailable()
{
    return(Serial.available());
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::readByte
// Description: TODO
// Inputs:
// Output: TODO
// Return: int
/**************************************************************/
int UARTMessageController::readByte()
{
    return(Serial.read());
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::readBytes
// Description: TODO
// Inputs: uint8_t*
// Output: TODO
// Return: int
/**************************************************************/
int UARTMessageController::readBytes(unsigned char* buffer, unsigned int bytesToRead)
{
    if(buffer != NULL)
    {
        return(Serial.readBytes(buffer, bytesToRead));
    }
    else
    {
        return 0;
    }
}

/**************************************************************/
// Date: 16 Dec 2017
// Function: UARTMessageController::write
// Description: TODO
// Inputs: uint8_t*, uint16_t
// Output: TODO
// Return: int
/**************************************************************/
int UARTMessageController::write(unsigned char* payload, unsigned int payloadSize)
{
    // Write to the target
    int writtenBytes = Serial.write(payload, payloadSize);

    // Flush the buffer
    Serial.flush();

    // Add a delay to ensure the buffer is flushed before the next call
    delay(10);

    return writtenBytes;
}

} /* namespace Controller */
