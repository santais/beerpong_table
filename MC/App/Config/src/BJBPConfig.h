/*
 * BJBPConfig.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _BJBPCONFIG_H_
#define _BJBPCONFIG_H_

/****************************************************
 * GENERAL
 ***************************************************/
/**
 * \brief Enable the debug messages
 */

#define DEBUG_ENABLE 1

/**
 * \brief Name of the logger file
 */
#define BJBP_LOGGER_DIR "bjbp_log.txt"

/****************************************************
 * UART CONFIG
 ***************************************************/
/**
 * \brief Size of the UART buffer
 */
#define UART_BUFFER_SIZE 1024u

/**
 * \brief Baud rate
 */
#define UART_BAUD_RATE 115200u

/**
 * \brief Minimum read bytes in buffer before a read
 * 		  is performed
 */
#define UART_MIN_READ_BYTES_IN_BUFFER 5u

/**
 * \brief Refresh rate of the Uart Listener Thread
 */
#define UART_THREAD_DELAY_MS 100u

/**
 * \brief Uart Dev POrt
 */
#define UART_DEV_PORT ((char*) "/dev/ttyACM0")

/**
 * \brief UART receiver buffer size
 */
#define UART_RECEIVE_BUFFER_SIZE 1024u

/**
 * \brief Length of the UART development port
 */
#define UART_MAX_DEV_LEN 50u

/**
 * \brief UART Retry Delay Time
 */
#define UART_RETRY_CONN_TIME_S 2u

/****************************************************
 * NETWORK SERVER
 ***************************************************/

/**
 * \brief Default Network server port
 */
#define DEFAULT_NETWORK_SERVER_PORT 22000

/**
 * \brief Refresh rate of the Server Liistener
 * 	      Thread
 */
#define SERVER_THREAD_DELAY_MS 1000u

/**
 * \brief Maximum number of pending clients
 */
#define SERVER_MAX_NUM_PENDING_CLIENTS 3u

/**
 * \brief Server receiver buffer size
 */
#define SERVER_RECEIVE_BUFFER_SIZE 1024u

/****************************************************
 * NETWORK CLIENT
 ***************************************************/

/**
 * \brief Expected default client server port
 */
#define DEFAULT_NETWORK_CLIENT_PORT 22000u

#endif /* _BJBPCONFIG_H_ */
