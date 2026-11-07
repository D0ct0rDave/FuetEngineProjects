// ----------------------------------------------------------------------------
/*! \brief Linux implementation of the CFECore system functionality
 *         (logging, assertions, timing, TRC and parallel processes).
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_LINUX)
// ----------------------------------------------------------------------------
#include "../CFECore.h"
#include "CFECGlobals.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <pthread.h>
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
		typedef struct TThreadData
		{
			pthread_t			m_oThread;
			TCoreProcessFunc	m_pFunc;
			FEPointer			m_pParam;
			volatile bool		m_bFinished;

		}TThreadData;

		static void* ThreadMain(void* _pData)
		{
			TThreadData* poTD = (TThreadData*)_pData;
			poTD->m_pFunc(poTD->m_pParam);
			poTD->m_bFinished = true;
			return(NULL);
		}
	}
}
// ----------------------------------------------------------------------------
// String functionality
// ----------------------------------------------------------------------------
int CFECore::String::iStrICmp(const char* _szA,const char* _szB)
{
    return( strcasecmp(_szA,_szB) );
}
// ----------------------------------------------------------------------------
int CFECore::String::iStrNICmp(const char* _szA,const char* _szB,uint _uiChars)
{
    return( strncasecmp(_szA,_szB,_uiChars) );
}
// ----------------------------------------------------------------------------
// Logging Functionality
// ----------------------------------------------------------------------------
void CFECore::Log::Print(const char * format, va_list arg)
{
	#ifndef MASTER_RELEASE_VERSION
	char* szProcString = CFECore::String::szFormatString(format,arg);

	if (LogFuncCallback != NULL)
		LogFuncCallback(szProcString);
	else
		printf("%s",szProcString);
    #endif
}
// ----------------------------------------------------------------------------
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
// Assert functionality
// ----------------------------------------------------------------------------
void CFECore::Check(FEBool _bCondition,const CFEString& _sMessage)
{
    if (_bCondition != true)
    {
        fprintf(stderr,"%s\n",_sMessage.szString());
        exit(0);
    }
}
// ----------------------------------------------------------------------------
// Timing functionality
// ----------------------------------------------------------------------------
FETick CFECore::oGetEngineTicks()
{
    timespec oTime;
    clock_gettime(CLOCK_MONOTONIC,&oTime);
    return( (FETick)(oTime.tv_sec*1000ULL + oTime.tv_nsec/1000000ULL) );
}
// ----------------------------------------------------------------------------
FEReal CFECore::rGetTickTime(FETick _oTicks)
{
	return ( ((float)(_oTicks)/1000.0f) );
}
// ----------------------------------------------------------------------------
// TRC management stuff.
// ----------------------------------------------------------------------------
void CFECore::TRC::Init() {}
void CFECore::TRC::CheckStep() {}
// ----------------------------------------------------------------------------
void CFECore::TRC::RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc)
{
	OpenMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
void CFECore::TRC::RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc)
{
	iMessageBoxStepFunc = _pFunc;
}
// ----------------------------------------------------------------------------
void CFECore::TRC::RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc)
{
	bCloseMessageBoxFunc = _pFunc;
}
// ----------------------------------------------------------------------------
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
FEBool CFECore::TRC::bCloseMessageBox()
{
	if (bCloseMessageBoxFunc != NULL)
		return(bCloseMessageBoxFunc());
	else
		return(true);
}
// ----------------------------------------------------------------------------
void CFECore::TRC::Finish() {}
// ----------------------------------------------------------------------------
// Parallel processes (threads)
// ----------------------------------------------------------------------------
FEHandler CFECore::Process::hStartProcess(TCoreProcessFunc _pProcess,FEPointer _pParam)
{
	TThreadData* poTD = new TThreadData;
	poTD->m_pFunc     = _pProcess;
	poTD->m_pParam    = _pParam;
	poTD->m_bFinished = false;

	if (pthread_create(&poTD->m_oThread,NULL,ThreadMain,poTD) != 0)
	{
		// Could not create the thread: run it synchronously.
		_pProcess(_pParam);
		poTD->m_bFinished = true;
		poTD->m_oThread   = 0;
	}

	return((FEHandler)poTD);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Process::bIsProcessFinished(FEHandler _hProcess)
{
	if (_hProcess == NULL) return(true);
	return( ((TThreadData*)_hProcess)->m_bFinished );
}
// ----------------------------------------------------------------------------
void CFECore::Process::EndProcess(FEHandler _hProcess)
{
	if (_hProcess == NULL) return;

	TThreadData* poTD = (TThreadData*)_hProcess;
	if (poTD->m_oThread != 0)
		pthread_join(poTD->m_oThread,NULL);

	delete poTD;
}
// ----------------------------------------------------------------------------
#endif // (TARGETPLATFORM == USE_LINUX)
// ----------------------------------------------------------------------------
