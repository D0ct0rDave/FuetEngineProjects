// ----------------------------------------------------------------------------
/*! \class
 *  \brief Config files functionality through BinaryConfigFiles.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CBinConfigFile.h"
#include "system/CFESystem.h"
#include "support/file/CFEFile.h"
#include "support/misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
const unsigned int IDENTIFIER_ALIGNMENT = 4;
const unsigned int SECTION_FLAG = 0x80000000;
// ----------------------------------------------------------------------------
static unsigned int ROUNDUP(unsigned int _uiSize)
{
	return ((_uiSize+(IDENTIFIER_ALIGNMENT-1)) & (~(IDENTIFIER_ALIGNMENT-1)) );
}
// ----------------------------------------------------------------------------
CBinConfigFile::CBinConfigFile() : m_pDictData(NULL), m_pData(NULL), m_oTable(NULL), m_uiTableWidth(0)
{
}
// ----------------------------------------------------------------------------
CBinConfigFile::CBinConfigFile(const char* _szFilename) : m_pDictData(NULL), m_pData(NULL), m_oTable(NULL), m_uiTableWidth(0)
{
	bOpen(_szFilename);
}
// ----------------------------------------------------------------------------
CBinConfigFile::~CBinConfigFile()
{
	Close();
}
// ----------------------------------------------------------------------------
bool CBinConfigFile::bOpen(const char* _szFilename)
{
	CFEFile oFile;
	if (! oFile.bOpen(_szFilename,FOM_READ)) return(false);
	
	char szID[5] = {0,0,0,0,0};
	oFile.uiRead((FEPointer)&szID,4);
	
	if (CFESystem::String::iStrNCmp(szID,"BCFG",4) != 0)
	{
		// not a binary config file.
		oFile.Close();
		return(false);
	}
	
	// Read dictionary size
	uint uiDicSize;
	oFile.uiReadArray32((FEPointer)&uiDicSize,1);

    if (uiDicSize>0)
    {
	    // Alloc space for the pool of data
	    m_pDictData = (FEPointer)CFESystem::Mem::pAlloc(uiDicSize);
        oFile.uiRead((FEPointer)m_pDictData,uiDicSize);

        // Process dictionary.
        char* szStr = (char*)m_pDictData;
        while (uiDicSize>0)
        {
            m_oDictionary.push_back(szStr);

            uint uiStrLen = strlen(szStr);
            szStr     += (uiStrLen + 1);
            uiDicSize -= (uiStrLen + 1);
        }
    }

	// Read table width.
	oFile.uiReadArray32((FEPointer)&m_uiTableWidth,1);
	m_oTable = new THashedTable[m_uiTableWidth];
    
    // Read the number of elements of each of the subtables.
    uint uiTotalElems = 0;
    for (uint i=0;i<m_uiTableWidth;i++)
    {
        oFile.uiReadArray32((FEPointer)&m_oTable[i].m_uiNumElems,1);
        uiTotalElems += m_oTable[i].m_uiNumElems;
    }

    // Read the variable data.
    m_pData = CFESystem::Mem::pAlloc( uiTotalElems*sizeof(TVariable) );
    oFile.uiReadArray32(m_pData,uiTotalElems*2);

    TVariable* poVars = (TVariable*)m_pData;
    for (uint i=0;i<m_uiTableWidth;i++)
    {
        m_oTable[i].m_poVars = poVars;
        poVars += m_oTable[i].m_uiNumElems;
    }

	oFile.Close();
	return(true);
}
// ----------------------------------------------------------------------------
void CBinConfigFile::Close()
{
	if (m_pDictData != NULL)
	    CFESystem::Mem::Free((FEPointer)m_pDictData);

	if (m_pData != NULL)
	    CFESystem::Mem::Free((FEPointer)m_pData);	

    if (m_oTable != NULL)
        delete []m_oTable;

	m_pData     = NULL;
	m_pDictData = NULL;
	m_oTable    = NULL;
}
// ----------------------------------------------------------------------------
bool CBinConfigFile::ReadString(const char* _szVar,char** _pszValue)
{
	int* piData = (int*)pFindVar(_szVar);
	if (piData == NULL)
		return(false);
	else
	{
		*_pszValue = m_oDictionary[*piData];
		return(true);	
	}
}
// ----------------------------------------------------------------------------
bool CBinConfigFile::ReadInt(const char* _szVar,int* _piValue)
{
	int* piData = (int*)pFindVar(_szVar);
	if (piData == NULL)
		return(false);
	else
	{
		*_piValue = *piData;
		return(true);
	}
}
// ----------------------------------------------------------------------------
bool CBinConfigFile::ReadFloat(const char* _szVar,float* _pfValue)
{
	float* pfData = (float*)pFindVar(_szVar);
	if (pfData == NULL)
		return(false);
	else
	{
		*_pfValue = *pfData;
		return(true);	
	}
}
// ----------------------------------------------------------------------------
bool CBinConfigFile::ReadBool(const char* _szVar,bool* _pbValue)
{
	bool* pbData = (bool*)pFindVar(_szVar);
	if (pbData == NULL)
		return(false);
	else
	{
		*_pbValue = *pbData;
		return(true);	
	}
}
// ----------------------------------------------------------------------------
void* CBinConfigFile::pFindVar(const char* _szVar)
{
	return( pFindVar(_szVar,m_pData) );
}
// ----------------------------------------------------------------------------
void* CBinConfigFile::pFindVar(const char* _szVar,void* _pData)
{
    uint uiHash = CFEStringUtils::uiGetHash( CFEString(_szVar) );
    uint uiIdx = uiHash % m_uiTableWidth;
    for (uint i=0;i<m_oTable[uiIdx].m_uiNumElems;i++)
    {
        if (m_oTable[uiIdx].m_poVars[i].m_uiHashID == uiHash)
        {
            return( &m_oTable[uiIdx].m_poVars[i].m_uiValue );
        }
    }

	return(NULL);
}
// ----------------------------------------------------------------------------
 