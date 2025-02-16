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
static char gszLocalBuff[8192];
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
    vsprintf_s(gszLocalBuff,8192,fmt,argptr);
    va_end  (argptr);

    AddMessage(gszLocalBuff);
}
//-----------------------------------------------------------------------------
void CFELogger::Warning(char *fmt,...)
{
	sprintf_s(gszLocalBuff,8192,"WARNING: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gszLocalBuff+9,8192-9,fmt,argptr);
    va_end  (argptr);

    AddMessage(gszLocalBuff);
    AddMessage("\n");
}
//-----------------------------------------------------------------------------
void CFELogger::Error(char *fmt,...)
{
	sprintf_s(gszLocalBuff,8192,"ERROR: ");

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gszLocalBuff+7,8192-7,fmt,argptr);
    va_end  (argptr);

    AddMessage(gszLocalBuff);
    AddMessage("\n");
}
// ----------------------------------------------------------------------------
void CFELogger::AddMessage(char* _szMessage)
{
    // Initialize the logger in case it isn't
    if (m_pucBuffer == NULL)
    {
		CFESystem::Log::Print(_szMessage);
	}
	else
	{
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
