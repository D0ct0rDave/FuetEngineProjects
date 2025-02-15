//---------------------------------------------------------------------------
//
// Name:        FGTSViewerGUI.h
// Author:      PepeMagnifico
// Created:     13/04/2009 12:12:10
// Description: CGTSViewerGUI class declaration
//
//---------------------------------------------------------------------------

#ifndef __CGTSVIEWERGUI_h__
#define __CGTSVIEWERGUI_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/frame.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/scrolbar.h>
#include <wx/menu.h>
#include <wx/filedlg.h>
#include <wx/timer.h>
#include <wx/checkbox.h>
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/fontdlg.h>
#include <wx/statusbr.h>
////Header Include End

////Dialog Style Start
#undef CGTSViewerGUI_STYLE
#define CGTSViewerGUI_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class wxInputPanel;

class CActorEditorGUI : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		CActorEditorGUI(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("GTSViewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = CGTSViewerGUI_STYLE);
		virtual ~CActorEditorGUI();
	/*
	void Mnuopengtsfile1001Click(wxCommandEvent& event);
	void Mnusavegemfile1002Click(wxCommandEvent& event);
	void Mnuquit1004Click(wxCommandEvent& event);
	void Mnufile1005Click(wxCommandEvent& event);
	void Mnuopengtsfile1005Click(wxCommandEvent& event);
		void WxTimer1Timer(wxTimerEvent& event);
	void Mnuopengtsfile1005Click1(wxCommandEvent& event);
	*/
		void Mnuquit1004Click0(wxCommandEvent& event);

		void Mnusaveasstandardpose1032Click(wxCommandEvent& event);
		void MnuloadActorAnim1021Click(wxCommandEvent& event);
		void MnuSaveActorAnim1022Click(wxCommandEvent& event);
		void WxTimer1Timer(wxTimerEvent& event);
		void sb_KeyFrameScroll(wxScrollEvent& event);
		void cb_ShowLabelsClick(wxCommandEvent& event);

	private:
    public:	
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxSlider *sld_AnimTime;
		wxBoxSizer *WxBoxSizer7;
		wxCheckBox *cb_Loop;
		wxButton *bt_Play;
		wxBoxSizer *WxBoxSizer6;
		wxScrollBar *sb_KeyFrame;
		wxInputPanel *pn_Display;
		wxMenuBar *WxMenuBar1;
		wxFileDialog *dlg_SaveFont;
		wxFileDialog *dlg_OpenFont;
		wxTimer *WxTimer1;
		wxCheckBox *cb_ShowLabels;
		wxStaticText *lb_Zoom;
		wxStaticText *lb_Trans;
		wxStaticText *lb_Frame;
		wxBoxSizer *WxBoxSizer2;
		wxPanel *pn_PreviewDisplay;
		wxBoxSizer *WxBoxSizer4;
		wxBoxSizer *WxBoxSizer5;
		wxBoxSizer *WxBoxSizer3;
		wxBoxSizer *WxBoxSizer1;
		wxFontDialog *dlgSelectFont;
		wxStatusBar *CActorEditorGUIBAR;
		////GUI Control Declaration End
		
		void Mnuloadstandardpose1045Click(wxCommandEvent& event);
		void Mnuloadframesequencelist1047Click(wxCommandEvent& event);
		void WxButton1Click(wxCommandEvent& event);
		void cb_LoopClick(wxCommandEvent& event);
		void WxSlider1Slider(wxCommandEvent& event);
		void Mnusnapkeyframestosequence1065Click(wxCommandEvent& event);
	private:
		// void OnPaint(wxPaintEvent& WXUNUSED(event) );
    
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_SLD_ANIMTIME = 1064,
			ID_CB_LOOP = 1062,
			ID_BT_PLAY = 1061,
			ID_SB_KEYFRAME = 1059,
			ID_PN_DISPLAY = 1058,
			ID_MNU_FILEMENU_1001 = 1001,
			ID_MNU_LOADACTORANIM_1021 = 1021,
			ID_MNU_SAVEACTORANIM_1022 = 1022,
			ID_MNU_QUIT_1004 = 1004,
			ID_MNU_MISC_1031 = 1031,
			ID_MNU_SAVEASSTANDARDPOSE_1032 = 1032,
			ID_MNU_LOADSTANDARDPOSE_1045 = 1045,
			ID_MNU_LOADFRAMESEQUENCELIST_1047 = 1047,
			ID_MNU_SNAPKEYFRAMESTOSEQUENCE_1065 = 1065,
			
			ID_WXTIMER1 = 1033,
			ID_CB_SHOWLABELS = 1044,
			ID_LB_ZOOM = 1049,
			ID_LB_TRANS = 1048,
			ID_LB_FRAME = 1043,
			ID_PN_PREVIEWDISPLAY = 1056,
			ID_CFONTEDITORGUI = 1002,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	private:
        void OnIdle(wxIdleEvent& event);
		void OnClose(wxCloseEvent& event);
		// void OnPaint(wxPaintEvent& event);
        void OnMouseDown(wxMouseEvent& event);
        void OnMouseMove(wxMouseEvent& event);
        void OnMouseUp(wxMouseEvent& event);

		void CreateGUIControls();		
};

#endif


