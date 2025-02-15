// ----------------------------------------------------------------------------
/*! \class CFEFile
 *  \brief SFile class implementation
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <assert.h>
// ----------------------------------------------------------------------------
// CFEFile
#include "CFEFile.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
CFEFile::CFEFile() : m_hFD(NULL)
{
}
// ----------------------------------------------------------------------------
CFEFile::~CFEFile()
{
}
// ----------------------------------------------------------------------------
bool CFEFile::bOpen(const CFEString& _sFilename, EFEFileOpenMode _eMode)
{
    m_hFD = CFESystem::File::hOpen(_sFilename,_eMode);
    if (m_hFD != NULL)
    {
        m_sFilename = _sFilename;
        return(true);       
    }
    else
        return(false);
}
// ----------------------------------------------------------------------------
void CFEFile::Close ()
{    
    if (m_hFD != NULL)
    {
        CFESystem::File::Close(m_hFD);
        m_hFD = NULL;
    }
}
// ----------------------------------------------------------------------------
uint CFEFile::uiRead(FEPointer _pData, uint _uiSize)
{
    CFESystemCheck(_pData != NULL,"NULL Data buffer");
    return ( CFESystem::File::uiRead(m_hFD,_pData,_uiSize) );
}
// ----------------------------------------------------------------------------
uint CFEFile::uiWrite (FEPointer _pData, uint _uiSize)
{
    CFESystemCheck(_pData != NULL,"NULL Data buffer");
    return ( CFESystem::File::uiWrite(m_hFD,_pData,_uiSize) );
}
// ----------------------------------------------------------------------------
int CFEFile::iSeek (int _iOffset, EFEFileSeekMode _eMode)
{    
    return ( CFESystem::File::iSeek(m_hFD,_iOffset,_eMode) );
}
// ----------------------------------------------------------------------------
uint CFEFile::uiLength ()
{
    uint uiCurPos = CFESystem::File::uiPos(m_hFD);
    CFESystem::File::iSeek(m_hFD, 0, FSM_END);

    uint uiLength = CFESystem::File::uiPos(m_hFD);
    CFESystem::File::iSeek(m_hFD, uiCurPos, FSM_SET);

    return (uiLength);
}
// ----------------------------------------------------------------------------
uint CFEFile::uiPos ()
{
    return( CFESystem::File::uiPos(m_hFD) );
}
// -----------------------------------------------------------------------------
inline void Swap32(FEPointer _pData)
{
	char cAux = _pData[0];
    _pData[0] = _pData[3];
    _pData[3] = cAux;
    cAux      = _pData[1];
    _pData[1] = _pData[2];
    _pData[2] = cAux;
}
// -----------------------------------------------------------------------------
inline void Swap16(FEPointer _pData)
{
	char cAux = _pData[0];
    _pData[0] = _pData[1];
    _pData[1] = cAux;
}
// ----------------------------------------------------------------------------
uint CFEFile::uiReadArray32(FEPointer _pData, uint _uiSize)
{
	uint uiRes = uiRead(_pData,_uiSize*4);

	#ifdef BIG_ENDIAN
	for (uint i=0;i<_uiSize;i++)
    	Swap32( _pData + i*4 );
    #endif

	return(uiRes);
}
// ----------------------------------------------------------------------------
uint CFEFile::uiReadArray16(FEPointer _pData, uint _uiSize)
{
	uint uiRes = uiRead(_pData,_uiSize*2);

	#ifdef BIG_ENDIAN
	for (uint i=0;i<_uiSize;i++)
    	Swap16(_pData + i*2);
    #endif

	return(uiRes);	
}
// ----------------------------------------------------------------------------

    
