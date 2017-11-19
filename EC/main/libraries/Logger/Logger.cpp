/*
 * Logger.cpp
 *
 *  Created on: Feb 2, 2017
 *      Author: markpovlsen
 */

#include <stdio.h>
#include <stdarg.h>

#include "Logger.h"
#include "NetworkController.h"

#include <errno.h>
#include <string.h>

/* Maximum buffer size */
#define DEBUG_STR_MAX_LEN 240u

/* Package variables */
static uint8_t debugStrPayload[DEBUG_STR_MAX_LEN];
static const eHeaderModuleOption_t eHeaderModuleOption = E_LOG_MSG;
static const eHeaderTargetOption_t eHeaderTargetOption = E_TARGET_LOG;

static inline uint32_t bjbp_log_prefix(const char* prefix)
{
	if(prefix)
	{
		return (sprintf((char*) &debugStrPayload[0], "%s", prefix));
	}
}

void bjbp_log_msg(const char* strlvl, const char* format, ...)
{
	va_list list;
	uint32_t stringItr = 0;
	uint32_t stringLen = strlen(format);
	stPackage package;

	// Clear the buffer
	memset(&debugStrPayload[0], 0x00, DEBUG_STR_MAX_LEN);

	stringItr = bjbp_log_prefix(strlvl);
	va_start(list, format);
	{
		vsnprintf((char*) &debugStrPayload[stringItr], DEBUG_STR_MAX_LEN - stringItr, format, list);
	}

	// Transmit the package
	NetworkComm_CreatePackage(&package, eHeaderTargetOption, eHeaderModuleOption, DEBUG_STR_MAX_LEN,
		&debugStrPayload[0], 0);
	NetworkComm_TransmitPacket(&package);

	va_end(list);
}


