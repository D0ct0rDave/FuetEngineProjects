// ----------------------------------------------------------------------------
/*! \class CFETRC
 *  \brief Basic TRC Functionality. Wrapper of the CFESystem functionality.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFETRC.h"
#include "system/CFESystem.h"
// ----------------------------------------------------------------------------
/// Initializes the TRC Technical Requirements system
void CFETRC::Init()
{
	CFESystem::TRC::Init();
}
// ----------------------------------------------------------------------------
/// Performs a check step in the TRC system.
void CFETRC::CheckStep()
{
	CFESystem::TRC::CheckStep();
}
// ----------------------------------------------------------------------------
/// Registers a the function to open a message box.
void CFETRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	CFESystem::TRC::RegisterOpenMessageBoxFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Registers a the function to write a message in a message box.
void CFETRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	CFESystem::TRC::RegisterMessageBoxStepFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Registers a the function to close a message box.
void CFETRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	CFESystem::TRC::RegisterCloseMessageBoxFunc(_pFunc);
}
// ----------------------------------------------------------------------------
/// Finalizes the TRC system.
void CFETRC::Finish()
{
	CFESystem::TRC::Finish();
}	
// ----------------------------------------------------------------------------
