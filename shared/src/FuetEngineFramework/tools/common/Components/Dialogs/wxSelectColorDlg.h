///-----------------------------------------------------------------
///
/// @file      wxSelectColorDlg.h
/// @author    pepe
/// Created:   04/05/2010 19:10:51
/// @section   DESCRIPTION
///            wxSelectColorDlg class declaration
///
///------------------------------------------------------------------

#ifndef __WXSELECTCOLORDLG_H__
#define __WXSELECTCOLORDLG_H__

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
#include <wx/slider.h>
#include <wx/clrpicker.h>
#include <wx/stattext.h>
////Header Include End

bool bSelectColor(const wxString& _sTitle,unsigned int _uiInitialValue,unsigned int* _uiOutput);

////Dialog Style Start
#undef wxSelectColorDlg_STYLE
#define wxSelectColorDlg_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class wxSelectColorDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		void WxButton2Click(wxCommandEvent& event);
		void WxButton1Click(wxCommandEvent& event);
		
	public:
	    wxSelectColorDlg(wxWindow *parent, const wxString &title,unsigned int _uiInitialValue); 
		wxSelectColorDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select Color Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSelectColorDlg_STYLE);
		virtual ~wxSelectColorDlg();
	
	    unsigned int uiGetValue();
    	bool bOK();
		void sldAlphaSlider(wxCommandEvent& event);
		void colpLayerColorColourChanged(wxColourPickerEvent& event);

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton2;
		wxButton *WxButton1;
		wxSlider *sldAlpha;
		wxStaticText *WxStaticText8;
		wxColourPickerCtrl *colpLayerColor;
		wxStaticText *WxStaticText7;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON2 = 1012,
			ID_WXBUTTON1 = 1011,
			ID_SLDALPHA = 1007,
			ID_WXSTATICTEXT8 = 1008,
			ID_COLPLAYERCOLOR = 1009,
			ID_WXSTATICTEXT7 = 1010,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		
		
        unsigned int m_uiInitialValue;
		bool  m_bOK;
		wxString m_sTitle;		
};

#endif
