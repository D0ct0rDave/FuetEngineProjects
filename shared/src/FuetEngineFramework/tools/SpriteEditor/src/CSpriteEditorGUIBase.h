///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CSPRITEEDITORGUIBASE_H__
#define __CSPRITEEDITORGUIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class wxInputPanel;

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/scrolbar.h>
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>
#include <wx/statline.h>
#include <wx/tglbtn.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/scrolwin.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CSpriteEDITORGUIBASE 1000
#define ID_SBSIZERBONEPROPS 1001
#define ID_M_GDTIMELINE 1002
#define ID_M_BTNEWACTION 1003
#define ID_M_BTACTIONUP 1004
#define ID_M_BTACTIONDOWN 1005
#define ID_M_BTDELETEACTION 1006
#define ID_M_PNPREVDISPLAY 1007
#define ID_PLAYANIM 1008
#define ID_RELOADMATERIALS 1009
#define ID_PN_FRAMESEQ 1010
#define ID_M_BTCOPYFRAME 1011
#define ID_M_BTPASTEFRAME 1012
#define ID_KEYFRAME 1013
#define ID_M_BTNEWFRAME 1014
#define ID_M_BTDELETEFRAME 1015
#define ID_PN_DISPLAY 1016
#define ID_M_LBSNAPFACTOR 1017
#define ID_M_CBSNAPFACTOR 1018
#define wxID_FRAMEINFO 1019
#define wxID_MY_FRAME_PROPS 1020
#define ID_M_BTLOADMATERIAL 1021
#define ID_M_BTCLEARMATERIAL 1022
#define ID_ED_ICONSPRITE 1023
#define ID_M_BTAFFECTALLFRAMES 1024
#define ID_M_SLDDELAYBLENDFACT 1025
#define ID_M_SLDFRAMETIME 1026
#define ID_M_LBFRAMETIME 1027
#define ID_m_btAutoAdjustTopLeft 1028
#define ID_m_btAutoAdjustCenter 1029
#define ID_m_btAutoAdjustBottomCenter 1030
#define ID_m_btAutoAdjustSize 1031
#define ID_m_btAdjustSizeManually 1032
#define ID_M_LBIMGRECT 1033
#define ID_M_LBRECTSIZE 1034
#define ID_M_LBUVRECT 1035
#define ID_M_LBFRAMERECTSIZE 1036
#define ID_M_LBPIVOTIMAGECOORDS 1037
#define ID_M_EDPIVOTIMAGEXCOORD 1038
#define ID_M_EDPIVOTIMAGEYCOORD 1039
#define ID_m_btAdjustPivotVerticalTop 1040
#define ID_m_btAdjustPivotTopLeft 1041
#define ID_m_btAdjustPivotTopCenter 1042
#define ID_m_btAdjustPivotTopRight 1043
#define ID_m_btAdjustPivotVerticalCenter 1044
#define ID_m_btAdjustPivotCenterLeft 1045
#define ID_m_btAdjustPivotCenterCenter 1046
#define ID_m_btAdjustPivotCenterRight 1047
#define ID_m_btAdjustPivotVerticalBottom 1048
#define ID_m_btAdjustPivotBottomLeft 1049
#define ID_m_btAdjustPivotBottomCenter 1050
#define ID_m_btAdjustPivotBottomRight 1051
#define ID_m_btAdjustPivotHorizontalLeft 1052
#define ID_m_btAdjustPivotHorizontalCenter 1053
#define ID_m_btAdjustPivotHorizontalRight 1054
#define ID_MENUITEM0 1055
#define ID_MENUITEM1 1056
#define ID_MENUITEM11 1057
#define ID_MENUITEM_3 1058
#define ID_MO_GENHSCROLL 1059
#define ID_MO_GENVSCROLL 1060
#define ID_MO_ABOUT 1061

///////////////////////////////////////////////////////////////////////////////
/// Class CSpriteEditorGUIBase
///////////////////////////////////////////////////////////////////////////////
class CSpriteEditorGUIBase : public wxFrame 
{
	private:
	
	protected:
		wxPanel* WxPanel1;
		wxStaticText* m_staticText23;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText24;
		wxStaticText* m_staticText241;
		wxStaticText* m_staticText2411;
		wxStaticLine* m_staticline32;
		wxToggleButton* m_btAffectAllFrames;
		wxStaticText* m_staticText17;
		wxStaticText* m_staticText18;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText292;
		wxStaticLine* m_staticline3;
		wxStaticText* m_staticText29;
		wxStaticText* m_staticText5;
		wxStaticText* m_staticText6;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText16;
		wxStaticLine* m_staticline31;
		wxStaticText* m_staticText291;
		wxStaticText* m_staticText28;
		wxStaticText* m_staticText8;
		wxMenuBar* WxMenuBar1;
		wxMenu* menu0;
		wxMenu* m_menu51;
		wxMenu* m_menu5;
		wxStatusBar* CSpriteEditorGUIBAR;
		
