//--------------------------------------------------------------------------------//
//	Name:
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//
//	Description:
//
//
//-------------------------------------------------------------------------------//

#include "NetworkClient.h"
#include "Logger.h"

// System includes
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <iomanip>


/*
 * \brief Close a fd
 *
 * \param void
 */
NetworkClient::NetworkClient(struct sockaddr_in sockAddr)
	: m_sockAddr(sockAddr),
	  m_port(sockAddr.sin_port)
{
	char* addr = inet_ntoa(sockAddr.sin_addr); // return the IP
	strncpy(&m_strAddr[0], addr, INET_ADDRSTRLEN);

	// The default port is always
	m_sockAddr.sin_port = htons(DEFAULT_NETWORK_CLIENT_PORT);
}


/*
 * \brief Close a fd
 *
 * \param void
 */
NetworkClient::~NetworkClient()
{}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkClient::open(void)
{
	// Open a socket
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_fd < 0)
    {
        BJBP_LOG_ERR("Unable to create a Network Client socket\n");
    	return BJBP_ERR_OPEN_FD;
    };

    // Connect to the client.
    if(connect(m_fd, (struct sockaddr *)&m_sockAddr, sizeof(m_sockAddr)) < 0)
    {
        BJBP_LOG_ERR("Unable to connect to the Network Client with ip: %s\n", getIpAddrAsString());
    	// Close the socket
    	close();
    	return BJBP_ERR_CONNECT;
    }


	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t NetworkClient::write(uint8_t* buffer, uint32_t size)
{
	int bytesSend = send(m_fd, buffer, size, 0);
    if(bytesSend < 0)
    {
        BJBP_LOG_ERR("Unable to send message to Network Client with ip: %s\n", getIpAddrAsString());
    	return BJBP_ERR_WRITE_FD;
    }
    else if ((uint32_t) bytesSend != size)
    {
    	BJBP_LOG_ERR("Expected to send %i bytes but only send %i to ip: %s\n", size, bytesSend,
    			getIpAddrAsInt());
    	return BJBP_ERR_WRITE_FD;
    }
    return BJBP_SUCCESS;
}


/*
 * \brief Close a fd
 *
 * \param void
 */
char* NetworkClient::getIpAddrAsString(void)
{
	return &m_strAddr[0];
}

/*
 * \brief Close a fd
 *
 * \param void
 */
uint32_t NetworkClient::getIpAddrAsInt(void)
{
	return m_sockAddr.sin_addr.s_addr;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
int NetworkClient::getIpPort(void)
{
	return m_port;
}
