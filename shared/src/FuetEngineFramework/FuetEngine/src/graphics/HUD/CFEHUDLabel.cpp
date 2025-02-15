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
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
//-----------------------------------------------------------------------------
CFEHUDLabel::~CFEHUDLabel()
{
	SetText("");
}
//-----------------------------------------------------------------------------
void CFEHUDLabel::SetText(char *fmt,...)
{
	va_list argptr;
	va_start(argptr,fmt);
	vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
	va_end  (argptr);

	SetText( CFEString(gsszLocalBuff) );
}
//-----------------------------------------------------------------------------
