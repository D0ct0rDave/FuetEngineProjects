// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
// IMPORTANT NOTE a NULL string can be one with:
// 1 - m_szData == NULL
// 2 - (m_szData != NULL) && (m_szData[0] = 0)
// ----------------------------------------------------------------------------
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "CFEString.h"
#include "core/CFECore.h"
#include "Support/Mem/CFEMem.h"
#include <string> // for rfind (reverse find)
// ----------------------------------------------------------------------------
static CFEString gsNULLString = CFEString("");

const uint BUFF_SIZE = 2048;
static char gsszLocalBuff[BUFF_SIZE];

const CFEString& CFEString::sNULL()
{
    return(gsNULLString);
}
// ----------------------------------------------------------------------------
inline char TOUPPER(char c)
{
    if ( (c >= 'a') && (c <= 'z') )
        return (c - 32);
    else
        return(c);
}
// ----------------------------------------------------------------------------
inline char TOLOWER(char c)
{
    if ( (c >= 'A') && (c <= 'Z') )
        return (c + 32);
    else
        return(c);
}
// ----------------------------------------------------------------------------
CFEString::~CFEString()
{
    Free();
}
// ----------------------------------------------------------------------------
void CFEString::Alloc(uint _uiSize)
{
    if ( m_szData != NULL )
        Free();

    if ( _uiSize == 0 ) return;

	// EFEMemAllocPolicy eOldPolicy =CFEMem::eGetMemPolicy();
	// CFEMem::SwitchPolicy(MP_STATIC_ALLOCATION); 

    // m_szData = (char*)CFEMem::pAlloc(_uiSize);
    
    // CFEMem::SwitchPolicy(eOldPolicy);

    m_szData = (char*)CFECore::Mem::pAlloc(_uiSize);
}
// ----------------------------------------------------------------------------
void CFEString::Free()
{
    if ( m_szData != NULL )
    {
        // CFEMem::Free((FEPointer)m_szData);
        CFECore::Mem::Free((FEPointer)m_szData);
        m_szData = NULL;
    }
}
// ----------------------------------------------------------------------------
/// Copy the formatted string to the string object
void CFEString::Format(const char* fmt,...)
{
    if ( fmt == NULL ) return;

    va_list argptr;
    va_start(argptr,fmt);
    vsprintf_s(gsszLocalBuff,BUFF_SIZE,fmt,argptr);
    va_end  (argptr);

    // -----------------------------
    Assign( gsszLocalBuff );
}
// ----------------------------------------------------------------------------
/// Copy the string contents to the current
void CFEString::Assign(const char* _szStr)
{		
	if (( _szStr == NULL ) || (_szStr[0] == 0))
	{
		if (m_szData != NULL) Free();
		return; // that's all ...
	}
	
	uint uiStrLen = strlen(_szStr);
 	if ((m_szData == NULL) || (uiStrLen > strlen(m_szData)))
	{
		if (m_szData != NULL) Free();
		Alloc(uiStrLen + 1);
	}

	memcpy(m_szData,_szStr,uiStrLen + 1);

	
	/*
    if (m_szData != NULL) Free();
    if ( _szStr == NULL ) return;

    uint uiLen = strlen(_szStr);
    if ( uiLen == 0 ) return;

    Alloc(uiLen + 1);
    memcpy(m_szData,_szStr,uiLen + 1);
    */
}
// ----------------------------------------------------------------------------
/// Links the string contents to the given char* string.
/// Be careful with assignments since internal deallocations can be dangerous.
/// One can also relink to a NULL pointer, to ensure deallocations will not hurt.
/*
void CFEString::Link(char* _szStr)
{
    m_szData = _szStr;
}
*/
// ----------------------------------------------------------------------------
inline uint uiLen_Internal(const char* _szStr)
{
	if ( _szStr == NULL )
		return(0);
	else
		return ( strlen(_szStr) );
}
// ----------------------------------------------------------------------------
/// Returns the length of the string
uint CFEString::uiLen() const
{
	return(uiLen_Internal(m_szData));
}
// ----------------------------------------------------------------------------
/// Returns the position of a substring inside the string, -1 if not found
int CFEString::iPos(const CFEString& _oSubStr) const
{
    const char* szStr = _oSubStr.szString();
    if ( szStr == NULL ) return(-1);

    return ( iPos(szStr) );
}
// ----------------------------------------------------------------------------
int CFEString::iPos(const char* _szStr) const
{
    if ( _szStr == NULL ) return(-1);
    if ( m_szData == NULL ) return(-1);

    char* ptr = strstr( m_szData,_szStr );

    if ( ptr == NULL )
        return(-1);
    else
        return(ptr - m_szData);
}
// ----------------------------------------------------------------------------
/// Returns the position of a substring inside the string, from the end of the string and going backwards, -1 if not found.
int CFEString::iRPos(const CFEString& _oSubStr) const
{
    const char* szStr = _oSubStr.szString();
    if ( szStr == NULL ) return(-1);

    return ( iPos(szStr) );
}
// ----------------------------------------------------------------------------
int CFEString::iRPos(const char* _szStr) const
{
    if ( _szStr == NULL ) return(-1);
    if ( m_szData == NULL ) return(-1);

    std::string s( szString() );
    return ( (int)s.rfind( _szStr ) );
}
// ----------------------------------------------------------------------------
CFEString CFEString::SubString(uint _uiPos,uint _uiLen) const
{
    CFEString oRes;

    uint uiLen = this->uiLen();
    if ( _uiPos >= uiLen )
        return(oRes);

    if ( (_uiPos + _uiLen) > uiLen )
        _uiLen = uiLen - _uiPos;

    oRes.Alloc(_uiLen + 1);
    memcpy( (void*)oRes.szString(),m_szData + _uiPos,_uiLen );
    ( (char*)oRes.szString() )[_uiLen] = 0;

    return(oRes);
}
// ----------------------------------------------------------------------------
/// Returns the string in upper case
CFEString CFEString::UpperCase() const
{
    CFEString asRes(*this);
    asRes.ToUpperCase();
    return (asRes);
}
// ----------------------------------------------------------------------------
/// Converts the string to upper case
void CFEString::ToUpperCase()
{
    char* szBase = (char*)szString();
    uint uiLen = this->uiLen();

    for ( uint i = 0; i < uiLen; i++ )
        szBase[i] = TOUPPER( szBase[i] );
}
// ----------------------------------------------------------------------------
/// Returns a copy of the string in lower case
CFEString CFEString::LowerCase() const
{
    CFEString asRes(*this);
    asRes.ToLowerCase();
    return (asRes);
}
// ----------------------------------------------------------------------------
/// Converts to lower case
void CFEString::ToLowerCase()
{
    char* szBase = (char*)szString();
    uint uiLen = this->uiLen();

    for ( uint i = 0; i < uiLen; i++ )
        szBase[i] = TOLOWER( szBase[i] );
}
// ----------------------------------------------------------------------------
void CFEString::Concat(const char* _szStr)
{
	uint uiDLen = uiLen_Internal(_szStr);
	if ( uiDLen == 0 ) return;      // Nothing to concat

	uint uiSLen = this->uiLen();
	uint uiTLen = uiSLen + uiDLen;

	char* szNewStr = (char*)CFECore::Mem::pAlloc(uiTLen + 1);
	if ( uiSLen > 0 )
		memcpy( (void*)szNewStr,m_szData,uiSLen );

	memcpy( (void*)(szNewStr + uiSLen),_szStr,uiDLen );
	szNewStr[uiTLen] = 0;

	Free();
	m_szData = szNewStr;
}

