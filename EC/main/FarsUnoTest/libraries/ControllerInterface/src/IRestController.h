/*
 * IController.h
 *
 *  Created on: 11 Dec 2017
 *      Author: markpovlsen
 */

#ifndef LIBRARIES_CONTROLLERINTERFACE_SRC_IRESTCONTROLLER_H_
#define LIBRARIES_CONTROLLERINTERFACE_SRC_IRESTCONTROLLER_H_

#include <stdint.h>

class IRestController
{
public:
    virtual int read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten) = 0;
    virtual int write(uint8_t* ptrPayload, uint8_t payloadSize) = 0;
    virtual int handlePost(uint8_t* ptrPayload, uint8_t payloadSize) = 0;
    virtual int handleDelete(uint8_t* ptrPayload, uint8_t payloadSize) = 0;
};

#endif /* LIBRARIES_CONTROLLERINTERFACE_SRC_IRESTCONTROLLER_H_ */
