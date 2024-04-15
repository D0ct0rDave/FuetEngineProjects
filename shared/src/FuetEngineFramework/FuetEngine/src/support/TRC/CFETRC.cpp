// ----------------------------------------------------------------------------
/*! \class CFETRC
 *  \brief Basic TRC Functionality. Wrapper of the CFESystem functionality.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFETRC.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
/// Initializes the TRC Technical Requirements system
void CFETRC::Init()
{
	CFECore::TRC::Init();
}
// ----------------------------------------------------------------------------
/// Performs a check step in the TRC system.
void CFETRC::CheckStep()
{
	CFECore::TRC::CheckStep();
}
// ----------------------------------------------------------------------------
/// Registers a the function to open a message box.
void CFETRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	CFECore::TRC::RegisterOpenMessageBoxFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Registers a the function to write a message in a message box.
void CFETRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	CFECore::TRC::RegisterMessageBoxStepFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Registers a the function to close a message box.
void CFETRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	CFECore::TRC::RegisterCloseMessageBoxFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Finalizes the TRC system.
void CFETRC::Finish()
{
	CFECore::TRC::Finish();
}	
// ----------------------------------------------------------------------------
