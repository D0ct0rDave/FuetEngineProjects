//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CActorEditorFileOpsH
#define CActorEditorFileOpsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CSkeleton.h"
//---------------------------------------------------------------------------
void SaveSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton);
void LoadSkeleton(const CFEString& _sFilename,CSkeleton* _poSkeleton);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------