// ----------------------------------------------------------------------------
/*! \class CFECoundMgr
 *  \brief Sound Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if ((TARGETPLATFORM == USE_WINDOWS) || (TARGETPLATFORM == USE_DEVELOPMENT))
// ----------------------------------------------------------------------------
#include "../CFECore.h"
#include "CFECGlobals.h"

#include <windows.h>
// ----------------------------------------------------------------------------
namespace CFECore
{
	namespace TRC
	{

		static TOpenMessageBoxFunc 	OpenMessageBoxFunc = NULL;
		static TMessageBoxStepFunc	iMessageBoxStepFunc= NULL;
		static TCloseMessageBoxFunc bCloseMessageBoxFunc= NULL;
	}
	
	namespace Log
	{
		static TCoreLogFunc			LogFuncCallback = NULL;
	}
	
	namespace Process
	{
		#define USE_THREADS
		#ifdef USE_THREADS
		typedef struct TThreadData
		{
			HANDLE	m_hHnd;
			DWORD   m_dwID;

		}TThreadData;
		#endif
	}
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// String functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int CFECore::String::iStrICmp(const char* _szA,const char* _szB)
{
    return( _stricmp(_szA,_szB) );
}
// ----------------------------------------------------------------------------
int CFECore::String::iStrNICmp(const char* _szA,const char* _szB,uint _uiChars)
{
    return( _strnicmp(_szA,_szB,_uiChars) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Logging Functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Log::Print(const char * format, va_list arg)
{
	#ifndef MASTER_RELEASE_VERSION
	char* szProcString = CFECore::String::szFormatString(format,arg);

	if (IsDebuggerPresent())
	{
		OutputDebugString(szProcString);
	}
	else
	{
		if (LogFuncCallback != NULL)
		{
			LogFuncCallback(szProcString);
		}
		else
		{
			printf(szProcString);
		}
	}
    #endif
}
//-----------------------------------------------------------------------------
void CFECore::Log::Print(const char *fmt,...)
{
	#ifndef MASTER_RELEASE_VERSION
		va_list argptr;
		va_start(argptr,fmt);
		Print(fmt,argptr);
		va_end  (argptr);
	#endif
}
// ----------------------------------------------------------------------------
void CFECore::Log::SetSystemLogFunc(TCoreLogFunc _pLogFunc)
{
	LogFuncCallback = _pLogFunc;
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Assert functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFECore::Check(FEBool _bCondition,const CFEString& _sMessage)
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
FETick CFECore::oGetEngineTicks()
{
	return( GetTickCount() );
}
// ------------------------------------------------------------------------
/// Retrieves time in seconds of the given system tics.
FEReal CFECore::rGetTickTime(FETick _oTicks)
{
	return ( ((float)(_oTicks)/1000.0f) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// TRC management stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Initializes the TRC Technical Requirements system
void CFECore::TRC::Init()
{
}
// ----------------------------------------------------------------------------
/// Performs a check step in the TRC system.
void CFECore::TRC::CheckStep()
{
}
// ----------------------------------------------------------------------------
/// Registers a the function to open a message box.
void CFECore::TRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	OpenMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to write a message in a message box.
void CFECore::TRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	iMessageBoxStepFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Registers a the function to close a message box.
void CFECore::TRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	bCloseMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
/// Opens a message box.
void CFECore::TRC::OpenMessageBox(const CFEString& _sMessage,const CFEString& _sOptions)
{
	if (OpenMessageBoxFunc != NULL)
		OpenMessageBoxFunc(_sMessage,_sOptions);
}
// ----------------------------------------------------------------------------		
int CFECore::TRC::iMessageBoxStep()
{
	if (iMessageBoxStepFunc != NULL)
		return( iMessageBoxStepFunc() );
	else 
		return(-1);
}
// ----------------------------------------------------------------------------
/// Closes the message box.
FEBool CFECore::TRC::bCloseMessageBox()
{
	if (bCloseMessageBoxFunc != NULL)
		return(bCloseMessageBoxFunc());
	else
		return(true);
}	
// ----------------------------------------------------------------------------
/// Finalizes the TRC system.
void CFECore::TRC::Finish()
{
}
// ----------------------------------------------------------------------------
/// Starts a process to be executed in parallel to the main process (thread)
FEHandler CFECore::Process::hStartProcess(TCoreProcessFunc _pProcess,FEPointer _pParam)
{
	#ifdef USE_THREADS
	
		TThreadData* poTD = new TThreadData;

		poTD->m_hHnd = CreateThread(NULL,								// default security attributes
									0,									// use default stack size
									(LPTHREAD_START_ROUTINE)_pProcess,	// thread function name
									_pParam,							// argument to thread function
									0,									// use default creation flags
									&poTD->m_dwID);						// returns the thread identifier

		return((FEHandler)poTD);

	#else

		_pProcess(_pParam);
		return((FEHandler)1);

	#endif
}
// ----------------------------------------------------------------------------
/// Tells whether a parallel process has finished its execution or not.
FEBool CFECore::Process::bIsProcessFinished(FEHandler _hProcess)
{
	#ifdef USE_THREADS

		TThreadData* poTD = (TThreadData*)_hProcess;
		return(WaitForSingleObject(poTD->m_hHnd,0) == WAIT_OBJECT_0);
	#else

		return(true);

	#endif
}
// ----------------------------------------------------------------------------
/// Frees the resources allocated by an already finished process.
void CFECore::Process::EndProcess(FEHandler _hProcess)
{
	#ifdef USE_THREADS	
		TThreadData* poTD = (TThreadData*)_hProcess;
		
		WaitForSingleObject(poTD->m_hHnd,INFINITE);
		CloseHandle(poTD->m_hHnd);
		
		delete poTD;
	#else

	#endif
}
// ----------------------------------------------------------------------------
#endif // ((TARGETPLATFORM == USE_WINDOWS) || (TARGETPLATFORM == USE_DEVELOPMENT))
// ---------------------------------------------------------------------------
