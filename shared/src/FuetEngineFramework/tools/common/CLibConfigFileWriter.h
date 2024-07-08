//-----------------------------------------------------------------------------
/*! \class CLibConfigFileWriter
 *  \brief Class helper to write libconfig files
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 1999-2009
 *  \par Copyright (c) 2016 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CLibConfigFileWriterH
#define CLibConfigFileWriterH
// ----------------------------------------------------------------------------
#include <string>
#include <stdio.h>
#include "CConfigFileWriter.h"
// ----------------------------------------------------------------------------
class CLibConfigFileWriter : public CConfigFileWriter
{
	public:
	    CLibConfigFileWriter();
	    ~CLibConfigFileWriter();
		CLibConfigFileWriter(const std::string& _sFilename);

        virtual bool bCreate(const std::string& _sFilename);
        virtual void Write();
        
        virtual void OpenSection(const std::string& _sSectionName);
        virtual void OpenSection(const std::string& _sSectionName,unsigned int _uiIndex);
        virtual void CloseSection();
        virtual void NewLine();
        
        /// Add Variables and values
        virtual void AddVar(const std::string& _sVariable,int _iValue,int _iDefaultValue);
        virtual void AddVar(const std::string& _sVariable,bool _bValue,bool _bDefaultValue);
        virtual void AddVar(const std::string& _sVariable,float _fValue,float _fDefaultValue,unsigned int _uiDecimals = 4);
        virtual void AddVar(const std::string& _sVariable,const char* _szValue,const char* _szDefaultValue);
        virtual void AddVar(const std::string& _sVariable,const std::string _sValue,const std::string _sDefaultValue);
        virtual void AddComment(const std::string& _sComment);
        // void AddMisc(char* _szFmt,...);

    protected:

        FILE*           m_poFD;
        std::string     m_sIndent;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
