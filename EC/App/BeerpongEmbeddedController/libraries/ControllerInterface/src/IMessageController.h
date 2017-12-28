/*
 * IMessageController.h
 *
 *  Created on: 13 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_CONTROLLERINTERFACE_IMESSAGECONTROLLER_H_
#define LIBRARIES_CONTROLLERINTERFACE_IMESSAGECONTROLLER_H_

#include <stdint.h>

class IMessageController
{
public:
    virtual bool dataAvailable() = 0;
    virtual int readByte() = 0;
    virtual int readBytes(unsigned char* buffer, unsigned int bytesToRead) = 0;
    virtual int write(unsigned char* payload, unsigned int payloadSize) = 0;
};



#endif /* LIBRARIES_CONTROLLERINTERFACE_IMESSAGECONTROLLER_H_ */
