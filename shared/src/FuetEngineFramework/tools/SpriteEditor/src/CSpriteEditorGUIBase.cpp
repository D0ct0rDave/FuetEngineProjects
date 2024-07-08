///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "components/wxInputPanel.h"

#include "CSpriteEditorGUIBase.h"

#include "../../common/Images/adjust_frame.xpm"
#include "../../common/Images/adjust_frame_bottomcenter.xpm"
#include "../../common/Images/adjust_frame_center.xpm"
#include "../../common/Images/adjust_frame_top_left.xpm"
#include "../../common/Images/adjust_pivot_bottom_center.xpm"
#include "../../common/Images/adjust_pivot_bottom_left.xpm"
#include "../../common/Images/adjust_pivot_bottom_right.xpm"
#include "../../common/Images/adjust_pivot_center_center.xpm"
#include "../../common/Images/adjust_pivot_center_left.xpm"
#include "../../common/Images/adjust_pivot_center_right.xpm"
#include "../../common/Images/adjust_pivot_horizontal_center.xpm"
#include "../../common/Images/adjust_pivot_horizontal_left.xpm"
#include "../../common/Images/adjust_pivot_horizontal_right.xpm"
#include "../../common/Images/adjust_pivot_manually.xpm"
#include "../../common/Images/adjust_pivot_top_center.xpm"
#include "../../common/Images/adjust_pivot_top_left.xpm"
#include "../../common/Images/adjust_pivot_top_right.xpm"
#include "../../common/Images/adjust_pivot_vertical_bottom.xpm"
#include "../../common/Images/adjust_pivot_vertical_center.xpm"
#include "../../common/Images/adjust_pivot_vertical_top.xpm"
#include "../../common/Images/copy_to_clipboard.xpm"
#include "../../common/Images/delete_icon.xpm"
#include "../../common/Images/down_arrow.xpm"
#include "../../common/Images/icon_layer_icon.xpm"
#include "../../common/Images/load_sprite_icon.xpm"
#include "../../common/Images/new_layer_icon.xpm"
#include "../../common/Images/reset_icon.xpm"
#include "../../common/Images/selection_paste_in_place.xpm"
#include "../../common/Images/up_arrow.xpm"

///////////////////////////////////////////////////////////////////////////

