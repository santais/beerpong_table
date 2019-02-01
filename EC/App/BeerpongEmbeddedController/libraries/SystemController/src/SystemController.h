//--------------------------------------------------------------------------------//
//	Name: 
//	Date: 04-10-16
//	Author: Mark Ulletved Povlsen
//	
//	Description: 
//
//
//-------------------------------------------------------------------------------//

#ifndef SYSTEMCONTROLLER_H_
#define SYSTEMCONTROLLER_H_

//-------------------------------------------------------------------------------//
// INCLUDES 
//-------------------------------------------------------------------------------//
#include <IRestController.h>

//-------------------------------------------------------------------------------//
// DEFINES 
//-------------------------------------------------------------------------------//

namespace Controller
{

//-------------------------------------------------------------------------------//
// ENUM DECLARATION
//-------------------------------------------------------------------------------//
enum class ControllerState
{
	E_STATE_IDLE		    = 0x00,
	E_STATE_MANUAL		= 0x01,
	E_STATE_AUTOMATIC	= 0x02,
	E_STATE_SEQUENCE	    = 0x03
};
	
//-------------------------------------------------------------------------------//
// STRUCT DECLARATION
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// EXTERN VARIABLES
//-------------------------------------------------------------------------------//

//-------------------------------------------------------------------------------//
// CLASS
//-------------------------------------------------------------------------------//

class SystemController : public IRestController
{
public:
    SystemController();
    virtual ~SystemController();

    virtual int read(uint8_t* ptrBuffer, uint8_t* ptrBytesWritten);
    virtual int write(uint8_t* ptrPayload, uint8_t payloadSize);

    ControllerState getCtrlState() {return m_ctrlState;}
private:
    bool checkValidSetRequest(uint8_t ui8ControlState);

    ControllerState m_ctrlState;
};

};

#endif /* SYSTEMCONTROLLER_H_ */
