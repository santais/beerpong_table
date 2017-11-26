/*
 * CommonType.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _COMMONTYPES_H_
#define _COMMONTYPES_H_

// System includes
#include <stdint.h>
#include <netinet/in.h>
#include <functional>
#include <unordered_map>

namespace BJBPCore
{


/*
 * \brief Common return types of
 * the BJBP core
 */
typedef uint32_t BJBPErr_t;

enum _BJBPErr_t
{
	BJBP_SUCCESS,

	/* File Descriptor errors */
	BJBP_ERR_OPEN_FD,
	BJBP_ERR_CLOSE_FD,
	BJBP_ERR_WRITE_FD,
	BJBP_ERR_READ_FD,
	BJBP_ERR_TIMEOUT_FD,
	BJBP_ERR_SET_ATTRIBUTES,
	BJBP_ERR_NO_FD,

	/* Networking */
	BJBP_ERR_CONNECT,
	BJBP_ERR_BIND,
	BJBP_ERR_CLIENT_ACCEPT,
	BJBP_ERR_NO_CLIENT_FOUND,

	/* Protocol Err */
	BJBP_ERR_INVALID_PACKAGE,
	BJBP_ERR_PACKAGE_LENGTH
};

/**
 * \brief Network Server callback
 */
typedef std::function< void(struct sockaddr_in* sockAddr, uint8_t* buffer, uint32_t size) > NetServerCallback;

/**
 * \brief UART server callback
 */
typedef std::function< void(uint8_t* buffer, uint32_t size) > UartServerCallback;

enum _BJBPReturn_t
{
	BJBP_TRUE = 0,
	BJBP_FALSE = -1
};

}



#endif /* _COMMONTYPES_H_ */
