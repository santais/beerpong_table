/*
 * AbstractConnector.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */


#include "AbstractConnector.h"

AbstractConnector::AbstractConnector(void)
	: m_fd(0)
{}

AbstractConnector::~AbstractConnector(void)
{}

BJBPErr_t AbstractConnector::open(void)
{
	// Must be overwritten
	return BJBP_ERR_OPEN_FD;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t AbstractConnector::close(void)
{
	int result = ::close(m_fd);

	if(result < 0)
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
BJBPErr_t AbstractConnector::write(uint8_t* buffer, uint32_t size)
{
	int result = 0;

	if(m_fd < 0)
	{
		return BJBP_ERR_NO_FD;
	}

	result = ::write(m_fd, &buffer[0], size);

	if(result < 0)
	{
		return BJBP_ERR_WRITE_FD;
	}

	return BJBP_SUCCESS;
}

/*
 * \brief Close a fd
 *
 * \param void
 */
BJBPErr_t AbstractConnector::read(uint8_t* buffer, uint32_t size, uint32_t& readBytes)
{
	if(m_fd < 0)
	{
		return BJBP_ERR_NO_FD;
	}

	readBytes = ::read(m_fd, &buffer[0], size);

	if(readBytes < 0)
	{
		return BJBP_ERR_READ_FD;
	}

	return BJBP_SUCCESS;
}


