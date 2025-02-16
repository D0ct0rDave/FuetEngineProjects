// Standard Gen Includes
#include "CLibConfigFileWriter.h"
// ------------------------------------------------------------------------
bool bIsDeterminate(float _fNumber)
{
	return ( *(unsigned int*)(&_fNumber) != 0xffc00000 );
}

bool bIsNAN(float _fNumber)
{
	return (_fNumber != _fNumber);
} 
// ----------------------------------------------------------------------------
CLibConfigFileWriter::CLibConfigFileWriter(const std::string& _sFilename) : CConfigFileWriter(_sFilename), m_poFD(NULL)
{
    bCreate(_sFilename);
}
// ----------------------------------------------------------------------------
CLibConfigFileWriter::CLibConfigFileWriter() : m_poFD(NULL)
{
}
// ----------------------------------------------------------------------------
CLibConfigFileWriter::~CLibConfigFileWriter()
{
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::Write()
{
    if (m_poFD != NULL)
        fclose(m_poFD);

    m_poFD = NULL;
}
// ----------------------------------------------------------------------------
bool CLibConfigFileWriter::bCreate(const std::string& _sFilename)
{
    m_poFD = fopen(_sFilename.c_str(),"wb");
    return(m_poFD != NULL) ;
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::OpenSection(const std::string& _sSectionName)
{
    if (m_poFD == NULL) return;

    fprintf(m_poFD,"%s%s:\n",m_sIndent.c_str(),_sSectionName.c_str());
    fprintf(m_poFD,"%s{\n",m_sIndent.c_str());
    
    // Increase indentation
    m_sIndent += "\t";
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::OpenSection(const std::string& _sSectionName,unsigned int _uiIndex)
{
    if (m_poFD == NULL) return;

    fprintf(m_poFD,"%s%s%d:\n",m_sIndent.c_str(),_sSectionName.c_str(),_uiIndex);
    fprintf(m_poFD,"%s{\n",m_sIndent.c_str());
    
    // Increase indentation
    m_sIndent += "\t";
}        
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::CloseSection()
{
    if (m_poFD == NULL) return;

    // Decrease indentation
    m_sIndent = m_sIndent.substr(0,m_sIndent.size()-1);
    fprintf(m_poFD,"%s};\n",m_sIndent.c_str());
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::NewLine()
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"\n");
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,int _iValue,int _iDefaultValue)
{
    if (m_poFD == NULL) return;
    if (_iValue == _iDefaultValue) return;

    fprintf(m_poFD,"%s%s = %d;\n",m_sIndent.c_str(),_sVariable.c_str(),_iValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,unsigned int _uiValue,unsigned int _uiDefaultValue)
{
    AddVar(_sVariable,(int)_uiValue,(int)_uiDefaultValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,bool _bValue,bool _bDefaultValue)
{
    if (m_poFD == NULL) return;
    if (_bValue == _bDefaultValue) return;

    fprintf(m_poFD,"%s%s = %s;\n",m_sIndent.c_str(),_sVariable.c_str(),_bValue?"true":"false");
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,const std::string _sValue,const std::string _sDefaultValue)
{
    AddVar(_sVariable,_sValue.c_str(),_sDefaultValue.c_str());
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,const char* _szValue,const char* _szDefaultValue)
{
    if (m_poFD == NULL) return;
    if (_szValue==NULL) return;
    if ((_szDefaultValue!= NULL) && (strcmp(_szValue,_szDefaultValue)==0)) return;

    fprintf(m_poFD,"%s%s = \"%s\";\n",m_sIndent.c_str(),_sVariable.c_str(),_szValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const std::string& _sVariable,float _fValue,float _fDefaultValue,unsigned int _uiDecimals)
{
    if (m_poFD == NULL) return;
    if (_fValue == _fDefaultValue) return;
	
	/// 
	if ((! bIsDeterminate(_fValue)) || (bIsNAN(_fValue)))
	{
		_fValue = 1e9f;
	}

    char szFormat[16];
    sprintf_s(szFormat,16,"%s%d%s","%s%s = %.",_uiDecimals,"f;\n");
    fprintf(m_poFD,szFormat,m_sIndent.c_str(),_sVariable.c_str(),_fValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddComment(const std::string& _sComment)
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"%s// %s;\n",m_sIndent.c_str(),_sComment.c_str());
}
// ----------------------------------------------------------------------------
/*
void CLibConfigFileWriter::AddMisc(char* _szFmt,...)
{
}
*/
// ----------------------------------------------------------------------------
