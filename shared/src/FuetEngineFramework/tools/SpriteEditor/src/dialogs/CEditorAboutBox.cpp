///-----------------------------------------------------------------
///
/// @file      CEditorAboutBox.cpp
/// @author    pepe
/// Created:   13/05/2010 8:46:39
/// @section   DESCRIPTION
///            CEditorAboutBox class implementation
///
///------------------------------------------------------------------

#include "CEditorAboutBox.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/editor_about.xpm"
#include "SpriteEditorVersion.h"
#include "version.h"
////Header Include End

//----------------------------------------------------------------------------
// CEditorAboutBox
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CEditorAboutBox,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(CEditorAboutBox::OnClose)
	EVT_BUTTON(ID_WXBUTTON1,CEditorAboutBox::WxButton1Click)
END_EVENT_TABLE()
////Event Table End

CEditorAboutBox::CEditorAboutBox(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

CEditorAboutBox::~CEditorAboutBox()
{
} 

void CEditorAboutBox::CreateGUIControls()
{
	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	wxInitAllImageHandlers();   //Initialize graphic format handlers

	WxBoxSizer1 = new wxBoxSizer(wxVERTICAL);
	this->SetSizer(WxBoxSizer1);
	this->SetAutoLayout(true);

	WxBoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer2, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	wxBitmap WxStaticBitmap1_BITMAP(about_icon_xpm);
	WxStaticBitmap1 = new wxStaticBitmap(this, ID_WXSTATICBITMAP1, WxStaticBitmap1_BITMAP, wxPoint(5, 24), wxSize(256, 209));
	WxBoxSizer2->Add(WxStaticBitmap1,0,wxALIGN_CENTER | wxALL,5);

	WxBoxSizer3 = new wxBoxSizer(wxVERTICAL);
	WxBoxSizer2->Add(WxBoxSizer3, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);

	wxStaticBox* WxStaticBoxSizer1_StaticBoxObj = new wxStaticBox(this, wxID_ANY, wxT(""));
	WxStaticBoxSizer1 = new wxStaticBoxSizer(WxStaticBoxSizer1_StaticBoxObj, wxHORIZONTAL);
	WxBoxSizer3->Add(WxStaticBoxSizer1, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	WxMemo1 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxPoint(10, 20), wxSize(220, 89), wxTE_READONLY | wxTE_RICH | wxTE_WORDWRAP | wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemo1"));
	WxMemo1->SetMaxLength(0);
	WxMemo1->AppendText(wxT("FuetEngine SpriteEditor\n"));
	wxString wxVersion = wxT(SpriteEditorVersion::FULLVERSION_STRING);
	WxMemo1->AppendText( wxT("version ") + wxVersion + wxT("\n") );

	wxString wxDate =	wxString(wxT(SpriteEditorVersion::DATE)) + wxString(wxT("-")) +
						wxString(wxT(SpriteEditorVersion::MONTH)) + wxString(wxT("-")) +
						wxString(wxT(SpriteEditorVersion::YEAR)) + wxString(" (") + wxString( wxT(__TIME__) ) + wxString(")");

	WxMemo1->AppendText(wxT("build date ") + wxDate + wxT("\n") );

	WxMemo1->AppendText(wxT("by\n"));
	WxMemo1->AppendText(wxT("David Márquez de la Cruz.\n"));
	WxMemo1->SetFocus();
	WxMemo1->SetInsertionPointEnd();
	WxStaticBoxSizer1->Add(WxMemo1,1,wxALIGN_CENTER | wxEXPAND | wxALL,5);

	wxStaticBox* WxStaticBoxSizer2_StaticBoxObj = new wxStaticBox(this, wxID_ANY, wxT("Used libraries:"));
	WxStaticBoxSizer2 = new wxStaticBoxSizer(WxStaticBoxSizer2_StaticBoxObj, wxHORIZONTAL);
	WxBoxSizer3->Add(WxStaticBoxSizer2, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 5);

	WxMemo2 = new wxTextCtrl(this, ID_WXMEMO1, wxEmptyString, wxPoint(10, 20), wxSize(220, 89), wxTE_READONLY | wxTE_RICH | wxTE_AUTO_URL | wxTE_WORDWRAP | wxTE_MULTILINE, wxDefaultValidator, wxT("WxMemo2"));
	WxMemo2->SetMaxLength(0);
	WxMemo2->AppendText(wxT("FuetEngine Accelerated 2D Engine\n"));
	wxVersion = wxT(AutoVersion::FULLVERSION_STRING);
	WxMemo2->AppendText(wxT("version ") + wxVersion + wxT("\n") );
	WxMemo2->AppendText(wxT("\n"));
	WxMemo2->AppendText(wxT("Free Image Library\n"));
	WxMemo2->AppendText(wxT("http://freeimage.sourceforge.net/\n"));
	WxMemo2->AppendText(wxT("\n"));
	WxMemo2->AppendText(wxT("libconfig – C/C++ Configuration File "));
	WxMemo2->AppendText(wxT("Library\n"));
	WxMemo2->AppendText(wxT("http://www.hyperrealm.com/libconfig/\n"));
	WxMemo2->AppendText(wxT("\n"));
	WxMemo2->SetFocus();
	WxMemo2->SetInsertionPointEnd();
	WxStaticBoxSizer2->Add(WxMemo2,1,wxALIGN_CENTER | wxEXPAND | wxALL,5);

	WxBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	WxBoxSizer1->Add(WxBoxSizer4, 0, wxALIGN_CENTER | wxALL, 0);

	WxButton1 = new wxButton(this, ID_WXBUTTON1, wxT("Close"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, wxT("WxButton1"));
	WxBoxSizer4->Add(WxButton1,0,wxALIGN_CENTER | wxALL,5);

	SetTitle(wxT("About ..."));
	SetIcon(wxNullIcon);
	
	GetSizer()->Layout();
	GetSizer()->Fit(this);
	GetSizer()->SetSizeHints(this);
	Center();
	
	////GUI Items Creation End
	WxMemo2->ShowPosition(0);
}

void CEditorAboutBox::OnClose(wxCloseEvent& /*event*/)
{
	Destroy();
}

/*
 * WxButton1Click
 */
void CEditorAboutBox::WxButton1Click(wxCommandEvent& event)
{
	// insert your code here
	Close();
}
