//-----------------------------------------------------------------------------
/*! \class Editor Utilities
 *  \brief 
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 1999-2009
 *  \par Copyright (c) 2016 David Márquez de la Cruz
 *  \par License Rally
 */
//-----------------------------------------------------------------------------
#ifndef EdUtilsH
#define EdUtilsH
// ----------------------------------------------------------------------------
#include <string>

#ifdef __WXMSW__
#include <wx/colour.h>
#include <wx/string.h>
#include <FuetEngine.h>
#endif
//-----------------------------------------------------------------------------
std::string sRemoveExtension(const std::string & _sFilename);
std::string sGetFilename(const std::string & _sFilename);
std::string sNormalizePath(const std::string& _sFilename);
std::string sNormalizePathWithBackslashes(const std::string& _sFilename);

bool bIsReadOnly(const std::string& _sFilename);
bool bFileExists(const std::string& _sFilename);

#ifdef __WXMSW__
bool bIsReadOnly(const wxString& _sFilename);
bool bIsReadOnly(const CFEString& _sFilename);

bool bFileExists(const wxString& _sFilename);
bool bFileExists(const CFEString& _sFilename);

std::string sRelativizePath(const std::string & _sFilename,const std::string & _sBasePath);
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath);
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath);

unsigned int FEColor2UIColor(const CFEColor& _oColor);
CFEColor UIColor2FEColor(unsigned int _uiColor);
wxColor FEColor2WXColor(const CFEColor& _oColor);
CFEColor WXColor2FEColor(const wxColor& _oColor);

void ShowFileErrorMessage(const wxString& _sFilename);
void MakeBackupOfFile(const wxString& _sFilename);
#endif

// rounds a given real number depending on the fractional part.
int iRound (float _fReal);
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
