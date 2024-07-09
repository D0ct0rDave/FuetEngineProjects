///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CACTOREDITORGUIBASE_H__
#define __CACTOREDITORGUIBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
class wxMyGrid;
class wxSpinCtrlDbl;
class wxTreeListCtrl;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/choice.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/scrolbar.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/clrpicker.h>
#include <wx/textctrl.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CACTOREDITORGUIBASE 1000
#define ID_PN_DISPLAY 1001
#define ID_CB_SHOWSPRITES 1002
#define ID_SB_KEYFRAME 1003
#define ID_BT_ADDKEYFRAME 1004
#define ID_BT_DECKEYFRAME 1005
#define ID_M_GDTIMELINE 1006
#define ID_CB_SHOWPREVBONES 1007
#define ID_CB_SHOWPREVGRID 1008
#define ID_PLAYANIM 1009
#define ID_LOOPANIMATION 1010
#define ID_ANIMTIME 1011
#define ID_M_EDBONEXCOORD 1012
#define wxID_ANIMATEPIVOT 1013
#define ID_PNICONLAYER 1014
#define ID_SBSIZERBONEPROPS 1015
#define ID_M_EDBONEYCOORD 1016
#define ID_M_POBONENODES 1017
#define ID_M_SLDNODEANGLE 1018
#define ID_M_EDNODEDEPTH 1019
#define ID_M_CPSELECTCOLOR 1020
#define ID_M_SLDNODEALPHA 1021
#define ID_M_PNSPRITEPROPS 1022
#define ID_ED_ICONSPRITE 1023
#define ID_LOADSPRITE 1024
#define ID_CLEARSPRITE 1025
#define ID_M_CBACTIONLST 1026
#define ID_TRICKY_MENU_PANEL 1027
#define ID_MNU_COPYPOSE 1028
#define ID_MNU_PASTEPOSE 1029
#define ID_MNU_PASTEPOSEFORCE 1030
#define ID_M_MENUITEM14 1031
#define wxID_POSE_SEL_STATE 1032
#define wxID_POSE_SEL_COPY 1033
#define wxID_POSE_SEL_CLEAR 1034
#define wxID_POSE_SEL_DELETE 1035
#define ID_MNU_COPYINIVAL 1036
#define ID_MNU_DELETEKEYFRAME 1037
#define ID_MNU_CONSTANT 1038
#define ID_MNU_LINEAR 1039
#define ID_MNU_SINUSOIDAL 1040
#define ID_MNU_EXPONENTIAL 1041
#define ID_MNU_INVSIN 1042
#define ID_MNU_RANDOM 1043
#define ID_MNU_SINSIN 1044
#define ID_MNU_EXPLOG 1045
#define ID_M_MENUITEM161 1046
#define ID_M_MENUITEM291 1047
#define ID_M_MENUITEM181 1048
#define ID_M_MENUITEM191 1049
#define ID_M_MENUITEM201 1050
#define ID_M_MENUITEM301 1051
#define ID_M_MENUITEM211 1052
#define ID_M_MENUITEM221 1053
#define ID_M_MENUITEM231 1054
#define ID_MENU_LOADANIM 1055
#define ID_MENU_SAVEANIM 1056
#define ID_MENUI_SAVEASANIM 1057
#define ID_MENU_IMPORTSKELETON 1058
#define ID_MENU_EXPORTSKELETON 1059
#define ID_MENU_EXPORT_ANIMATION 1060
#define wxID_RESTART_WITH_SKEL_DEF 1061
#define ID_MENU_QUIT 1062
#define wxID_RELOADMATERIALS 1063
#define ID_MENUITEM4 1064
#define ID_MENUITEM5 1065
#define ID_MENUITEM7 1066
#define ID_MENUITEM71 1067
#define ID_MENUITEM13 1068
#define ID_MO_ABOUT 1069

