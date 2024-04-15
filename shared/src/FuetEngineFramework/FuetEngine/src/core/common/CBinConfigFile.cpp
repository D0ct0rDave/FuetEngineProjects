// ----------------------------------------------------------------------------
/*! \class
 *  \brief Config files functionality through BinaryConfigFiles.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "FEConsts.h"
#include "CBinConfigFile.h"
#include "core/CFECore.h"
#include "support/file/CFEMemFile.h"
#include "support/misc/CFEStringUtils.h"
// ----------------------------------------------------------------------------
#if 0
const unsigned int IDENTIFIER_ALIGNMENT = 4;
const unsigned int SECTION_FLAG = 0x80000000;

static unsigned int ROUNDUP(unsigned int _uiSize)
{
	return ((_uiSize+(IDENTIFIER_ALIGNMENT-1)) & (~(IDENTIFIER_ALIGNMENT-1)) );
}
#endif
// ----------------------------------------------------------------------------
CBinConfigFile::CBinConfigFile() : m_uiTableWidth(0), m_oTable(NULL), m_pData(NULL), m_pDictData(NULL)
{
}
// ----------------------------------------------------------------------------
CBinConfigFile::CBinConfigFile(const CFEString& _sFilename) : m_uiTableWidth(0), m_oTable(NULL), m_pData(NULL), m_pDictData(NULL)
{
	bOpen(_sFilename);
}
// ----------------------------------------------------------------------------
CBinConfigFile::~CBinConfigFile()
{
	Close();
}
// ----------------------------------------------------------------------------
FEBool CBinConfigFile::bOpen(const CFEString& _sFilename)
{
	CFEMemFile oFile;
	if (! oFile.bOpen(_sFilename,FOM_READ)) return(false);

	char szID[5] = {0,0,0,0,0};
	oFile.uiRead((FEPointer)&szID,4);

	if (CFECore::String::iStrNCmp(szID,"BCFG",4) != 0)
	{
		// not a binary config file.
		#ifndef MASTER_RELEASE_VERSION
		CFECore::Log::Print("Config file doesn't seem to be binary.\n");
		#endif

		oFile.Close();
		return(false);
	}

	// Read dictionary size
	uint uiDicSize;
	oFile.uiReadArray32((FEPointer)&uiDicSize,1);

    if (uiDicSize>0)
    {
	    // Alloc space for the pool of data
	    m_pDictData = (FEPointer)CFECore::Mem::pAlloc(uiDicSize);
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
    m_pData = CFECore::Mem::pAlloc( uiTotalElems*sizeof(TVariable) );
    oFile.uiReadArray32(m_pData,uiTotalElems*2);

    TVariable* poVars = (TVariable*)m_pData;
    for (uint i=0;i<m_uiTableWidth;i++)
    {
        m_oTable[i].m_poVars = poVars;
        poVars += m_oTable[i].m_uiNumElems;
    }

	oFile.Close();

	#ifdef CHECK_COLLISIONS
	m_sFilename = _sFilename;
	m_oColCheckTable.Init(1,m_uiTableWidth);
	m_oColCheckTable.Invalidate();
	#endif

	return(true);
}
// ----------------------------------------------------------------------------
void CBinConfigFile::Close()
{
	#ifdef CHECK_COLLISIONS
	for (uint i=0;i<m_uiTableWidth;i++)
	{
		CFELookUpDatabase<CColHashEntry>* poLD = m_oColCheckTable.poGetList(i);

		for (uint j=0;j<poLD->uiNumElems();j++)
		{
			CColHashEntry* poCHE = poLD->poGetAt(j);
			if (poCHE->m_sVars.size()>1)
			{
				// possible collision
				uint k = 0;
				FEBool bAllNULLS = true;
 				for (k=0;k<poCHE->m_sVars.size();k++)
 				{
 					if (pFindVar(poCHE->m_sVars[k].szString(),m_pData) != NULL)
 						bAllNULLS = false;
 				}

 				if (! bAllNULLS)
 				{
 					CFECore::Log::Print("**************************************************\n");
					CFECore::Log::Print("WARNING COLLISION in file %s:\n",m_sFilename.szString());
					CFECore::Log::Print("REPORT: variables\n");

					for (k=0;k<poCHE->m_sVars.size();k++)
						CFECore::Log::Print("%s\n",poCHE->m_sVars[k].szString());

					CFECore::Log::Print("collide!!! \n");
					CFECore::Log::Print("**************************************************\n");
 					
 					_asm
 					{
 						int 3;
 					}
 				}
			}

			delete poCHE;
		}

		poLD->Clear();
	}
	#endif

	if (m_pDictData != NULL)
	    CFECore::Mem::Free((FEPointer)m_pDictData);

	if (m_pData != NULL)
	    CFECore::Mem::Free((FEPointer)m_pData);

    if (m_oTable != NULL)
        delete []m_oTable;

	m_pData     = NULL;
	m_pDictData = NULL;
	m_oTable    = NULL;
}
// ----------------------------------------------------------------------------
FEBool CBinConfigFile::ReadString(const CFEString& _sVar,char** _pszValue)
{
	int* piData = (int*)pFindVar(_sVar);
	if (piData == NULL)
		return(false);
	else
	{
		*_pszValue = m_oDictionary[*piData];
		return(true);
	}
}
// ----------------------------------------------------------------------------
FEBool CBinConfigFile::ReadInt(const CFEString& _sVar,int* _piValue)
{
	int* piData = (int*)pFindVar(_sVar);
	if (piData == NULL)
		return(false);
	else
	{
		*_piValue = *piData;
		return(true);
	}
}
// ----------------------------------------------------------------------------
FEBool CBinConfigFile::ReadFloat(const CFEString& _sVar,float* _pfValue)
{
	float* pfData = (float*)pFindVar(_sVar);
	if (pfData == NULL)
		return(false);
	else
	{
		*_pfValue = *pfData;
		return(true);
	}
}
// ----------------------------------------------------------------------------
FEBool CBinConfigFile::ReadBool(const CFEString& _sVar,FEBool* _pbValue)
{
	FEBool* pbData = (FEBool*)pFindVar(_sVar);
	if (pbData == NULL)
		return(false);
	else
	{
		*_pbValue = *pbData;
		return(true);
	}
}
// ----------------------------------------------------------------------------
void* CBinConfigFile::pFindVar(const CFEString& _sVar)
{
	#ifdef CHECK_COLLISIONS
	uint uiHash = _sVar.uiGetHash();

	// NOTE 09-Oct-2013: A note on hash collisions.
	// We want to avoid collisions even with the hashes themselves!
	// Hash("HUD.Element2.Action2.ObjAction0.ColorFunc.KeyFrame1.r") = 1935738204
	// Hash("HUD.Element2.Action2.ObjAction0.ColorFunc.KeyFrame1.g") = 1935738185
	// A = 1935738204 = 0x7361055c;
	// B = 1935738185 = 0x73610549;
	// Hash("7361055c") == Hash("73610549") !!! a collision occurs inside the pGetVar!!

	/*
	// SEE NOTE
	CFEString sStr;
	sStr.Format("%x",uiHash);
	CColHashEntry* poCHE = m_oColCheckTable.poGetVar(sStr);
	*/

	CColHashEntry* poCHE = m_oColCheckTable.poGetVar(_sVar);
	if (poCHE == NULL)
	{
		// variable entry doesn't exist. Create the first.
		poCHE = new CColHashEntry;
		poCHE->m_sVars.push_back(_sVar);
		poCHE->m_uiHash = uiHash;
		m_oColCheckTable.AddVar(_sVar,poCHE);
		// m_oColCheckTable.AddVar(sStr,poCHE);	// SEE NOTE
	}
	else
	{
		// variable entry exists: look if there is a collision
		if (poCHE->m_sVars[0] != _sVar)
		{
			// collision!
			uint i = 1;
			while ((i<poCHE->m_sVars.size()) && (poCHE->m_sVars[i] != _sVar))
				i++;

			if (i==poCHE->m_sVars.size())
			{
				// new collision ?!?!
				poCHE->m_sVars.push_back(_sVar);
			}
 		}
	}
	#endif

 	return( pFindVar(_sVar,m_pData) );
}
// ----------------------------------------------------------------------------
void* CBinConfigFile::pFindVar(const CFEString& _sVar,void* _pData)
{
    uint uiHash = _sVar.uiGetHash();
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
FEBool CBinConfigFile::bExists(const CFEString& _sVar)
{
	return ( pFindVar(_sVar,NULL) != NULL );
}
// ----------------------------------------------------------------------------

