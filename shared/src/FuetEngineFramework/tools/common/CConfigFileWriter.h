//-----------------------------------------------------------------------------
/*! \class CConfigFileWriter
 *  \brief Generic class to write configuration files.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef CConfigFileWriterH
#define CConfigFileWriterH
// ----------------------------------------------------------------------------
#include <string>
#include <stdio.h>
// ----------------------------------------------------------------------------
class CConfigFileWriter
{
	public:
		CConfigFileWriter()
		{
		}

	    CConfigFileWriter(const std::string& _sFilename){};

        virtual bool bCreate(const std::string& _sFilename) = 0;

        virtual void Write() = 0;

        virtual void OpenSection(const std::string& _sSectionName) = 0;
        virtual void OpenSection(const std::string& _sSectionName,unsigned int _uiIndex) = 0;
        virtual void CloseSection() = 0;
        virtual void NewLine() = 0;

        /// Add Variables and values
        virtual void AddVar(const std::string& _sVariable,int _iValue,int _iDefaultValue) = 0;
        virtual void AddVar(const std::string& _sVariable,unsigned int _uiValue,unsigned int _uiDefaultValue) = 0;
        virtual void AddVar(const std::string& _sVariable,bool _bValue,bool _bDefaultValue) = 0;
        virtual void AddVar(const std::string& _sVariable,float _fValue,float _fDefaultValue,unsigned int _uiDecimals = 4) = 0;
        virtual void AddVar(const std::string& _sVariable,const char* _szValue,const char* _szDefaultValue) = 0;
        virtual void AddVar(const std::string& _sVariable,const std::string _sValue,const std::string _sDefaultValue) = 0;
        virtual void AddComment(const std::string& _sComment) = 0;
        // void AddMisc(char* _szFmt,...);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
