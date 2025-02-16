// ---------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
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

    while (*szStr)
    {
        if (*szStr == '\n')
        {
        	if (rLen > rMaxLen) rMaxLen = rLen;
        	rLen = _0r;
        }
        else
			rLen += (rCharWidth(*szStr) + rCharKerning());

        szStr++;
    }

	if (rLen > rMaxLen) rMaxLen = rLen;

    return(rMaxLen);
}
// ----------------------------------------------------------------------------
