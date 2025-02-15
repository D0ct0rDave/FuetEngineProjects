// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_WINDOWS)
// ----------------------------------------------------------------------------

#include "../CFESystem.h"
#include "CFESGlobals.h"

#include <windows.h>

// ----------------------------------------------------------------------------
namespace CFESystem { namespace TRC {

	static TOpenMessageBoxFunc 	OpenMessageBoxFunc = NULL;
	static TMessageBoxStepFunc	iMessageBoxStepFunc= NULL;
	static TCloseMessageBoxFunc bCloseMessageBoxFunc= NULL;
}}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// String functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int CFESystem::String::iStrICmp(const CFEString& _sA,const CFEString& _sB)
{
    return( stricmp(_sA.szString(),_sB.szString()) );
}
// ----------------------------------------------------------------------------
int CFESystem::String::iStrNICmp(const CFEString& _sA,const CFEString& _sB,uint _uiChars)
{
    return( strnicmp(_sA.szString(),_sB.szString(),_uiChars) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Logging Functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];

void CFESystem::Log::Print(char *fmt,...)
{
	#ifdef _DEBUG
	if (IsDebuggerPresent())
	{
		va_list argptr;
		va_start(argptr,fmt);
		vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
		va_end  (argptr);
		
		OutputDebugString(gsszLocalBuff);
	}
	#endif
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Assert functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFESystem::Check(bool _bCondition,const CFEString& _sMessage)
{
    if (_bCondition != true)
    {
        printf(_sMessage.szString());
        MessageBox(0,_sMessage.szString(), "Error", MB_OK);
        exit(0);
    }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Timing functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
uint CFESystem::uiGetEngineTicks()
{
	return( (uint)GetTickCount() );
}
// ------------------------------------------------------------------------
/// Retrieves time in seconds of the given system tics.
FEReal CFESystem::rGetTickTime(uint _uiTicks)
{
	return ( ((float)(_uiTicks)/1000.0f) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// TRC management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Initializes the TRC Technical Requirements system
void CFESystem::TRC::Init()
{
}
// ----------------------------------------------------------------------------
/// Performs a check step in the TRC system.
void CFESystem::TRC::CheckStep()
{
}
// ----------------------------------------------------------------------------
/// Registers a the function to open a message box.
void CFESystem::TRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	OpenMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to write a message in a message box.
void CFESystem::TRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	iMessageBoxStepFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to close a message box.
void CFESystem::TRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	bCloseMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Opens a message box.
void CFESystem::TRC::OpenMessageBox(const CFEString& _sMessage,const CFEString& _sOptions)
{
	if (OpenMessageBoxFunc != NULL)
		OpenMessageBoxFunc(_sMessage,_sOptions);
}
// ----------------------------------------------------------------------------		
int CFESystem::TRC::iMessageBoxStep()
{
	if (iMessageBoxStepFunc != NULL)
		return( iMessageBoxStepFunc() );
	else 
		return(-1);
}
// ----------------------------------------------------------------------------
/// Closes the message box.
bool CFESystem::TRC::bCloseMessageBox()
{
	if (bCloseMessageBoxFunc != NULL)
		return(bCloseMessageBoxFunc());
	else
		return(false);
}	
// ----------------------------------------------------------------------------
/// Finalizes the TRC system.
void CFESystem::TRC::Finish()
{
}
// ----------------------------------------------------------------------------
#endif // (TARGETPLATFORM == USE_WINDOWS)
// ---------------------------------------------------------------------------
