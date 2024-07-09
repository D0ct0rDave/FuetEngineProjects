//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CPoseFileOpsH
#define CPoseFileOpsH
//---------------------------------------------------------------------------
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class CSkeleton;
//---------------------------------------------------------------------------
void SavePoseFile(const CFEString& _sFilename,CSkeleton* _poSkeleton,float _fAnimTime);
void LoadPoseFile(const CFEString& _sFilename,CSkeleton* _poSkeleton,float _fAnimTime);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------