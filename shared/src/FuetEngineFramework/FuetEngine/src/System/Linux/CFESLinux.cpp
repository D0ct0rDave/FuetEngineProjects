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
#if (TARGETPLATFORM == USE_LINUX)
// ----------------------------------------------------------------------------

#include "CFESGlobals.h"
#include "../CFESystem.h"

// timing functionality
#include <sys/time.h>

// malloc / free
#include <stdlib.h>

// File Management
#include <stdio.h>

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Assert functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFESystem::Check(bool _bCondition,const CFEString& _sMessage)
{
    if (_bCondition != true)
    {
        printf("%s",_sMessage.szString());
        exit(0);
    }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// String functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
int CFESystem::String::iStrICmp(const CFEString& _sA,const CFEString& _sB)
{
    return( strcasecmp(_sA.szString(),_sB.szString()) );
}
// ----------------------------------------------------------------------------
int CFESystem::String::iStrNICmp(const CFEString& _sA,const CFEString& _sB,uint _uiChars)
{
    return( strncasecmp(_sA.szString(),_sB.szString(),_uiChars) );
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Timing functinoality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
FEReal CFESystem::rGetEngineTime()
{
    timeval oTime;
    if (gettimeofday(&oTime,NULL) == 0)
    {
        unsigned long long ullTime = oTime.tv_sec*1000000 + oTime.tv_usec;
        return( (float)(ullTime - FESglobals.m_ullStartTime) / 1000000.0f );
    }
    else
        return(0.0);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
