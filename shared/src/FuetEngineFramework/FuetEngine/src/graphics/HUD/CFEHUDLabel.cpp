// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDLabel.h"
#include "system/CFESystem.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//-----------------------------------------------------------------------------
void CFEHUDLabel::SetText(char *fmt,...)
{
 	char szLocalBuff[8192];

	va_list argptr;
	va_start(argptr,fmt);
	vsprintf_s(szLocalBuff,8192,fmt,argptr);
	va_end  (argptr);

	SetText( CFEString(szLocalBuff) );
}
//-----------------------------------------------------------------------------
