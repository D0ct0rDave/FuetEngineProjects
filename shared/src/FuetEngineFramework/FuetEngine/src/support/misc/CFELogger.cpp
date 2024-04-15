// ----------------------------------------------------------------------------
/*! \class CFELogger
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
#include "CFELogger.h"

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//-----------------------------------------------------------------------------
CFELogger::CFELogger(uint _uiBufferSize) : m_pucBuffer(NULL)
{
	m_uiBufferSize = _uiBufferSize;

	if (m_uiBufferSize>0)
	{
		m_pucBuffer = (char*)CFECore::Mem::pAlloc(m_uiBufferSize);
		m_pucBuffer[0] = 0;
	}
}
//-----------------------------------------------------------------------------
void CFELogger::Print(const char *fmt,...)
{
	va_list argptr;
	va_start(argptr,fmt);
	char* str = CFECore::String::szFormatString(fmt,&argptr);
	va_end  (argptr);
	AddMessage(str);
}
//-----------------------------------------------------------------------------
void CFELogger::Warning(const char *fmt,...)
{
	AddMessage("WARNING: ");

	va_list argptr;
	va_start(argptr,fmt);
	char* str = CFECore::String::szFormatString(fmt,&argptr);
	va_end  (argptr);

	AddMessage(str);
	AddMessage("\n");
}
//-----------------------------------------------------------------------------
void CFELogger::Error(const char *fmt,...)
{
	AddMessage("ERROR: ");

	va_list argptr;
	va_start(argptr,fmt);
	char* str = CFECore::String::szFormatString(fmt,&argptr);
	va_end  (argptr);

	AddMessage(str);
	AddMessage("\n");
}
// ----------------------------------------------------------------------------
void CFELogger::AddMessage(const char* _szMessage)
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
	#ifdef WIN32
	strcat_s(m_pucBuffer,m_uiBufferSize,_szMessage);
	#else
	strcat(m_pucBuffer,_szMessage);
	#endif
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