///////////////////////////////////////////////////////////////////////////////
/// Class CActorEditorGUIBase
///////////////////////////////////////////////////////////////////////////////
class CActorEditorGUIBase : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* m_staticText1611;
		wxStaticText* m_staticText161;
		wxButton* bt_AddKeyframe;
		wxButton* bt_DelKeyframe;
		wxPanel* pn_PreviewDisplay;
		wxStaticText* m_staticText19;
		wxStaticText* m_staticText12;
		wxStaticText* m_staticText13;
		wxButton* m_btResetBonePos;
		wxStaticText* m_staticText822;
		wxStaticText* m_staticText121;
		wxStaticText* m_staticText131;
		wxStaticText* m_staticText821;
		wxStaticText* m_staticText82;
		wxStaticText* m_staticText823;
		wxStaticText* m_staticText8221;
		wxStaticText* WxStaticText4;
		wxStaticText* WxStaticText41;
		wxPanel* TRICKY_MENU_PANEL;
		wxMenu* m_pupmKeyFrameOps;
		wxMenu* m_menu3;
		wxPanel* m_panel6;
		wxMenu* m_popKeyFrameMenu;
		wxMenu* m_menu2;
		wxPanel* m_panel7;
		wxMenu* m_popKeyFrameFuncMenu;
		wxMenu* m_menu21;
		wxMenu* menu0;
		wxMenu* menu1;
		wxMenu* m_menu5;
		wxStatusBar* CActorEditorGUIBAR;
		
		// Virtual event handlers, overide them in your derived class
		virtual void GUIOnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void GUIOnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnKeyUp( wxKeyEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnLeftDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnLeftUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnMiddleDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnMiddleUp( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnMotion( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnMouseWheel( wxMouseEvent& event ) { event.Skip(); }
		virtual void m_pnDisplayOnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void m_edBackXOffsetOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edBackYOffsetOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edBackXScaleOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edBackYScaleOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_cbEditModeOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btUndoOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_btRedoOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void sb_KeyFrameOnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void bt_AddKeyframeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void bt_DelKeyframeOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_gdTimelineOnGridCellLeftClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdTimelineOnGridCellRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdTimelineOnGridLabelLeftClick( wxGridEvent& event ) { event.Skip(); }
		virtual void m_gdTimelineOnGridLabelRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnPlayAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLoopAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_AnimTimeOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_edAnimTimeOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edBoneXCoordOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edBoneYCoordOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_button5OnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_poBoneNodesOnTreeSelChanged( wxTreeEvent& event ) { event.Skip(); }
		virtual void m_edNodePosXOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_edNodePosYOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_sldNodeAngleOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void m_edNodeDepthOnSpinCtrl( wxSpinEvent& event ) { event.Skip(); }
		virtual void m_cpSelectColorOnColourChanged( wxColourPickerEvent& event ) { event.Skip(); }
		virtual void m_sldNodeAlphaOnCommandScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void bt_LoadSpriteOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void bt_ClearSpriteOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_cbSpriteActionListOnChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem121OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem131OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem1311OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem14OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem141OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miPoseSelStateOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miInsertPoseSelOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miClearPoseSelOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miDeletePoseSelOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem17OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem16OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem29OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem18OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem19OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem20OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem30OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem21OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem22OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem23OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem161OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem291OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem181OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem191OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem201OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem301OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem211OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem221OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem231OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem0OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem1OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem11OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem12OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem10OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem11OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_moRestartWithSkelDefOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem3OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miUndoOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_miRedoOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void ReloadMaterialsClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem4OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem5OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem7SelectBackgroundColor( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem7OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem13OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void mo_AboutOnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxPanel* WxPanel1;
		wxPanel* m_pnDisplay;
		wxStaticText* lb_Frame;
		wxStaticText* lb_Trans;
		wxStaticText* lb_Zoom;
		wxCheckBox* cb_ShowBackground;
		wxSpinCtrlDbl* m_edBackXOffset;
		wxSpinCtrlDbl* m_edBackYOffset;
		wxSpinCtrlDbl* m_edBackXScale;
		wxSpinCtrlDbl* m_edBackYScale;
		wxCheckBox* cb_ShowLabels;
		wxCheckBox* cb_ShowBones;
		wxCheckBox* cb_ShowSprites;
		wxSlider* m_sldSpritesAlpha;
		wxChoice* m_cbEditMode;
		wxBitmapButton* m_btUndo;
		wxBitmapButton* m_btRedo;
		wxCheckBox* m_cbUpdateScrollBar;
		wxScrollBar* sb_KeyFrame;
		wxMyGrid* m_gdTimeline;
		wxCheckBox* cb_ShowPrevBones;
		wxCheckBox* cb_ShowGrid;
		wxCheckBox* cb_ShapeMode;
		wxButton* bt_Play;
		wxCheckBox* cb_Loop;
		wxSlider* sld_AnimTime;
		wxSpinCtrlDbl* m_edAnimTime;
		wxCheckBox* m_cbAnimatePivot;
		wxPanel* pnIconLayer;
		wxStaticBoxSizer* sbSizerBoneProps;
		wxSpinCtrlDbl* m_edBoneXCoord;
		wxSpinCtrlDbl* m_edBoneYCoord;
		wxStaticText* m_lbBoneOffset;
		wxTreeListCtrl* m_poBoneNodes;
		wxPanel* m_pnNodeProps;
		wxSpinCtrlDbl* m_edNodePosX;
		wxSpinCtrlDbl* m_edNodePosY;
		wxSlider* m_sldNodeAngle;
		wxSpinCtrlDbl* m_edNodeDepth;
		wxColourPickerCtrl* m_cpSelectColor;
		wxSlider* m_sldNodeAlpha;
		wxPanel* m_pnSpriteProps;
		wxTextCtrl* m_edIconSprite;
		wxBitmapButton* bt_LoadSprite;
		wxBitmapButton* bt_ClearSprite;
		wxChoice* m_cbSpriteActionList;
		wxMenuBar* m_mbMainMenu;
		
		CActorEditorGUIBase( wxWindow* parent, wxWindowID id = ID_CACTOREDITORGUIBASE, const wxString& title = wxT("ActorEditor"), const wxPoint& pos = wxPoint( 8,8 ), const wxSize& size = wxSize( 1285,900 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxMINIMIZE_BOX|wxSYSTEM_MENU );
		
		~CActorEditorGUIBase();
		
		void TRICKY_MENU_PANELOnContextMenu( wxMouseEvent &event )
		{
			TRICKY_MENU_PANEL->PopupMenu( m_pupmKeyFrameOps, event.GetPosition() );
		}
		
		void m_panel6OnContextMenu( wxMouseEvent &event )
		{
			m_panel6->PopupMenu( m_popKeyFrameMenu, event.GetPosition() );
		}
		
		void m_panel7OnContextMenu( wxMouseEvent &event )
		{
			m_panel7->PopupMenu( m_popKeyFrameFuncMenu, event.GetPosition() );
		}
	
};

#endif //__CACTOREDITORGUIBASE_H__
