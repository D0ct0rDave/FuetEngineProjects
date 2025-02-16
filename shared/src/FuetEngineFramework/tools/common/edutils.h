//-----------------------------------------------------------------------------
/*! \class Editor Utilities
 *  \brief 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef EdUtilsH
#define EdUtilsH
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string>

#ifdef __WXMSW__
#include <wx/string.h>
#include <FuetEngine.h>
#endif
//-----------------------------------------------------------------------------
std::string sRemoveExtension(const std::string & _sFilename);
std::string sGetFilename(const std::string & _sFilename);
std::string sNormalizePath(const std::string& _sFilename);

#ifdef __WXMSW__
std::string sRelativizePath(const std::string & _sFilename,const std::string & _sBasePath);
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath);
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath);
#endif

// rounds a given real number depending on the fractional part.
int round (float _fReal);
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