void CFEString::Concat(const CFEString& _oStr)
{
	Concat(_oStr.szString());
}
// ----------------------------------------------------------------------------
FEBool CFEString::operator == (const CFEString& _oStr) const
{
	return( CFEString::operator==(_oStr.m_szData) );
}

FEBool CFEString::operator == (const char* _szStr) const
{
	if (_szStr == NULL)
	{
		if (m_szData == NULL)
			// a == b
			return(true);
		else
		{
			return(*m_szData == 0);
		}
	}
else if (m_szData == NULL)
	{
		// a > b
		return(*_szStr == 0);
	}

    return (CFECore::String::iStrCmp(m_szData,_szStr) == 0);
}
// ----------------------------------------------------------------------------
FEBool CFEString::operator |= (const CFEString& _oStr) const
{
    return( CFEString::operator|=(_oStr.m_szData) );
}

FEBool CFEString::operator |= (const char* _szStr) const
{
	if (_szStr == NULL)
	{
		if (m_szData == NULL)
			// a == b
			return(true);
		else
		{
			return(*m_szData == 0);
		}
	}
else if (m_szData == NULL)
	{
		// a > b
		return(*_szStr == 0);
	}

	return (CFECore::String::iStrICmp(m_szData,_szStr) == 0);
}
// ----------------------------------------------------------------------------
FEBool CFEString::operator != (const CFEString& _oStr) const
{
	return( CFEString::operator!=(_oStr.m_szData) );
}

FEBool CFEString::operator !=(const char* _szStr) const
{
	return( ! CFEString::operator==(_szStr) );
}
// ----------------------------------------------------------------------------