CSpriteEditorGUIBase::CSpriteEditorGUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 1024,768 ), wxDefaultSize );
	this->SetBackgroundColour( wxColour( 134, 150, 157 ) );
	
	wxBoxSizer* WxBoxSizer1;
	WxBoxSizer1 = new wxBoxSizer( wxVERTICAL );
	
	WxPanel1 = new wxPanel( this, wxID_ANY, wxPoint( 5,5 ), wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* WxBoxSizer3;
	WxBoxSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* ParteIzquierda;
	ParteIzquierda = new wxBoxSizer( wxVERTICAL );
	
	sbSizerBoneProps = new wxStaticBoxSizer( new wxStaticBox( WxPanel1, ID_SBSIZERBONEPROPS, wxT("Sprite Action List:") ), wxVERTICAL );
	
	m_gdSpriteActions = new wxGrid( WxPanel1, ID_M_GDTIMELINE, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	m_gdSpriteActions->CreateGrid( 1, 5 );
	m_gdSpriteActions->EnableEditing( false );
	m_gdSpriteActions->EnableGridLines( false );
	m_gdSpriteActions->EnableDragGridSize( false );
	m_gdSpriteActions->SetMargins( 0, 0 );
	
	// Columns
	m_gdSpriteActions->EnableDragColMove( false );
	m_gdSpriteActions->EnableDragColSize( false );
	m_gdSpriteActions->SetColLabelSize( 0 );
	m_gdSpriteActions->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gdSpriteActions->EnableDragRowSize( true );
	m_gdSpriteActions->SetRowLabelSize( 0 );
	m_gdSpriteActions->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gdSpriteActions->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_gdSpriteActions->SetMinSize( wxSize( -1,200 ) );
	
	sbSizerBoneProps->Add( m_gdSpriteActions, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* WxBoxSizer81;
	WxBoxSizer81 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btNewAction = new wxBitmapButton( WxPanel1, ID_M_BTNEWACTION, wxBitmap( new_layer_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btNewAction->SetToolTip( wxT("Add a new action into the sprite action list.") );
	
	WxBoxSizer81->Add( m_btNewAction, 0, wxALL, 5 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btActionUp = new wxBitmapButton( WxPanel1, ID_M_BTACTIONUP, wxBitmap( up_arrow_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btActionUp->SetToolTip( wxT("Move the current sprite action one step up in the action list.") );
	
	bSizer20->Add( m_btActionUp, 1, wxALL|wxEXPAND, 5 );
	
	m_btActionDown = new wxBitmapButton( WxPanel1, ID_M_BTACTIONDOWN, wxBitmap( down_arrow_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btActionDown->SetToolTip( wxT("Move the current sprite action one step down in the action list.") );
	
	bSizer20->Add( m_btActionDown, 1, wxALL, 5 );
	
	
	WxBoxSizer81->Add( bSizer20, 1, wxEXPAND, 5 );
	
	m_btDeleteAction = new wxBitmapButton( WxPanel1, ID_M_BTDELETEACTION, wxBitmap( delete_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btDeleteAction->SetToolTip( wxT("Delete the currently selected action in the sprite list.") );
	
	WxBoxSizer81->Add( m_btDeleteAction, 0, wxALIGN_RIGHT|wxALL, 5 );
	
	
	sbSizerBoneProps->Add( WxBoxSizer81, 0, wxEXPAND, 5 );
	
	
	ParteIzquierda->Add( sbSizerBoneProps, 4, wxEXPAND, 5 );
	
	m_pnPrevDisplay = new wxInputPanel( WxPanel1, ID_M_PNPREVDISPLAY, wxDefaultPosition, wxSize( 400,-1 ), wxTAB_TRAVERSAL );
	m_pnPrevDisplay->SetMinSize( wxSize( 400,300 ) );
	m_pnPrevDisplay->SetMaxSize( wxSize( 400,-1 ) );
	
	ParteIzquierda->Add( m_pnPrevDisplay, 0, wxALL, 10 );
	
	wxBoxSizer* WxBoxSizer10;
	WxBoxSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_Play = new wxButton( WxPanel1, ID_PLAYANIM, wxT("Play"), wxPoint( 5,5 ), wxSize( 75,25 ), 0 );
	bt_Play->SetDefault(); 
	bt_Play->SetToolTip( wxT("Plays or stops the current sprite action rendering.") );
	
	WxBoxSizer10->Add( bt_Play, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer( wxVERTICAL );
	
	
	WxBoxSizer10->Add( bSizer21, 1, wxEXPAND, 5 );
	
	m_staticText23 = new wxStaticText( WxPanel1, wxID_ANY, wxT("Reload Materials:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText23->Wrap( -1 );
	WxBoxSizer10->Add( m_staticText23, 0, wxALL, 5 );
	
	m_btReloadMaterials = new wxBitmapButton( WxPanel1, ID_RELOADMATERIALS, wxBitmap( reset_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btReloadMaterials->SetToolTip( wxT("Press to force the reload of any of the used materials from disk. Useful when you update the textures of sprites and want to see the results without closing/opening the application every time.") );
	
	WxBoxSizer10->Add( m_btReloadMaterials, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	ParteIzquierda->Add( WxBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxEXPAND|wxFIXED_MINSIZE, 4 );
	
	
	WxBoxSizer3->Add( ParteIzquierda, 0, wxEXPAND|wxFIXED_MINSIZE, 0 );
	
	wxBoxSizer* ParteDerecha;
	ParteDerecha = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxHORIZONTAL );
	
	bSizer26->SetMinSize( wxSize( 400,-1 ) ); 
	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer( wxVERTICAL );
	
	m_pnFrameSeq = new wxInputPanel( WxPanel1, ID_PN_FRAMESEQ, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_pnFrameSeq->SetMinSize( wxSize( -1,128 ) );
	
	bSizer27->Add( m_pnFrameSeq, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer202;
	bSizer202 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btCopyFrame = new wxBitmapButton( WxPanel1, ID_M_BTCOPYFRAME, wxBitmap( copy_to_clipboard_xpm ), wxDefaultPosition, wxSize( 28,28 ), wxBU_AUTODRAW );
	m_btCopyFrame->SetToolTip( wxT("Copies the current frame settings into the clipboard.") );
	
	bSizer202->Add( m_btCopyFrame, 0, wxALL, 5 );
	
	m_btPasteFrame = new wxBitmapButton( WxPanel1, ID_M_BTPASTEFRAME, wxBitmap( selection_paste_in_place_xpm ), wxDefaultPosition, wxSize( 28,28 ), wxBU_AUTODRAW );
	m_btPasteFrame->Enable( false );
	m_btPasteFrame->SetToolTip( wxT("Pastes the content of the clipboard into the current frame. Creates a frame if there isn't any in the current sprite action.") );
	
	bSizer202->Add( m_btPasteFrame, 0, wxALL, 5 );
	
	sb_KeyFrame = new wxScrollBar( WxPanel1, ID_KEYFRAME, wxPoint( -1,-1 ), wxSize( -1,-1 ), wxSB_HORIZONTAL );
	sb_KeyFrame->SetToolTip( wxT("Use the scroll bar to see and move across all the frames of the sequence of the current sprite action.") );
	
	bSizer202->Add( sb_KeyFrame, 1, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	m_btNewFrame = new wxBitmapButton( WxPanel1, ID_M_BTNEWFRAME, wxBitmap( icon_layer_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btNewFrame->SetToolTip( wxT("Adds a new frame in the sequence of frames of the current sprite action.") );
	
	bSizer202->Add( m_btNewFrame, 0, wxBOTTOM|wxLEFT|wxTOP, 5 );
	
	m_btDeleteFrame = new wxBitmapButton( WxPanel1, ID_M_BTDELETEFRAME, wxBitmap( delete_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btDeleteFrame->SetToolTip( wxT("Deletes the current frame from the sequence of frames of the current sprite action.") );
	
	bSizer202->Add( m_btDeleteFrame, 0, wxALIGN_LEFT|wxALL, 5 );
	
	
	bSizer191->Add( bSizer202, 0, wxEXPAND, 5 );
	
	
	bSizer27->Add( bSizer191, 0, wxEXPAND, 5 );
	
	pn_Display = new wxInputPanel( WxPanel1, ID_PN_DISPLAY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	pn_Display->SetMinSize( wxSize( 200,200 ) );
	
	bSizer27->Add( pn_Display, 1, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* WxBoxSizer2;
	WxBoxSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_lbSnapFactor = new wxStaticText( WxPanel1, ID_M_LBSNAPFACTOR, wxT("Snap Factor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbSnapFactor->Wrap( -1 );
	WxBoxSizer2->Add( m_lbSnapFactor, 0, wxALL, 5 );
	
	wxString m_cbSnapFactorChoices[] = { wxT("None"), wxT("1 pix"), wxT("2 pix"), wxT("4 pix"), wxT("5 pix"), wxT("8 pix"), wxT("10 pix"), wxT("16 pix"), wxT("20 pix"), wxT("25 pix"), wxT("32 pix"), wxT("50 pix"), wxT("64 pix"), wxT("100 pix"), wxT("128 pix") };
	int m_cbSnapFactorNChoices = sizeof( m_cbSnapFactorChoices ) / sizeof( wxString );
	m_cbSnapFactor = new wxChoice( WxPanel1, ID_M_CBSNAPFACTOR, wxDefaultPosition, wxDefaultSize, m_cbSnapFactorNChoices, m_cbSnapFactorChoices, 0 );
	m_cbSnapFactor->SetSelection( 0 );
	WxBoxSizer2->Add( m_cbSnapFactor, 0, wxALL, 2 );
	
	lb_Frame = new wxStaticText( WxPanel1, wxID_ANY, wxT("Coords:---"), wxPoint( 5,5 ), wxSize( 100,-1 ), wxALIGN_CENTRE );
	lb_Frame->Wrap( -1 );
	lb_Frame->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	WxBoxSizer2->Add( lb_Frame, 1, wxALL|wxEXPAND, 5 );
	
	lb_Trans = new wxStaticText( WxPanel1, wxID_ANY, wxT("Offset:---"), wxPoint( 199,13 ), wxSize( 100,17 ), 0 );
	lb_Trans->Wrap( 0 );
	lb_Trans->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	WxBoxSizer2->Add( lb_Trans, 1, wxALL|wxEXPAND, 5 );
	
	lb_Zoom = new wxStaticText( WxPanel1, wxID_ANY, wxT("Zoom:---"), wxPoint( 348,13 ), wxSize( 100,17 ), 0 );
	lb_Zoom->Wrap( 0 );
	lb_Zoom->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	WxBoxSizer2->Add( lb_Zoom, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer27->Add( WxBoxSizer2, 0, wxALIGN_CENTER|wxEXPAND, 5 );
	
	
	bSizer26->Add( bSizer27, 1, wxEXPAND, 5 );
	
	
	ParteDerecha->Add( bSizer26, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( WxPanel1, wxID_FRAMEINFO, wxT("Sprite Action Frame Properties:") ), wxVERTICAL );
	
	m_pnFrameProps = new wxScrolledWindow( WxPanel1, wxID_MY_FRAME_PROPS, wxDefaultPosition, wxSize( 290,-1 ), wxVSCROLL );
	m_pnFrameProps->SetScrollRate( 0, 5 );
	m_pnFrameProps->SetMinSize( wxSize( 290,-1 ) );
	m_pnFrameProps->SetMaxSize( wxSize( 310,-1 ) );
	
	wxBoxSizer* bSizer221;
	bSizer221 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* m_poFrameMaterial;
	m_poFrameMaterial = new wxBoxSizer( wxVERTICAL );
	
	m_staticText35 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Frame Material:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText35->Wrap( -1 );
	m_staticText35->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText35->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	m_poFrameMaterial->Add( m_staticText35, 0, wxALL, 2 );
	
	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btLoadMaterial = new wxBitmapButton( m_pnFrameProps, ID_M_BTLOADMATERIAL, wxBitmap( load_sprite_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btLoadMaterial->SetToolTip( wxT("Click to select a material to be used by the current frame in the sprite action sequence.") );
	
	bSizer28->Add( m_btLoadMaterial, 0, wxALL, 5 );
	
	m_btClearMaterial = new wxBitmapButton( m_pnFrameProps, ID_M_BTCLEARMATERIAL, wxBitmap( delete_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btClearMaterial->Hide();
	m_btClearMaterial->SetToolTip( wxT("Click to clear the material used by the current frame in the sprite action sequence.") );
	
	bSizer28->Add( m_btClearMaterial, 0, wxALL, 5 );
	
	m_edFrameMaterial = new wxTextCtrl( m_pnFrameProps, ID_ED_ICONSPRITE, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY );
	m_edFrameMaterial->SetMaxLength( 0 ); 
	m_edFrameMaterial->SetToolTip( wxT("This box shows the current material being used by the current frame in the sprite action sequence.") );
	m_edFrameMaterial->SetMinSize( wxSize( 50,-1 ) );
	
	bSizer28->Add( m_edFrameMaterial, 1, wxALIGN_RIGHT|wxALL, 5 );
	
	
	m_poFrameMaterial->Add( bSizer28, 0, wxEXPAND, 5 );
	
	
	bSizer221->Add( m_poFrameMaterial, 0, wxEXPAND, 0 );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText24 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("S Wrap Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText24->Wrap( -1 );
	fgSizer2->Add( m_staticText24, 0, wxALL, 5 );
	
	m_cbSWrapMode = new wxComboBox( m_pnFrameProps, wxID_ANY, wxT("Clamp"), wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	m_cbSWrapMode->Append( wxT("Clamp") );
	m_cbSWrapMode->Append( wxT("Repeat") );
	m_cbSWrapMode->Append( wxT("Mirrored Repeat") );
	fgSizer2->Add( m_cbSWrapMode, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText241 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("T Wrap Mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText241->Wrap( -1 );
	fgSizer2->Add( m_staticText241, 0, wxALL, 5 );
	
	m_cbTWrapMode = new wxComboBox( m_pnFrameProps, wxID_ANY, wxT("Clamp"), wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	m_cbTWrapMode->Append( wxT("Clamp") );
	m_cbTWrapMode->Append( wxT("Repeat") );
	m_cbTWrapMode->Append( wxT("Mirrored Repeat") );
	fgSizer2->Add( m_cbTWrapMode, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText2411 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Filter:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2411->Wrap( -1 );
	fgSizer2->Add( m_staticText2411, 0, wxALL, 5 );
	
	m_cbFilter = new wxComboBox( m_pnFrameProps, wxID_ANY, wxT("Linear"), wxDefaultPosition, wxSize( -1,-1 ), 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	m_cbFilter->Append( wxT("Nearest") );
	m_cbFilter->Append( wxT("Linear") );
	m_cbFilter->SetSelection( 1 );
	fgSizer2->Add( m_cbFilter, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer221->Add( fgSizer2, 0, wxEXPAND, 5 );
	
	m_staticline32 = new wxStaticLine( m_pnFrameProps, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer221->Add( m_staticline32, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* m_poFrameHelpers;
	m_poFrameHelpers = new wxBoxSizer( wxVERTICAL );
	
	m_btAffectAllFrames = new wxToggleButton( m_pnFrameProps, ID_M_BTAFFECTALLFRAMES, wxT("Click to affect all frames"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_btAffectAllFrames->SetToolTip( wxT("Leave pressed the button to affect all the frames in the sprite action sequence with the values you enter in the sliders at the right.\nTo handle only the current frame, leave the button unpressed.") );
	
	m_poFrameHelpers->Add( m_btAffectAllFrames, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxALIGN_CENTER_HORIZONTAL|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText17 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Delay"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_staticText17->Wrap( -1 );
	m_staticText17->SetMinSize( wxSize( 70,-1 ) );
	m_staticText17->SetMaxSize( wxSize( 70,-1 ) );
	
	bSizer24->Add( m_staticText17, 0, wxALL, 5 );
	
	m_sldDelayBlendFact = new wxSlider( m_pnFrameProps, ID_M_SLDDELAYBLENDFACT, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_sldDelayBlendFact->SetToolTip( wxT("This is a factor that represents how much of the frame time is performed in the initial delay of the rendering of the frame, and how much time is performed on the blending with the next frame of the sprite action.") );
	
	bSizer24->Add( m_sldDelayBlendFact, 0, wxALL, 5 );
	
	m_staticText18 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Blend"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_staticText18->Wrap( -1 );
	bSizer24->Add( m_staticText18, 0, wxALL, 5 );
	
	
	bSizer19->Add( bSizer24, 0, 0, 0 );
	
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText19 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Frame Time:"), wxDefaultPosition, wxSize( 70,-1 ), 0 );
	m_staticText19->Wrap( -1 );
	m_staticText19->SetMinSize( wxSize( 70,-1 ) );
	m_staticText19->SetMaxSize( wxSize( 70,-1 ) );
	
	bSizer18->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldFrameTime = new wxSlider( m_pnFrameProps, ID_M_SLDFRAMETIME, 50, 1, 10000, wxDefaultPosition, wxDefaultSize, wxSL_BOTH );
	m_sldFrameTime->SetToolTip( wxT("This is the total amount of time (in millisecons) this frame will be visible respect the whole sprite action.") );
	
	bSizer18->Add( m_sldFrameTime, 0, wxALL, 5 );
	
	m_lbFrameTime = new wxStaticText( m_pnFrameProps, ID_M_LBFRAMETIME, wxT("10.00 sec"), wxDefaultPosition, wxSize( -1,-1 ), 0 );
	m_lbFrameTime->Wrap( -1 );
	bSizer18->Add( m_lbFrameTime, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer19->Add( bSizer18, 0, 0, 0 );
	
	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 0, 3, 0, 0 );
	
	m_btAutoAdjustTopLeft = new wxBitmapButton( m_pnFrameProps, ID_m_btAutoAdjustTopLeft, wxBitmap( adjust_frame_top_left_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAutoAdjustTopLeft->SetToolTip( wxT("Adjust the frame to the complete size of the image. Also places the frame pivot in the middle bottom of the image.") );
	
	gSizer6->Add( m_btAutoAdjustTopLeft, 0, wxALL, 5 );
	
	m_btAutoAdjustCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAutoAdjustCenter, wxBitmap( adjust_frame_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAutoAdjustCenter->SetToolTip( wxT("Adjust the frame to the complete size of the image. Also places the frame pivot in the center of the image.") );
	
	gSizer6->Add( m_btAutoAdjustCenter, 0, wxALL, 5 );
	
	m_btAutoAdjustBottomCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAutoAdjustBottomCenter, wxBitmap( adjust_frame_bottomcenter_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAutoAdjustBottomCenter->SetToolTip( wxT("Adjust the frame to the complete size of the image. Also places the frame pivot in the middle bottom of the image.") );
	
	gSizer6->Add( m_btAutoAdjustBottomCenter, 0, wxALL, 5 );
	
	m_btAutoAdjustSize = new wxBitmapButton( m_pnFrameProps, ID_m_btAutoAdjustSize, wxBitmap( adjust_frame_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAutoAdjustSize->SetToolTip( wxT("Adjust the frame to the complete size of the image. Also places the frame pivot in the center of the image.") );
	
	gSizer6->Add( m_btAutoAdjustSize, 0, wxALL, 5 );
	
	m_btAdjustSizeManually = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustSizeManually, wxBitmap( adjust_pivot_manually_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustSizeManually->SetToolTip( wxT("Press this button to enter numerically the size of the frame in pixels.") );
	
	gSizer6->Add( m_btAdjustSizeManually, 0, wxALL, 5 );
	
	
	bSizer19->Add( gSizer6, 0, wxALIGN_CENTER_HORIZONTAL|wxTOP|wxBOTTOM, 15 );
	
	
	m_poFrameHelpers->Add( bSizer19, 0, 0, 5 );
	
	
	bSizer221->Add( m_poFrameHelpers, 0, wxEXPAND, 5 );
	
	m_staticText292 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Flags:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText292->Wrap( -1 );
	m_staticText292->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText292->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer221->Add( m_staticText292, 0, wxALL, 5 );
	
	wxGridSizer* gSizer31;
	gSizer31 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_cbScaleXUEqually = new wxCheckBox( m_pnFrameProps, wxID_ANY, wxT("Scale X and U Equ"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer31->Add( m_cbScaleXUEqually, 0, wxALL, 5 );
	
	m_cbUWorldCoords = new wxCheckBox( m_pnFrameProps, wxID_ANY, wxT("U world coord"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer31->Add( m_cbUWorldCoords, 0, wxALL, 5 );
	
	m_cbScaleYVEqually = new wxCheckBox( m_pnFrameProps, wxID_ANY, wxT("Scale Y and V Equ"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer31->Add( m_cbScaleYVEqually, 0, wxALL, 5 );
	
	m_cbVWorldCoords = new wxCheckBox( m_pnFrameProps, wxID_ANY, wxT("V world coord"), wxDefaultPosition, wxDefaultSize, 0 );
	gSizer31->Add( m_cbVWorldCoords, 0, wxALL, 5 );
	
	
	bSizer221->Add( gSizer31, 0, wxEXPAND, 5 );
	
	m_staticline3 = new wxStaticLine( m_pnFrameProps, wxID_ANY, wxDefaultPosition, wxSize( -1,-1 ), wxLI_HORIZONTAL );
	bSizer221->Add( m_staticline3, 0, wxALL|wxEXPAND, 2 );
	
	m_staticText29 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Frame size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	m_staticText29->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText29->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer221->Add( m_staticText29, 0, wxALL, 0 );
	
	wxGridSizer* gSizer3;
	gSizer3 = new wxGridSizer( 0, 2, 0, 0 );
	
	m_staticText5 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Image Rect:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	gSizer3->Add( m_staticText5, 0, wxALL, 5 );
	
	m_lbImgRect = new wxStaticText( m_pnFrameProps, ID_M_LBIMGRECT, wxT("---"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbImgRect->Wrap( -1 );
	m_lbImgRect->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	gSizer3->Add( m_lbImgRect, 0, wxALL, 5 );
	
	m_staticText6 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Image Rect Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	gSizer3->Add( m_staticText6, 0, wxALL, 5 );
	
	m_lbImageRectSize = new wxStaticText( m_pnFrameProps, ID_M_LBRECTSIZE, wxT("---"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbImageRectSize->Wrap( -1 );
	m_lbImageRectSize->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	gSizer3->Add( m_lbImageRectSize, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("UV Rect:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	gSizer3->Add( m_staticText7, 0, wxALL, 5 );
	
	m_lbUVRect = new wxStaticText( m_pnFrameProps, ID_M_LBUVRECT, wxT("---"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbUVRect->Wrap( -1 );
	m_lbUVRect->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	gSizer3->Add( m_lbUVRect, 0, wxALL, 5 );
	
	m_staticText16 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Frame Rect Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText16->Wrap( -1 );
	gSizer3->Add( m_staticText16, 0, wxALL, 5 );
	
	m_lbFrameRectSize = new wxStaticText( m_pnFrameProps, ID_M_LBFRAMERECTSIZE, wxT("---"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbFrameRectSize->Wrap( -1 );
	m_lbFrameRectSize->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	gSizer3->Add( m_lbFrameRectSize, 0, wxALL, 5 );
	
	
	bSizer221->Add( gSizer3, 0, 0, 5 );
	
	m_staticline31 = new wxStaticLine( m_pnFrameProps, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer221->Add( m_staticline31, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText291 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Frame pivot:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText291->Wrap( -1 );
	m_staticText291->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	m_staticText291->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	bSizer221->Add( m_staticText291, 0, wxALL, 2 );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText28 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Inside frame UV coords:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText28->Wrap( -1 );
	bSizer22->Add( m_staticText28, 0, wxALL, 5 );
	
	m_lbPivotFrameCoords = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("----"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbPivotFrameCoords->Wrap( -1 );
	bSizer22->Add( m_lbPivotFrameCoords, 0, wxALL, 5 );
	
	
	bSizer221->Add( bSizer22, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer29;
	bSizer29 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText8 = new wxStaticText( m_pnFrameProps, wxID_ANY, wxT("Image Coords:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer29->Add( m_staticText8, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lbPivotImageXCoord = new wxStaticText( m_pnFrameProps, ID_M_LBPIVOTIMAGECOORDS, wxT("X"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbPivotImageXCoord->Wrap( -1 );
	m_lbPivotImageXCoord->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	fgSizer1->Add( m_lbPivotImageXCoord, 0, wxALL, 5 );
	
	m_edPivotImageXCoord = new wxSpinCtrl( m_pnFrameProps, ID_M_EDPIVOTIMAGEXCOORD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -65536, 65536, 0 );
	fgSizer1->Add( m_edPivotImageXCoord, 1, wxALL|wxEXPAND, 5 );
	
	m_lbPivotImageYCoord = new wxStaticText( m_pnFrameProps, ID_M_LBPIVOTIMAGECOORDS, wxT("Y"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbPivotImageYCoord->Wrap( -1 );
	m_lbPivotImageYCoord->SetForegroundColour( wxColour( 0, 0, 128 ) );
	
	fgSizer1->Add( m_lbPivotImageYCoord, 0, wxALL, 5 );
	
	m_edPivotImageYCoord = new wxSpinCtrl( m_pnFrameProps, ID_M_EDPIVOTIMAGEYCOORD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -65536, 65536, 0 );
	fgSizer1->Add( m_edPivotImageYCoord, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer29->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	
	bSizer221->Add( bSizer29, 0, wxEXPAND, 5 );
	
	wxGridSizer* gSizer8;
	gSizer8 = new wxGridSizer( 4, 4, 2, 2 );
	
	m_btAdjustPivotVerticalTop = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotVerticalTop, wxBitmap( adjust_pivot_vertical_top_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotVerticalTop->SetToolTip( wxT("Adjust the pivot y component to the center of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotVerticalTop, 0, wxALL, 1 );
	
	m_btAdjustPivotTopLeft = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotTopLeft, wxBitmap( adjust_pivot_top_left_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotTopLeft->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotTopLeft, 0, wxALL, 1 );
	
	m_btAdjustPivotTopCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotTopCenter, wxBitmap( adjust_pivot_top_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotTopCenter->SetToolTip( wxT("Adjust the pivot y component to the center of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotTopCenter, 0, wxALL, 1 );
	
	m_btAdjustPivotTopRight = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotTopRight, wxBitmap( adjust_pivot_top_right_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotTopRight->SetToolTip( wxT("Adjust the pivot y component to the center of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotTopRight, 0, wxALL, 1 );
	
	m_btAdjustPivotVerticalCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotVerticalCenter, wxBitmap( adjust_pivot_vertical_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotVerticalCenter->SetToolTip( wxT("Adjust the pivot y component to the center of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotVerticalCenter, 0, wxALL, 1 );
	
	m_btAdjustPivotCenterLeft = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotCenterLeft, wxBitmap( adjust_pivot_center_left_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotCenterLeft->SetToolTip( wxT("Adjust the pivot y component to the center of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotCenterLeft, 0, wxALL, 1 );
	
	m_btAdjustPivotCenterCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotCenterCenter, wxBitmap( adjust_pivot_center_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotCenterCenter->SetToolTip( wxT("Adjust the pivot y component to the bottom of the selected frame.") );
	
	gSizer8->Add( m_btAdjustPivotCenterCenter, 0, wxALL, 1 );
	
	m_btAdjustPivotCenterRight = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotCenterRight, wxBitmap( adjust_pivot_center_right_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotCenterRight->SetToolTip( wxT("Adjust the pivot of the selected frame to the center.") );
	
	gSizer8->Add( m_btAdjustPivotCenterRight, 0, wxALL, 1 );
	
	m_btAdjustPivotVerticalBottom = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotVerticalBottom, wxBitmap( adjust_pivot_vertical_bottom_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotVerticalBottom->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotVerticalBottom, 0, wxALL, 1 );
	
	m_btAdjustPivotBottomLeft = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotBottomLeft, wxBitmap( adjust_pivot_bottom_left_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotBottomLeft->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotBottomLeft, 0, wxALL, 1 );
	
	m_btAdjustPivotBottomCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotBottomCenter, wxBitmap( adjust_pivot_bottom_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotBottomCenter->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotBottomCenter, 0, wxALL, 1 );
	
	m_btAdjustPivotBottomRight = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotBottomRight, wxBitmap( adjust_pivot_bottom_right_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotBottomRight->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotBottomRight, 0, wxALL, 1 );
	
	
	gSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_btAdjustPivotHorizontalLeft = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotHorizontalLeft, wxBitmap( adjust_pivot_horizontal_left_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotHorizontalLeft->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotHorizontalLeft, 0, wxALL, 1 );
	
	m_btAdjustPivotHorizontalCenter = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotHorizontalCenter, wxBitmap( adjust_pivot_horizontal_center_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotHorizontalCenter->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotHorizontalCenter, 0, wxALL, 1 );
	
	m_btAdjustPivotHorizontalRight = new wxBitmapButton( m_pnFrameProps, ID_m_btAdjustPivotHorizontalRight, wxBitmap( adjust_pivot_horizontal_right_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	m_btAdjustPivotHorizontalRight->SetToolTip( wxT("Adjust the pivot of the selected frame to the bottom center.") );
	
	gSizer8->Add( m_btAdjustPivotHorizontalRight, 0, wxALL, 1 );
	
	
	bSizer221->Add( gSizer8, 0, wxALIGN_CENTER|wxTOP|wxBOTTOM, 12 );
	
	
	m_pnFrameProps->SetSizer( bSizer221 );
	m_pnFrameProps->Layout();
	sbSizer5->Add( m_pnFrameProps, 1, wxALL|wxEXPAND, 5 );
	
	
	ParteDerecha->Add( sbSizer5, 0, wxEXPAND, 5 );
	
	
	WxBoxSizer3->Add( ParteDerecha, 1, wxEXPAND, 5 );
	
	
	WxPanel1->SetSizer( WxBoxSizer3 );
	WxPanel1->Layout();
	WxBoxSizer3->Fit( WxPanel1 );
	WxBoxSizer1->Add( WxPanel1, 1, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( WxBoxSizer1 );
	this->Layout();
	WxMenuBar1 = new wxMenuBar( 0 );
	menu0 = new wxMenu();
	wxMenuItem* menuitem0;
	menuitem0 = new wxMenuItem( menu0, ID_MENUITEM0, wxString( wxT("&Load Sprite") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem0 );
	
	wxMenuItem* menuitem1;
	menuitem1 = new wxMenuItem( menu0, ID_MENUITEM1, wxString( wxT("&Save Sprite") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem1 );
	
	menu0->AppendSeparator();
	
	wxMenuItem* menuitem11;
	menuitem11 = new wxMenuItem( menu0, ID_MENUITEM11, wxString( wxT("&Save Sprite as animated GIF") ) + wxT('\t') + wxT("Ctrl+G"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem11 );
	
	menu0->AppendSeparator();
	
	wxMenuItem* menuitem3;
	menuitem3 = new wxMenuItem( menu0, ID_MENUITEM_3, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem3 );
	
	WxMenuBar1->Append( menu0, wxT("&File") ); 
	
	m_menu51 = new wxMenu();
	wxMenuItem* mo_GenerateHScroll;
	mo_GenerateHScroll = new wxMenuItem( m_menu51, ID_MO_GENHSCROLL, wxString( wxT("Generate Horizontal Scroll Action") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu51->Append( mo_GenerateHScroll );
	
	wxMenuItem* mo_GenerateVScroll;
	mo_GenerateVScroll = new wxMenuItem( m_menu51, ID_MO_GENVSCROLL, wxString( wxT("Generate Vertical Scroll Action") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu51->Append( mo_GenerateVScroll );
	
	WxMenuBar1->Append( m_menu51, wxT("&Misc") ); 
	
	m_menu5 = new wxMenu();
	wxMenuItem* mo_About;
	mo_About = new wxMenuItem( m_menu5, ID_MO_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( mo_About );
	
	WxMenuBar1->Append( m_menu5, wxT("Help") ); 
	
	this->SetMenuBar( WxMenuBar1 );
	
	CSpriteEditorGUIBAR = this->CreateStatusBar( 1, wxST_SIZEGRIP|wxDOUBLE_BORDER|wxSTATIC_BORDER, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CSpriteEditorGUIBase::CSpriteEditorGUIBaseOnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( CSpriteEditorGUIBase::CSpriteEditorGUIBaseOnIdle ) );
	m_gdSpriteActions->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( CSpriteEditorGUIBase::m_gdSpriteActionsOnGridCellLeftClick ), NULL, this );
	m_gdSpriteActions->Connect( wxEVT_GRID_CELL_LEFT_DCLICK, wxGridEventHandler( CSpriteEditorGUIBase::m_gdSpriteActionsOnGridCellLeftDClick ), NULL, this );
	m_gdSpriteActions->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( CSpriteEditorGUIBase::m_gdSpriteActionsOnGridCellRightClick ), NULL, this );
	m_btNewAction->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btNewActionOnButtonClick ), NULL, this );
	m_btActionUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btActionUpOnButtonClick ), NULL, this );
	m_btActionDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btActionDownOnButtonClick ), NULL, this );
	m_btDeleteAction->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btDeleteActionOnButtonClick ), NULL, this );
	bt_Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::OnPlayAnimationClick ), NULL, this );
	m_btReloadMaterials->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btReloadMaterialsOnButtonClick ), NULL, this );
	m_btCopyFrame->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btCopyFrameOnButtonClick ), NULL, this );
	m_btPasteFrame->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btPasteFrameOnButtonClick ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CSpriteEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	m_btNewFrame->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btNewFrameOnButtonClick ), NULL, this );
	m_btDeleteFrame->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btDeleteFrameOnButtonClick ), NULL, this );
	pn_Display->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CSpriteEditorGUIBase::pn_DisplayOnMouseWheel ), NULL, this );
	pn_Display->Connect( wxEVT_SIZE, wxSizeEventHandler( CSpriteEditorGUIBase::pn_DisplayOnSize ), NULL, this );
	m_cbSnapFactor->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbSnapFactorOnChoice ), NULL, this );
	m_btLoadMaterial->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btLoadMaterialOnButtonClick ), NULL, this );
	m_btClearMaterial->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btClearMaterialOnButtonClick ), NULL, this );
	m_cbSWrapMode->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbSWrapModeOnCombobox ), NULL, this );
	m_cbTWrapMode->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbTWrapModeOnCombobox ), NULL, this );
	m_cbFilter->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbFilterOnCombobox ), NULL, this );
	m_btAffectAllFrames->Connect( wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAffectAllFramesOnToggleButton ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldDelayBlendFact->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldDelayBlendFactOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_sldFrameTime->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CSpriteEditorGUIBase::m_sldFrameTimeOnCommandScroll ), NULL, this );
	m_btAutoAdjustTopLeft->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAutoAdjustTopLeftOnButtonClick ), NULL, this );
	m_btAutoAdjustCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAutoAdjustCenterOnButtonClick ), NULL, this );
	m_btAutoAdjustBottomCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAutoAdjustBottomCenterOnButtonClick ), NULL, this );
	m_btAutoAdjustSize->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAutoAdjustSizeOnButtonClick ), NULL, this );
	m_btAdjustSizeManually->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustSizeManuallyOnButtonClick ), NULL, this );
	m_cbScaleXUEqually->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbScaleXUEquallyOnCheckBox ), NULL, this );
	m_cbUWorldCoords->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbUWorldCoordsOnCheckBox ), NULL, this );
	m_cbScaleYVEqually->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbScaleYVEquallyOnCheckBox ), NULL, this );
	m_cbVWorldCoords->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_cbVWorldCoordsOnCheckBox ), NULL, this );
	m_edPivotImageXCoord->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CSpriteEditorGUIBase::m_edPivotImageXCoordOnKeyDown ), NULL, this );
	m_edPivotImageXCoord->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CSpriteEditorGUIBase::m_edPivotImageXCoordOnSpinCtrl ), NULL, this );
	m_edPivotImageXCoord->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CSpriteEditorGUIBase::m_edPivotImageXCoordOnSpinCtrlText ), NULL, this );
	m_edPivotImageYCoord->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CSpriteEditorGUIBase::m_edPivotImageYCoordOnKeyDown ), NULL, this );
	m_edPivotImageYCoord->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CSpriteEditorGUIBase::m_edPivotImageYCoordOnSpinCtrl ), NULL, this );
	m_edPivotImageYCoord->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CSpriteEditorGUIBase::m_edPivotImageYCoordOnSpinCtrlText ), NULL, this );
	m_btAdjustPivotVerticalTop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotVerticalTopOnButtonClick ), NULL, this );
	m_btAdjustPivotTopLeft->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotTopLeftOnButtonClick ), NULL, this );
	m_btAdjustPivotTopCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotTopCenterOnButtonClick ), NULL, this );
	m_btAdjustPivotTopRight->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotTopRightOnButtonClick ), NULL, this );
	m_btAdjustPivotVerticalCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotVerticalCenterOnButtonClick ), NULL, this );
	m_btAdjustPivotCenterLeft->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotCenterLeftOnButtonClick ), NULL, this );
	m_btAdjustPivotCenterCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotCenterCenterOnButtonClick ), NULL, this );
	m_btAdjustPivotCenterRight->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotCenterRightOnButtonClick ), NULL, this );
	m_btAdjustPivotVerticalBottom->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotVerticalBottomOnButtonClick ), NULL, this );
	m_btAdjustPivotBottomLeft->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotBottomLeftOnButtonClick ), NULL, this );
	m_btAdjustPivotBottomCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotBottomCenterOnButtonClick ), NULL, this );
	m_btAdjustPivotBottomRight->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotBottomRightOnButtonClick ), NULL, this );
	m_btAdjustPivotHorizontalLeft->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotHorizontalLeftOnButtonClick ), NULL, this );
	m_btAdjustPivotHorizontalCenter->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotHorizontalCenterOnButtonClick ), NULL, this );
	m_btAdjustPivotHorizontalRight->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CSpriteEditorGUIBase::m_btAdjustPivotHorizontalRightOnButtonClick ), NULL, this );
	this->Connect( menuitem0->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::menuitem0OnMenuSelection ) );
	this->Connect( menuitem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::menuitem1OnMenuSelection ) );
	this->Connect( menuitem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::menuitem11OnMenuSelection ) );
	this->Connect( menuitem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::menuitem3OnMenuSelection ) );
	this->Connect( mo_GenerateHScroll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::mo_GenerateHScrollOnMenuSelection ) );
	this->Connect( mo_GenerateVScroll->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::mo_GenerateVScrollOnMenuSelection ) );
	this->Connect( mo_About->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CSpriteEditorGUIBase::mo_AboutOnMenuSelection ) );
}

CSpriteEditorGUIBase::~CSpriteEditorGUIBase()
{
}
