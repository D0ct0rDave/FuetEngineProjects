// ----------------------------------------------------------------------------
/*! \class CFELogger
 *  \brief Logging racilities implementation.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "System/CFESystem.h"
#include "FEConsts.h"
#include "CFELogger.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//-----------------------------------------------------------------------------
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
//-----------------------------------------------------------------------------
CFELogger::CFELogger(uint _uiBufferSize) : m_pucBuffer(NULL)
{
	m_uiBufferSize = _uiBufferSize;

	if (m_uiBufferSize>0)
	{
		m_pucBuffer = (char*)CFESystem::Mem::pAlloc(m_uiBufferSize);
		m_pucBuffer[0] = 0;
	}
}
//-----------------------------------------------------------------------------
void CFELogger::Print(char *fmt,...)
{
    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
    va_end  (argptr);

    AddMessage(gsszLocalBuff);
}
//-----------------------------------------------------------------------------
void CFELogger::Warning(char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"WARNING: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff+9,BUFF_SIZE-9,fmt,argptr);
    va_end  (argptr);

    AddMessage(gsszLocalBuff);
    AddMessage("\n");
}
//-----------------------------------------------------------------------------
void CFELogger::Error(char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"ERROR: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff+7,BUFF_SIZE-7,fmt,argptr);
    va_end  (argptr);

    AddMessage(gsszLocalBuff);
    AddMessage("\n");
}
// ----------------------------------------------------------------------------
void CFELogger::AddMessage(char* _szMessage)
{
	if (m_pucBuffer==NULL) return;

	uint uiLen = strlen(_szMessage);
	if (strlen(m_pucBuffer) + strlen(_szMessage) > m_uiBufferSize)
	{
		// Resets the buffer.
		m_pucBuffer[0] = 0;

		if (uiLen > m_uiBufferSize)
		{
			// Discard message: doesn't fit into the log buffer
			return;
		}
	}

	// Append the string to the end of the buffer (include the trailing zero)
	strcat_s(m_pucBuffer,m_uiBufferSize,_szMessage);
}
//-----------------------------------------------------------------------------
void CFELogger::Clean()
{
	if (m_pucBuffer != NULL)
		m_pucBuffer[0] = 0;
}
//-----------------------------------------------------------------------------
const char* CFELogger::szGetLog()
{
    return(m_pucBuffer);
}
// ----------------------------------------------------------------------------
/// Prints a message to the log.
void CFELogger::Sys::Print(char *fmt,...)
{
    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
    va_end  (argptr);

	CFESystem::Log::Print(gsszLocalBuff);
}
// ----------------------------------------------------------------------------
/// Writes an warning to the log.
void CFELogger::Sys::Warning(char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"WARNING: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff+9,BUFF_SIZE-9,fmt,argptr);
    va_end  (argptr);

	CFESystem::Log::Print(gsszLocalBuff);
	CFESystem::Log::Print("\n");
}
// ----------------------------------------------------------------------------
/// Writes an error to the log.
void CFELogger::Sys::Error(char *fmt,...)
{
	sprintf_s(gsszLocalBuff,BUFF_SIZE,"ERROR: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff+7,BUFF_SIZE-7,fmt,argptr);
    va_end  (argptr);

	CFESystem::Log::Print(gsszLocalBuff);
	CFESystem::Log::Print("\n");
}
// ----------------------------------------------------------------------------
