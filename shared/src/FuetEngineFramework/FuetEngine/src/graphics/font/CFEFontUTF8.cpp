// ---------------------------------------------------------------------------
/*! \class CFEFontUTF8
 *  \brief Font object. Stores all the information related to a given font.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFontUTF8.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Graphics/CFEMaterialMgr.h"
// ----------------------------------------------------------------------------
const uint HASH_WIDTH = 256;
// ----------------------------------------------------------------------------
CFEFontUTF8::CFEFontUTF8()/* : m_oCharTable(NULL), m_poKT(NULL), m_hMaterial(NULL)*/
{
	m_oCharTable.Init(4,HASH_WIDTH);

	CCharInfo oCI;
	AddChar(0,oCI);
	AddChar(' ',oCI);
}
// ----------------------------------------------------------------------------
CFEFontUTF8::~CFEFontUTF8()
{
	for (uint i=0;i<m_oCharTable.uiNumCols();i++)
	{
		CFELookUpDatabase<CCharInfo>* poCILst = m_oCharTable.poGetList(i);
		for (uint j=0;j<poCILst->uiNumElems();j++)
		{
			CCharInfo* poCharInf = poCILst->poGetAt(j);
			delete poCharInf;
		}
		poCILst->Clear();
	}

	m_oCharTable.Finish();
}
// ----------------------------------------------------------------------------
// do not take into account \n ...
// ----------------------------------------------------------------------------
uint CFEFontUTF8::uiStringLen(char* _szStart,char* _szEnd) const
{
	if ((_szStart == NULL) || (_szEnd == NULL)) return(0);

	uint uiNumChars = 0;
	while (_szStart < _szEnd)
	{
		_szStart = szGotoNextChar(_szStart);
		uiNumChars++;
	}
	return(uiNumChars);
}
// ----------------------------------------------------------------------------
uint CFEFontUTF8::uiGetCurrentChar(char *_szStr) const
{
/*
	uint uiValue = *(uint*)_szStr;
	if (uiValue <     0x7f) return(uiValue &= 0x000000ff);
	if (uiValue <   0x7fff) return(uiValue &= 0x0000ffff);
	if (uiValue < 0x7fffff) return(uiValue &= 0x00ffffff);
	return(uiValue);
*/
	// ((char & 11000000) == 10000000) -> continue
	// range(char) == range(_szStr[x]) € (-128..127)

	// if _szStr[0] >= 0, _szStr[0]<0x80 -> simple ASCII char
	if (_szStr[0] >= 0) return( (uint) *((unsigned int*)_szStr) & 0x000000ff );  // return directly
	
	// now szStr[0] > 0x80 -> 
	// in UTF8 if szStr[0] == 11xx xxxx -> there is always a second char
	// if ((_szStr[1] & 0xc0) != 0x80)) // comment because there is always a second char

	// But a third may be not!
	if ((_szStr[2] & 0xc0) != 0x80) return( (uint) *((unsigned int*)_szStr) & 0x0000ffff );
	if ((_szStr[3] & 0xc0) != 0x80) return( (uint) *((unsigned int*)_szStr) & 0x00ffffff );
	return( *(uint*)_szStr );
}
// ----------------------------------------------------------------------------
char* CFEFontUTF8::szGotoNextChar(char *_szStr) const
{
	do 
	{
		_szStr++;
	} while ((*_szStr & 0xc0) == 0x80);
	return(_szStr);
}
// ----------------------------------------------------------------------------
// PRE: ((char*)&_uiChar)[3] = 0;  !!!
// ----------------------------------------------------------------------------
void CFEFontUTF8::AddChar(uint _uiChar,const CCharInfo& _oCI)
{
	// ALL OF THIS IS A REALLY DIRTY QUICK UGLY HACK!!!
	if (_uiChar == 0) _uiChar = 1;

	CFEString sStr;
	CFEStringData* poStrData = (CFEStringData*)&sStr;
	poStrData->m_szData = (char*)&_uiChar;
	poStrData->m_uiLen  = 3;
	poStrData->m_uiHash = _uiChar;
	poStrData->m_bDirty = false;

		m_oCharTable.AddVar(sStr,new CCharInfo(_oCI));

	poStrData->m_szData = NULL;
}
// ----------------------------------------------------------------------------
const CCharInfo* CFEFontUTF8::poGetCharInfo(uint _uiChar) const
{
	// ALL OF THIS IS A REALLY DIRTY QUICK UGLY HACK!!!
	if (_uiChar == 0) _uiChar = 1;

	CFEString sStr;
	CFEStringData* poStrData = (CFEStringData*)&sStr;
	poStrData->m_szData = (char*)&_uiChar;
	poStrData->m_uiLen  = 3;
	poStrData->m_uiHash = _uiChar;
	poStrData->m_bDirty = false;

		CCharInfo* poRes = m_oCharTable.poGetVar(sStr);

	poStrData->m_szData = NULL;

	return(poRes);
}
// ----------------------------------------------------------------------------
