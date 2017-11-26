/*
 * UartServer.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _UARTSERVER_H_
#define _UARTSERVER_H_

#include <UartClient.h>
#include "CommonTypes.h"
#include "BJBPConfig.h"
#include <thread>

using namespace BJBPCore;

class UartServer {
public:
	UartServer(UartServerCallback callback);
	~UartServer();

	/* Start the uart server */
	BJBPErr_t startServer(void);

	/* Stop the uart server */
	BJBPErr_t stopServer(void);

private:
	void uartServerThread(void);

private:
	/* \brief Indicates the server is running */
	bool m_running;

	/* \brief callback */
	UartServerCallback m_callback;

	/* \brief UartConnector */
	UartClient m_uartClient;

	/* \brief Uart buffer */
	uint8_t m_buffer[UART_RECEIVE_BUFFER_SIZE];

};

#endif /* _UARTSERVER_H_ */
