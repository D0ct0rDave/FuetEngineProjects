//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CSpriteEditorFileOpsH
#define CSpriteEditorFileOpsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>

CFEString sGetBlendMode(EFEBlendMode _eBlendMode);
bool bLoadSprite(const CFEString& _sFilename);
bool bSaveSprite(CFESprite* _poSprite,const CFEString& _sFilename);
bool bSaveSpriteAsAnimatedGIF(CFESprite* _poSprite,const CFEString& _sFilename);

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------