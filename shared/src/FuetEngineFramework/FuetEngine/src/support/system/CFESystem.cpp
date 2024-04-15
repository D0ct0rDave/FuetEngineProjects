// ----------------------------------------------------------------------------
/*! \class CFESystem
 *  \brief Logging racilities implementation.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "core/CFECore.h"
#include "FEConsts.h"
#include "CFESystem.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//-----------------------------------------------------------------------------
CFEMomentInTime CFESystem::oGetMomentInTime()
{
	return( CFECore::oGetMomentInTime() );
}
//-----------------------------------------------------------------------------
/// Prints a message to the system log.
// ----------------------------------------------------------------------------

/// Starts a process to be executed in parallel to the main process (thread)
FEHandler CFESystem::hStartProcess(TProcessFunc _pProcess,FEPointer _pParam)
{
	return(CFECore::Process::hStartProcess(_pProcess,_pParam));
}

/// Tells whether a parallel process has finished its execution or not.
FEBool CFESystem::bIsProcessFinished(FEHandler _hProcess)
{
	return(CFECore::Process::bIsProcessFinished(_hProcess));
}

/// Frees the resources allocated by an already finished process.
void CFESystem::EndProcess(FEHandler _hProcess)
{
	CFECore::Process::EndProcess(_hProcess);
}
//-----------------------------------------------------------------------------
/// Prints a message to the system log.
// ----------------------------------------------------------------------------
#ifndef MASTER_RELEASE_VERSION 
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
// ----------------------------------------------------------------------------
void CFESystem::SetSystemLogFunc(TSystemLogFunc _pProcess)
{
	CFECore::Log::SetSystemLogFunc(_pProcess);
}
// ----------------------------------------------------------------------------
void CFESystem::Print(const char *fmt,...)
{
	va_list argptr;
	va_start(argptr,fmt);
	CFECore::Log::Print(fmt,&argptr);
    va_end  (argptr);
}
// ----------------------------------------------------------------------------
/// Writes an warning to the log.
void CFESystem::Warning(const char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"WARNING: %s\n",fmt);
	
	va_list argptr;
	va_start(argptr,fmt);
	CFECore::Log::Print(gsszLocalBuff,&argptr);
    va_end  (argptr);
}
// ----------------------------------------------------------------------------
/// Writes an error to the log.
void CFESystem::Error(const char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"ERROR: %s\n",fmt);

	va_list argptr;
	va_start(argptr,fmt);
	CFECore::Log::Print(gsszLocalBuff,&argptr);
    va_end  (argptr);
}
#else
// ----------------------------------------------------------------------------
void CFESystem::SetSystemLogFunc(TSystemLogFunc) { }
void CFESystem::Print(const char *,...) { }
void CFESystem::Warning(const char *,...) { }
void CFESystem::Error(const char *,...) { }
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
