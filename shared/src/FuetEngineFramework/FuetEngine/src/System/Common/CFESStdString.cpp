// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (STRING    == USE_STDSTRING)
// ----------------------------------------------------------------------------
#include <string.h>
#include "../CFESystem.h"

// ----------------------------------------------------------------------------
int CFESystem::String::iStrCmp(const CFEString& _sA,const CFEString& _sB)
{
    return( strcmp(_sA.szString(),_sB.szString()) );
}
// ----------------------------------------------------------------------------
int CFESystem::String::iStrNCmp(const CFEString& _sA,const CFEString& _sB,uint _uiChars)
{
    return( strncmp(_sA.szString(),_sB.szString(),_uiChars) );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
