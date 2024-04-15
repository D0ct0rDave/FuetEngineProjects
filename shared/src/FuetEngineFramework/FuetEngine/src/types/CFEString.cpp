// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
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
#include "support/misc/CFEStringUtils.h"
#include <string> // for rfind (reverse find)
// ----------------------------------------------------------------------------
static CFEString gsNULLString = CFEString("");
// ----------------------------------------------------------------------------
const CFEString& CFEString::sNULL()
{
    return(gsNULLString);
}
// ----------------------------------------------------------------------------
inline uint uiLen_Internal(const char* _szStr)
{
	if ( _szStr == NULL )
		return(0);
	else
		return ( strlen(_szStr) );
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
inline void CreateCombined(CFEStringData* _poData,const char* _szA,uint _uiALen,const char* _szB,uint _uiBLen)
{
	uint uiTLen	= _uiALen + _uiBLen;
	if (uiTLen == 0) return;

	char* szNewStr = (char*)CFECore::Mem::pAlloc(uiTLen+1);

	if (_uiALen>0) memcpy( (void*)szNewStr            ,_szA,_uiALen);
	if (_uiBLen>0) memcpy( (void*)(szNewStr + _uiALen),_szB,_uiBLen);
	szNewStr[uiTLen] = 0;

	if (_poData->m_szData != NULL) CFECore::Mem::Free( (FEPointer)_poData->m_szData );

	_poData->m_szData  = szNewStr;
	_poData->m_uiLen   = uiTLen;
	_poData->m_bDirty  = true;
}
// ----------------------------------------------------------------------------
inline void CreateCombined(CFEStringData* _poData,const CFEString& _sA,const char* _szB)
{
	CreateCombined(_poData,_sA.szString(),_sA.uiLen(),_szB,uiLen_Internal(_szB));
}
// ----------------------------------------------------------------------------
inline void CreateCombined(CFEStringData* _poData,const CFEString& _sA,const CFEString& _sB)
{
	CreateCombined(_poData,_sA.szString(),_sA.uiLen(),_sB.szString(),_sB.uiLen());
}
// ----------------------------------------------------------------------------
CFEString::~CFEString()
{
    if (m_oData.m_szData != NULL)
		Free();
}
// ----------------------------------------------------------------------------
void CFEString::Alloc(uint _uiSize)
{
    if ( m_oData.m_szData != NULL )
        Free();

    if ( _uiSize == 0 ) return;
    
    m_oData.m_szData = (char*)CFECore::Mem::pAlloc(_uiSize);
    m_oData.m_uiLen = _uiSize-1;
}
// ----------------------------------------------------------------------------
void CFEString::Free()
{
    if ( m_oData.m_szData != NULL )
    {
        // CFEMem::Free((FEPointer)m_szData);
        CFECore::Mem::Free((FEPointer)m_oData.m_szData);
        m_oData.m_szData = NULL;
    }
	
    m_oData.m_bDirty = true;
    m_oData.m_uiHash = 0;
    m_oData.m_uiLen  = 0;
}
// ----------------------------------------------------------------------------
/// Copy the formatted string to the string object
void CFEString::Format(const char* fmt,...)
{
    if ( fmt == NULL ) return;

    va_list argptr;
    va_start(argptr,fmt);
    char* szStr = CFECore::String::szFormatString(fmt,&argptr);
    va_end  (argptr);

    Assign( szStr );
}
// ----------------------------------------------------------------------------
CFEString CFEString::sFormat(const char* fmt,...)
{
    if ( fmt == NULL ) return(CFEString::sNULL());

	va_list argptr;
	va_start(argptr, fmt);
	char* szStr = CFECore::String::szFormatString(fmt, &argptr);
	va_end(argptr);

    return( CFEString(szStr) );	
}
// ----------------------------------------------------------------------------
void CFEString::Assign(const CFEString& _sStr)
{
	if (( _sStr.m_oData.m_szData == NULL ) || (_sStr.m_oData.m_szData[0] == 0))
	{
		if (m_oData.m_szData != NULL) Free();
		return; // that's all ...
	}

 	if ((m_oData.m_szData == NULL) || (_sStr.m_oData.m_uiLen > m_oData.m_uiLen))
	{
		if (m_oData.m_szData != NULL) Free();
		Alloc(_sStr.m_oData.m_uiLen + 1);
	}

	memcpy(m_oData.m_szData,_sStr.m_oData.m_szData,_sStr.m_oData.m_uiLen + 1);

	m_oData.m_bDirty = _sStr.m_oData.m_bDirty;
	m_oData.m_uiHash = _sStr.m_oData.m_uiHash;
	m_oData.m_uiLen  = _sStr.m_oData.m_uiLen;
}
// ----------------------------------------------------------------------------
/// Copy the string contents to the current
void CFEString::Assign(const char* _szStr)
{		
	if (( _szStr == NULL ) || (_szStr[0] == 0))
	{
		if (m_oData.m_szData != NULL) Free();
		return; // that's all ...
	}

	uint uiStrLen = strlen(_szStr);
 	if ((m_oData.m_szData == NULL) || (uiStrLen > m_oData.m_uiLen))
	{
		if (m_oData.m_szData != NULL) Free();
		Alloc(uiStrLen + 1);
	}

	memcpy(m_oData.m_szData,_szStr,uiStrLen + 1);
	m_oData.m_uiLen = uiStrLen;

	SetDirty();

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
/// Returns the length of the string
uint CFEString::uiLen() const
{
	return(m_oData.m_uiLen);
}
// ----------------------------------------------------------------------------
/// Returns the position of a substring inside the string, -1 if not found
int CFEString::iPos(const CFEString& _oSubStr) const
{
    return ( iPos(_oSubStr.szString()) );
}
// ----------------------------------------------------------------------------
int CFEString::iPos(const char* _szStr) const
{
    if ( _szStr == NULL ) return(-1);
    if ( m_oData.m_szData == NULL ) return(-1);

    char* ptr = strstr( m_oData.m_szData,_szStr );

    if ( ptr == NULL )
        return(-1);
    else
        return(ptr - m_oData.m_szData);
}
// ----------------------------------------------------------------------------
/// Returns the position of a substring inside the string, from the end of the string and going backwards, -1 if not found.
int CFEString::iRPos(const CFEString& _oSubStr) const
{
    return ( iRPos(_oSubStr.szString()) );   
}
// ----------------------------------------------------------------------------
int CFEString::iRPos(const char* _szStr) const
{
    if ( _szStr == NULL ) return(-1);
    if ( m_oData.m_szData == NULL ) return(-1);

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
    memcpy( (void*)oRes.szString(),m_oData.m_szData + _uiPos,_uiLen );
    ( (char*)oRes.szString() )[_uiLen] = 0;
	oRes.SetDirty();

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

    SetDirty();
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

    SetDirty();
}
// ----------------------------------------------------------------------------
void CFEString::Concat(const char* _szStr)
{
	uint uiDLen = uiLen_Internal(_szStr);
	if ( uiDLen == 0 ) return;      // Nothing to concat

	CreateCombined(&m_oData,*this,_szStr);
}
// ----------------------------------------------------------------------------
void CFEString::Concat(const CFEString& _oStr)
{
	uint uiDLen = _oStr.uiLen();
	if ( uiDLen == 0 ) return;      // Nothing to concat

	CreateCombined(&m_oData,szString(),uiLen(),_oStr.szString(),_oStr.uiLen());
}
// ----------------------------------------------------------------------------
/*
FEBool CFEString::operator == (const CFEString& _oStr) const
{
    if ( m_szData == NULL ) return(_oStr.m_szData == NULL);
    if ( _oStr.m_szData == NULL ) return(false);

    return (CFECore::String::iStrCmp(*this,_oStr) == 0);
}
// ----------------------------------------------------------------------------
FEBool CFEString::operator != (const CFEString& _oStr) const
{
	if ( m_szData == NULL ) return(_oStr.m_szData != NULL);
	if ( _oStr.m_szData == NULL ) return(true);

	return (CFECore::String::iStrCmp(*this,_oStr) != 0);
}
*/
// ----------------------------------------------------------------------------
FEBool CFEString::operator ==(const char* _szStr) const
{
	CFEString sStr = _szStr;
	return(sStr == *this);
}
// ----------------------------------------------------------------------------
#ifdef _DEBUG
int string_compare_path_1 = 0;
int string_compare_path_2 = 0;
int string_compare_path_3 = 0;
#endif

FEBool CFEString::operator == (const CFEString& _right) const
{
	if ((uiGetHash() != _right.uiGetHash()) || (uiLen() != _right.uiLen()) )
	{
		// most of the time will go through this path ...
		#ifdef _DEBUG
		string_compare_path_1++;
		#endif
		return(false);		
	}
	else
	{
		if ((m_oData.m_szData == NULL) || (_right.m_oData.m_szData == NULL))
		{
			// At least one of them is NULL
			#ifdef _DEBUG
			string_compare_path_2++;
			#endif

			if (m_oData.m_szData == _right.m_oData.m_szData)
				// if both are NULL, they'll go through this branch
				return(true);

			// At this point one and only one of them is NULL!
			else if (m_oData.m_szData == NULL)
				return(_right.m_oData.m_szData[0] == 0);

			else 
				return(m_oData.m_szData[0] == 0);
		}
		else
		{
			// if both hash are equal we cannot guarantee they are exactly the same value ...
			#ifdef _DEBUG
			string_compare_path_3++;
			#endif

			return(! memcmp(m_oData.m_szData, _right.m_oData.m_szData, m_oData.m_uiLen));
		}
	}
}
// ----------------------------------------------------------------------------
FEBool CFEString::operator !=(const char* _right) const
{
	return( ! operator==(_right) );
}

FEBool CFEString::operator != (const CFEString& _right) const
{
	return( ! operator==(_right) );
}
// ----------------------------------------------------------------------------
/*
FEBool CFEString::operator != (const CFEString& _oStr) const
{
	return( CFEString::operator!=(_oStr.m_szData) );
}

FEBool CFEString::operator !=(const char* _szStr) const
{
	return( ! CFEString::operator==(_szStr) );
}
*/
// ----------------------------------------------------------------------------
FEBool CFEString::operator |= (const CFEString& _oStr) const
{
    return( operator|=(_oStr.szString()) );
}

FEBool CFEString::operator |= (const char* _szStr) const
{
	if (_szStr == NULL)
	{
		if (m_oData.m_szData == NULL)
			// a == b
			return(true);
		else
		{
			return(m_oData.m_szData[0] == 0);
		}
	}
else if (m_oData.m_szData == NULL)
	{
		// a > b
		return(*_szStr == 0);
	}

	return (CFECore::String::iStrICmp(m_oData.m_szData,_szStr) == 0);
}
// ----------------------------------------------------------------------------
void CFEString::ComputeHash() const
{
	// Dirty trick to preserve const-ness
	CFEString& oRef = const_cast<CFEString&>(*this);

	if (oRef.m_oData.m_szData == NULL )
		oRef.m_oData.m_uiHash	= 0;
	else
		oRef.m_oData.m_uiHash = CFEStringUtils::uiGetHash(oRef.m_oData.m_szData);

	oRef.m_oData.m_bDirty = false;
}
// ----------------------------------------------------------------------------
CFEString CFEString::operator + (const CFEString& _oStr) const
{
	CFEString sRes;
	CreateCombined(&sRes.m_oData,*this,_oStr);
    return(sRes);
}

CFEString CFEString::operator + (const char* _szStr) const
{
	CFEString sRes;
	CreateCombined(&sRes.m_oData,*this,_szStr);
    return(sRes);
}

CFEString CFEString::operator + (const char _cChar) const
{
	CFEString sRes;	
	char szStr[4] = {_cChar,0};
	CreateCombined(&sRes.m_oData,*this,szStr);
    return(sRes);
}
// ----------------------------------------------------------------------------
