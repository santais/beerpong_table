/*
 * NetworkServer.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#include <NetworkServer.h>
#include "Logger.h"

// System includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <iomanip>

#include <iostream>

NetworkServer::NetworkServer(NetServerCallback callback)
	: m_running(false),
	  m_callback(callback)
{}

NetworkServer::~NetworkServer()
{}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkServer::open(uint32_t ipAddr, uint32_t port)
{
    struct sockaddr_in server;

    //Create socket
    m_fd = socket(AF_INET , SOCK_STREAM , 0);
    if (m_fd < 0)
    {
        BJBP_LOG_ERR("Unable to create a Network Server socket\n");
    	return BJBP_ERR_OPEN_FD;
    }

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_NETWORK_SERVER_PORT );

    //Bind
    if( bind(m_fd,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        BJBP_LOG_ERR("Unable to bind to the Network Server socket. Err msg: %s\n", errno);
    	return BJBP_ERR_BIND;
    }

    //Listen
    listen(m_fd , SERVER_MAX_NUM_PENDING_CLIENTS);

    // Start the server
    static std::thread serverThread = std::thread(&NetworkServer::serverListenerThread, this);
    serverThread.detach();
    m_running = true;

    return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkServer::close(void)
{
	m_running = false;

	if(::close(m_fd) < 0)
	{
		return BJBP_ERR_CLOSE_FD;
	}

	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkServer::write(uint8_t* buffer, uint32_t size)
{
	// Not supported
	return BJBP_ERR_WRITE_FD;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkServer::read(uint8_t* buffer, uint32_t size)
{
	// Not supported
	return BJBP_ERR_READ_FD;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
void NetworkServer::serverListenerThread(void)
{
	BJBPErr_t err = BJBP_SUCCESS;
	struct sockaddr_in client;
	int32_t sockAddrLen = sizeof(struct sockaddr_in);

	BJBP_LOG_INFO("Starting Network Server Listener Thread\n");
	while(m_running)
	{
        // Accept connection from an incoming client
        int32_t clientSocket = accept(m_fd, (struct sockaddr *)&client, (socklen_t*)&sockAddrLen);
        if (clientSocket < 0)
        {
            BJBP_LOG_ERR("Unable to accept new client\n");
        	err = BJBP_ERR_CLIENT_ACCEPT;
        }

        if(err == BJBP_SUCCESS)
        {
        	// Receive the message from the client
        	uint32_t msgSize = recv(clientSocket, m_buffer, SERVER_RECEIVE_BUFFER_SIZE, 0);

        	if(msgSize < 0)
        	{
        		BJBP_LOG_ERR("Failed to recv message from new Network Client\n");
        	}
        	else
        	{
        		// Notify the controller of a new client and the message
        		m_callback(&client, m_buffer, msgSize);
        	}
        }
	}
}
