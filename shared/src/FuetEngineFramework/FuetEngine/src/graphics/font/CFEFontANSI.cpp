// ---------------------------------------------------------------------------
/*! \class CFEFontANSI
 *  \brief Font object. Stores all the information related to a given font.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFontANSI.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Graphics/CFEMaterialMgr.h"
// ----------------------------------------------------------------------------
CFEFontANSI::CFEFontANSI()
{
	for (uint i=0;i<MAX_CHAR;i++)
	{
		m_oCharTable[i].m_cLC = 0;
		m_oCharTable[i].m_cRC = 0;
		m_oCharTable[i].m_sYOfs = 0;
		m_oCharTable[i].m_usCH = 0;
		m_oCharTable[i].m_usCW = 0;
		m_oCharTable[i].m_oUV.m_oIni.x = _0r;
		m_oCharTable[i].m_oUV.m_oIni.y = _0r;
		m_oCharTable[i].m_oUV.m_oEnd.x = _0r;
		m_oCharTable[i].m_oUV.m_oEnd.y = _0r;
	}
}
// ----------------------------------------------------------------------------
CFEFontANSI::~CFEFontANSI()
{
}
// ----------------------------------------------------------------------------
// do not take into account \n ...
// ----------------------------------------------------------------------------
uint CFEFontANSI::uiStringLen(char* _szStart,char* _szEnd) const
{
	if ((_szStart == NULL) || (_szEnd == NULL)) return(0);
	return(_szEnd-_szStart);
}
// ----------------------------------------------------------------------------
uint CFEFontANSI::uiGetCurrentChar(char *_szStr) const
{
	return( (uint)((unsigned char)(*_szStr)) );
}
// ----------------------------------------------------------------------------
char* CFEFontANSI::szGotoNextChar(char *_szStr) const
{
	return( _szStr+1 );
}
// ----------------------------------------------------------------------------
void CFEFontANSI::AddChar(uint _uiChar,const CCharInfo& _oCI)
{
	if (_uiChar>=256) return;
	m_oCharTable[_uiChar] = _oCI;
}
// ----------------------------------------------------------------------------
const CCharInfo* CFEFontANSI::poGetCharInfo(uint _uiChar) const
{
	if (_uiChar>=256) return(NULL);
	return( &m_oCharTable[_uiChar] );
}
// ----------------------------------------------------------------------------
