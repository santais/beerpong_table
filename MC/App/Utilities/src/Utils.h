/*
 * Utils.h
 *
 *  Created on: Jan 29, 2017
 *      Author: markpovlsen
 */

#ifndef APP_INCLUDE_UTILS_H_
#define APP_INCLUDE_UTILS_H_

#include "CommonTypes.h"

using namespace BJBPCore;

/**
 * \brief Convert the Ip array to an uint32_t
 */
void convertIpArrToInt(uint8_t* ipAddr, uint32_t& ipAddrAsInt)
{
	ipAddrAsInt = ipAddr[0];
	ipAddrAsInt |= (uint32_t) (ipAddr[1] << 8);
	ipAddrAsInt |= (uint32_t) (ipAddr[2] << 16);
	ipAddrAsInt |= (uint32_t) (ipAddr[3] << 24);
}


#endif /* APP_INCLUDE_UTILS_H_ */
