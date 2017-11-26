/*
 * NetworkServer.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _NETWORKSERVER_H_
#define _NETWORKSERVER_H_

#include "AbstractConnector.h"
#include "BJBPConfig.h"
#include "CommonTypes.h"

// System includes
#include <thread>

using namespace BJBPCore;

class NetworkServer : public AbstractConnector {
public:
	NetworkServer(NetServerCallback callback);
	virtual ~NetworkServer();

	/* \brief Open the socket */
	BJBPErr_t open(uint32_t ipAddr, uint32_t port);

	/* \brief Close the socket */
	BJBPErr_t close(void);

	/* \brief Write to target. NOT SUPPORTED */
	BJBPErr_t write(uint8_t* buffer, uint32_t size);

	/* \brief Read from target. NOT SUPPORTED */
	BJBPErr_t read(uint8_t* buffer, uint32_t size);

private:
	/* \brief Server listener thread */
	void serverListenerThread(void);

private:
	/* \brief Indicates if a server is running */
	bool m_running;

	/* \brief Read buffer */
	uint8_t m_buffer[SERVER_RECEIVE_BUFFER_SIZE];

	/* \brief Callback initiated when a new client has been received */
	NetServerCallback m_callback;
};

#endif /* APP_INCLUDE_NETWORKSERVER_H_ */
