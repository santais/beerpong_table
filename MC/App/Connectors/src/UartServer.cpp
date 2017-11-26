/*
 * UartServer.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#include "UartServer.h"
#include "Logger.h"

UartServer::UartServer(UartServerCallback callback)
	: m_running(false),
	  m_callback(callback),
	  m_uartClient(UartClient(UART_DEV_PORT))
{}

UartServer::~UartServer()
{}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t UartServer::startServer(void)
{
	m_running = true;

    // Start the server
    std::thread serverThread = std::thread(&UartServer::uartServerThread, this);
    serverThread.detach();

	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t UartServer::stopServer(void)
{
	m_running = false;

	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
void UartServer::uartServerThread(void)
{
	BJBPErr_t err;
	uint32_t bytesRead = 0;
	uint32_t retryTimes = 0;

	// Open the uart controller
	err = m_uartClient.open();

	// If it fails to open, try forever
	while((err != BJBP_SUCCESS) && m_running)
	{
		// Log error
		BJBP_LOG_ERR("Failed to open uartClient. Tried %lu times\n", retryTimes);

		// Sleep for x seconds
		std::this_thread::sleep_for(std::chrono::milliseconds(UART_RETRY_CONN_TIME_S * 1000));

		// Retry
		err = m_uartClient.open();

		// Inc retry cnt'er
		retryTimes++;
	}

	while(m_running)
	{

		if(err == BJBP_SUCCESS)
		{
			err = m_uartClient.read(m_buffer, UART_RECEIVE_BUFFER_SIZE, bytesRead);
		}
		else
		{
			BJBP_LOG_ERR("Failed to open uartClient. Err code %d\n", err);
		}

		if((err == BJBP_SUCCESS) && (bytesRead >= 0))
		{
			m_callback(m_buffer, bytesRead);
		}
		else
		{
			BJBP_LOG_ERR("Failed to read the uartClient. Error code: %i\n", err);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(UART_THREAD_DELAY_MS));

	}
	err = m_uartClient.close();
	if(err != BJBP_SUCCESS)
	{
		BJBP_LOG_ERR("Failed to close the uartclient\n");
	}

}
