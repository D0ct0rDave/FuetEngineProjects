///-----------------------------------------------------------------
///
/// @file      WXSELECTVARIABLE.h
/// @author    pepe
/// Created:   04/05/2010 23:24:30
/// @section   DESCRIPTION
///            WXSELECTVARIABLE class declaration
///
///------------------------------------------------------------------

#ifndef __WXSELECTVARIABLE_H__
#define __WXSELECTVARIABLE_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
	#include "wx/things/spinctld.h"	
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
////Header Include End

////Dialog Style Start
#undef wxSelectVariable_STYLE
#define wxSelectVariable_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class wxSelectVariable : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
	    wxSelectVariable(wxWindow *parent, const wxString &title,const wxString &variable,const wxString &value,const wxString &variablelabel,const wxString &valuelabel);
		wxSelectVariable(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select Variable"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSelectVariable_STYLE);
		virtual ~wxSelectVariable();

		wxString sGetVariable();
		wxString sGetValue();
		bool bOK();
		void WxButton1Click(wxCommandEvent& event);
		void WxButton2Click(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton1;
		wxButton *WxButton2;
		wxTextCtrl *m_poValue;
		wxStaticText *m_poValueLabel;
		wxTextCtrl *m_poVariable;
		wxStaticText *m_poVariableLabel;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_WXBUTTON1 = 1007,
			ID_WXBUTTON2 = 1010,
			ID_M_POVALUE = 1015,
			ID_M_POVALUELABEL = 1017,
			ID_M_POVARIABLE = 1014,
			ID_M_POVARIABLELABEL = 1016,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};

	private:

		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();

		bool		m_bOK;
		wxString	m_sTitle;
		wxString	m_sVariable;
		wxString	m_sValue;
		wxString	m_sVariableLabel;
		wxString	m_sValueLabel;
};
///-----------------------------------------------------------------
#endif
///-----------------------------------------------------------------