		// Virtual event handlers, overide them in your derived class
		virtual void CSpriteEditorGUIBaseOnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void CSpriteEditorGUIBaseOnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void m_gdSpriteActionsOnGridCellLeftClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdSpriteActionsOnGridCellLeftDClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdSpriteActionsOnGridCellRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_btNewActionOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btActionUpOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btActionDownOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btDeleteActionOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlayAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btReloadMaterialsOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btCopyFrameOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btPasteFrameOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void sb_KeyFrameOnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_btNewFrameOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btDeleteFrameOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void pn_DisplayOnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void pn_DisplayOnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void m_cbSnapFactorOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btLoadMaterialOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btClearMaterialOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbSWrapModeOnCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbTWrapModeOnCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbFilterOnCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAffectAllFramesOnToggleButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_sldDelayBlendFactOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_sldFrameTimeOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_btAutoAdjustTopLeftOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAutoAdjustCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAutoAdjustBottomCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAutoAdjustSizeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustSizeManuallyOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbScaleXUEquallyOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbUWorldCoordsOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbScaleYVEquallyOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbVWorldCoordsOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageXCoordOnKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageXCoordOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageXCoordOnSpinCtrlText( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageYCoordOnKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageYCoordOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edPivotImageYCoordOnSpinCtrlText( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotVerticalTopOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotTopLeftOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotTopCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotTopRightOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotVerticalCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotCenterLeftOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotCenterCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotCenterRightOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotVerticalBottomOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotBottomLeftOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotBottomCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotBottomRightOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotHorizontalLeftOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotHorizontalCenterOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btAdjustPivotHorizontalRightOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem0OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem1OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem11OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem3OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mo_GenerateHScrollOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mo_GenerateVScrollOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mo_AboutOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxStaticBoxSizer* sbSizerBoneProps;
		wxGrid* m_gdSpriteActions;
		wxBitmapButton* m_btNewAction;
		wxBitmapButton* m_btActionUp;
		wxBitmapButton* m_btActionDown;
		wxBitmapButton* m_btDeleteAction;
		wxInputPanel* m_pnPrevDisplay;
		wxButton* bt_Play;
		wxBitmapButton* m_btReloadMaterials;
		wxInputPanel* m_pnFrameSeq;
		wxBitmapButton* m_btCopyFrame;
		wxBitmapButton* m_btPasteFrame;
		wxScrollBar* sb_KeyFrame;
		wxBitmapButton* m_btNewFrame;
		wxBitmapButton* m_btDeleteFrame;
		wxInputPanel* pn_Display;
		wxStaticText* m_lbSnapFactor;
		wxChoice* m_cbSnapFactor;
		wxStaticText* lb_Frame;
		wxStaticText* lb_Trans;
		wxStaticText* lb_Zoom;
		wxScrolledWindow* m_pnFrameProps;
		wxBitmapButton* m_btLoadMaterial;
		wxBitmapButton* m_btClearMaterial;
		wxTextCtrl* m_edFrameMaterial;
		wxComboBox* m_cbSWrapMode;
		wxComboBox* m_cbTWrapMode;
		wxComboBox* m_cbFilter;
		wxSlider* m_sldDelayBlendFact;
		wxSlider* m_sldFrameTime;
		wxStaticText* m_lbFrameTime;
		wxBitmapButton* m_btAutoAdjustTopLeft;
		wxBitmapButton* m_btAutoAdjustCenter;
		wxBitmapButton* m_btAutoAdjustBottomCenter;
		wxBitmapButton* m_btAutoAdjustSize;
		wxBitmapButton* m_btAdjustSizeManually;
		wxCheckBox* m_cbScaleXUEqually;
		wxCheckBox* m_cbUWorldCoords;
		wxCheckBox* m_cbScaleYVEqually;
		wxCheckBox* m_cbVWorldCoords;
		wxStaticText* m_lbImgRect;
		wxStaticText* m_lbImageRectSize;
		wxStaticText* m_lbUVRect;
		wxStaticText* m_lbFrameRectSize;
		wxStaticText* m_lbPivotFrameCoords;
		wxStaticText* m_lbPivotImageXCoord;
		wxSpinCtrl* m_edPivotImageXCoord;
		wxStaticText* m_lbPivotImageYCoord;
		wxSpinCtrl* m_edPivotImageYCoord;
		wxBitmapButton* m_btAdjustPivotVerticalTop;
		wxBitmapButton* m_btAdjustPivotTopLeft;
		wxBitmapButton* m_btAdjustPivotTopCenter;
		wxBitmapButton* m_btAdjustPivotTopRight;
		wxBitmapButton* m_btAdjustPivotVerticalCenter;
		wxBitmapButton* m_btAdjustPivotCenterLeft;
		wxBitmapButton* m_btAdjustPivotCenterCenter;
		wxBitmapButton* m_btAdjustPivotCenterRight;
		wxBitmapButton* m_btAdjustPivotVerticalBottom;
		wxBitmapButton* m_btAdjustPivotBottomLeft;
		wxBitmapButton* m_btAdjustPivotBottomCenter;
		wxBitmapButton* m_btAdjustPivotBottomRight;
		wxBitmapButton* m_btAdjustPivotHorizontalLeft;
		wxBitmapButton* m_btAdjustPivotHorizontalCenter;
		wxBitmapButton* m_btAdjustPivotHorizontalRight;
		
		CSpriteEditorGUIBase( wxWindow* parent, wxWindowID id = ID_CSpriteEDITORGUIBASE, const wxString& title = wxT("SpriteEditor"), const wxPoint& pos = wxPoint( 8,8 ), const wxSize& size = wxSize( 1501,1024 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxMINIMIZE_BOX|wxSYSTEM_MENU );
		
		~CSpriteEditorGUIBase();
	
};

#endif //__CSPRITEEDITORGUIBASE_H__
