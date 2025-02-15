// ---------------------------------------------------------------------------
/*! \class CFEFont
 *  \brief Font object. Stores all the information related to a given font.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEFont.h"
#include "Support/Mem/CFEMem.h"
#include "Support/Graphics/CFEMaterialMgr.h"
// ----------------------------------------------------------------------------
CFEFont::CFEFont() : m_hMaterial(NULL), m_oCharTable(NULL), m_poKT(NULL)
{
}
// ----------------------------------------------------------------------------
CFEFont::~CFEFont()
{
	if (m_oCharTable != NULL)
		CFEMem::Free((FEPointer)m_oCharTable);
	
	if (m_poKT != NULL)
		delete m_poKT;
}
// ----------------------------------------------------------------------------
void CFEFont::ParseCoords(FEPointer _pBuff)
{
    // Set default char len for all the chars
	m_oCharTable = (TCharInfo*)_pBuff;
}
// ----------------------------------------------------------------------------
FEReal CFEFont::rStringLen(const CFEString& _sString) const
{
    char* szStr = (char*)_sString.szString();
    if (szStr == NULL) return(_0r);
    
    FEReal rLen = _0r;
    FEReal rMaxLen = _0r;
	char cPrevChar = 0;
    while (*szStr)
    {
		char c = *szStr;
        switch (c)
        {
			case '\n':
			{
        		if (rLen > rMaxLen)
        			rMaxLen = rLen;

        		rLen = _0r;
        		cPrevChar = 0;
        	}
        	break;

			case ' ':
            {
                //
                rLen += rCharWidth(' ');
                cPrevChar = 0;
            }
            break;

			default:
			{
				if (cPrevChar > 0)
				{
					rLen += rCharKerning(cPrevChar,c);
				}

				rLen += rCharWidth(c);
				cPrevChar = c;
			}
		}
		
        szStr++;
    }

	if (rLen > rMaxLen)
		rMaxLen = rLen;

    return(rMaxLen);
}
// ----------------------------------------------------------------------------
