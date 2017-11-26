/*
 * MainController.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _MAINCONTROLLER_H_
#define _MAINCONTROLLER_H_

#include "BJBPConfig.h"
#include "CommonTypes.h"
#include "NetworkClient.h"
#include "NetworkServer.h"
#include "UartClient.h"
#include "UartServer.h"
#include "ProtocolTypes.h"

// System includes
#include <unordered_map>

using namespace BJBPCore;


/**
 * \brief typedef for the client map
 */
typedef std::unordered_map<int, NetworkClient> ClientMap;
typedef std::unordered_map<int, NetworkClient>::const_iterator ClientMapItr;


class MainController {
public:
	/** \brief Get the maincontroller instance */
	static MainController* getInstance(void);
	virtual ~MainController();

	/* \brief Start the controller */
	BJBPErr_t start(void);

	/* \brief Stop the controller */
	BJBPErr_t stop(void);

private:
	MainController();

	/* \brief Uart Server callback */
	void uartCallback(uint8_t* buffer, uint32_t size);

	/* \brief Network Server callback */
	void networkCallback(struct sockaddr_in* sockAddr, uint8_t* buffer, uint32_t size);

	/* \brief Copies a datapackage into a raw buffer */
	BJBPErr_t loadBufferFromPackage(DataPackage_t* dataPackage, uint8_t* buffer, uint32_t len);

	/* \brief Copies a raw buffer into a package */
	BJBPErr_t loadPackageFromBuffer(DataPackage_t* dataPackage, uint8_t* buffer, uint32_t len);

	/* \brief Send message to a client */
	BJBPErr_t sendMsgToClient(uint32_t ipAddr, uint8_t* buffer, uint32_t size);

	/* \brief Notify all client */
	BJBPErr_t notifyAllClients(uint8_t* buffer, uint32_t size);

	/** \brief Add new client to the map */
	void addClientToMap(struct sockaddr_in* sockAddr);

private:
	/* \brief UART controller server */
	UartServer m_uartServer;

	/* \brief UART Client */
	UartClient m_uartClient;

	/* \brief Network controller server */
	NetworkServer m_networkServer;

	/* \brief Client map */
	ClientMap m_clientMap;
};

#endif /* _MAINCONTROLLER_H_ */
