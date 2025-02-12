// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#if defined(_MSC_VER) && (_MSC_VER < 1900)
// since c++11
#define va_copy(a,b) a = b
#endif
//-----------------------------------------------------------------------------
const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];
static char gsszLocalFmt[BUFF_SIZE];
//-----------------------------------------------------------------------------
static char* szCheckAndFixFmtString(const char* fmt,va_list* pArgptr)
{
	char* szDstFmt = gsszLocalFmt;	
    char* szArg = NULL;

	va_list argptr;

	va_copy(argptr, *pArgptr);
	// va_start(argptr, fmt);

		int i = 0;
		for(;fmt[i] != '\0'; ++i )
		{		
			szDstFmt[i] = fmt[i];

			if (fmt[i] == '%')
			{
				i++;
				if (fmt[i] != '%')
				{
					szArg = va_arg(argptr,char*);

					if ((fmt[i] == 's') && (szArg == NULL))
					{						
						szDstFmt[i] = 'd';
					}
					else
					{
						szDstFmt[i] = fmt[i];
					}
				}
				else
				{
					szDstFmt[i] = '%';
				}
			}
		}

	va_end(argptr);

	// NULL terminate this string
	szDstFmt[i] ='\0';
	return(gsszLocalFmt);
}
//-----------------------------------------------------------------------------
char* CFECore::String::szFormatString(const char * format, va_list* pArg)
{
	char* szFixedFormat = szCheckAndFixFmtString(format,pArg);
	vsprintf_s(gsszLocalBuff,BUFF_SIZE,szFixedFormat,*pArg);	
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
