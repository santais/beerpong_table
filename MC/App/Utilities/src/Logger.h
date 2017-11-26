/*
 * Logger.h
 *
 *  Created on: Feb 2, 2017
 *      Author: markpovlsen
 */

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


void bjbp_log_msg(const char* strlvl, const char* format, ...);
void bjbp_log_debug_error(const char err);

#define S1(x) #x
#define S2(x) S1(x)

#define LOCATION __FILE__ " (line " S2(__LINE__) ")"
#define BJBP_LOG_INFO(...) bjbp_log_msg("[INFO] " , ##__VA_ARGS__)
#define BJBP_LOG_WARN(...) bjbp_log_msg("[WARN] " LOCATION ": ", ##__VA_ARGS__)
#define BJBP_LOG_ERR(...) bjbp_log_msg("[ERROR] " LOCATION ": ", ##__VA_ARGS__)

// UART log from Arduino
#define BJBP_LOG_UART(...) bjbp_log_msg("[UART] " , ##__VA_ARGS__)

#define bjbp_error_set(err, out) \
	do { \
		BJBP_LOG_ERR("err = %d\n", err); \
		out = err \
	} while(0)

#ifdef __cplusplus
}
#endif

#endif /* _LOGGER_H_ */
