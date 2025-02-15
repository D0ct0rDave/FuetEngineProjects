//-----------------------------------------------------------------------------
/*! \class CLibConfigFileWriter
 *  \brief Class helper to write libconfig files
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CLibConfigFileWriterH
#define CLibConfigFileWriterH
// ----------------------------------------------------------------------------
#include <string>
#include <stdio.h>
#include <FuetEngine.h>
// ----------------------------------------------------------------------------
class CLibConfigFileWriter
{
	public:
	    CLibConfigFileWriter();
	    ~CLibConfigFileWriter();
		CLibConfigFileWriter(const CFEString& _sFilename);

        virtual bool bCreate(const CFEString& _sFilename);
        virtual void Write();
        
        virtual void OpenSection(const CFEString& _sSectionName);
        virtual void OpenSection(const CFEString& _sSectionName,uint _uiIndex);
        virtual void CloseSection();
        virtual void NewLine();
        
        /// Add Variables and values
        virtual void AddVar(const CFEString& _sVariable,int _iValue);
        virtual void AddVar(const CFEString& _sVariable,uint _uiValue);
        virtual void AddVar(const CFEString& _sVariable,bool _bValue);
        virtual void AddVar(const CFEString& _sVariable,float _fValue,uint _uiDecimals = 4);
        virtual void AddVar(const CFEString& _sVariable,const char* _szValue);
        virtual void AddVar(const CFEString& _sVariable,const CFEString _sValue);
        virtual void AddComment(const CFEString& _sComment);
        // void AddMisc(char* _szFmt,...);

    protected:

        FILE*           m_poFD;
        CFEString     m_sIndent;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
