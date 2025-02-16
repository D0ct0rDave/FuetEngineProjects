///-----------------------------------------------------------------
///
/// @file      wxSelect2DVector.cpp
/// @author    pepe
/// Created:   04/05/2010 23:24:30
/// @section   DESCRIPTION
///            wxSelect2DVector class implementation
///
///------------------------------------------------------------------

#include "wxSelect2DVector.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// wxSelect2DVector
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxSelect2DVector,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(wxSelect2DVector::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,wxSelect2DVector::WxButton1Click)
	EVT_BUTTON(ID_WXBUTTON2,wxSelect2DVector::WxButton2Click)
	EVT_SPINCTRL(ID_M_POSELECTYVALUE,wxSelect2DVector::m_poSelectYValueUpdated)
	EVT_SPINCTRL(ID_M_POSELECTXVALUE,wxSelect2DVector::m_poSelectXValueUpdated)
	EVT_SLIDER(ID_M_POXSLIDER,wxSelect2DVector::m_poXSliderSlider)
	EVT_SLIDER(ID_M_POYSLIDER,wxSelect2DVector::m_poYSliderSlider)
END_EVENT_TABLE()
////Event Table End

//---------------------------------------------------------------------------
wxSelect2DVector::~wxSelect2DVector()
{
} 
//---------------------------------------------------------------------------
wxSelect2DVector::wxSelect2DVector(wxWindow *parent,const wxString &title,float _fInitialXValue,float _fMinX,float _fMaxX,float _fInitialYValue,float _fMinY,float _fMaxY) :
	wxDialog(parent, 1, title, wxDefaultPosition, wxDefaultSize, wxSelect2DVector_STYLE)
{
	m_fMinX = _fMinX;
	m_fMaxX = _fMaxX;
	m_fMinY = _fMinY;
	m_fMaxY = _fMaxY;
	m_fInitialXValue = _fInitialXValue;
	m_fInitialYValue = _fInitialYValue;
	m_sTitle = title;

	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelect2DVector::wxSelect2DVector(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}
//---------------------------------------------------------------------------
void wxSelect2DVector::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Cancel"), wxPoint(56, 72), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("OK"), wxPoint(160, 72), wxSize(89, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	m_poSelectYValue = new wxSpinCtrlDbl(this, ID_M_POSELECTYVALUE, wxT("0"), wxPoint(213, 37), wxSize(74, 22),wxSP_ARROW_KEYS, 0, 100, 0);

	m_poSelectXValue = new wxSpinCtrlDbl(this, ID_M_POSELECTXVALUE, wxT("0"), wxPoint(213, 7), wxSize(74, 22),wxSP_ARROW_KEYS, 0, 100, 0);

	m_poXSlider = new wxSlider(this, ID_M_POXSLIDER, 0, 0, 10, wxPoint(32, 12), wxSize(177, 22), wxSL_HORIZONTAL | wxSL_SELRANGE , wxDefaultValidator, wxT("m_poXSlider"));
	m_poXSlider->SetRange(0,10);
	m_poXSlider->SetValue(0);

	m_poYSlider = new wxSlider(this, ID_M_POYSLIDER, 0, 0, 10, wxPoint(32, 36), wxSize(177, 22), wxSL_HORIZONTAL | wxSL_SELRANGE , wxDefaultValidator, wxT("m_poYSlider"));
	m_poYSlider->SetRange(0,10);
	m_poYSlider->SetValue(0);

	SetTitle(wxT("Select 2D Vector"));
	SetIcon(wxNullIcon);
	SetSize(8,8,324,155);
	Center();
	
	////GUI Items Creation End
	SetTitle(m_sTitle);

	m_poSelectXValue->SetRange(m_fMinX,m_fMaxX);
	m_poSelectXValue->SetValue(m_fInitialXValue);
	m_poSelectXValue->SetIncrement(0.1f);

	m_poSelectYValue->SetRange(m_fMinY,m_fMaxY);
	m_poSelectYValue->SetValue(m_fInitialYValue);
	m_poSelectYValue->SetIncrement(0.1f);

	m_poXSlider->SetRange(m_fMinX,m_fMaxX);
	m_poXSlider->SetValue(m_fInitialXValue);

	m_poYSlider->SetRange(m_fMinY,m_fMaxY);
	m_poYSlider->SetValue(m_fInitialYValue);

    // In case the dialog is closed through the X button
    m_bOK = false;	
}
//---------------------------------------------------------------------------
void wxSelect2DVector::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
//---------------------------------------------------------------------------
float wxSelect2DVector::fGetXValue()
{
	return(m_poSelectXValue->GetValue());
}
//---------------------------------------------------------------------------
float wxSelect2DVector::fGetYValue()
{
	return(m_poSelectYValue->GetValue());
}
//---------------------------------------------------------------------------
bool wxSelect2DVector::bOK()
{
	return ( m_bOK);
}
//---------------------------------------------------------------------------
void wxSelect2DVector::m_poXSliderSlider(wxCommandEvent& event)
{
	// insert your code here
	m_poSelectXValue->SetValue (  m_poXSlider->GetValue() );
}
//---------------------------------------------------------------------------
void wxSelect2DVector::m_poYSliderSlider(wxCommandEvent& event)
{
	// insert your code here
	m_poSelectYValue->SetValue (  m_poYSlider->GetValue() );
}
//---------------------------------------------------------------------------
void wxSelect2DVector::m_poSelectYValueUpdated(wxSpinEvent& event )
{
	// insert your code here
	 m_poYSlider->SetValue( m_poSelectYValue->GetValue() );
}
//---------------------------------------------------------------------------
void wxSelect2DVector::m_poSelectXValueUpdated(wxSpinEvent& event )
{
	// insert your code here
	m_poXSlider->SetValue( m_poSelectXValue->GetValue() );
}
//---------------------------------------------------------------------------
void wxSelect2DVector::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = false;
	Close();
}
//---------------------------------------------------------------------------
void wxSelect2DVector::WxButton2Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = true;
	Close();
}
//---------------------------------------------------------------------------
