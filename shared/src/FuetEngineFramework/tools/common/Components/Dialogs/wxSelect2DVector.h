///-----------------------------------------------------------------
///
/// @file      wxSelect2DVector.h
/// @author    pepe
/// Created:   04/05/2010 23:24:30
/// @section   DESCRIPTION
///            wxSelect2DVector class declaration
///
///------------------------------------------------------------------

#ifndef __WXSELECT2DVECTOR_H__
#define __WXSELECT2DVECTOR_H__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
	#include <wx/dialog.h>
	// #include "wx/spinreal.h"
	#include "wx/things/spinctld.h"	
#else
	#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/button.h>
#include <wx/spinctrl.h>
#include <wx/slider.h>
////Header Include End

////Dialog Style Start
#undef wxSelect2DVector_STYLE
#define wxSelect2DVector_STYLE wxCAPTION | wxSYSTEM_MENU | wxDIALOG_NO_PARENT | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class wxSelect2DVector : public wxDialog
{
	private:
		DECLARE_EVENT_TABLE();
		
	public:
	    wxSelect2DVector(wxWindow *parent, const wxString &title,float _fInitialXValue,float _fMinX,float _fMaxX,float _fInitialYValue,float _fMinY,float _fMaxY);
		wxSelect2DVector(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("Select 2D Vector"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxSelect2DVector_STYLE);
		virtual ~wxSelect2DVector();

		float fGetXValue();
		float fGetYValue();
		bool bOK();
		void m_poXSliderSlider(wxCommandEvent& event);
		void m_poYSliderSlider(wxCommandEvent& event);
		void m_poSelectYValueUpdated(wxSpinEvent& event );
		void m_poSelectXValueUpdated(wxSpinEvent& event );
		void WxButton1Click(wxCommandEvent& event);
		void WxButton2Click(wxCommandEvent& event);

	private:
		//Do not add custom control declarations between 
		//GUI Control Declaration Start and GUI Control Declaration End.
		//wxDev-C++ will remove them. Add custom code after the block.
		////GUI Control Declaration Start
		wxButton *WxButton1;
		wxButton *WxButton2;
		wxSpinCtrlDbl *m_poSelectYValue;
		wxSlider *m_poYSlider;
		wxSpinCtrlDbl *m_poSelectXValue;
		wxSlider *m_poXSlider;
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
			ID_M_POSELECTYVALUE = 1013,
			ID_M_POYSLIDER = 1008,
			ID_M_POSELECTXVALUE = 1009,
			ID_M_POXSLIDER = 1012,
			////GUI Enum Control ID End
			ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
		};
	
	private:
		void OnClose(wxCloseEvent& event);
		void CreateGUIControls();

		float m_fMinX;
		float m_fMaxX;
		float m_fInitialXValue;

		float m_fMinY;
		float m_fMaxY;
		float m_fInitialYValue;

		bool  m_bOK;
		wxString m_sTitle;		
};

#endif
