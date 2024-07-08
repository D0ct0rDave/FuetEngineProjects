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
#include "EdUtils.h"
// ----------------------------------------------------------------------------
#ifdef __WXMSW__
#include <wx/string.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#endif

#include <windows.h>
//---------------------------------------------------------------------------
int iRound (float _fReal)
{
    if (_fReal >= 0.0f)
    {
        float fFrac = _fReal - (int)_fReal;
        if (fFrac >= 0.5f)
            return( 1 + (int)_fReal);
        else
            return((int)_fReal);
    }
    else
    {
        float fFrac = _fReal - (int)_fReal;
        if (fFrac <= -0.5f)
            return( -1 +(int)_fReal);
        else
            return( (int)_fReal );
    }
}
// ----------------------------------------------------------------------------
std::string sGetFilename(const std::string & _sFilename)
{
	std::string sFilename = _sFilename; // sNormalizePath(_sFilename);
    int iSlashPos = sFilename.rfind("\\");

	if (iSlashPos != -1)
		return( _sFilename.substr(iSlashPos+1, _sFilename.size() - (iSlashPos +1)) );
	else
		return( _sFilename );
}
//-----------------------------------------------------------------------------
/// Changes all the backslashes to slashes in the path and returns the result.
std::string sNormalizePath(const std::string& _sFilename)
{
	std::string sNormalizedFilename = "";

	char* szFilename = (char*)_sFilename.c_str();
	for (unsigned int i=0;i<_sFilename.size();i++)
	{
		if (szFilename[i] == '\\')
			sNormalizedFilename += '/';
		else
			sNormalizedFilename += szFilename[i];
	}

	return(sNormalizedFilename);
}
// ---------------------------------------------------------------------------
std::string sNormalizePathWithBackslashes(const std::string& _sFilename)
{
	std::string sRes = _sFilename;
    for (unsigned int i=0;i<sRes.size();i++)
    {
        if (sRes[i+1] == '/')
            sRes[i+1] = '\\';
    }

    return(sRes);
}
//-----------------------------------------------------------------------------
std::string sRemoveExtension(const std::string & _sFilename)
{
    // Remove extension
    int iDotPos = _sFilename.rfind(".");

    if (iDotPos == std::string::npos)
        return(_sFilename);
    else
        return( _sFilename.substr(0,iDotPos) );
}
// ----------------------------------------------------------------------------
bool bIsReadOnly(const std::string& _sFilename)
{
	DWORD dwAttr = GetFileAttributesA( _sFilename.c_str() );
	return(dwAttr & FILE_ATTRIBUTE_READONLY);
}
//-----------------------------------------------------------------
bool bFileExists(const std::string& _sFilename)
{
	FILE* fd; 
	fopen_s(&fd,_sFilename.c_str(),"rb");
	if (fd != NULL)
	{
		fclose(fd);
		return(true);
	}
	else
		return(false);
}
//-----------------------------------------------------------------
#ifdef __WXMSW__
//-----------------------------------------------------------------
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath)
{	
	if (_sFilename == "") return("");
	if (_sBasePath== "") return(_sFilename);

	std::string sRes = sRelativizePath( std::string(_sFilename.szString()), std::string(_sBasePath.szString()) );
	return(CFEString(sRes.c_str()));
}
//-----------------------------------------------------------------
std::string sRelativizePath(const std::string & _sFilename,const std::string & _sBasePath)
{
	if (_sFilename == "") return("");
	if (_sBasePath== "") return(_sFilename);

	wxString sFilename = wxString::FromAscii(_sFilename.c_str());
	wxString sBasePath = wxString::FromAscii(_sBasePath.c_str());
	
	wxString sRelPath = sRelativizePath(sFilename,sBasePath);
	return( std::string(sRelPath.char_str(wxConvUTF8)) );
}
//-----------------------------------------------------------------
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath)
{
	if (_sFilename == wxT("")) return(wxT(""));

	/// retrieve proper base dir.
	_TCHAR szFullBasePath[1024];
	if (GetFullPathName(_sBasePath.c_str(),1024,szFullBasePath,NULL) == 0)
		return(_sFilename);

	// control current directory
	_TCHAR szCurDir[1024];
	GetCurrentDirectory(1024,szCurDir);
	if (wxString(szCurDir) != wxString(szFullBasePath))
	{
		// warning!!!
		SetCurrentDirectory(_sBasePath);
	}

	// retrieve correct filename
	_TCHAR szFullFilename[1024];
	if (GetFullPathName(_sFilename.c_str(),1024,szFullFilename,NULL) == 0)
		return(_sFilename);

	wxFileName sFilename(szFullFilename);
	sFilename.MakeRelativeTo(szFullBasePath);

	wxString sRelativePath = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName()/* + sFilename.GetExt() ????????*/;

	// take the slash from the beginning of the filename in case.
	if ((sRelativePath.Find(_T("\\")) == 0) || (sRelativePath.Find(_T("/")) == 0))
		sRelativePath = sRelativePath.SubString(1,sRelativePath.Len());

    sRelativePath.Replace(_T("../"),_T("**/"),true);
    sRelativePath.Replace(_T("./"),_T(""),true);
    sRelativePath.Replace(_T("**/"),_T("../"),true);
    
    // finally substitute any "\" per "/"
    sRelativePath.Replace(_T("\\"),_T("/"),true);

	if (wxString(szCurDir) != wxString(szFullBasePath))
	{
		// warning!!!
		SetCurrentDirectory(szCurDir);
	}
	
	return( sRelativePath);
}
///-----------------------------------------------------------------
wxColor FEColor2WXColor(const CFEColor& _oColor)
{
	wxColor oColor;
	oColor.Set(FEColor2UIColor(_oColor));
	return(oColor);
}
///-----------------------------------------------------------------
CFEColor WXColor2FEColor(const wxColor& _oColor)
{
	CFEColor oColor;
	oColor.r = (FEReal)_oColor.Red() / _255r;
	oColor.g = (FEReal)_oColor.Green() / _255r;
	oColor.b = (FEReal)_oColor.Blue() / _255r;
	oColor.a = (FEReal)_oColor.Alpha() / _255r;
    return(oColor);
} 
///-----------------------------------------------------------------
unsigned int FEColor2UIColor(const CFEColor& _oColor)
{
    unsigned int uiColor = ((uint)(_oColor.a*255.0) << 24)
                         | ((uint)(_oColor.b*255.0) << 16)
                         | ((uint)(_oColor.g*255.0) <<  8)
                         | ((uint)(_oColor.r*255.0));
    return(uiColor);
}
//---------------------------------------------------------------------------
CFEColor UIColor2FEColor(unsigned int _uiColor)
{
    CFEColor oColor;

    oColor.r = (FEReal) (_uiColor & 0x000000ff)        / 255.0;
    oColor.g = (FEReal)((_uiColor >>  8) & 0x000000ff) / 255.0;
    oColor.b = (FEReal)((_uiColor >> 16) & 0x000000ff) / 255.0;
    oColor.a = (FEReal)((_uiColor >> 24) & 0x000000ff) / 255.0;

    return(oColor);
}
// ----------------------------------------------------------------------------
bool bIsReadOnly(const wxString& _sFilename)
{
	return( bIsReadOnly( std::string(_sFilename.char_str())) );
}

