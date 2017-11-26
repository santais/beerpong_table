/*
 * UartClient.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: markpovlsen
 */

#include <stdio.h>
#include <string.h>
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <UartClient.h>
#include "Logger.h"

/**
 * \brief Default constructor
 */
UartClient::UartClient(char* devPort)
{
	strncpy(m_devPort, devPort, strlen(devPort));
}

/**
 * \brief Destructor
 */
UartClient::~UartClient(void)
{}

/*
 * \brief Opens a fd
 *
 * \param devPort Development port in which the fd is
 * 		 	      to opened at (etc. /options/ACM0)
 */
BJBPErr_t UartClient::open()
{
	// Open the port
	m_fd = ::open(m_devPort, O_RDWR | O_NOCTTY | O_SYNC | O_NDELAY);

	if(m_fd < 0)
	{
		return BJBP_ERR_OPEN_FD;
	}

	if(setAttributes(m_fd, UART_BAUD_RATE) != BJBP_SUCCESS)
	{
		return BJBP_ERR_SET_ATTRIBUTES;
	}

	return BJBP_SUCCESS;
}

/*
 * \brief Opens a fd
 *
 * \param devPort Development port in which the fd is
 * 		 	      to opened at (etc. /options/ACM0)
 */
BJBPErr_t UartClient::close(void)
{
	sleep(2);
	// tcflush(m_fd, TCIOFLUSH);
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
int32_t UartClient::setAttributes(int32_t& fd, int32_t baudRate)
{
	struct termios options;
    memset (&options, 0, sizeof(options));

    /* Save old options parameters */
    if(tcgetattr(fd, &options) != 0)
    {
    	BJBP_LOG_ERR("Failed to retrieve UART attributes\n");
    	return BJBP_FALSE;
    }

    /* Set Baud Rate */
    cfsetospeed (&options, (speed_t)baudRate);
    cfsetispeed (&options, (speed_t)B115200);

     options.c_cflag = (options.c_cflag & ~CSIZE) | CS8;     		// 8-bit chars
     // disable IGNBRK for mismatched speed tests; otherwise receive break
     // as \000 chars
     options.c_iflag &= ~IGNBRK;         							// disable break processing
     options.c_lflag = 0;              							    // no signaling chars, no echo,
                                     	 	 	 	 	 	 	 	// no canonical processing
     options.c_oflag = 0;                							// no remapping, no delays
     options.c_cc[VMIN]  = UART_MIN_READ_BYTES_IN_BUFFER;           // read doesn't block
     options.c_cc[VTIME] = 5;            						    // 0.5 seconds read timeout

     options.c_iflag &= ~(IXON | IXOFF | IXANY);				    // shut off xon/xoff ctrl

     options.c_cflag |= (CLOCAL | CREAD);							// ignore modem controls,
                                     	 	 	 	 	 	 	 	// enable reading
     options.c_cflag &= ~(PARENB | PARODD);      					// shut off parity
     options.c_cflag |= 0; 											// No Parity
     options.c_cflag &= ~CSTOPB;
     options.c_cflag &= ~CRTSCTS;

     // Set the attributes
     if (tcsetattr (fd, TCSANOW, &options) != 0)
     {
    	 BJBP_LOG_ERR("Failed to set UART attributes\n");
		 return BJBP_FALSE;
     }

     return BJBP_TRUE;
}
