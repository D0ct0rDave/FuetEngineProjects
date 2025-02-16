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
#include "EdUtils.h"
// ----------------------------------------------------------------------------
#ifdef __WXMSW__
#include <wx/string.h>
#include <wx/filename.h>
#endif
//---------------------------------------------------------------------------
int round (float _fReal)
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
//-----------------------------------------------------------------
#ifdef __WXMSW__
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath)
{	
	wxFileName sFilename(_sBasePath+"/"+_sFilename);
	sFilename.MakeRelativeTo(_sBasePath);

	wxString sRelativePath = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName()/* + sFilename.GetExt() ????????*/;
	
	// take the slash from the beginning of the filename in case.
	if ((sRelativePath.Find("\\") == 0) || (sRelativePath.Find("/") == 0))
		sRelativePath = sRelativePath.SubString(1,sRelativePath.Len());

    sRelativePath.Replace(_T("../"),_T("**/"),true);
    sRelativePath.Replace(_T("./"),_T(""),true);
    sRelativePath.Replace(_T("**/"),_T("../"),true);
    
    // finally substitute any "\" per "/"
    sRelativePath.Replace(_T("\\"),_T("/"),true);

	return( sRelativePath);
}
//-----------------------------------------------------------------
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath)
{	
	wxString sFilename = sRelativizePath( wxString(_sFilename.szString()),wxString(_sBasePath.szString()));
	return( CFEString(sFilename.char_str()) );
}
//-----------------------------------------------------------------
std::string sRelativizePath(const std::string & _sFilename,const std::string & _sBasePath)
{
	wxString sStr = sRelativizePath( wxString(_sFilename.c_str()),wxString(_sBasePath.c_str()));
	return( std::string( sStr.char_str()) );
}
#endif
// ----------------------------------------------------------------------------
