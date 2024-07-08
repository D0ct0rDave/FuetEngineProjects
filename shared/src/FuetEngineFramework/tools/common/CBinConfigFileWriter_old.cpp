// Standard Gen Includes
#include "CBinConfigFileWriter.h"
#include <vector>
// ----------------------------------------------------------------------------
class CBinConfigInternals
{
	public:
		std::vector<unsigned int> m_uiPosStack;
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
}
// ----------------------------------------------------------------------------
CBinConfigFileWriter::CBinConfigFileWriter() : m_poFD(NULL)
{
}
// ----------------------------------------------------------------------------
CBinConfigFileWriter::~CBinConfigFileWriter()
{
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::Write()
{
    if (m_poFD != NULL)
        fclose(m_poFD);

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
void CBinConfigFileWriter::WriteIdentifier(const std::string& _sIdentifier)
{
	unsigned int uiLen = _sIdentifier.size()+1;
	unsigned int uiReminder = ROUNDUP(uiLen) - uiLen;

    // write section header.
    char szString[1024];
    sprintf(szString,"%s",_sIdentifier.c_str());
    fwrite(szString,uiLen,1,m_poFD);

    // write padding
    for (unsigned int i=0;i<uiReminder;i++)
    {
		unsigned char a	= 0;
		fwrite(&a,1,1,m_poFD);
    }
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::OpenSection(const std::string& _sSectionName)
{
    if (m_poFD == NULL) return;
	WriteIdentifier(_sSectionName);

    // Increase indentation
    unsigned int uiPos = ftell(m_poFD);
    m_poData->m_uiPosStack.push_back( uiPos );

    // Leave blank in the file
    fwrite(&uiPos,4,1,m_poFD);
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

    // Decrease indentation
    unsigned int uiLenVarPos = m_poData->m_uiPosStack[m_poData->m_uiPosStack.size()-1];
    m_poData->m_uiPosStack.pop_back();

    unsigned int uiDataStartPos = uiLenVarPos+4;
    unsigned int uiDataEndPos   = ftell(m_poFD);
	unsigned int uiLen = (uiDataEndPos - uiDataStartPos) | SECTION_FLAG; // Last bit is a flag to indicate this is a section not a variable

	// Go backward to section length position.
	fseek(m_poFD,uiLenVarPos,SEEK_SET);

	// Write the len of the whole section.
	fwrite(&uiLen,4,1,m_poFD);
	
	// Return to the end position.
	fseek(m_poFD,uiDataEndPos,SEEK_SET);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::NewLine()
{
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,int _iValue)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = sizeof(int);
    fwrite(&uiSize,4,1,m_poFD);
    fwrite(&_iValue,4,1,m_poFD);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,unsigned int _uiValue)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = sizeof(unsigned int);
    fwrite(&uiSize,4,1,m_poFD);
    fwrite(&_uiValue,4,1,m_poFD);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,bool _bValue)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = sizeof(unsigned int);
    fwrite(&uiSize,4,1,m_poFD);
    fwrite(&_bValue,4,1,m_poFD);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,const std::string _sValue)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = ROUNDUP(_sValue.size()+1);
    fwrite(&uiSize,4,1,m_poFD);    
    WriteIdentifier(_sValue);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,const char* _szValue)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = ROUNDUP(strlen(_szValue)+1);
    fwrite(&uiSize,4,1,m_poFD);    
    WriteIdentifier(_szValue);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddVar(const std::string& _sVariable,float _fValue,unsigned int _uiDecimals)
{
    if (m_poFD == NULL) return;
    WriteIdentifier(_sVariable);
    unsigned int uiSize = sizeof(float);
    fwrite(&uiSize,4,1,m_poFD);
    fwrite(&_fValue,4,1,m_poFD);
}
// ----------------------------------------------------------------------------
void CBinConfigFileWriter::AddComment(const std::string& _sComment)
{
}
// ----------------------------------------------------------------------------
/*
void CBinConfigFileWriter::AddMisc(char* _szFmt,...)
{
}
*/
// ----------------------------------------------------------------------------
