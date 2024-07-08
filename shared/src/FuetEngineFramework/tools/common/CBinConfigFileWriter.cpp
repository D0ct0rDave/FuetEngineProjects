// Standard Gen Includes
#include "CBinConfigFileWriter.h"
#include <vector>

#include <FuetEngine.h>

// ----------------------------------------------------------------------------
// CRC functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
typedef struct TVariable{

    unsigned int m_uiHash;

    union
    {
        bool m_bValue;
        float m_fValue;
        int m_iValue;
        
        // bool, int, float, string
    };

	/// Comparaison operator for sorting purposes.
	bool operator < (const TVariable& b)
	{
		return( m_uiHash < b.m_uiHash );
	}

}TVariable;
// ---------------------------------------------------------------------------
class CBinConfigInternals
{
	public:
		std::vector<TVariable>      m_oVars;
		std::vector<std::string>    m_sDictionary;
        std::string                 m_sCurSection;
};
// ----------------------------------------------------------------------------
const unsigned int IDENTIFIER_ALIGNMENT = 4;
const unsigned int SECTION_FLAG = 0x80000000;
// ----------------------------------------------------------------------------
static unsigned int ROUNDUP(unsigned int _uiSize)
{
	return ((_uiSize+(IDENTIFIER_ALIGNMENT-1)) & (~(IDENTIFIER_ALIGNMENT-1)) );
}
// ----------------------------------------------------------------------------
CBinConfigFileWriter::CBinConfigFileWriter(const std::string& _sFilename) : m_poFD(NULL), m_poData(NULL), CConfigFileWriter(_sFilename)
{
    bCreate(_sFilename);

    m_poData = new CBinConfigInternals;  
    m_poData->m_sCurSection = "";
}
// ----------------------------------------------------------------------------
CBinConfigFileWriter::CBinConfigFileWriter() : m_poFD(NULL)
{
    m_poData = new CBinConfigInternals;  
    m_poData->m_sCurSection = "";
}
// ----------------------------------------------------------------------------
CBinConfigFileWriter::~CBinConfigFileWriter()
{
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::Write()
{
    unsigned int i;

    // Build the hash table.
    unsigned int uiWidth = m_poData->m_oVars.size() / 3; // average table depth
    if (uiWidth==0) uiWidth = 1;

    std::vector<TVariable>* oTable;
    oTable = new std::vector<TVariable>[uiWidth];

    for (i=0;i<m_poData->m_oVars.size();i++)
    {
        unsigned int uiIdx = m_poData->m_oVars[i].m_uiHash % uiWidth;
        oTable[uiIdx].push_back(m_poData->m_oVars[i]);
    }

    if (m_poFD != NULL)
    {
        // Write the size of the string pool.
        unsigned int uiSize = 0;
        for (i=0;i<m_poData->m_sDictionary.size();i++)
            uiSize += (m_poData->m_sDictionary[i].size()+1);

        fwrite(&uiSize,4,1,m_poFD);

        // Write dictionary
        for (i=0;i<m_poData->m_sDictionary.size();i++)
            fwrite(m_poData->m_sDictionary[i].c_str(),m_poData->m_sDictionary[i].size()+1,1,m_poFD);

        // Write hash table width
        fwrite(&uiWidth,4,1,m_poFD);
        
        // Write the number of elements per subtable.
        for (i=0;i<uiWidth;i++)
        {
            // Write this entry length
            uiSize = oTable[i].size();
            fwrite(&uiSize,4,1,m_poFD);
        }

        // Write the elements of the tables.
        for (i=0;i<uiWidth;i++)
        {
            // Write values
            for (unsigned int j=0;j<oTable[i].size();j++)
                fwrite(&oTable[i][j],sizeof(TVariable),1,m_poFD);
			/*
			// Comprobar colisiones ...
			std::sort(oTable[i].begin(),oTable[i].end());
			for (unsigned int j=1;j<oTable[i].size();j++)
			{
				if (oTable[i][j-1].m_uiHash == oTable[i][j].m_uiHash)
				{
					// warning!!!!!					
					printf("hash key collision detected!\n");
					CFELogger::Sys::Print("hash key collision detected! (%d)\n",oTable[i][j].m_uiHash);
				}
			}

			CFELogger::Sys::Print("%d\n",oTable[i].size());
			*/
        }

        // Close file
        fclose(m_poFD);
    }

	delete m_poData;
	m_poData = NULL;
    m_poFD = NULL;
}
// ----------------------------------------------------------------------------
bool CBinConfigFileWriter::bCreate(const std::string& _sFilename)
{
    m_poFD = fopen(_sFilename.c_str(),"wb");
    if (m_poFD != NULL)
    {
		m_poData = new CBinConfigInternals;
		fprintf(m_poFD,"BCFG");
	}

    return(m_poFD != NULL) ;
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::OpenSection(const std::string& _sSectionName)
{
    if (m_poFD == NULL) return;

    if (m_poData->m_sCurSection=="")
	    m_poData->m_sCurSection = _sSectionName;
	else
	    m_poData->m_sCurSection = m_poData->m_sCurSection + std::string(".") + _sSectionName;
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::OpenSection(const std::string& _sSectionName,unsigned int _uiIndex)
{
    if (m_poFD == NULL) return;

	char szString[1024];
    sprintf(szString,"%s%d",_sSectionName.c_str(),_uiIndex);

    OpenSection(szString);
}        
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::CloseSection()
{
    if (m_poFD == NULL) return;
    int iPos = m_poData->m_sCurSection.rfind(".");

    if (iPos == std::string::npos)
        return;

    m_poData->m_sCurSection = m_poData->m_sCurSection.substr(0,iPos);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::NewLine()
{
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,int _iValue,int _iDefaultValue)
{
    AddVarInt(_sVariable,_iValue);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,unsigned int _uiValue,unsigned int _uiDefaultValue)
{
    AddVarInt(_sVariable,_uiValue);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,bool _bValue,bool _bDefaultValue)
{
    AddVarInt(_sVariable,_bValue?1:0);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,float _fValue,float _fDefaultValue,unsigned int _uiDecimals)
{
    AddVarInt(_sVariable,*(unsigned int*)(&_fValue));
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,const char* _szValue,const char* _szDefaultValue)
{
    if (_szValue == NULL) return;
    AddVar(_sVariable,std::string(_szValue),std::string(_szDefaultValue));
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,const std::string _sValue,const std::string _sDefaultValue)
{
    if (m_poFD == NULL) return;

    // find string on dictionary.
	bool bFound = false;
    uint uiPos = 0;
    for (uint i=0;i<m_poData->m_sDictionary.size();i++)
    {
        if (m_poData->m_sDictionary[i] == _sValue)
        {
			bFound = true;
            uiPos = i;
            break;
        }
    }

    if (! bFound)
    {
        uiPos = m_poData->m_sDictionary.size();
        m_poData->m_sDictionary.push_back(_sValue);        
    }

    AddVarInt(_sVariable,uiPos);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddComment(const std::string& _sComment)
{
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVarInt(const std::string& _sVariable,unsigned int _uiValue)
{
    if (m_poFD == NULL) return;
    std::string str = m_poData->m_sCurSection + "." + _sVariable;

    TVariable oVar;
    oVar.m_uiHash = CFEStringUtils::uiGetHash( CFEString(str.c_str()) );
    oVar.m_iValue = _uiValue;
/*
	
	if (oVar.m_uiHash == 1772777166)
	{
		int a =  0;
	}

	if (str == "HUD.Element88.NumLayers")
	{
		int a =  0;
	}
*/
	#ifdef _DEBUG
	CFESystem::Print("%s\n",str.c_str(),_uiValue);
    #endif

    m_poData->m_oVars.push_back(oVar);  
}
// ----------------------------------------------------------------------------
