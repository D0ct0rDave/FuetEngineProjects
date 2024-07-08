//---------------------------------------------------------------------------
//
// Name:        CClipBoard.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#ifndef CClipBoardH
#define CClipBoardH
//---------------------------------------------------------------------------
#include <wx/string.h>
//---------------------------------------------------------------------------
typedef void (*TDeleteFunc)(const wxString& _sDataType,void* _pData);
//---------------------------------------------------------------------------
class CClipBoard
{
	public:

		static void Init();
		static void Finish();

		static void* pSetData(const wxString& _sDataType,void* _pData);
		static void* pGetData(const wxString& _sDataType);
		static bool  bExistData(const wxString& _sDataType);
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
