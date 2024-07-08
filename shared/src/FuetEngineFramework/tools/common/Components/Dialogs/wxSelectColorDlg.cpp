///-----------------------------------------------------------------
///
/// @file      wxSelectColorDlg.cpp
/// @author    pepe
/// Created:   04/05/2010 19:10:51
/// @section   DESCRIPTION
///            wxSelectColorDlg class implementation
///
///------------------------------------------------------------------

#include "wxSelectColorDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End

//----------------------------------------------------------------------------
// wxSelectColorDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(wxSelectColorDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(wxSelectColorDlg::OnClose)
	EVT_BUTTON(ID_WXBUTTON2,wxSelectColorDlg::WxButton2Click)
	EVT_BUTTON(ID_WXBUTTON1,wxSelectColorDlg::WxButton1Click)
	EVT_SLIDER(ID_SLDALPHA,wxSelectColorDlg::sldAlphaSlider)
	EVT_COLOURPICKER_CHANGED(ID_COLPLAYERCOLOR,wxSelectColorDlg::colpLayerColorColourChanged)
END_EVENT_TABLE()
////Event Table End

//---------------------------------------------------------------------------
wxSelectColorDlg::wxSelectColorDlg(wxWindow *parent, const wxString &title,unsigned int _uiInitialValue) :
	wxDialog(parent, 1, title, wxDefaultPosition, wxDefaultSize, wxSelectColorDlg_STYLE)
{
	m_uiInitialValue = _uiInitialValue;
	m_sTitle = title;
	
	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectColorDlg::wxSelectColorDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}
//---------------------------------------------------------------------------
wxSelectColorDlg::~wxSelectColorDlg()
{
} 
//---------------------------------------------------------------------------
void wxSelectColorDlg::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	WxButton2 = new wxButton(this, ID_WXBUTTON2, wxT("Cancel"), wxPoint(227, 80), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton2"));

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Ok"), wxPoint(15, 80), wxSize(97, 33), 0, wxDefaultValidator, wxT("WxButton1"));

	sldAlpha = new wxSlider(this, ID_SLDALPHA, 0, 0, 255, wxPoint(131, 18), wxSize(201, 40), wxSL_HORIZONTAL | wxSL_AUTOTICKS | wxSL_LABELS | wxSL_BOTH | wxSL_SELRANGE , wxDefaultValidator, wxT("sldAlpha"));
	sldAlpha->SetRange(0,255);
	sldAlpha->SetValue(0);

	WxStaticText8 = new wxStaticText(this, ID_WXSTATICTEXT8, wxT("Alpha:"), wxPoint(97, 40), wxDefaultSize, 0, wxT("WxStaticText8"));

	colpLayerColor = new wxColourPickerCtrl(this, ID_COLPLAYERCOLOR, *wxBLACK, wxPoint(65, 33), wxSize(24, 24), wxCLRP_DEFAULT_STYLE, wxDefaultValidator, wxT("colpLayerColor"));
	colpLayerColor->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("MS Shell Dlg")));

	WxStaticText7 = new wxStaticText(this, ID_WXSTATICTEXT7, wxT("Color:"), wxPoint(17, 40), wxDefaultSize, 0, wxT("WxStaticText7"));

	SetTitle(wxT("Select Color Dialog"));
	SetIcon(wxNullIcon);
	SetSize(589,343,352,159);
	Center();
	
	////GUI Items Creation End
	
	SetTitle(m_sTitle);
    sldAlpha->SetValue( m_uiInitialValue >> 24);
    colpLayerColor->SetColour( wxColour(m_uiInitialValue & 0x000000ff,(m_uiInitialValue >> 8)& 0x000000ff,(m_uiInitialValue >> 16)& 0x000000ff) );

    // In case the dialog is closed through the X button
    m_bOK = false;
}
///-----------------------------------------------------------------
void wxSelectColorDlg::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}
///-----------------------------------------------------------------
/*
 * WxButton1Click
 */
void wxSelectColorDlg::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = true;
	Close();	
}
///-----------------------------------------------------------------
/*
 * WxButton2Click
 */
void wxSelectColorDlg::WxButton2Click(wxCommandEvent& event)
{
	// insert your code here
	m_bOK = false;
	Close();	
}
///-----------------------------------------------------------------

/*
 * sldAlphaSlider
 */
void wxSelectColorDlg::sldAlphaSlider(wxCommandEvent& event)
{
	// insert your code here
}
///-----------------------------------------------------------------
/*
 * colpLayerColorColourChanged
 */
void wxSelectColorDlg::colpLayerColorColourChanged(wxColourPickerEvent& event)
{
	// insert your code here
}
///-----------------------------------------------------------------
unsigned int wxSelectColorDlg::uiGetValue()
{
    unsigned char ucAlpha = sldAlpha->GetValue();
    wxColour oCol = colpLayerColor->GetColour();
    
    unsigned int uiColor = (ucAlpha << 24) | (oCol.Blue() << 16) | (oCol.Green() << 8) | oCol.Red();
    return(uiColor);
}
///-----------------------------------------------------------------
bool wxSelectColorDlg::bOK()
{
	// insert your code here
	return ( m_bOK);
}
///-----------------------------------------------------------------
bool bSelectColor(const wxString& _sTitle,unsigned int _uiInitialValue,unsigned int* _uiOutput)
{
	wxSelectColorDlg* dlg = new wxSelectColorDlg(NULL, _sTitle, _uiInitialValue);
	dlg->ShowModal();

	if (dlg->bOK())
	{
	    *_uiOutput = dlg->uiGetValue();
	    delete dlg;
	    return(true);
	}

	return(false);
}
///-----------------------------------------------------------------