bool bIsReadOnly(const CFEString& _sFilename)
{
	return( bIsReadOnly( std::string(_sFilename.szString())) );
}
//---------------------------------------------------------------------------
void ShowFileErrorMessage(const wxString& _sFilename)
{
	wxString sMessage;
	
	if (bIsReadOnly(_sFilename))
	{
		sMessage = wxString(_T("File:\n\n")
							 + _sFilename
							 + _T("\n\n")
							 + _T("is readonly and cannot be written.\n")
							 + _T("Is it under version control?")
							);
	}

	wxMessageBox(sMessage,_T("Warning"));
}

bool bFileExists(const wxString& _sFilename)
{
	return( bFileExists(std::string(_sFilename.char_str())) );
}

bool bFileExists(const CFEString& _sFilename)
{
	return( bFileExists(std::string(_sFilename.szString())) );
}
//---------------------------------------------------------------------------
void MakeBackupOfFile(const wxString& _sFilename)
{
	std::string sFilename    = _sFilename.ToAscii();
	std::string sNewFilename = sFilename + ".bak";

	// make backup of file
	if (! CopyFileA(sFilename.c_str(),sNewFilename.c_str(),false))
	{
		wxMessageBox(_T("Cannot make a backup in\n")
					+ wxString::FromAscii(sNewFilename.c_str())
					+ _T("\n\n")
					+ _T("Is it under version control?")
					);
	}

	// remove readonly flag ... in case
	SetFileAttributesA(sNewFilename.c_str(),GetFileAttributesA(sNewFilename.c_str()) & ~FILE_ATTRIBUTE_READONLY);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
