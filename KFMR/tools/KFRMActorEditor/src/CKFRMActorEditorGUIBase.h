///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __CKFRMActorEditorGUIBase__
#define __CKFRMActorEditorGUIBase__

class wxInputPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/scrolbar.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/slider.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/combobox.h>
#include <wx/statbox.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

#define ID_CKFRMActorEditorGUIBASE 1000
#define ID_PN_DISPLAY 1001
#define ID_KEYFRAME 1002
#define ID_SHOWBACKGROUND 1003
#define ID_SHOWLABELS 1004
#define ID_RELOADMATERIALS 1005
#define ID_PLAYANIM 1006
#define ID_ANIMTIME 1007
#define ID_HSCALE 1008
#define ID_VSCALE 1009
#define ID_PNICONLAYER 1010
#define ID_SBSIZERBONEPROPS 1011
#define ID_LOADSPRITE 1012
#define ID_CBSPRITEACTION 1013
#define ID_SLD_XOFS 1014
#define ID_SLD_YOFS 1015
#define ID_MENUITEM12 1016
#define ID_MENUITEM10 1017
#define ID_MENUITEM_3 1018

///////////////////////////////////////////////////////////////////////////////
/// Class CKFRMActorEditorGUIBase
///////////////////////////////////////////////////////////////////////////////
class CKFRMActorEditorGUIBase : public wxFrame 
{
	DECLARE_EVENT_TABLE()
	private:
		
		// Private event handlers
		void _wxFB_CKFRMActorEditorGUIBaseOnClose( wxCloseEvent& event ){ CKFRMActorEditorGUIBaseOnClose( event ); }
		void _wxFB_CKFRMActorEditorGUIBaseOnIdle( wxIdleEvent& event ){ CKFRMActorEditorGUIBaseOnIdle( event ); }
		void _wxFB_sb_KeyFrameOnScroll( wxScrollEvent& event ){ sb_KeyFrameOnScroll( event ); }
		void _wxFB_cb_ShowBackgroundOnCheckBox( wxCommandEvent& event ){ cb_ShowBackgroundOnCheckBox( event ); }
		void _wxFB_cb_ShowLabelsOnCheckBox( wxCommandEvent& event ){ cb_ShowLabelsOnCheckBox( event ); }
		void _wxFB_ReloadMaterialsClick( wxCommandEvent& event ){ ReloadMaterialsClick( event ); }
		void _wxFB_OnPlayAnimationClick( wxCommandEvent& event ){ OnPlayAnimationClick( event ); }
		void _wxFB_sld_AnimTimeOnCommandScroll( wxCommandEvent& event ){ sld_AnimTimeOnCommandScroll( event ); }
		void _wxFB_sld_HScaleOnCommandScroll( wxCommandEvent& event ){ sld_HScaleOnCommandScroll( event ); }
		void _wxFB_sld_VScaleOnCommandScroll( wxCommandEvent& event ){ sld_VScaleOnCommandScroll( event ); }
		void _wxFB_bt_LoadSpriteOnButtonClick( wxCommandEvent& event ){ bt_LoadSpriteOnButtonClick( event ); }
		void _wxFB_cbSpriteActionOnCombobox( wxCommandEvent& event ){ cbSpriteActionOnCombobox( event ); }
		void _wxFB_sld_OfsOnCommandScroll( wxCommandEvent& event ){ sld_OfsOnCommandScroll( event ); }
		void _wxFB_sld_YOfsOnCommandScroll( wxCommandEvent& event ){ sld_YOfsOnCommandScroll( event ); }
		void _wxFB_m_menuItem12OnMenuSelection( wxCommandEvent& event ){ m_menuItem12OnMenuSelection( event ); }
		void _wxFB_m_menuItem10OnMenuSelection( wxCommandEvent& event ){ m_menuItem10OnMenuSelection( event ); }
		void _wxFB_menuitem3OnMenuSelection( wxCommandEvent& event ){ menuitem3OnMenuSelection( event ); }
		
	
	protected:
		wxPanel* WxPanel1;
		wxButton* bt_ReloadMaterials;
		wxButton* bt_Play;
		wxPanel* WxPanel2;
		wxStaticText* WxStaticText1;
		wxStaticText* WxStaticText11;
		wxStaticText* WxStaticText4;
		wxBitmapButton* bt_LoadSprite;
		wxStaticText* m_staticText7;
		wxStaticText* m_staticText8;
		wxMenuBar* WxMenuBar1;
		wxMenu* menu0;
		wxStatusBar* CKFRMActorEditorGUIBAR;
		
		// Virtual event handlers, overide them in your derived class
		virtual void CKFRMActorEditorGUIBaseOnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void CKFRMActorEditorGUIBaseOnIdle( wxIdleEvent& event ) { event.Skip(); }
		virtual void sb_KeyFrameOnScroll( wxScrollEvent& event ) { event.Skip(); }
		virtual void cb_ShowBackgroundOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void cb_ShowLabelsOnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void ReloadMaterialsClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPlayAnimationClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_AnimTimeOnCommandScroll( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_HScaleOnCommandScroll( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_VScaleOnCommandScroll( wxCommandEvent& event ) { event.Skip(); }
		virtual void bt_LoadSpriteOnButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void cbSpriteActionOnCombobox( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_OfsOnCommandScroll( wxCommandEvent& event ) { event.Skip(); }
		virtual void sld_YOfsOnCommandScroll( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem12OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void m_menuItem10OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void menuitem3OnMenuSelection( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		wxInputPanel* pn_Display;
		wxScrollBar* sb_KeyFrame;
		wxCheckBox* cb_ShowBackground;
		wxCheckBox* cb_ShowLabels;
		wxSlider* sld_AnimTime;
		wxSlider* sld_HScale;
		wxSlider* sld_VScale;
		wxPanel* pnIconLayer;
		wxTextCtrl* edIconSprite;
		wxComboBox* cbSpriteAction;
		wxSlider* sld_XOfs;
		wxSlider* sld_YOfs;
		
		CKFRMActorEditorGUIBase( wxWindow* parent, wxWindowID id = ID_CKFRMActorEditorGUIBASE, const wxString& title = wxT("KFRMActorEditor"), const wxPoint& pos = wxPoint( 8,8 ), const wxSize& size = wxSize( -1,-1 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxMINIMIZE_BOX|wxSYSTEM_MENU );
		~CKFRMActorEditorGUIBase();
	
};

#endif //__CKFRMActorEditorGUIBase__
