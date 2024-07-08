///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CFontEditorGUIBase.h"

#include "../../common/Images/AddLayer.xpm"
#include "../../common/Images/delete_icon.xpm"
#include "../../common/Images/rename_layer.xpm"
#include "Images/CFontEditorGUI_WxBitmapButton1_XPM.xpm"
#include "Images/CFontEditorGUI_WxBitmapButton2_XPM.xpm"

///////////////////////////////////////////////////////////////////////////

CFontEditorGUIBase::CFontEditorGUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,920 ), wxSize( -1,-1 ) );
	this->SetBackgroundColour( wxColour( 216, 175, 139 ) );
	
	m_statusBar1 = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText3 = new wxStaticText( this, wxID_ANY, wxT("Choose font to display:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer2->Add( m_staticText3, 0, wxALL, 5 );
	
	m_gdFontList = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxSize( 400,200 ), wxWANTS_CHARS );
	
	// Grid
	m_gdFontList->CreateGrid( 5, 3 );
	m_gdFontList->EnableEditing( false );
	m_gdFontList->EnableGridLines( false );
	m_gdFontList->EnableDragGridSize( false );
	m_gdFontList->SetMargins( 0, 0 );
	
	// Columns
	m_gdFontList->EnableDragColMove( false );
	m_gdFontList->EnableDragColSize( false );
	m_gdFontList->SetColLabelSize( 30 );
	m_gdFontList->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gdFontList->EnableDragRowSize( false );
	m_gdFontList->SetRowLabelSize( 0 );
	m_gdFontList->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gdFontList->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer2->Add( m_gdFontList, 3, wxALL|wxEXPAND, 5 );
	
	m_auinotebook1 = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_TAB_MOVE|wxAUI_NB_TAB_SPLIT );
	m_panel1 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1 = new wxStaticText( m_panel1, wxID_ANY, wxT("Write the characters to export:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	m_staticText1->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer5->Add( m_staticText1, 0, wxALL, 5 );
	
	mem_Chars = new wxTextCtrl( m_panel1, wxID_ANY, wxT("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"), wxDefaultPosition, wxSize( -1,-1 ), wxTE_CHARWRAP|wxTE_MULTILINE|wxTE_WORDWRAP );
	mem_Chars->SetMaxLength( 0 ); 
	mem_Chars->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 90, false, wxEmptyString ) );
	mem_Chars->SetForegroundColour( wxColour( 255, 255, 255 ) );
	mem_Chars->SetBackgroundColour( wxColour( 0, 0, 0 ) );
	
	bSizer5->Add( mem_Chars, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer21->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Font Properties:") ), wxVERTICAL );
	
	wxBoxSizer* bSizer341;
	bSizer341 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13111 = new wxStaticText( m_panel1, wxID_ANY, wxT("Typeface Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13111->Wrap( -1 );
	m_staticText13111->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer341->Add( m_staticText13111, 0, wxALL|wxEXPAND, 5 );
	
	m_oTypefaceColor = new wxColourPickerCtrl( m_panel1, wxID_ANY, wxColour( 255, 255, 255 ), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer341->Add( m_oTypefaceColor, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer341, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText13 = new wxStaticText( m_panel1, wxID_ANY, wxT("Font Style:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	m_staticText13->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer16->Add( m_staticText13, 0, wxALL, 5 );
	
	wxString m_chFontStyleChoices[] = { wxT("normal"), wxT("bold"), wxT("italic"), wxT("bold & italic") };
	int m_chFontStyleNChoices = sizeof( m_chFontStyleChoices ) / sizeof( wxString );
	m_chFontStyle = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chFontStyleNChoices, m_chFontStyleChoices, 0 );
	m_chFontStyle->SetSelection( 0 );
	bSizer16->Add( m_chFontStyle, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer16, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText5 = new wxStaticText( m_panel1, wxID_ANY, wxT("Decrease Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	m_staticText5->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer11->Add( m_staticText5, 0, wxALL, 5 );
	
	m_bpButton1 = new wxBitmapButton( m_panel1, wxID_ANY, wxBitmap( CFontEditorGUI_WxBitmapButton1_XPM_xpm ), wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW );
	m_bpButton1->SetMinSize( wxSize( 80,-1 ) );
	
	bSizer11->Add( m_bpButton1, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer10->Add( bSizer11, 1, 0, 5 );
	
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );
	
	m_lbFontSize = new wxStaticText( m_panel1, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbFontSize->Wrap( -1 );
	m_lbFontSize->SetFont( wxFont( 15, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer121->Add( m_lbFontSize, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 30 );
	
	m_staticText12 = new wxStaticText( m_panel1, wxID_ANY, wxT("(Dbl Click)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	m_staticText12->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer121->Add( m_staticText12, 0, wxALL, 5 );
	
	
	bSizer10->Add( bSizer121, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( m_panel1, wxID_ANY, wxT("Increase Size"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	m_staticText6->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer12->Add( m_staticText6, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_bpButton2 = new wxBitmapButton( m_panel1, wxID_ANY, wxBitmap( CFontEditorGUI_WxBitmapButton2_XPM_xpm ), wxDefaultPosition, wxSize( -1,-1 ), wxBU_AUTODRAW );
	m_bpButton2->SetMinSize( wxSize( 80,-1 ) );
	
	bSizer12->Add( m_bpButton2, 0, wxALIGN_RIGHT|wxALL|wxEXPAND, 5 );
	
	
	bSizer10->Add( bSizer12, 1, wxEXPAND, 5 );
	
	
	sbSizer2->Add( bSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	wxBoxSizer* bSizer162;
	bSizer162 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText131 = new wxStaticText( m_panel1, wxID_ANY, wxT("Antialias:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	m_staticText131->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer162->Add( m_staticText131, 0, wxALL, 5 );
	
	wxString m_chAntialiasTypeChoices[] = { wxT("none"), wxT("light"), wxT("strong"), wxT("super") };
	int m_chAntialiasTypeNChoices = sizeof( m_chAntialiasTypeChoices ) / sizeof( wxString );
	m_chAntialiasType = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_chAntialiasTypeNChoices, m_chAntialiasTypeChoices, 0 );
	m_chAntialiasType->SetSelection( 0 );
	bSizer162->Add( m_chAntialiasType, 0, wxALL, 5 );
	
	
	sbSizer2->Add( bSizer162, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, wxT("Stroke") ), wxVERTICAL );
	
	wxBoxSizer* bSizer34;
	bSizer34 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText1311 = new wxStaticText( m_panel1, wxID_ANY, wxT("Stroke Color:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1311->Wrap( -1 );
	m_staticText1311->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer34->Add( m_staticText1311, 0, wxALL|wxEXPAND, 5 );
	
	m_oStrokeColor = new wxColourPickerCtrl( m_panel1, wxID_ANY, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer34->Add( m_oStrokeColor, 0, wxALL, 5 );
	
	
	sbSizer3->Add( bSizer34, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer33;
	bSizer33 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText61 = new wxStaticText( m_panel1, wxID_ANY, wxT("Decrease Stroke"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	m_staticText61->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer122->Add( m_staticText61, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	m_btLessStroke = new wxButton( m_panel1, wxID_ANY, wxT("-"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer122->Add( m_btLessStroke, 0, wxALL, 5 );
	
	
	bSizer33->Add( bSizer122, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1211;
	bSizer1211 = new wxBoxSizer( wxVERTICAL );
	
	m_lbStrokeSize = new wxStaticText( m_panel1, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbStrokeSize->Wrap( -1 );
	m_lbStrokeSize->SetFont( wxFont( 15, 70, 90, 90, false, wxEmptyString ) );
	
	bSizer1211->Add( m_lbStrokeSize, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP, 30 );
	
	m_staticText121 = new wxStaticText( m_panel1, wxID_ANY, wxT("(Dbl Click)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	m_staticText121->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer1211->Add( m_staticText121, 0, wxALL, 5 );
	
	
	bSizer33->Add( bSizer1211, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer111;
	bSizer111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText51 = new wxStaticText( m_panel1, wxID_ANY, wxT("Increase Stroke"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText51->Wrap( -1 );
	m_staticText51->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer111->Add( m_staticText51, 0, wxALL, 5 );
	
	m_btMoreStroke = new wxButton( m_panel1, wxID_ANY, wxT("+"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer111->Add( m_btMoreStroke, 0, wxALL, 5 );
	
	
	bSizer33->Add( bSizer111, 1, 0, 5 );
	
	
	sbSizer3->Add( bSizer33, 1, wxEXPAND, 5 );
	
	
	sbSizer2->Add( sbSizer3, 1, wxEXPAND, 5 );
	
	
	bSizer21->Add( sbSizer2, 0, wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( bSizer21 );
	m_panel1->Layout();
	bSizer21->Fit( m_panel1 );
	m_auinotebook1->AddPage( m_panel1, wxT("Export String"), true, wxNullBitmap );
	m_panel3 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxVERTICAL );
	
	m_gdSpecialChars = new wxGrid( m_panel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	
	// Grid
	m_gdSpecialChars->CreateGrid( 1, 7 );
	m_gdSpecialChars->EnableEditing( true );
	m_gdSpecialChars->EnableGridLines( true );
	m_gdSpecialChars->EnableDragGridSize( false );
	m_gdSpecialChars->SetMargins( 0, 0 );
	
	// Columns
	m_gdSpecialChars->SetColSize( 0, 57 );
	m_gdSpecialChars->SetColSize( 1, 49 );
	m_gdSpecialChars->SetColSize( 2, 55 );
	m_gdSpecialChars->SetColSize( 3, 48 );
	m_gdSpecialChars->SetColSize( 4, 64 );
	m_gdSpecialChars->SetColSize( 5, 101 );
	m_gdSpecialChars->SetColSize( 6, 45 );
	m_gdSpecialChars->EnableDragColMove( false );
	m_gdSpecialChars->EnableDragColSize( true );
	m_gdSpecialChars->SetColLabelSize( 30 );
	m_gdSpecialChars->SetColLabelValue( 0, wxT("Char ID") );
	m_gdSpecialChars->SetColLabelValue( 1, wxT("R Left") );
	m_gdSpecialChars->SetColLabelValue( 2, wxT("R Right") );
	m_gdSpecialChars->SetColLabelValue( 3, wxT("R Top") );
	m_gdSpecialChars->SetColLabelValue( 4, wxT("R Bottom") );
	m_gdSpecialChars->SetColLabelValue( 5, wxT("Kerning Class Eq") );
	m_gdSpecialChars->SetColLabelValue( 6, wxT("Color") );
	m_gdSpecialChars->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gdSpecialChars->EnableDragRowSize( false );
	m_gdSpecialChars->SetRowLabelSize( 40 );
	m_gdSpecialChars->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gdSpecialChars->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_gdSpecialChars->SetMinSize( wxSize( 500,-1 ) );
	
	bSizer161->Add( m_gdSpecialChars, 1, wxALL, 5 );
	
	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btNewSC = new wxBitmapButton( m_panel3, wxID_ANY, wxBitmap( AddLayer_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer17->Add( m_btNewSC, 0, wxALL, 5 );
	
	m_btEdiSC = new wxBitmapButton( m_panel3, wxID_ANY, wxBitmap( rename_layer_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btEdiSC->SetMinSize( wxSize( 28,28 ) );
	
	bSizer17->Add( m_btEdiSC, 0, wxALL, 5 );
	
	m_btDeleteSC = new wxBitmapButton( m_panel3, wxID_ANY, wxBitmap( delete_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	bSizer17->Add( m_btDeleteSC, 0, wxALL, 5 );
	
	
	bSizer161->Add( bSizer17, 0, wxEXPAND, 5 );
	
	
	m_panel3->SetSizer( bSizer161 );
	m_panel3->Layout();
	bSizer161->Fit( m_panel3 );
	m_auinotebook1->AddPage( m_panel3, wxT("Special Characters"), false, wxNullBitmap );
	m_panel2 = new wxPanel( m_auinotebook1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, wxT("Export Info:") ), wxVERTICAL );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText2 = new wxStaticText( m_panel2, wxID_ANY, wxT("Set Dimensions Constraints:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	m_staticText2->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer9->Add( m_staticText2, 0, wxALIGN_CENTER|wxALL, 5 );
	
	wxString ch_ConstraintsChoices[] = { wxT("No constraints"), wxT("Multiple of 4"), wxT("Multiple of 8"), wxT("Power of 2") };
	int ch_ConstraintsNChoices = sizeof( ch_ConstraintsChoices ) / sizeof( wxString );
	ch_Constraints = new wxChoice( m_panel2, wxID_ANY, wxDefaultPosition, wxSize( 150,-1 ), ch_ConstraintsNChoices, ch_ConstraintsChoices, 0 );
	ch_Constraints->SetSelection( 0 );
	bSizer9->Add( ch_Constraints, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	
	sbSizer1->Add( bSizer9, 0, wxTOP, 15 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( m_panel2, wxID_ANY, wxT("Border Pixels:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	m_staticText7->SetForegroundColour( wxColour( 0, 0, 128 ) );
	m_staticText7->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer13->Add( m_staticText7, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldBorderPixels = new wxSlider( m_panel2, wxID_M_BORDERPIX, 0, 0, 25, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer13->Add( m_sldBorderPixels, 1, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer13, 0, wxEXPAND|wxTOP, 15 );
	
	wxBoxSizer* bSizer131;
	bSizer131 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText71 = new wxStaticText( m_panel2, wxID_ANY, wxT("Forced Tracking:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	m_staticText71->SetForegroundColour( wxColour( 0, 0, 128 ) );
	m_staticText71->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer131->Add( m_staticText71, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldForcedTracking = new wxSlider( m_panel2, wxID_M_FORCEDTRACKING, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer131->Add( m_sldForcedTracking, 1, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer131, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText711 = new wxStaticText( m_panel2, wxID_ANY, wxT("Point Scale:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText711->Wrap( -1 );
	m_staticText711->SetForegroundColour( wxColour( 0, 0, 128 ) );
	m_staticText711->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer1311->Add( m_staticText711, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldPointScale = new wxSlider( m_panel2, wxID_M_POINTSCALE, 100, 0, 1000, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer1311->Add( m_sldPointScale, 1, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer1311, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer13111;
	bSizer13111 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7111 = new wxStaticText( m_panel2, wxID_ANY, wxT("Y Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7111->Wrap( -1 );
	m_staticText7111->SetForegroundColour( wxColour( 0, 0, 128 ) );
	m_staticText7111->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer13111->Add( m_staticText7111, 0, wxALIGN_BOTTOM|wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldYOffset = new wxSlider( m_panel2, wxID_M_YOFFSET, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_BOTH|wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer13111->Add( m_sldYOffset, 1, wxALL, 5 );
	
	
	sbSizer1->Add( bSizer13111, 0, wxEXPAND, 5 );
	
	m_cbUTF8 = new wxCheckBox( m_panel2, wxID_ANY, wxT("UTF 8 Font"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_cbUTF8, 1, wxALL, 5 );
	
	
	bSizer6->Add( sbSizer1, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( bSizer6 );
	m_panel2->Layout();
	bSizer6->Fit( m_panel2 );
	m_auinotebook1->AddPage( m_panel2, wxT("Export Options"), false, wxNullBitmap );
	
	bSizer2->Add( m_auinotebook1, 4, wxALL|wxEXPAND, 5 );
	
	
	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer1 );
	this->Layout();
	m_menubar1 = new wxMenuBar( 0 );
	menu = new wxMenu();
	wxMenuItem* menuitem1;
	menuitem1 = new wxMenuItem( menu, wxID_OPEN_FONT, wxString( wxT("&Open Font") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem1 );
	
	wxMenuItem* menuitem2;
	menuitem2 = new wxMenuItem( menu, wxID_SAVE_FONT, wxString( wxT("&Save Font") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem2 );
	
	wxMenuItem* menuitem22;
	menuitem22 = new wxMenuItem( menu, wxID_SAVE_FONT2, wxString( wxT("Save Font Experimental") ) , wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem22 );
	
	menu->AppendSeparator();
	
	wxMenuItem* menuitem21;
	menuitem21 = new wxMenuItem( menu, wxID_SAVE_FONT_DEF, wxString( wxT("&Save Font Definition only") ) + wxT('\t') + wxT("Ctrl+D"), wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem21 );
	
	wxMenuItem* menuitem211;
	menuitem211 = new wxMenuItem( menu, wxID_SAVE_FONT_TEX, wxString( wxT("&Save Font texture only") ) + wxT('\t') + wxT("Ctrl+T"), wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem211 );
	
	menu->AppendSeparator();
	
	wxMenuItem* menuitem;
	menuitem = new wxMenuItem( menu, wxID_QUIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	menu->Append( menuitem );
	
	m_menubar1->Append( menu, wxT("File") ); 
	
	this->SetMenuBar( m_menubar1 );
	
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_gdFontList->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CFontEditorGUIBase::m_gdFontListOnGridSelectCell ), NULL, this );
	m_gdFontList->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CFontEditorGUIBase::m_gdFontListOnKeyDown ), NULL, this );
	m_chFontStyle->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::m_chFontStyleOnChoice ), NULL, this );
	m_bpButton1->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_bpButton1OnButtonClick ), NULL, this );
	m_lbFontSize->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( CFontEditorGUIBase::m_lbFontSizeOnLeftDClick ), NULL, this );
	m_bpButton2->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_bpButton2OnButtonClick ), NULL, this );
	m_chAntialiasType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::m_chFontStyleOnChoice ), NULL, this );
	m_btLessStroke->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_btLessStrokeOnButtonClick ), NULL, this );
	m_lbStrokeSize->Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( CFontEditorGUIBase::m_lbStrokeSizeOnLeftDClick ), NULL, this );
	m_btMoreStroke->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_btMoreStrokeOnButtonClick ), NULL, this );
	m_gdSpecialChars->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( CFontEditorGUIBase::m_gdSpecialCharsOnGridCmdCellLeftDClick ), NULL, this );
	m_gdSpecialChars->Connect( wxEVT_GRID_SELECT_CELL, wxGridEventHandler( CFontEditorGUIBase::m_gdSpecialCharsOnGridSelectCell ), NULL, this );
	m_btNewSC->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_btNewSCOnButtonClick ), NULL, this );
	m_btEdiSC->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_btEdiSCOnButtonClick ), NULL, this );
	m_btDeleteSC->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CFontEditorGUIBase::m_btDeleteSCOnButtonClick ), NULL, this );
	ch_Constraints->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::ch_ConstraintsOnChoice ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldBorderPixels->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CFontEditorGUIBase::m_sldBorderPixelsOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldForcedTracking->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CFontEditorGUIBase::m_sldForcedTrackingOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldPointScale->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CFontEditorGUIBase::m_sldPointScaleOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	m_sldYOffset->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CFontEditorGUIBase::m_sldYOffsetOnCommandScroll ), NULL, this );
	this->Connect( menuitem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitem1OnMenuSelection ) );
	this->Connect( menuitem2->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitem2OnMenuSelection ) );
	this->Connect( menuitem22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitem22OnMenuSelection ) );
	this->Connect( menuitem21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitem21OnMenuSelection ) );
	this->Connect( menuitem211->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitem211OnMenuSelection ) );
	this->Connect( menuitem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFontEditorGUIBase::menuitemOnMenuSelection ) );
}

CFontEditorGUIBase::~CFontEditorGUIBase()
{
}
