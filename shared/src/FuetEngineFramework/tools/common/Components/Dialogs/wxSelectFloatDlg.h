//---------------------------------------------------------------------------
//
// Name:        wxSelectFloatDlg.h
// Author:      David Márquez
// Created:     26/01/2010 14:03:16
// Description: wxSelectFloatDlg class declaration
//
//---------------------------------------------------------------------------

#ifndef __WXSELECTFLOATDLG_h__
#define __WXSELECTFLOATDLG_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
	#include "wx/spinreal.h"
	#include "wx/things/spinctld.h"
	
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/spinctrl.h>
////Header Include End


bool bSelectFloat(const wxString& _sTitle,float _fInitialValue,float _fMin,float _fMax,float* _pfOutput);


////Dialog Style Start
#undef wxSelectFloatDlg_STYLE
#define wxSelectFloatDlg_STYLE wxCAPTION | wxDIALOG_NO_PARENT
////Dialog Style End

class wxSelectFloatDlg : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
		wxSelectFloatDlg(wxWindow *parent, const wxString &title,float _fInitialValue,float _fMin,float _fMax);
		wxSelectFloatDlg(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select Float Value"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSelectFloatDlg_STYLE);

		virtual ~wxSelectFloatDlg();
		float fGetValue();
		bool bOK();

		void WxButton1Click(wxCommandEvent& event);
		void WxButton2Click(wxCommandEvent& event);
		void WxSlider1Slider(wxCommandEvent& event);
		void m_poSelectValueSpinDown(wxSpinEvent& event );
		void m_poSelectValueSpinUp(wxSpinEvent& event);
		void m_poSelectValueUpdated(wxSpinEvent& event );
	
	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxSlider *m_poSlider;
		wxButton *WxButton2;
		wxButton *WxButton1;
		wxSpinCtrlDbl *m_poSelectValue;
		////GUI Control Declaration End
		
	private:
		//Note: if you receive any error with these enum IDs, then you need to
		//change your old form code that are based on the #define control IDs.
		//#defines may replace a numeric value for the enum names.
		//Try copy and pasting the below block in your old form header files.
		enum
		{
			////GUI Enum Control ID Start
			ID_M_POSLIDER = 1004,
			ID_WXBUTTON2 = 1003,
			ID_WXBUTTON1 = 1002,
			ID_M_POSELECTVALUE = 1001,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();
		
		float m_fMin;
		float m_fMax;
		float m_fInitialValue;
		bool  m_bOK;
		wxString m_sTitle;
};

#endif
