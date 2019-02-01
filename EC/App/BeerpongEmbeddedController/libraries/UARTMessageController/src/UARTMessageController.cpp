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

UARTMessageController::UARTMessageController(int32_t baudRate)
{
    Serial.begin(baudRate);
}

UARTMessageController::~UARTMessageController()
{
    // TODO Auto-generated destructor stub
}

bool UARTMessageController::dataAvailable()
{
    return(Serial.available());
}

int UARTMessageController::readByte()
{
    return(Serial.read());
}

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

int UARTMessageController::write(unsigned char* payload, unsigned int payloadSize)
{
    int writtenBytes = Serial.write(payload, payloadSize);

    Serial.flush();

    delay(10);

    return writtenBytes;
}

} /* namespace Controller */
