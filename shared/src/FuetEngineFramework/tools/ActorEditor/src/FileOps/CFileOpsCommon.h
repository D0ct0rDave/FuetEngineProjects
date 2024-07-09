//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CFileOpsCommonH
#define CFileOpsCommonH
//---------------------------------------------------------------------------
#include <string>
#include <FuetEngine.h>
#include "CSkeleton.h"
//---------------------------------------------------------------------------
class CConfigFileWriter;
//---------------------------------------------------------------------------
const uint BONE_PIVOT = 0;

const FEReal DEF_ANIM_TIME = 1.0f;

const FEReal DEF_POS_X = 0.0f;
const FEReal DEF_POS_Y = 0.0f;
const FEReal DEF_SCALE_X = 1.0f;
const FEReal DEF_SCALE_Y = 1.0f;
const FEReal DEF_ANGLE = 0.0f;
const FEReal DEF_DEPTH = 0.0f;

const FEReal DEF_TIME  = 0.0f;
const uint DEF_KEYFRAMES = 0;
const bool DEF_VISIBILITY = true;

const std::string DEF_LERP = "linear";
const std::string DEF_WRAP = "loop";

const CFEString DEF_FELERP = "linear";
const CFEString DEF_FEWRAP = "loop";

const int	DEF_ACTION = -1;
//---------------------------------------------------------------------------
void CloneNodes(CFESkelAnimNode* _poDstNode,CFESkelAnimNode* _poLoadedSrc);
void WriteFloatKeyFrames(CFEKFBFunc<float>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode);
void WriteVectKeyFrames(CFEKFBFunc<CFEVect2>& _oCoords,CConfigFileWriter* _poFWriter,const std::string& _sWrapMode);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------