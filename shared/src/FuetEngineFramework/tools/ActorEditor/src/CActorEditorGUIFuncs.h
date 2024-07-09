//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CActorEditorGUIFuncsH
#define CActorEditorGUIFuncsH
//---------------------------------------------------------------------------
#include <wx/bitmap.h>
#include <wx/string.h>
#include <wx/event.h>
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class wxInputPanel;
class wxMouseEvent;
class CBoneSkelNode;
//---------------------------------------------------------------------------
void SaveTGAFile(wxBitmap* _poBmp,unsigned int _uiWidth,unsigned int _uiHeight,const wxString& _sFilename);
void OrientBone(uint _iBoneIdx, uint _iOrientToBoneIdx, float _fTime);
void MoveBoneTo(uint _iBoneIdx,float _fTime,const CFEVect2& _oNewPos);
void RescaleSkeleton(float _fHScale,float _fVScale);
void SetupSkeleton(const wxString& _sFilename);

void RescaleAnimationTime(float _fFactor);

void AssignSpriteToBoneNode(CBoneSkelNode* _poNode,FEHandler _hSpriteInst);
void AssignSpriteActionToBoneNode(CBoneSkelNode* _poNode,int _iAction);

void FillSpriteActionList(wxString& _sSprite);
void FillMeshActionList(wxString _sSprite);
void RefreshBoneControls();
void SetupBoneControls();
void SetupControls();
void SetupBoneNodeControls(CBoneSkelNode* _poNode);

void DisplayPanelOnMiddleMouseDown(wxMouseEvent& event);
void DisplayPanelOnMiddleMouseUp(wxMouseEvent& event);

void DisplayPanelOnMouseDown(wxMouseEvent& event);
void DisplayPanelOnMouseUp(wxMouseEvent& event);
void DisplayPanelOnMouseMove(wxMouseEvent& event);
void DisplayPanelOnMouseWheel(wxMouseEvent& event);
void DisplayPanelOnKeyDown(wxKeyEvent& event);
void DisplayPanelOnKeyUp(wxKeyEvent& event);

void UpdateNodeInstance(CFESkelAnimNode* _poSrcNode);


//---------------------------------------------------------------------------
void PlayStopAnimation();
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
