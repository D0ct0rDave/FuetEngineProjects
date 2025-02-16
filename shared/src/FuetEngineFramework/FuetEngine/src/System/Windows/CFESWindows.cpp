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
void CFESystem::Log::Print(char *fmt,...)
{
	#ifdef _DEBUG
    char szLocalBuff[8192];

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(szLocalBuff,8192,fmt,argptr);
    va_end  (argptr);

	OutputDebugString(szLocalBuff);
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
FEReal CFESystem::rGetEngineTime()
{
    // ------------------------------------------------------------------------
    // BUG: debería llamarse al constructor de FESglobals. No sé pq no se llama,
    if (FESglobals.m_uiStartTime == 0)
    {
        FESglobals.m_uiStartTime = GetTickCount();
    }
    // ------------------------------------------------------------------------

    uint uiTime = GetTickCount();

	// Retrieve DeltaT
	FEReal rDeltaT = (FEReal)(uiTime - FESglobals.m_uiStartTime) / 1000.0f ;

	// Accumulate DeltaT   
    FESglobals.m_rSystemTime += rDeltaT;

	// Reset StartTime
    FESglobals.m_uiStartTime = uiTime;

    return( FESglobals.m_rSystemTime );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
