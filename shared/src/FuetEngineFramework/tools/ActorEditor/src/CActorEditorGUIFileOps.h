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

#include "FileOps/CFileOpsCommon.h"
#include "FileOps/CAEAFileOps.h"
#include "FileOps/CPoseFileOps.h"
#include "FileOps/CSKEFileOps.h"
#include "FileOps/CSKAFileOps.h"
//---------------------------------------------------------------------------
bool bLoadFrameSequence(const CFEString& _sFilename);

//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------