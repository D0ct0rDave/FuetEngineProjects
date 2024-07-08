//---------------------------------------------------------------------------
//
// Name:        wxSelectFloatDlg.cpp
// Author:      David Márquez
// Created:     26/01/2010 14:03:16
// Description: wxSelectFloatDlg class implementation
//
//---------------------------------------------------------------------------

// #include <wx/wx_pch.h>  //added to precompile
#include "wxSelectFloatDlg.h"
//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// wxSelectFloatDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxSelectFloatDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(wxSelectFloatDlg::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,wxSelectFloatDlg::WxButton1Click)
	EVT_BUTTON(ID_WXBUTTON2,wxSelectFloatDlg::WxButton2Click)
	EVT_SPINCTRL(ID_M_POSELECTVALUE,wxSelectFloatDlg::m_poSelectValueUpdated)
	EVT_SLIDER(ID_M_POSLIDER,wxSelectFloatDlg::WxSlider1Slider)
END_EVENT_TABLE()
////Event Table End
//---------------------------------------------------------------------------
wxSelectFloatDlg::wxSelectFloatDlg(wxWindow *parent, const wxString &title,float _fInitialValue,float _fMin,float _fMax) :
	wxDialog(parent, 1, title, wxDefaultPosition, wxDefaultSize, wxSelectFloatDlg_STYLE)
{
	m_fMin = _fMin;
	m_fMax = _fMax;
	m_fInitialValue = _fInitialValue;
	m_sTitle = title;
	
	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectFloatDlg::wxSelectFloatDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectFloatDlg::~wxSelectFloatDlg()
{
} 
//---------------------------------------------------------------------------
void wxSelectFloatDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Cancel"), wxPoint(121, 50), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("OK"), wxPoint(11, 50), wxSize(89, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	m_poSelectValue = new wxSpinCtrlDbl(this, ID_M_POSELECTVALUE, wxT("0"), wxPoint(152, 12), wxSize(57, 22),wxSP_ARROW_KEYS, 0, 100, 0);

	m_poSlider = new wxSlider(this, ID_M_POSLIDER, 0, 0, 10, wxPoint(16, 12), wxSize(129, 22), wxSL_HORIZONTAL | wxSL_SELRANGE , wxDefaultValidator, wxT("m_poSlider"));
	m_poSlider->SetRange(0,10);
	m_poSlider->SetValue(0);

	SetTitle(wxT("Select Float Value"));
	SetIcon(wxNullIcon);
	SetSize(655,403,240,129);
	Center();
	
	////GUI Items Creation End

	// wxString wxInitialValue;
	// wxInitialValue.Format(wxT("%.3f"),m_fInitialValue);
	// delete m_poSelectValue;
	// m_poSelectValue = new wxSpinCtrlDbl(*this, ID_M_POSELECTVALUE, wxInitialValue, wxPoint(152, 40), wxSize(57, 22),wxSP_ARROW_KEYS, m_fMin, m_fMax,m_fInitialValue,(m_fMax-m_fMin) / 100.0f);
	
	SetTitle(m_sTitle);

	m_poSelectValue->SetRange(m_fMin,m_fMax);
	m_poSelectValue->SetValue(m_fInitialValue);
	m_poSelectValue->SetIncrement(0.01f);

	m_poSlider->SetRange(m_fMin*100,m_fMax*100);
	m_poSlider->SetValue(m_fInitialValue*100);

    // In case the dialog is closed through the X button
    m_bOK = false;	
}
//---------------------------------------------------------------------------
void wxSelectFloatDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

//---------------------------------------------------------------------------
/*
 * WxButton1Click
 */
void wxSelectFloatDlg::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here	
	m_bOK = false;
	Close();
}
//---------------------------------------------------------------------------
/*
 * WxButton2Click
 */
void wxSelectFloatDlg::WxButton2Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = true;
	Close();
}

//---------------------------------------------------------------------------
/*
 * WxSlider1Slider
 */
void wxSelectFloatDlg::WxSlider1Slider(wxCommandEvent& event)
{
	// insert your code here
	m_poSelectValue->SetValue( (float)m_poSlider->GetValue() / 100.0f );
}
//---------------------------------------------------------------------------
/*
 * m_poSelectValueUpdated
 */
void wxSelectFloatDlg::m_poSelectValueUpdated(wxSpinEvent& event )
{
	// insert your code here
	m_poSlider->SetValue( m_poSelectValue->GetValue()*100 );
}
//---------------------------------------------------------------------------
float wxSelectFloatDlg::fGetValue()
{
	return(m_poSelectValue->GetValue());
}
//---------------------------------------------------------------------------
bool wxSelectFloatDlg::bOK()
{
	return ( m_bOK);
}
//---------------------------------------------------------------------------
bool bSelectFloat(const wxString& _sTitle,float _fInitialValue,float _fMin,float _fMax,float* _pfOutput)
{
	wxSelectFloatDlg* dlg = new wxSelectFloatDlg(NULL, _sTitle, _fInitialValue,_fMin,_fMax);
	dlg->ShowModal();

	if (dlg->bOK())
	{
	    *_pfOutput = dlg->fGetValue();
	    delete dlg;
	    return(true);
	}

	return(false);
}
//---------------------------------------------------------------------------
