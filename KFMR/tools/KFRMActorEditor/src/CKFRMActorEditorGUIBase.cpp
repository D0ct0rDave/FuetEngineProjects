///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "components/wxInputPanel.h"

#include "CKFRMActorEditorGUIBase.h"

#include "Images/CKFRMActorEditorGUI_WxBitmapButton8_XPM.xpm"

///////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE( CKFRMActorEditorGUIBase, wxFrame )
	EVT_CLOSE( CKFRMActorEditorGUIBase::_wxFB_CKFRMActorEditorGUIBaseOnClose )
	EVT_IDLE( CKFRMActorEditorGUIBase::_wxFB_CKFRMActorEditorGUIBaseOnIdle )
	EVT_SCROLL( CKFRMActorEditorGUIBase::_wxFB_sb_KeyFrameOnScroll )
	EVT_CHECKBOX( ID_SHOWBACKGROUND, CKFRMActorEditorGUIBase::_wxFB_cb_ShowBackgroundOnCheckBox )
	EVT_CHECKBOX( ID_SHOWLABELS, CKFRMActorEditorGUIBase::_wxFB_cb_ShowLabelsOnCheckBox )
	EVT_BUTTON( ID_RELOADMATERIALS, CKFRMActorEditorGUIBase::_wxFB_ReloadMaterialsClick )
	EVT_BUTTON( ID_PLAYANIM, CKFRMActorEditorGUIBase::_wxFB_OnPlayAnimationClick )
	EVT_SLIDER( ID_ANIMTIME, CKFRMActorEditorGUIBase::_wxFB_sld_AnimTimeOnCommandScroll )
	EVT_SLIDER( ID_HSCALE, CKFRMActorEditorGUIBase::_wxFB_sld_HScaleOnCommandScroll )
	EVT_SLIDER( ID_VSCALE, CKFRMActorEditorGUIBase::_wxFB_sld_VScaleOnCommandScroll )
	EVT_BUTTON( ID_LOADSPRITE, CKFRMActorEditorGUIBase::_wxFB_bt_LoadSpriteOnButtonClick )
	EVT_COMBOBOX( ID_CBSPRITEACTION, CKFRMActorEditorGUIBase::_wxFB_cbSpriteActionOnCombobox )
	EVT_SLIDER( ID_SLD_XOFS, CKFRMActorEditorGUIBase::_wxFB_sld_OfsOnCommandScroll )
	EVT_SLIDER( ID_SLD_YOFS, CKFRMActorEditorGUIBase::_wxFB_sld_YOfsOnCommandScroll )
	EVT_MENU( ID_MENUITEM12, CKFRMActorEditorGUIBase::_wxFB_m_menuItem12OnMenuSelection )
	EVT_MENU( ID_MENUITEM10, CKFRMActorEditorGUIBase::_wxFB_m_menuItem10OnMenuSelection )
	EVT_MENU( ID_MENUITEM_3, CKFRMActorEditorGUIBase::_wxFB_menuitem3OnMenuSelection )
END_EVENT_TABLE()

