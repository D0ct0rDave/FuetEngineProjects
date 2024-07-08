//---------------------------------------------------------------------------
//
// Name:        CHUDEditorApp.cpp
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: 
//
//---------------------------------------------------------------------------
#include "CSpriteEditorGUI_ClipboardFuncs.h"
#include "CClipBoard.h"
#include "FuetEngine.h"
//---------------------------------------------------------------------------
void ClipBoard_DeleteFunc(const wxString& _sDataType,void* _pData)
{
	if (_sDataType == "FUETENGINE_SPRITEEDITOR_SPRITEFRAME")
	{
		CFESpriteFrame* pData = (CFESpriteFrame*)_pData;
		delete pData;
	}
	
	// DO not delete this!
	// "FUETENGINE_HUDEDITOR_HUDOBJCOPY");
}
//---------------------------------------------------------------------------