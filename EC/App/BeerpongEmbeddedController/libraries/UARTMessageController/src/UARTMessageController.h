/*
 * UARTMessageController.h
 *
 *  Created on: 16 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_UARTMESSAGECONTROLLER_SRC_UARTMESSAGECONTROLLER_H_
#define LIBRARIES_UARTMESSAGECONTROLLER_SRC_UARTMESSAGECONTROLLER_H_

#include <IMessageController.h>

namespace Controller
{

class UARTMessageController: public IMessageController
{
public:
    UARTMessageController(int32_t baudRate);
    virtual ~UARTMessageController();

    virtual bool dataAvailable();
    virtual int readByte();
    virtual int readBytes(unsigned char* buffer, unsigned int bytesToRead);
    virtual int write(unsigned char* payload, unsigned int payloadSize);
};

} /* namespace Controller */

#endif /* LIBRARIES_UARTMESSAGECONTROLLER_SRC_UARTMESSAGECONTROLLER_H_ */