CKFRMActorEditorGUIBase::CKFRMActorEditorGUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* WxBoxSizer1;
	WxBoxSizer1 = new wxBoxSizer( wxVERTICAL );
	
	WxPanel1 = new wxPanel( this, wxID_ANY, wxPoint( 5,5 ), wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* WxBoxSizer3;
	WxBoxSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* WxBoxSizer5;
	WxBoxSizer5 = new wxBoxSizer( wxVERTICAL );
	
	WxBoxSizer5->SetMinSize( wxSize( 640,480 ) ); 
	pn_Display = new wxInputPanel( WxPanel1, ID_PN_DISPLAY, wxDefaultPosition, wxSize( 640,480 ), wxTAB_TRAVERSAL );
	pn_Display->SetMinSize( wxSize( 640,480 ) );
	pn_Display->SetMaxSize( wxSize( 640,480 ) );
	
	WxBoxSizer5->Add( pn_Display, 0, wxALIGN_CENTER|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	sb_KeyFrame = new wxScrollBar( WxPanel1, ID_KEYFRAME, wxPoint( 5,495 ), wxSize( 640,17 ), wxSB_HORIZONTAL );
	sb_KeyFrame->SetMinSize( wxSize( 640,-1 ) );
	sb_KeyFrame->SetMaxSize( wxSize( 640,-1 ) );
	
	WxBoxSizer5->Add( sb_KeyFrame, 0, wxALL, 5 );
	
	wxBoxSizer* WxBoxSizer2;
	WxBoxSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	cb_ShowBackground = new wxCheckBox( WxPanel1, ID_SHOWBACKGROUND, wxT("Show Background"), wxPoint( 448,13 ), wxSize( 97,17 ), 0 );
	cb_ShowBackground->SetValue(true); 
	WxBoxSizer2->Add( cb_ShowBackground, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	cb_ShowLabels = new wxCheckBox( WxPanel1, ID_SHOWLABELS, wxT("Show labels"), wxPoint( 448,13 ), wxSize( 97,17 ), 0 );
	cb_ShowLabels->SetValue(true); 
	WxBoxSizer2->Add( cb_ShowLabels, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bt_ReloadMaterials = new wxButton( WxPanel1, ID_RELOADMATERIALS, wxT("Reload Sprites"), wxDefaultPosition, wxSize( 75,25 ), 0 );
	WxBoxSizer2->Add( bt_ReloadMaterials, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bt_Play = new wxButton( WxPanel1, ID_PLAYANIM, wxT("Play"), wxPoint( -1,-1 ), wxSize( 75,25 ), 0 );
	bt_Play->SetDefault(); 
	WxBoxSizer2->Add( bt_Play, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	sld_AnimTime = new wxSlider( WxPanel1, ID_ANIMTIME, 1, 1, 1000, wxPoint( 150,5 ), wxSize( 150,45 ), wxSL_LABELS|wxSL_BOTH );
	WxBoxSizer2->Add( sld_AnimTime, 1, wxALL|wxEXPAND, 5 );
	
	WxBoxSizer5->Add( WxBoxSizer2, 1, wxEXPAND, 5 );
	
	WxBoxSizer3->Add( WxBoxSizer5, 0, wxEXPAND, 5 );
	
	wxBoxSizer* WxBoxSizer4;
	WxBoxSizer4 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* WxBoxSizer9;
	WxBoxSizer9 = new wxBoxSizer( wxVERTICAL );
	
	WxPanel2 = new wxPanel( WxPanel1, wxID_ANY, wxPoint( 5,5 ), wxSize( 289,160 ), wxTAB_TRAVERSAL );
	WxPanel2->SetMinSize( wxSize( -1,160 ) );
	
	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer( wxVERTICAL );
	
	WxStaticText1 = new wxStaticText( WxPanel2, wxID_ANY, wxT("Horizonal Scale:"), wxPoint( 16,18 ), wxSize( 84,-1 ), 0 );
	WxStaticText1->Wrap( -1 );
	bSizer171->Add( WxStaticText1, 0, wxALL|wxEXPAND, 5 );
	
	sld_HScale = new wxSlider( WxPanel2, ID_HSCALE, 100, 1, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer171->Add( sld_HScale, 0, wxALL|wxEXPAND, 5 );
	
	WxStaticText11 = new wxStaticText( WxPanel2, wxID_ANY, wxT("Vertical Scale:"), wxPoint( 16,18 ), wxSize( 84,-1 ), 0 );
	WxStaticText11->Wrap( -1 );
	bSizer171->Add( WxStaticText11, 1, wxALL|wxEXPAND, 5 );
	
	sld_VScale = new wxSlider( WxPanel2, ID_VSCALE, 100, 1, 200, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer171->Add( sld_VScale, 0, wxALL|wxEXPAND, 5 );
	
	WxPanel2->SetSizer( bSizer171 );
	WxPanel2->Layout();
	WxBoxSizer9->Add( WxPanel2, 1, wxALL|wxBOTTOM|wxEXPAND|wxLEFT, 1 );
	
	WxBoxSizer4->Add( WxBoxSizer9, 0, wxEXPAND, 5 );
	
	pnIconLayer = new wxPanel( WxPanel1, ID_PNICONLAYER, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizerBoneProps;
	sbSizerBoneProps = new wxStaticBoxSizer( new wxStaticBox( pnIconLayer, ID_SBSIZERBONEPROPS, wxT("Bone Props:") ), wxVERTICAL );
	
	wxBoxSizer* WxBoxSizer8;
	WxBoxSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	WxStaticText4 = new wxStaticText( pnIconLayer, wxID_ANY, wxT("Sprite:"), wxPoint( 32,18 ), wxSize( 36,17 ), 0 );
	WxStaticText4->Wrap( -1 );
	WxBoxSizer8->Add( WxStaticText4, 0, wxALL, 5 );
	
	edIconSprite = new wxTextCtrl( pnIconLayer, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 200,-1 ), wxTE_READONLY );
	WxBoxSizer8->Add( edIconSprite, 0, wxALL, 5 );
	
	bt_LoadSprite = new wxBitmapButton( pnIconLayer, ID_LOADSPRITE, wxBitmap( CKFRMActorEditorGUI_WxBitmapButton8_XPM_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	WxBoxSizer8->Add( bt_LoadSprite, 0, wxALL, 5 );
	
	sbSizerBoneProps->Add( WxBoxSizer8, 1, wxALL|wxEXPAND, 1 );
	
	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText7 = new wxStaticText( pnIconLayer, wxID_ANY, wxT("Action:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer12->Add( m_staticText7, 1, wxALL, 5 );
	
	cbSpriteAction = new wxComboBox( pnIconLayer, ID_CBSPRITEACTION, wxT("0"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer12->Add( cbSpriteAction, 1, wxALL, 5 );
	
	sbSizerBoneProps->Add( bSizer12, 2, wxEXPAND, 1 );
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( pnIconLayer, wxID_ANY, wxT("Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer13->Add( m_staticText8, 0, wxALL|wxEXPAND, 5 );
	
	sld_XOfs = new wxSlider( pnIconLayer, ID_SLD_XOFS, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer13->Add( sld_XOfs, 0, wxALL|wxEXPAND, 5 );
	
	sld_YOfs = new wxSlider( pnIconLayer, ID_SLD_YOFS, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer13->Add( sld_YOfs, 0, wxALL|wxEXPAND, 5 );
	
	sbSizerBoneProps->Add( bSizer13, 4, wxEXPAND, 1 );
	
	pnIconLayer->SetSizer( sbSizerBoneProps );
	pnIconLayer->Layout();
	sbSizerBoneProps->Fit( pnIconLayer );
	WxBoxSizer4->Add( pnIconLayer, 1, wxALL|wxEXPAND, 5 );
	
	WxBoxSizer3->Add( WxBoxSizer4, 1, wxEXPAND, 5 );
	
	WxPanel1->SetSizer( WxBoxSizer3 );
	WxPanel1->Layout();
	WxBoxSizer3->Fit( WxPanel1 );
	WxBoxSizer1->Add( WxPanel1, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( WxBoxSizer1 );
	this->Layout();
	WxBoxSizer1->Fit( this );
	WxMenuBar1 = new wxMenuBar( 0 );
	menu0 = new wxMenu();
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( menu0, ID_MENUITEM12, wxString( wxT("Load Skeleton") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_menuItem12 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( menu0, ID_MENUITEM10, wxString( wxT("Save Skeleton") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_menuItem10 );
	
	wxMenuItem* m_separator2;
	m_separator2 = menu0->AppendSeparator();
	
	wxMenuItem* menuitem3;
	menuitem3 = new wxMenuItem( menu0, ID_MENUITEM_3, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem3 );
	
	WxMenuBar1->Append( menu0, wxT("&File") ); 
	
	this->SetMenuBar( WxMenuBar1 );
	
	CKFRMActorEditorGUIBAR = this->CreateStatusBar( 1, wxST_SIZEGRIP|wxDOUBLE_BORDER|wxSTATIC_BORDER, wxID_ANY );
}

CKFRMActorEditorGUIBase::~CKFRMActorEditorGUIBase()
{
}
