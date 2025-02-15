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
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class wxInputPanel;
class wxMouseEvent;
//---------------------------------------------------------------------------
wxString sRelativizePath(const wxString& _sFilename,const wxString& _sBasePath);
CFEString sRelativizePath(const CFEString& _sFilename,const CFEString& _sBasePath);

void SaveTGAFile(wxBitmap* _poBmp,unsigned int _uiWidth,unsigned int _uiHeight,const wxString& _sFilename);
void OrientBone(uint _iBoneIdx, uint _iOrientToBoneIdx, float _fTime);
void MoveBoneTo(uint _iBoneIdx,float _fTime,const CFEVect2& _oNewPos);
void RescaleSkeleton(float _fHScale,float _fVScale);
void SetupSkeleton();

void FillSpriteActionList(wxString& _sSprite);
void SetupBoneControls();
void SetupControls();

void DisplayPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseUp(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseMove(wxInputPanel& panel,wxMouseEvent& event);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------



