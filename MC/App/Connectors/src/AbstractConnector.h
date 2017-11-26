/*
 * AbstractConnector.h
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#ifndef _AbstractConnector_H_
#define _AbstractConnector_H_

#include "BJBPConfig.h"
#include "CommonTypes.h"

// System Includes
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>


using namespace BJBPCore;

/*
 * \brief Abstract socket class which is used to perform some of the common
 * 	      IO operations for different sockets.
 */

class AbstractConnector {
public:
	AbstractConnector();
	virtual ~AbstractConnector();

	/* \brief Open the socket */
	virtual BJBPErr_t open(void);

	/* \brief Close the socket */
	virtual BJBPErr_t close(void);

	/* \brief Write the socket */
	virtual BJBPErr_t write(uint8_t* buffer, uint32_t size);

	/* \brief Read the socket */
	virtual BJBPErr_t read(uint8_t* buffer, uint32_t size, uint32_t& readBytes);

	/* \brief Return the filedescriptor */
	int32_t getFileDescriptor(void) { return m_fd;}

protected:
	/* \brief Socket Filedescriptor */
	int32_t m_fd;

};


#endif /* _AbstractConnector_H_ */
