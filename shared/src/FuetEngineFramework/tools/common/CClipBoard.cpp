//---------------------------------------------------------------------------
//
// Name:        CHUDEditorApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "CClipBoard.h"
//---------------------------------------------------------------------------
#include <wx/clipbrd.h>
#include "FuetEngine.h"
//---------------------------------------------------------------------------
static TDeleteFunc				m_pDeleteFunc = NULL;
static CFELookUpDatabase<void*> m_oCBDB;
//---------------------------------------------------------------------------
void CClipBoard::Init()
{
	// ::wxOpenClipboard();
}
///-----------------------------------------------------------------
void CClipBoard::Finish()
{	
	m_oCBDB.Clear();
	// ::wxCloseClipboard();
}
///-----------------------------------------------------------------
void* CClipBoard::pSetData(const wxString& _sDataType,void* _pData)
{
	void* pOldData = m_oCBDB.poGet( CFEString(_sDataType.char_str()) );

	// Add new data	
	m_oCBDB.uiAdd(CFEString(_sDataType.char_str()),(void**)_pData);
	
	return(pOldData);

	/*
	int iType = ::wxRegisterClipboardFormat((wxChar*)_sDataType.c_str());
	if (iType >= 0)
	{
		::wxSetClipboardData(iType,_pData);
	}
	*/
}
///-----------------------------------------------------------------
void* CClipBoard::pGetData(const wxString& _sDataType)
{
	return( m_oCBDB.poGet( CFEString(_sDataType.char_str()) ) );

	/*
	int iType = ::wxRegisterClipboardFormat((wxChar*)_sDataType.c_str());
	if (iType >= 0)
	{
		return(::wxGetClipboardData(iType));
	}
	*/

	return(NULL);
}
///-----------------------------------------------------------------
bool CClipBoard::bExistData(const wxString& _sDataType)
{
	return( m_oCBDB.poGet( CFEString(_sDataType.char_str()) ) != NULL );

	/*
	int iType = ::wxRegisterClipboardFormat((wxChar*)_sDataType.c_str());
	if (iType >= 0)
	{
		void* pObj = ::wxGetClipboardData(iType);
		return(pObj != NULL);
	}
	*/

	return(false);
}
///-----------------------------------------------------------------
