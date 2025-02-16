//-----------------------------------------------------------------------------
/*! \class CBinConfigFileWriter
 *  \brief Class helper to write binary config files
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CBinConfigFileWriterH
#define CBinConfigFileWriterH
// ----------------------------------------------------------------------------
#include <string>
#include <stdio.h>
#include "CConfigFileWriter.h"
// ----------------------------------------------------------------------------
class CBinConfigInternals;
// ----------------------------------------------------------------------------
class CBinConfigFileWriter : public CConfigFileWriter
{
	public:
	    CBinConfigFileWriter();
	    ~CBinConfigFileWriter();
		CBinConfigFileWriter(const std::string& _sFilename);

        virtual bool bCreate(const std::string& _sFilename);
        virtual void Write();
        
        virtual void OpenSection(const std::string& _sSectionName);
        virtual void OpenSection(const std::string& _sSectionName,unsigned int _uiIndex);
        virtual void CloseSection();
        virtual void NewLine();
        
        /// Add Variables and values
        virtual void AddVar(const std::string& _sVariable,int _iValue,int _iDefaultValue);
        virtual void AddVar(const std::string& _sVariable,unsigned int _uiValue,unsigned int _uiDefaultValue);
        virtual void AddVar(const std::string& _sVariable,bool _bValue,bool _bDefaultValue);
        virtual void AddVar(const std::string& _sVariable,float _fValue,float _fDefaultValue,unsigned int _uiDecimals = 4);
        virtual void AddVar(const std::string& _sVariable,const char* _szValue,const char* _szDefaultValue);
        virtual void AddVar(const std::string& _sVariable,const std::string _sValue,const std::string _sDefaultValue);
        virtual void AddComment(const std::string& _sComment);
        // void AddMisc(char* _szFmt,...);

    protected:

        virtual void AddVarInt(const std::string& _sVariable,unsigned int _uiValue);
		void WriteIdentifier(const std::string& _sIdentifier);

        FILE*					m_poFD;
        CBinConfigInternals*	m_poData;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
