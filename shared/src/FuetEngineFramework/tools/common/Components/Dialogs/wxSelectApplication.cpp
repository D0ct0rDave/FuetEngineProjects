///-----------------------------------------------------------------
///
/// @file      wxSelectApplication.cpp
/// @author    pepe
/// Created:   04/05/2010 23:24:30
/// @section   DESCRIPTION
///            wxSelectApplication class implementation
///
///------------------------------------------------------------------

#include "wxSelectApplication.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/wxSelectApplication_WxBitmapButton1_XPM.xpm"
////Header Include End

//----------------------------------------------------------------------------
// wxSelectApplication
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxSelectApplication,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(wxSelectApplication::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,wxSelectApplication::WxButton1Click)
	EVT_BUTTON(ID_WXBUTTON2,wxSelectApplication::WxButton2Click)
	EVT_BUTTON(ID_WXBITMAPBUTTON1,wxSelectApplication::WxBitmapButton1Click)
END_EVENT_TABLE()
////Event Table End


//---------------------------------------------------------------------------
wxSelectApplication::~wxSelectApplication()
{
} 
//---------------------------------------------------------------------------
wxSelectApplication::wxSelectApplication(wxWindow *parent, const wxString &title,const wxString &application,const wxString &applicationlabel)
: wxDialog(parent, 1, title, wxDefaultPosition, wxDefaultSize, wxSelectApplication_STYLE)
{
	m_sTitle			= title;	
	m_sApplication		= application;
	m_sApplicationLabel = applicationlabel;

	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectApplication::wxSelectApplication(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}
//---------------------------------------------------------------------------
void wxSelectApplication::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxInitAllImageHandlers();   //Initialize graphic format handlers

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Cancel"), wxPoint(248, 76), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("OK"), wxPoint(129, 76), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	wxBitmap WxBitmapButton1_BITMAP (wxSelectApplication_WxBitmapButton1_XPM);
	WxBitmapButton1 = new wxBitmapButton(this, ID_WXBITMAPBUTTON1, WxBitmapButton1_BITMAP, wxPoint(440, 40), wxSize(24, 24), wxBU_AUTODRAW, wxDefaultValidator, wxT("WxBitmapButton1"));

	m_edApplication = new wxTextCtrl(this, ID_M_EDAPPLICATION, wxT("Application"), wxPoint(14, 39), wxSize(418, 24), wxTE_READONLY, wxDefaultValidator, wxT("m_edApplication"));

	m_lbApplicationLabel = new wxStaticText(this, ID_M_POAPPLICATIONLABEL, wxT("Application to open:"), wxPoint(16, 16), wxDefaultSize, 0, wxT("m_lbApplicationLabel"));

	SetTitle(wxT("Select Application"));
	SetIcon(wxNullIcon);
	SetSize(463,341,488,157);
	Center();
	
	////GUI Items Creation End
	SetTitle(m_sTitle);
	m_edApplication->SetValue(m_sApplication);
	m_lbApplicationLabel->SetLabel(m_sApplicationLabel);
	
    // In case the dialog is closed through the X button
    m_bOK = false;	
}
//---------------------------------------------------------------------------
void wxSelectApplication::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
//---------------------------------------------------------------------------
bool wxSelectApplication::bOK()
{
	return (m_bOK);
}
//---------------------------------------------------------------------------
void wxSelectApplication::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = false;
	Close();
}
//---------------------------------------------------------------------------
void wxSelectApplication::WxButton2Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = true;
	Close();
}
//---------------------------------------------------------------------------
wxString wxSelectApplication::sGetApplication()
{
	return(m_edApplication->GetValue());
}
//---------------------------------------------------------------------------
/*
 * WxBitmapButton1Click
 */
void wxSelectApplication::WxBitmapButton1Click(wxCommandEvent& event)
{
	// insert your code here
	wxFileDialog dlgLoadFile(this, wxT("Select file"), wxT(""), wxT(""), wxT("Applications and batch files (*.exe;*.bat)|*.exe;*.bat|Applications (*.exe)|*.exe|Batch files (*.bat)|*.bat"),wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);
    if (dlgLoadFile.ShowModal() == wxID_OK)
	{
		m_edApplication->SetValue( dlgLoadFile.GetPath() );
	}
}

