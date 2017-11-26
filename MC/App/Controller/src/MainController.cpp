/*
 * MainController.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#include "MainController.h"

#include "Logger.h"
#include "NetworkServer.h"
#include "UartServer.h"
#include "Utils.h"
#include <iostream>

MainController::MainController()
	: m_uartServer(UartServer(std::bind(&MainController::uartCallback, this,
			std::placeholders::_1, std::placeholders::_2))),
	  m_uartClient(UartClient(UART_DEV_PORT)),
	  m_networkServer(NetworkServer(std::bind(&MainController::networkCallback, this,
			std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)))
{}

MainController::~MainController() {}

/*
 * \brief Close a fd
 *
 * \param void
 */
MainController* MainController::getInstance(void)
{
	static MainController* mainController = NULL;

	if(mainController == NULL)
	{
		mainController = new MainController();
	}

	return mainController;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t MainController::start(void)
{
	// Start the UART server
	BJBPErr_t res = m_uartServer.startServer();
	if(res != BJBP_SUCCESS)
	{
		BJBP_LOG_ERR("Unable to open uart server. err code: %d\n", res);
		return res;
	}

	// Start the network server
	res = m_networkServer.open(INADDR_ANY, DEFAULT_NETWORK_SERVER_PORT);

	return res;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t MainController::stop(void)
{
	BJBPErr_t res = m_uartServer.stopServer();

	// Stop the UART server
	if(res != BJBP_SUCCESS)
	{
		return res;
	}

	// Stop the network server
	res = m_networkServer.close();

	return res;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
void MainController::uartCallback(uint8_t* buffer, uint32_t size)
{
	BJBPErr_t err;
	// Get the datapackage
	if(size > 0 && size < UART_BUFFER_SIZE)
	{
		DataPackage_t dataPackage;
		if(loadPackageFromBuffer(&dataPackage, buffer, size) != BJBP_SUCCESS)
		{
			//BJBP_LOG_ERR("Received invalid package from UART\n");
			return;
		}
		uint32_t ipAddr = 0;
		convertIpArrToInt(dataPackage.ip, ipAddr);

		uint8_t module = dataPackage.header & PROTOCOL_MODULE_MASK;

		switch(module)
		{
		case E_GET_REQUEST:
		case E_GET_ALL_REQUEST:
			err = sendMsgToClient(ipAddr, buffer, size);
			if(err != BJBP_SUCCESS)
			{
				BJBP_LOG_ERR("Unable to send msg to network client. Err code: %d\n", err);
			}
			break;
		case E_NOTIFY_ALL_REQUEST:
			err = notifyAllClients(buffer, size);
			if(err != BJBP_SUCCESS)
			{
				BJBP_LOG_ERR("Unable to notify all clients. Err code: %d\n", err);
			}
			break;
		case E_LOG_MSG:
		{
			char buf[255] = {};
			memcpy(buf, dataPackage.payload, dataPackage.payloadSize);
			BJBP_LOG_UART(buf);
			break;
		}
		default:
			//BJBP_LOG_WARN("Unknown module request: %d\n", module);
			break;
		}
	}
}

/*
 * \brief Close a fd
 *
 * \param void
 */
void MainController::networkCallback(struct sockaddr_in* sockAddr, uint8_t* buffer, uint32_t size)
{
    DataPackage_t dataPackage;

    /* Add the client to the list */
    addClientToMap(sockAddr);

    // Load and validate the datapackage
    // Check length of the package
    if(loadPackageFromBuffer(&dataPackage, buffer, size) != BJBP_SUCCESS)
    {
    	BJBP_LOG_ERR("Failed to load package from buffer\n");
    }
    else
    {
    	BJBP_LOG_INFO("Received new message from Network Client. Message size: %i and"
    			" message header: %i\n", size, dataPackage.header);
    	// Forward the mssage to the uart
    	BJBPErr_t res = m_uartClient.open();
    	if(res != BJBP_SUCCESS)
    	{
    		BJBP_LOG_ERR("Failed to open uart client. Err code: %d\n", res);
    		return;
    	}

    	res = m_uartClient.write(buffer, size);
    	if(res != BJBP_SUCCESS)
    	{
    		BJBP_LOG_ERR("Failed to write to Uart Client. Err code: %d\n", res);
    		return;
    	}

    	res = m_uartClient.close();
    	if(res != BJBP_SUCCESS)
    	{
    		BJBP_LOG_ERR("Failed to close the Uart Client. Err code: %d\n", res);
    		return;
    	}
    }
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t MainController::loadBufferFromPackage(DataPackage_t* dataPackage, uint8_t* buffer, uint32_t len)
{
	// Load the initial prototol into the buffer
	int bufferItr = PROTOCOL_INITIAL_SIZE;

	// Validate package
	if((dataPackage->sof != E_SOF) || (dataPackage->eof != E_EOF))
	{
		return BJBP_ERR_INVALID_PACKAGE;
	}

	memcpy(&buffer[0], dataPackage, PROTOCOL_INITIAL_SIZE);

	// load the payload
	memcpy(&buffer[bufferItr], dataPackage->payload, dataPackage->payloadSize);

	// Load the eof flag
	bufferItr += dataPackage->payloadSize;
	buffer[bufferItr] = dataPackage->eof;

	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t MainController::loadPackageFromBuffer(DataPackage_t* dataPackage, uint8_t* buffer, uint32_t len)
{
	// Load the data buffer into the datapackage
	int bufferItr = PROTOCOL_INITIAL_SIZE;

	memcpy(dataPackage, &buffer[0], PROTOCOL_INITIAL_SIZE);

	// Load the payload data into the buffer
	memcpy(dataPackage->payload, &buffer[bufferItr], dataPackage->payloadSize);

	// Set the eof flag
	bufferItr += dataPackage->payloadSize;
	dataPackage->eof = buffer[bufferItr];

	if((dataPackage->sof != E_SOF) || (dataPackage->eof != E_EOF))
	{
		return BJBP_ERR_INVALID_PACKAGE;
	}

	return BJBP_SUCCESS;
}

/* \brief Send message to a client */
BJBPErr_t MainController::sendMsgToClient(uint32_t ipAddr, uint8_t* buffer, uint32_t size)
{
	// Find the client in the map
	ClientMapItr itr = m_clientMap.find(ipAddr);

	if(itr == m_clientMap.end())
	{
		return BJBP_ERR_NO_CLIENT_FOUND;
	}
	else
	{
		BJBPErr_t err;
		NetworkClient client = itr->second;

		err = client.open();
		if(err != BJBP_SUCCESS)
		{
			return err;
		}

		err = client.write(buffer, size);
		if(err != BJBP_SUCCESS)
		{
			return err;
		}

	}

	return BJBP_SUCCESS;
}

/* \brief Notify all client */
BJBPErr_t MainController::notifyAllClients(uint8_t* buffer, uint32_t size)
{
	BJBPErr_t err;
	for(ClientMapItr itr = m_clientMap.begin(); itr != m_clientMap.end(); itr++)
	{
		NetworkClient client = itr->second;

		err = client.open();
		if(err != BJBP_SUCCESS)
		{
			BJBP_LOG_ERR("Unable to open Network Client. Err code: %d\n", err);
			return err;
		}

		err = client.write(buffer, size);
		if(err != BJBP_SUCCESS)
		{
			BJBP_LOG_ERR("Unable to write to Network Client. Err code: %d\n", err);
			return err;
		}

		// Close the client
		err = client.close();
		if(err != BJBP_SUCCESS)
		{
			BJBP_LOG_ERR("Unable to close Network Client. Err code: %d\n", err);
			return err;
		}

		BJBP_LOG_INFO("Successfully notified client with ip: %s\n", client.getIpAddrAsString());
	}

	return BJBP_SUCCESS;
}


/** \brief Add new client to the map */
void MainController::addClientToMap(struct sockaddr_in* sockAddr)
{
	// Add the client to the client list
    NetworkClient networkClient(*sockAddr);

    if(m_clientMap.insert({networkClient.getIpAddrAsInt(), networkClient}).second)
    {
    	BJBP_LOG_INFO("Added new client with ip: %s\n", networkClient.getIpAddrAsString());
    }
}
