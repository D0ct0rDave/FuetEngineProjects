//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------
#ifndef CSpriteEditorGUIFuncsH
#define CSpriteEditorGUIFuncsH
//---------------------------------------------------------------------------
#include <wx/bitmap.h>
#include <wx/string.h>
#include <FuetEngine.h>
//---------------------------------------------------------------------------
class wxInputPanel;
class wxMouseEvent;
//---------------------------------------------------------------------------
void SaveTGAFile(wxBitmap* _poBmp,unsigned int _uiWidth,unsigned int _uiHeight,const wxString& _sFilename);

void SelectAction(uint _uiActionIdx);
void SelectFrame(uint _uiSelectedFrame);

void FillSpriteActionList(wxString& _sSprite);
void SetupActionControls();
void SetupControls();

void DisplayPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseUp(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseMiddleDown(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseMiddleUp(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelMouseMove(wxInputPanel& panel,wxMouseEvent& event);
void DisplayPanelOnMouseWheel(wxMouseEvent& event);

void FrameSeqPanelMouseDown(wxInputPanel& panel,wxMouseEvent& event);

void SetFrameValues(CFESpriteFrame* _poFrame,FEReal _rFrameTime,FEReal _rDelayBlendFact);
void SetFrameGeometry(CFESpriteFrame* _poFrame,const CFEVect2& _oIUV,const CFEVect2& _oFUV,const CFEVect2& _oPivot);
void RecalcFrameTimes(CFESpriteAction* _poAction);
int iGetGridSize();
void AdjustPivot(const CFEVect2& _oPivot,FEBool _bAdjustX,FEBool _bAdjustY,FEBool _bAffectAllFrames);
CFEVect2 oGetCurrentPivotPos();
void SetPivot(const CFEVect2& _oScrPos);
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------



