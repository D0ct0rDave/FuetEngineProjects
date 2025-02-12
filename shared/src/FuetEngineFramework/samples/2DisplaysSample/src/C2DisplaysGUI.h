///-----------------------------------------------------------------
///
/// @file      C2DisplaysGUI.h
/// @author    Pepe
/// Created:   19/04/2010 22:21:44
/// @section   DESCRIPTION
///            C2DisplaysGUI class declaration
///
///------------------------------------------------------------------

#ifndef __C2DisplaysGUI_H__
#define __C2DisplaysGUI_H__

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
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/filedlg.h>
#include <wx/menu.h>
////Header Include End

////Dialog Style Start
#undef C2DisplaysGUI_STYLE
#define C2DisplaysGUI_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class C2DisplaysGUI : public wxFrame
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		C2DisplaysGUI(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("MapEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = C2DisplaysGUI_STYLE);
		virtual ~C2DisplaysGUI();
		
	private:
		//Do not add custom control declarations between
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxPanel *m_pnLowerScreen;
		wxPanel *m_pnUpperScreen;
		wxBoxSizer *WxBoxSizer1;
		wxFileDialog *WxOpenFileDialog1;
		wxMenuBar *WxMenuBar1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_M_PNLOWERSCREEN = 1018,
			ID_M_PNUPPERSCREEN = 1017,
			ID_MNU_FILE_1007 = 1007,
			ID_MNU_LOADMAP_1010 = 1010,
			ID_MNU_SAVEMAP_1011 = 1011,
			ID_MNU_SAVEMAPAS____1012 = 1012,
			ID_MNU_QUIT_1014 = 1014,
			
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
		
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		void OnIdle(wxIdleEvent& event);
};

#endif
