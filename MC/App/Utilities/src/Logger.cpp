/*
 * Logger.cpp
 *
 *  Created on: Feb 2, 2017
 *      Author: markpovlsen
 */

#include <stdio.h>
#include <stdarg.h>

#include "Logger.h"
#include "BJBPConfig.h"

#include <errno.h>
#include <string.h>

static FILE* log_file = NULL;
static uint8_t log_isInit = 0;

static void bjbp_log_init()
{
	if (BJBP_LOGGER_DIR != NULL)
	{
		log_file = fopen(BJBP_LOGGER_DIR, "w+");
		if(log_file)
		{
			log_isInit = 1;
		}
	}
	else
	{
		log_isInit = 0;
	}
}

static inline void bjbp_log_prefix(const char* prefix)
{
	if(prefix)
	{
		printf("%s", prefix);
		if(log_file)
		{
			fprintf(log_file, "%s", prefix);
		}
	}
}

void bjbp_log_msg(const char* strlvl, const char* format, ...)
{
	va_list list;

	if(!log_isInit)
	{
		bjbp_log_init();
	}

	bjbp_log_prefix(strlvl);
	va_start(list, format);
	{
		char buf[512];
		vsnprintf(buf, 512, format, list);
		printf("%s", buf);
		fflush(stdout);
	}
	va_end(list);

	va_start(list, format);
	if(log_file)
	{
		va_start(list, format);
		vfprintf(log_file, format, list);
		fflush(log_file);
	}

	va_end(list);
}


