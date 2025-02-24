///-----------------------------------------------------------------
///
/// @file      CEditorAboutBox.h
/// @author    pepe
/// Created:   13/05/2010 8:46:39
/// @section   DESCRIPTION
///            CEditorAboutBox class declaration
///
///------------------------------------------------------------------

#ifndef __CEditorABOUTBOX_H__
#define __CEditorABOUTBOX_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/statbmp.h>
#include <wx/sizer.h>
////Header Include End

////Dialog Style Start
#undef CEditorAboutBox_STYLE
#define CEditorAboutBox_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class CEditorAboutBox : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		CEditorAboutBox(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("About ..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = CEditorAboutBox_STYLE);
		virtual ~CEditorAboutBox();
		void WxButton1Click(wxCommandEvent& event);
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton1;
		wxBoxSizer *WxBoxSizer4;
		wxTextCtrl *WxMemo2;
		wxStaticBoxSizer *WxStaticBoxSizer2;
		wxTextCtrl *WxMemo1;
		wxStaticBoxSizer *WxStaticBoxSizer1;
		wxBoxSizer *WxBoxSizer3;
		wxStaticBitmap *WxStaticBitmap1;
		wxBoxSizer *WxBoxSizer2;
		wxBoxSizer *WxBoxSizer1;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON1 = 1012,
			ID_WXMEMO1 = 1005,
			ID_WXSTATICBITMAP1 = 1002,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
};

#endif
