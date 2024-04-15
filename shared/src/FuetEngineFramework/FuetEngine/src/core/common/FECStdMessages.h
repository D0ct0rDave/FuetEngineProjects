// ----------------------------------------------------------------------------
/*! \class Fuet Engine Profile Standard System Messages
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FECStdMessagesH
#define FECStdMessagesH
// ----------------------------------------------------------------------------
extern char* GENERIC_LOAD_MESSAGE;
extern char* GENERIC_LOAD_MESSAGE_OPTIONS;

extern char* GENERIC_SAVE_MESSAGE;
extern char* GENERIC_SAVE_MESSAGE_OPTIONS;

extern char* RECOVER_SAVE_MESSAGE;
extern char* RECOVER_SAVE_MESSAGE_OPTIONS;

extern char* CREATE_SAVE_MESSAGE;
extern char* CREATE_SAVE_MESSAGE_OPTIONS;

extern char* FATAL_ERROR_MESSAGE;
extern char* FATAL_ERROR_MESSAGE_OPTIONS;

extern char* BADFSFORMAT_ERROR_MESSAGE;
extern char* BADFSFORMAT_ERROR_MESSAGE_OPTIONS;
// ----------------------------------------------------------------------------
namespace FECStdMessages{
	
	void Init();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
