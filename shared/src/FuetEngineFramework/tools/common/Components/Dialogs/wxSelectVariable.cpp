///-----------------------------------------------------------------
///
/// @file      wxSelectVariable.cpp
/// @author    pepe
/// Created:   04/05/2010 23:24:30
/// @section   DESCRIPTION
///            wxSelectVariable class implementation
///
///------------------------------------------------------------------

#include "wxSelectVariable.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// wxSelectVariable
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxSelectVariable,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(wxSelectVariable::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,wxSelectVariable::WxButton1Click)
	EVT_BUTTON(ID_WXBUTTON2,wxSelectVariable::WxButton2Click)
END_EVENT_TABLE()
////Event Table End

//---------------------------------------------------------------------------
wxSelectVariable::~wxSelectVariable()
{
} 
//---------------------------------------------------------------------------
wxSelectVariable::wxSelectVariable(wxWindow *parent, const wxString &title,const wxString &variable,const wxString &value,const wxString &variablelabel,const wxString &valuelabel) :
	wxDialog(parent, 1, title, wxDefaultPosition, wxDefaultSize, wxSelectVariable_STYLE)
{
	m_sTitle		= title;
	m_sVariable		= variable;
	m_sValue		= value;
	m_sVariableLabel = variablelabel;
	m_sValueLabel	= valuelabel;

	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectVariable::wxSelectVariable(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}
//---------------------------------------------------------------------------
void wxSelectVariable::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Cancel"), wxPoint(169, 130), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("OK"), wxPoint(47, 130), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	m_poValue = new wxTextCtrl(this, ID_M_POVALUE, wxT("m_poValue"), wxPoint(22, 87), wxSize(274, 19), 0, wxDefaultValidator, wxT("m_poValue"));

	m_poValueLabel = new wxStaticText(this, ID_M_POVALUELABEL, wxT("Value"), wxPoint(24, 64), wxDefaultSize, 0, wxT("m_poValueLabel"));

	m_poVariable = new wxTextCtrl(this, ID_M_POVARIABLE, wxT("m_poVariable"), wxPoint(22, 39), wxSize(274, 19), 0, wxDefaultValidator, wxT("m_poVariable"));

	m_poVariableLabel = new wxStaticText(this, ID_M_POVARIABLELABEL, wxT("Name"), wxPoint(24, 16), wxDefaultSize, 0, wxT("m_poVariableLabel"));

	SetTitle(wxT("Edit Variable"));
	SetIcon(wxNullIcon);
	SetSize(622,345,325,216);
	Center();
	
	////GUI Items Creation End
	SetTitle(m_sTitle);
	m_poVariableLabel->SetLabel(m_sVariableLabel);
	m_poValueLabel->SetLabel(m_sValueLabel);
	m_poValue->SetLabel(m_sValue);
	m_poVariable->SetLabel(m_sVariable);

    // In case the dialog is closed through the X button
    m_bOK = false;	
}
//---------------------------------------------------------------------------
void wxSelectVariable::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
//---------------------------------------------------------------------------
bool wxSelectVariable::bOK()
{
	return ( m_bOK);
}
//---------------------------------------------------------------------------
void wxSelectVariable::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = false;
	Close();
}
//---------------------------------------------------------------------------
void wxSelectVariable::WxButton2Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = true;
	Close();
}
//---------------------------------------------------------------------------
wxString wxSelectVariable::sGetVariable()
{
	return(m_poVariable->GetLabel());
}

wxString wxSelectVariable::sGetValue()
{
	return(m_poValue->GetLabel());
}
//---------------------------------------------------------------------------
