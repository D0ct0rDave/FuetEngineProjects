// ---------------------------------------------------------------------------
/*! \class CFEFont
 *  \brief Font object. Stores all the information related to a given font.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFont.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Graphics/CFEMaterialMgr.h"
// ----------------------------------------------------------------------------
CFEFont::CFEFont() : m_poKT(NULL), m_hMaterial(NULL)
{
}
// ----------------------------------------------------------------------------
CFEFont::~CFEFont()
{
	if (m_poKT != NULL)
		delete m_poKT;
}
// ----------------------------------------------------------------------------
FEReal CFEFont::rStringLen(const CFEString& _sString,uint _uiFrom,int _iLen) const
{
    char* szStr = (char*)_sString.szString() + _uiFrom;
    if (szStr == NULL) return(_0r);

    FEReal rLen = _0r;
	const CCharInfo* poPrevChar = 0;

	for (uint i=0;((i<(uint)_iLen) && (*szStr!=0));i++)
	{
		// --------------------
		uint c = uiGetCurrentChar(szStr);

		switch (c)
		{
			case ' ':
			{
				//
				rLen += rCharWidth(' ');
			}
			break;

			default:
			{
				const CCharInfo* poCurChar = poGetCharInfo(c);
				if (poCurChar!=NULL)
				{
					if (poPrevChar != NULL)
					{
						// add the kerning of this character and previous one.
						rLen += rCharKerning(poPrevChar,poCurChar);
					}

					rLen += poCurChar->m_usCW;
				}

				// Store character
				poPrevChar = poCurChar;
			}
		}

		// next char in the string.
		szStr = szGotoNextChar(szStr);
	}

    return(rLen);
}
// ----------------------------------------------------------------------------
