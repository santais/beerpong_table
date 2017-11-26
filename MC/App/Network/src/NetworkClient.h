//--------------------------------------------------------------------------------//
//	Name:
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//
//	Description:
//
//
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// INCLUDES
//-------------------------------------------------------------------------------//

#ifndef _NETWORK_CLIENT_
#define _NETWORK_CLIENT_

#include "AbstractConnector.h"
#include "BJBPConfig.h"
#include "CommonTypes.h"

// System includes
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace BJBPCore;

//-------------------------------------------------------------------------------//
// DEFINES
//-------------------------------------------------------------------------------//
#define IPV4_STRING_LENGTH INET_ADDRSTRLEN

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// CLASS
//-------------------------------------------------------------------------------//
class NetworkClient : public AbstractConnector
{
public:
    NetworkClient(struct sockaddr_in sockAddr);
    virtual ~NetworkClient();

	/* \brief Open the socket */
	BJBPErr_t open(void);

	/* \brief Open the socket */
	BJBPErr_t write(uint8_t* buffer, uint32_t size);

    /* \brief Get the ipAddress as a string */
    char* getIpAddrAsString(void);

    /* \brief Get the ipAddress as an int */
    uint32_t getIpAddrAsInt(void);

    /* \brief Get the port */
    int getIpPort(void);

private:
    /* \brief IPaddress of the client */
    struct sockaddr_in m_sockAddr;

    /* \brief String containing the IPV4 address */
    char m_strAddr[IPV4_STRING_LENGTH];

    /* \brief port */
    int m_port;
};

#endif /* _NETWORK_CLIENT_ */
