///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CSpecialCharGUIBase.h"

///////////////////////////////////////////////////////////////////////////

CSpecialCharGUIBase::CSpecialCharGUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( this, wxID_ANY, wxT("Char ID"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetMinSize( wxSize( 145,-1 ) );
	
	bSizer29->Add( m_staticText12, 0, wxALL, 5 );
	
	wxArrayString m_cbCharIDChoices;
	m_cbCharID = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cbCharIDChoices, 0 );
	m_cbCharID->SetSelection( 0 );
	bSizer29->Add( m_cbCharID, 0, wxALL, 0 );
	
	bSizer19->Add( bSizer29, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer18->Add( bSizer19, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer20->SetMinSize( wxSize( -1,160 ) ); 
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText141 = new wxStaticText( this, wxID_ANY, wxT("Space Reservation"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText141->Wrap( -1 );
	m_staticText141->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer13->Add( m_staticText141, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer( 5, 3, 0, 0 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText1411 = new wxStaticText( this, wxID_ANY, wxT("Top:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText1411->Wrap( -1 );
	m_staticText1411->SetMinSize( wxSize( 40,-1 ) );
	
	gSizer1->Add( m_staticText1411, 1, wxALIGN_BOTTOM|wxALL, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sbTop = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	gSizer1->Add( m_sbTop, 0, wxALL, 0 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText14112 = new wxStaticText( this, wxID_ANY, wxT("Left:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText14112->Wrap( -1 );
	gSizer1->Add( m_staticText14112, 0, wxALIGN_BOTTOM|wxALL, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText14113 = new wxStaticText( this, wxID_ANY, wxT("Right"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText14113->Wrap( -1 );
	gSizer1->Add( m_staticText14113, 0, wxALIGN_BOTTOM|wxALL, 5 );
	
	m_sbLeft = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	gSizer1->Add( m_sbLeft, 0, wxALL, 0 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sbRight = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	gSizer1->Add( m_sbRight, 0, wxALL, 0 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText14111 = new wxStaticText( this, wxID_ANY, wxT("Bottom:"), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT );
	m_staticText14111->Wrap( -1 );
	m_staticText14111->SetMinSize( wxSize( 40,-1 ) );
	
	gSizer1->Add( m_staticText14111, 0, wxALIGN_BOTTOM|wxALL, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	gSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_sbBottom = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 0, 1000, 0 );
	gSizer1->Add( m_sbBottom, 0, wxALL, 0 );
	
	bSizer13->Add( gSizer1, 0, 0, 5 );
	
	bSizer20->Add( bSizer13, 1, 0, 5 );
	
	bSizer18->Add( bSizer20, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer271;
	bSizer271 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText121 = new wxStaticText( this, wxID_ANY, wxT("Kerning Class Equivalence"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_staticText121->Wrap( -1 );
	bSizer271->Add( m_staticText121, 0, wxALL, 5 );
	
	wxArrayString m_cbKCEqChoices;
	m_cbKCEq = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cbKCEqChoices, 0 );
	m_cbKCEq->SetSelection( 0 );
	bSizer271->Add( m_cbKCEq, 0, wxALL, 0 );
	
	bSizer191->Add( bSizer271, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer18->Add( bSizer191, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer281;
	bSizer281 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText16 = new wxStaticText( this, wxID_ANY, wxT("Select editor color:"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	m_staticText16->Wrap( -1 );
	bSizer281->Add( m_staticText16, 0, wxALL, 5 );
	
	m_oCP = new wxColourPickerCtrl( this, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer281->Add( m_oCP, 0, wxALL, 0 );
	
	bSizer26->Add( bSizer281, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	bSizer18->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxHORIZONTAL );
	
	m_button1 = new wxButton( this, wxID_ANY, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_button1, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxVERTICAL );
	
	bSizer27->Add( bSizer28, 1, wxEXPAND, 5 );
	
	m_button2 = new wxButton( this, wxID_ANY, wxT("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer27->Add( m_button2, 1, wxALL|wxALIGN_BOTTOM|wxEXPAND, 5 );
	
	bSizer18->Add( bSizer27, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSizer18 );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CSpecialCharGUIBase::CSpecialCharGUIBaseOnClose ) );
	m_button1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpecialCharGUIBase::m_button1OnButtonClick ), NULL, this );
	m_button2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpecialCharGUIBase::m_button2OnButtonClick ), NULL, this );
}

CSpecialCharGUIBase::~CSpecialCharGUIBase()
{
}
