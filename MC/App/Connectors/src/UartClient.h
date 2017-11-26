/*
 * UartClient.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _UartClient_H_
#define _UartClient_H_

#include "AbstractConnector.h"
#include "BJBPConfig.h"

using namespace BJBPCore;

/*
 * \brief Uart Connector which is capable of connecting to a UART port and to
 * 		  read and write data to it.
 */

class UartClient : public AbstractConnector {
public:
	UartClient(char* devPort);
	~UartClient(void);

	/* \brief Open the socket */
	BJBPErr_t open(void);

	/* \brief Close the socekt */
	BJBPErr_t close(void);


private:
	/* \brief Init the UART controller */
	int32_t setAttributes(int32_t& fd, int32_t baudRate);

private:
	/* \brief Character to hold the development port */
	char m_devPort[UART_MAX_DEV_LEN];


};



#endif /* _UartClient_H_ */
