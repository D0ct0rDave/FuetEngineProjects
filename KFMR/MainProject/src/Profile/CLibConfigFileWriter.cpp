// Standard Gen Includes
#include "CLibConfigFileWriter.h"
// ----------------------------------------------------------------------------
CLibConfigFileWriter::CLibConfigFileWriter(const CFEString& _sFilename) : m_poFD(NULL)
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
bool CLibConfigFileWriter::bCreate(const CFEString& _sFilename)
{
    m_poFD = fopen(_sFilename.szString(),"wb");
    m_sIndent = " ";
    return(m_poFD != NULL) ;
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::OpenSection(const CFEString& _sSectionName)
{
    if (m_poFD == NULL) return;

    fprintf(m_poFD,"%s%s:\n",m_sIndent.szString(),_sSectionName.szString());
    fprintf(m_poFD,"%s{\n",m_sIndent.szString());
    
    // Increase indentation
    m_sIndent += "\t";
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::OpenSection(const CFEString& _sSectionName,uint _uiIndex)
{
    if (m_poFD == NULL) return;

    fprintf(m_poFD,"%s%s%d:\n",m_sIndent.szString(),_sSectionName.szString(),_uiIndex);
    fprintf(m_poFD,"%s{\n",m_sIndent.szString());
    
    // Increase indentation
    m_sIndent += "\t";
}        
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::CloseSection()
{
    if (m_poFD == NULL) return;

    // Decrease indentation
    m_sIndent = m_sIndent.SubString(0,m_sIndent.uiLen()-1);
    fprintf(m_poFD,"%s};\n",m_sIndent.szString());
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::NewLine()
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"\n");
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,int _iValue)
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"%s%s = %d;\n",m_sIndent.szString(),_sVariable.szString(),_iValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,uint _uiValue)
{
    AddVar(_sVariable,(int)_uiValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,bool _bValue)
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"%s%s = %s;\n",m_sIndent.szString(),_sVariable.szString(),_bValue?"true":"false");
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,const CFEString _sValue)
{
    AddVar(_sVariable,_sValue.szString());
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,const char* _szValue)
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"%s%s = \"%s\";\n",m_sIndent.szString(),_sVariable.szString(),_szValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddVar(const CFEString& _sVariable,float _fValue,uint _uiDecimals)
{
    if (m_poFD == NULL) return;
    char szFormat[16];
    sprintf_s(szFormat,16,"%s%d%s","%s%s = %.",_uiDecimals,"f;\n");
    fprintf(m_poFD,szFormat,m_sIndent.szString(),_sVariable.szString(),_fValue);
}
// ----------------------------------------------------------------------------
void CLibConfigFileWriter::AddComment(const CFEString& _sComment)
{
    if (m_poFD == NULL) return;
    fprintf(m_poFD,"%s// %s;\n",m_sIndent.szString(),_sComment.szString());
}
// ----------------------------------------------------------------------------
/*
void CLibConfigFileWriter::AddMisc(char* _szFmt,...)
{
}
*/
// ----------------------------------------------------------------------------
