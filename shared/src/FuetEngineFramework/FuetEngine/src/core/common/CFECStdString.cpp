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
#if (STRING == USE_STDSTRING)
// ----------------------------------------------------------------------------
#include <string.h>
#include <stdarg.h> 
#include "../CFECore.h"

//-----------------------------------------------------------------------------
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
static char gsszLocalFmt[BUFF_SIZE];
//-----------------------------------------------------------------------------
static char* szCheckAndFixFmtString(const char* fmt,va_list argptr)
{
	char* szDstFmt = gsszLocalFmt;	
    char* szArg = NULL;
	bool bChange = false;

	int i = 0;
    for(;fmt[i] != '\0'; ++i )
    {		
		szDstFmt[i] = bChange?'d':fmt[i];
		bChange = false;

		if (fmt[i] == '%')
		{
			if (fmt[i+1]!='%')
			{
				szArg = va_arg(argptr,char*);

				if ((fmt[i+1] == 's') && (szArg == NULL))
					bChange = true;
			}
			else
			{
				i++;
				szDstFmt[i] = '%';
			}
		}
	}

	// NULL terminate this string
	szDstFmt[i] ='\0';
	return(gsszLocalFmt);
}
//-----------------------------------------------------------------------------
char* CFECore::String::szFormatString(const char * format, va_list arg)
{
	char* szFixedFormat = szCheckAndFixFmtString(format,arg);
	vsprintf_s(gsszLocalBuff,BUFF_SIZE,szFixedFormat,arg);
	return(gsszLocalBuff);
}
// ----------------------------------------------------------------------------
int CFECore::String::iStrCmp(const char* _szA,const char* _szB)
{
    return( strcmp(_szA,_szB) );
}
// ----------------------------------------------------------------------------
int CFECore::String::iStrNCmp(const char* _szA,const char* _szB,uint _uiChars)
{
    return( strncmp(_szA,_szB,_uiChars) );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
