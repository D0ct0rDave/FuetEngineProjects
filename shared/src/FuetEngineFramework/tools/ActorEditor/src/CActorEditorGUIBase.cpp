///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb 26 2014)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CActorEditorGUI_GridRenderers.h"
#include "wx/things/spinctld.h"
#include "wx/treelistctrl.h"

#include "CActorEditorGUIBase.h"

#include "../../common/Images/delete_icon.xpm"
#include "../../common/Images/load_sprite_icon.xpm"
#include "../../common/Images/redo_disabled_icon.xpm"
#include "../../common/Images/redo_icon.xpm"
#include "../../common/Images/undo_disabled_icon.xpm"
#include "../../common/Images/undo_icon.xpm"

///////////////////////////////////////////////////////////////////////////

CActorEditorGUIBase::CActorEditorGUIBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxColour( 186, 202, 209 ) );
	
	wxBoxSizer* WxBoxSizer1;
	WxBoxSizer1 = new wxBoxSizer( wxVERTICAL );
	
	WxPanel1 = new wxPanel( this, wxID_ANY, wxPoint( 5,5 ), wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	wxBoxSizer* WxBoxSizer3;
	WxBoxSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* WxBoxSizer5;
	WxBoxSizer5 = new wxBoxSizer( wxVERTICAL );
	
	WxBoxSizer5->SetMinSize( wxSize( 660,-1 ) ); 
	m_pnDisplay = new wxPanel( WxPanel1, ID_PN_DISPLAY, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	m_pnDisplay->SetMinSize( wxSize( 480,360 ) );
	
	WxBoxSizer5->Add( m_pnDisplay, 3, wxALIGN_CENTER|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* WxBoxSizer2;
	WxBoxSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	WxBoxSizer2->SetMinSize( wxSize( -1,20 ) ); 
	lb_Frame = new wxStaticText( WxPanel1, wxID_ANY, wxT("Coords:-----------"), wxPoint( 5,5 ), wxSize( 138,-1 ), wxALIGN_CENTRE );
	lb_Frame->Wrap( -1 );
	WxBoxSizer2->Add( lb_Frame, 0, wxALL, 5 );
	
	lb_Trans = new wxStaticText( WxPanel1, wxID_ANY, wxT("Offset:-------------------------"), wxPoint( 199,13 ), wxSize( 139,17 ), 0 );
	lb_Trans->Wrap( 0 );
	WxBoxSizer2->Add( lb_Trans, 0, wxALL, 5 );
	
	lb_Zoom = new wxStaticText( WxPanel1, wxID_ANY, wxT("Zoom:--------------"), wxPoint( 348,13 ), wxSize( 90,17 ), 0 );
	lb_Zoom->Wrap( 0 );
	WxBoxSizer2->Add( lb_Zoom, 0, wxALL, 5 );
	
	
	WxBoxSizer5->Add( WxBoxSizer2, 0, wxALIGN_CENTER, 2 );
	
	wxBoxSizer* bSizer202;
	bSizer202 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticBoxSizer* sbSizer41;
	sbSizer41 = new wxStaticBoxSizer( new wxStaticBox( WxPanel1, wxID_ANY, wxT("Background:") ), wxHORIZONTAL );
	
	cb_ShowBackground = new wxCheckBox( WxPanel1, wxID_ANY, wxT("Show"), wxPoint( 448,13 ), wxSize( -1,-1 ), 0 );
	cb_ShowBackground->SetValue(true); 
	sbSizer41->Add( cb_ShowBackground, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText1611 = new wxStaticText( WxPanel1, wxID_ANY, wxT("Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1611->Wrap( -1 );
	sbSizer41->Add( m_staticText1611, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edBackXOffset = new wxSpinCtrlDbl( WxPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, -4000, 4000, 1 );
	sbSizer41->Add( m_edBackXOffset, 0, wxALL, 5 );
	
	m_edBackYOffset = new wxSpinCtrlDbl( WxPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, -4000, 4000, 1 );
	sbSizer41->Add( m_edBackYOffset, 0, wxALL, 5 );
	
	m_staticText161 = new wxStaticText( WxPanel1, wxID_ANY, wxT("Scale"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText161->Wrap( -1 );
	sbSizer41->Add( m_staticText161, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edBackXScale = new wxSpinCtrlDbl( WxPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, -4000, 4000, 2 );
	sbSizer41->Add( m_edBackXScale, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edBackYScale = new wxSpinCtrlDbl( WxPanel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxSP_ARROW_KEYS, -4000, 4000, 1 );
	sbSizer41->Add( m_edBackYScale, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer202->Add( sbSizer41, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer5;
	sbSizer5 = new wxStaticBoxSizer( new wxStaticBox( WxPanel1, wxID_ANY, wxT("Show:") ), wxHORIZONTAL );
	
	cb_ShowLabels = new wxCheckBox( WxPanel1, wxID_ANY, wxT("Labels"), wxPoint( 448,13 ), wxSize( -1,-1 ), 0 );
	cb_ShowLabels->SetValue(true); 
	sbSizer5->Add( cb_ShowLabels, 0, wxALL|wxEXPAND, 5 );
	
	cb_ShowBones = new wxCheckBox( WxPanel1, ID_CB_SHOWSPRITES, wxT("Bones"), wxPoint( 555,13 ), wxSize( -1,-1 ), 0 );
	cb_ShowBones->SetValue(true); 
	sbSizer5->Add( cb_ShowBones, 0, wxALL|wxEXPAND, 5 );
	
	cb_ShowSprites = new wxCheckBox( WxPanel1, ID_CB_SHOWSPRITES, wxT("Sprites"), wxPoint( 555,13 ), wxSize( -1,-1 ), 0 );
	cb_ShowSprites->SetValue(true); 
	sbSizer5->Add( cb_ShowSprites, 0, wxALL|wxEXPAND, 5 );
	
	m_sldSpritesAlpha = new wxSlider( WxPanel1, wxID_ANY, 255, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
	m_sldSpritesAlpha->SetMaxSize( wxSize( 60,-1 ) );
	
	sbSizer5->Add( m_sldSpritesAlpha, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer202->Add( sbSizer5, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( WxPanel1, wxID_ANY, wxT("Edit Mode:") ), wxVERTICAL );
	
	wxString m_cbEditModeChoices[] = { wxT("Skeleton"), wxT("Animation") };
	int m_cbEditModeNChoices = sizeof( m_cbEditModeChoices ) / sizeof( wxString );
	m_cbEditMode = new wxChoice( WxPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cbEditModeNChoices, m_cbEditModeChoices, 0 );
	m_cbEditMode->SetSelection( 0 );
	sbSizer6->Add( m_cbEditMode, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 2 );
	
	
	bSizer202->Add( sbSizer6, 0, wxEXPAND, 5 );
	
	
	WxBoxSizer5->Add( bSizer202, 0, wxEXPAND, 2 );
	
	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer( wxHORIZONTAL );
	
	m_btUndo = new wxBitmapButton( WxPanel1, wxID_UNDO, wxBitmap( undo_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_btUndo->SetBitmapDisabled( wxBitmap( undo_disabled_icon_xpm ) );
	m_btUndo->Enable( false );
	
	bSizer20->Add( m_btUndo, 0, wxALL, 5 );
	
	m_btRedo = new wxBitmapButton( WxPanel1, wxID_REDO, wxBitmap( redo_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	
	m_btRedo->SetBitmapDisabled( wxBitmap( redo_disabled_icon_xpm ) );
	m_btRedo->Enable( false );
	
	bSizer20->Add( m_btRedo, 0, wxALL, 5 );
	
	m_cbUpdateScrollBar = new wxCheckBox( WxPanel1, wxID_ANY, wxT("Update on Play?"), wxPoint( 448,13 ), wxSize( -1,-1 ), 0 );
	m_cbUpdateScrollBar->SetValue(true); 
	m_cbUpdateScrollBar->SetMinSize( wxSize( 50,-1 ) );
	
	bSizer20->Add( m_cbUpdateScrollBar, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND|wxFIXED_MINSIZE, 5 );
	
	sb_KeyFrame = new wxScrollBar( WxPanel1, ID_SB_KEYFRAME, wxPoint( 5,495 ), wxSize( 640,17 ), wxSB_HORIZONTAL );
	sb_KeyFrame->SetMinSize( wxSize( 640,-1 ) );
	sb_KeyFrame->SetMaxSize( wxSize( 640,-1 ) );
	
	bSizer20->Add( sb_KeyFrame, 1, wxALIGN_CENTER|wxALL|wxEXPAND, 5 );
	
	bt_AddKeyframe = new wxButton( WxPanel1, ID_BT_ADDKEYFRAME, wxT("+"), wxDefaultPosition, wxSize( 24,-1 ), 0 );
	bSizer20->Add( bt_AddKeyframe, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	bt_DelKeyframe = new wxButton( WxPanel1, ID_BT_DECKEYFRAME, wxT("-"), wxDefaultPosition, wxSize( 24,-1 ), 0 );
	bSizer20->Add( bt_DelKeyframe, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );
	
	
	WxBoxSizer5->Add( bSizer20, 0, wxEXPAND, 5 );
	
	m_gdTimeline = new wxMyGrid( WxPanel1, ID_M_GDTIMELINE, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	
	// Grid
	m_gdTimeline->CreateGrid( 20, 1 );
	m_gdTimeline->EnableEditing( false );
	m_gdTimeline->EnableGridLines( false );
	m_gdTimeline->EnableDragGridSize( false );
	m_gdTimeline->SetMargins( 0, 0 );
	
	// Columns
	m_gdTimeline->EnableDragColMove( false );
	m_gdTimeline->EnableDragColSize( false );
	m_gdTimeline->SetColLabelSize( 0 );
	m_gdTimeline->SetColLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Rows
	m_gdTimeline->EnableDragRowSize( true );
	m_gdTimeline->SetRowLabelSize( 0 );
	m_gdTimeline->SetRowLabelAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
	
	// Label Appearance
	
	// Cell Defaults
	m_gdTimeline->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	m_gdTimeline->SetMinSize( wxSize( -1,160 ) );
	
	WxBoxSizer5->Add( m_gdTimeline, 2, wxBOTTOM|wxFIXED_MINSIZE|wxLEFT|wxRIGHT|wxEXPAND, 2 );
	
	
	WxBoxSizer3->Add( WxBoxSizer5, 2, wxFIXED_MINSIZE|wxEXPAND, 5 );
	
	wxBoxSizer* WxBoxSizer4;
	WxBoxSizer4 = new wxBoxSizer( wxVERTICAL );
	
	WxBoxSizer4->SetMinSize( wxSize( 240,-1 ) ); 
	pn_PreviewDisplay = new wxPanel( WxPanel1, wxID_ANY, wxPoint( 5,152 ), wxSize( 256,150 ), wxTAB_TRAVERSAL );
	pn_PreviewDisplay->SetMinSize( wxSize( 256,150 ) );
	pn_PreviewDisplay->SetMaxSize( wxSize( 256,150 ) );
	
	WxBoxSizer4->Add( pn_PreviewDisplay, 0, wxALIGN_CENTER_HORIZONTAL|wxALL|wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* WxBoxSizer10;
	WxBoxSizer10 = new wxBoxSizer( wxHORIZONTAL );
	
	cb_ShowPrevBones = new wxCheckBox( WxPanel1, ID_CB_SHOWPREVBONES, wxT("Show Bones"), wxPoint( 5,5 ), wxSize( 97,-1 ), 0 );
	WxBoxSizer10->Add( cb_ShowPrevBones, 0, wxALL, 5 );
	
	cb_ShowGrid = new wxCheckBox( WxPanel1, ID_CB_SHOWPREVGRID, wxT("Show Grid"), wxPoint( 5,5 ), wxSize( 97,-1 ), 0 );
	WxBoxSizer10->Add( cb_ShowGrid, 0, wxALL, 5 );
	
	cb_ShapeMode = new wxCheckBox( WxPanel1, ID_CB_SHOWPREVGRID, wxT("Silhouette Mode"), wxPoint( 5,5 ), wxSize( -1,-1 ), 0 );
	WxBoxSizer10->Add( cb_ShapeMode, 0, wxALL|wxEXPAND, 5 );
	
	
	WxBoxSizer4->Add( WxBoxSizer10, 0, wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE, 5 );
	
	wxBoxSizer* WxBoxSizer7;
	WxBoxSizer7 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* WxBoxSizer6;
	WxBoxSizer6 = new wxBoxSizer( wxHORIZONTAL );
	
	bt_Play = new wxButton( WxPanel1, ID_PLAYANIM, wxT("Play"), wxPoint( 5,5 ), wxSize( 75,25 ), 0 );
	bt_Play->SetDefault(); 
	WxBoxSizer6->Add( bt_Play, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	cb_Loop = new wxCheckBox( WxPanel1, ID_LOOPANIMATION, wxT("Loop"), wxPoint( 90,9 ), wxSize( 40,17 ), 0 );
	WxBoxSizer6->Add( cb_Loop, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	
	WxBoxSizer7->Add( WxBoxSizer6, 0, wxEXPAND, 5 );
	
	sld_AnimTime = new wxSlider( WxPanel1, ID_ANIMTIME, 1, 1, 60000, wxPoint( 150,5 ), wxSize( 150,-1 ), wxSL_BOTH );
	WxBoxSizer7->Add( sld_AnimTime, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3 );
	
	m_edAnimTime = new wxSpinCtrlDbl( WxPanel1, ID_M_EDBONEXCOORD, wxEmptyString, wxDefaultPosition, wxSize( 60,-1 ), wxSP_ARROW_KEYS, 1, 60000, 100 );
	WxBoxSizer7->Add( m_edAnimTime, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxFIXED_MINSIZE, 3 );
	
	m_staticText19 = new wxStaticText( WxPanel1, wxID_ANY, wxT("milis"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText19->Wrap( -1 );
	WxBoxSizer7->Add( m_staticText19, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3 );
	
	m_cbAnimatePivot = new wxCheckBox( WxPanel1, wxID_ANIMATEPIVOT, wxT("Animate Pivot"), wxPoint( 5,5 ), wxSize( 97,17 ), 0 );
	WxBoxSizer7->Add( m_cbAnimatePivot, 0, wxALL|wxEXPAND, 3 );
	
	
	WxBoxSizer4->Add( WxBoxSizer7, 0, wxFIXED_MINSIZE|wxEXPAND, 5 );
	
	pnIconLayer = new wxPanel( WxPanel1, ID_PNICONLAYER, wxDefaultPosition, wxSize( -1,-1 ), wxTAB_TRAVERSAL );
	pnIconLayer->SetMinSize( wxSize( 200,200 ) );
	
	sbSizerBoneProps = new wxStaticBoxSizer( new wxStaticBox( pnIconLayer, ID_SBSIZERBONEPROPS, wxT("Bone Props:") ), wxVERTICAL );
	
	wxStaticBoxSizer* sbSizer51;
	sbSizer51 = new wxStaticBoxSizer( new wxStaticBox( pnIconLayer, wxID_ANY, wxT("Bone Position:") ), wxHORIZONTAL );
	
	m_staticText12 = new wxStaticText( pnIconLayer, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	sbSizer51->Add( m_staticText12, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edBoneXCoord = new wxSpinCtrlDbl( pnIconLayer, ID_M_EDBONEXCOORD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -4000, 4000, -1 );
	sbSizer51->Add( m_edBoneXCoord, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText13 = new wxStaticText( pnIconLayer, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText13->Wrap( -1 );
	sbSizer51->Add( m_staticText13, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edBoneYCoord = new wxSpinCtrlDbl( pnIconLayer, ID_M_EDBONEYCOORD, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -4000, 4000, 0 );
	sbSizer51->Add( m_edBoneYCoord, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_lbBoneOffset = new wxStaticText( pnIconLayer, wxID_ANY, wxT("Offset"), wxDefaultPosition, wxDefaultSize, 0 );
	m_lbBoneOffset->Wrap( -1 );
	m_lbBoneOffset->SetMinSize( wxSize( 130,-1 ) );
	
	sbSizer51->Add( m_lbBoneOffset, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_btResetBonePos = new wxButton( pnIconLayer, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer51->Add( m_btResetBonePos, 0, wxALL, 5 );
	
	
	sbSizerBoneProps->Add( sbSizer51, 0, wxEXPAND, 5 );
	
	m_poBoneNodes = new wxTreeListCtrl( pnIconLayer, ID_M_POBONENODES, wxDefaultPosition, wxDefaultSize, wxTR_FULL_ROW_HIGHLIGHT|wxTR_HAS_BUTTONS|wxTR_HIDE_ROOT|wxTR_ROW_LINES|wxTR_SINGLE );
	sbSizerBoneProps->Add( m_poBoneNodes, 1, wxALL|wxEXPAND, 5 );
	
	m_pnNodeProps = new wxPanel( pnIconLayer, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer( new wxStaticBox( m_pnNodeProps, wxID_ANY, wxT("Selected Node properties:") ), wxVERTICAL );
	
	wxBoxSizer* bSizer1722;
	bSizer1722 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText822 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Position:"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText822->Wrap( -1 );
	bSizer1722->Add( m_staticText822, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxBoxSizer* bSizer25;
	bSizer25 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText121 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("X:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText121->Wrap( -1 );
	bSizer25->Add( m_staticText121, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edNodePosX = new wxSpinCtrlDbl( m_pnNodeProps, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10000, 0 );
	bSizer25->Add( m_edNodePosX, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText131 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Y:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer25->Add( m_staticText131, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edNodePosY = new wxSpinCtrlDbl( m_pnNodeProps, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -10000, 10000, 0 );
	bSizer25->Add( m_edNodePosY, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer1722->Add( bSizer25, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1721;
	bSizer1721 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText821 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Rotation:"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText821->Wrap( -1 );
	bSizer1721->Add( m_staticText821, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldNodeAngle = new wxSlider( m_pnNodeProps, ID_M_SLDNODEANGLE, 0, -180, 180, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer1721->Add( m_sldNodeAngle, 1, wxALL, 5 );
	
	
	bSizer1722->Add( bSizer1721, 0, wxEXPAND, 5 );
	
	
	sbSizer4->Add( bSizer1722, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer172;
	bSizer172 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText82 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Depth:"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText82->Wrap( -1 );
	bSizer172->Add( m_staticText82, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edNodeDepth = new wxSpinCtrlDbl( m_pnNodeProps, ID_M_EDNODEDEPTH, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100, 100, 0 );
	bSizer172->Add( m_edNodeDepth, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText823 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Color:"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText823->Wrap( -1 );
	bSizer172->Add( m_staticText823, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_cpSelectColor = new wxColourPickerCtrl( m_pnNodeProps, ID_M_CPSELECTCOLOR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE );
	bSizer172->Add( m_cpSelectColor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText8221 = new wxStaticText( m_pnNodeProps, wxID_ANY, wxT("Alpha:"), wxDefaultPosition, wxSize( 50,-1 ), 0 );
	m_staticText8221->Wrap( -1 );
	bSizer172->Add( m_staticText8221, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_sldNodeAlpha = new wxSlider( m_pnNodeProps, ID_M_SLDNODEALPHA, 0, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	bSizer172->Add( m_sldNodeAlpha, 1, wxALL|wxEXPAND, 5 );
	
	
	sbSizer4->Add( bSizer172, 0, wxEXPAND, 5 );
	
	m_pnSpriteProps = new wxPanel( m_pnNodeProps, ID_M_PNSPRITEPROPS, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* WxBoxSizer8;
	WxBoxSizer8 = new wxBoxSizer( wxHORIZONTAL );
	
	WxStaticText4 = new wxStaticText( m_pnSpriteProps, wxID_ANY, wxT("Sprite:"), wxPoint( 32,18 ), wxSize( 50,-1 ), 0 );
	WxStaticText4->Wrap( -1 );
	WxBoxSizer8->Add( WxStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_edIconSprite = new wxTextCtrl( m_pnSpriteProps, ID_ED_ICONSPRITE, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), wxTE_READONLY );
	m_edIconSprite->SetMaxLength( 0 ); 
	WxBoxSizer8->Add( m_edIconSprite, 1, wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT|wxALL, 5 );
	
	bt_LoadSprite = new wxBitmapButton( m_pnSpriteProps, ID_LOADSPRITE, wxBitmap( load_sprite_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	WxBoxSizer8->Add( bt_LoadSprite, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	bt_ClearSprite = new wxBitmapButton( m_pnSpriteProps, ID_CLEARSPRITE, wxBitmap( delete_icon_xpm ), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	WxBoxSizer8->Add( bt_ClearSprite, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer18->Add( WxBoxSizer8, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer( wxHORIZONTAL );
	
	WxStaticText41 = new wxStaticText( m_pnSpriteProps, wxID_ANY, wxT("Action:"), wxPoint( 32,18 ), wxSize( 50,-1 ), 0 );
	WxStaticText41->Wrap( -1 );
	bSizer211->Add( WxStaticText41, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	wxArrayString m_cbSpriteActionListChoices;
	m_cbSpriteActionList = new wxChoice( m_pnSpriteProps, ID_M_CBACTIONLST, wxDefaultPosition, wxDefaultSize, m_cbSpriteActionListChoices, 0 );
	m_cbSpriteActionList->SetSelection( 0 );
	bSizer211->Add( m_cbSpriteActionList, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	
	bSizer18->Add( bSizer211, 1, wxEXPAND, 5 );
	
	
	m_pnSpriteProps->SetSizer( bSizer18 );
	m_pnSpriteProps->Layout();
	bSizer18->Fit( m_pnSpriteProps );
	sbSizer4->Add( m_pnSpriteProps, 1, wxALL|wxEXPAND, 0 );
	
	
	m_pnNodeProps->SetSizer( sbSizer4 );
	m_pnNodeProps->Layout();
	sbSizer4->Fit( m_pnNodeProps );
	sbSizerBoneProps->Add( m_pnNodeProps, 0, wxALL|wxEXPAND, 5 );
	
	
	pnIconLayer->SetSizer( sbSizerBoneProps );
	pnIconLayer->Layout();
	sbSizerBoneProps->Fit( pnIconLayer );
	WxBoxSizer4->Add( pnIconLayer, 1, wxALL|wxFIXED_MINSIZE|wxEXPAND, 5 );
	
	
	WxBoxSizer3->Add( WxBoxSizer4, 1, wxFIXED_MINSIZE|wxEXPAND, 5 );
	
	
	WxPanel1->SetSizer( WxBoxSizer3 );
	WxPanel1->Layout();
	WxBoxSizer3->Fit( WxPanel1 );
	WxBoxSizer1->Add( WxPanel1, 1, wxALL|wxEXPAND, 5 );
	
	TRICKY_MENU_PANEL = new wxPanel( this, ID_TRICKY_MENU_PANEL, wxDefaultPosition, wxSize( -1,80 ), wxTAB_TRAVERSAL );
	TRICKY_MENU_PANEL->Hide();
	TRICKY_MENU_PANEL->SetMinSize( wxSize( -1,80 ) );
	TRICKY_MENU_PANEL->SetMaxSize( wxSize( -1,80 ) );
	
	m_pupmKeyFrameOps = new wxMenu();
	wxMenuItem* m_menuItem121;
	m_menuItem121 = new wxMenuItem( m_pupmKeyFrameOps, ID_MNU_COPYPOSE, wxString( wxT("Copy current Pose values") ) , wxEmptyString, wxITEM_NORMAL );
	m_pupmKeyFrameOps->Append( m_menuItem121 );
	
	wxMenuItem* m_menuItem131;
	m_menuItem131 = new wxMenuItem( m_pupmKeyFrameOps, ID_MNU_PASTEPOSE, wxString( wxT("Paste copied Pose values (Optimized)") ) , wxEmptyString, wxITEM_NORMAL );
	m_pupmKeyFrameOps->Append( m_menuItem131 );
	
	wxMenuItem* m_menuItem1311;
	m_menuItem1311 = new wxMenuItem( m_pupmKeyFrameOps, ID_MNU_PASTEPOSEFORCE, wxString( wxT("Paste copied Pose values (Forced)") ) , wxEmptyString, wxITEM_NORMAL );
	m_pupmKeyFrameOps->Append( m_menuItem1311 );
	
	wxMenuItem* m_menuItem14;
	m_menuItem14 = new wxMenuItem( m_pupmKeyFrameOps, wxID_ANY, wxString( wxT("Clear Current Pose Keyframes") ) , wxEmptyString, wxITEM_NORMAL );
	m_pupmKeyFrameOps->Append( m_menuItem14 );
	
	wxMenuItem* m_menuItem141;
	m_menuItem141 = new wxMenuItem( m_pupmKeyFrameOps, ID_M_MENUITEM14, wxString( wxT("Reset Current Pose to Skeleton Pose") ) , wxEmptyString, wxITEM_NORMAL );
	m_pupmKeyFrameOps->Append( m_menuItem141 );
	
	m_pupmKeyFrameOps->AppendSeparator();
	
	m_menu3 = new wxMenu();
	wxMenuItem* m_miPoseSelState;
	m_miPoseSelState = new wxMenuItem( m_menu3, wxID_POSE_SEL_STATE, wxString( wxT("Start Selection of Poses") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_miPoseSelState );
	
	wxMenuItem* m_miInsertPoseSel;
	m_miInsertPoseSel = new wxMenuItem( m_menu3, wxID_POSE_SEL_COPY, wxString( wxT("Insert Selection of Poses") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_miInsertPoseSel );
	
	wxMenuItem* m_miClearPoseSel;
	m_miClearPoseSel = new wxMenuItem( m_menu3, wxID_POSE_SEL_CLEAR, wxString( wxT("Clear Selection of Poses") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_miClearPoseSel );
	
	wxMenuItem* m_miDeletePoseSel;
	m_miDeletePoseSel = new wxMenuItem( m_menu3, wxID_POSE_SEL_DELETE, wxString( wxT("Delete Selection of Poses") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu3->Append( m_miDeletePoseSel );
	
	m_pupmKeyFrameOps->Append( -1, wxT("Block of Poses"), m_menu3 );
	
	TRICKY_MENU_PANEL->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CActorEditorGUIBase::TRICKY_MENU_PANELOnContextMenu ), NULL, this ); 
	
	wxBoxSizer* bSizer181;
	bSizer181 = new wxBoxSizer( wxVERTICAL );
	
	m_panel6 = new wxPanel( TRICKY_MENU_PANEL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_popKeyFrameMenu = new wxMenu();
	wxMenuItem* m_menuItem17;
	m_menuItem17 = new wxMenuItem( m_popKeyFrameMenu, ID_MNU_COPYINIVAL, wxString( wxT("Copy initial values") ) , wxEmptyString, wxITEM_NORMAL );
	m_popKeyFrameMenu->Append( m_menuItem17 );
	
	wxMenuItem* m_menuItem16;
	m_menuItem16 = new wxMenuItem( m_popKeyFrameMenu, ID_MNU_DELETEKEYFRAME, wxString( wxT("Delete Keyframe") ) , wxEmptyString, wxITEM_NORMAL );
	m_popKeyFrameMenu->Append( m_menuItem16 );
	
	m_popKeyFrameMenu->AppendSeparator();
	
	m_menu2 = new wxMenu();
	wxMenuItem* m_menuItem29;
	m_menuItem29 = new wxMenuItem( m_menu2, ID_MNU_CONSTANT, wxString( wxT("Constant") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem29 );
	
	wxMenuItem* m_menuItem18;
	m_menuItem18 = new wxMenuItem( m_menu2, ID_MNU_LINEAR, wxString( wxT("Linear") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem18 );
	
	wxMenuItem* m_menuItem19;
	m_menuItem19 = new wxMenuItem( m_menu2, ID_MNU_SINUSOIDAL, wxString( wxT("Sinusoidal") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem19 );
	
	wxMenuItem* m_menuItem20;
	m_menuItem20 = new wxMenuItem( m_menu2, ID_MNU_EXPONENTIAL, wxString( wxT("Exponential") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem20 );
	
	wxMenuItem* m_menuItem30;
	m_menuItem30 = new wxMenuItem( m_menu2, ID_MNU_INVSIN, wxString( wxT("Inverse Sin") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem30 );
	
	wxMenuItem* m_menuItem21;
	m_menuItem21 = new wxMenuItem( m_menu2, ID_MNU_RANDOM, wxString( wxT("Random") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem21 );
	
	wxMenuItem* m_menuItem22;
	m_menuItem22 = new wxMenuItem( m_menu2, ID_MNU_SINSIN, wxString( wxT("SinSin") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem22 );
	
	wxMenuItem* m_menuItem23;
	m_menuItem23 = new wxMenuItem( m_menu2, ID_MNU_EXPLOG, wxString( wxT("ExpLog") ) , wxEmptyString, wxITEM_CHECK );
	m_menu2->Append( m_menuItem23 );
	
	m_popKeyFrameMenu->Append( -1, wxT("Select Interpolation"), m_menu2 );
	
	m_panel6->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_panel6OnContextMenu ), NULL, this ); 
	
	wxBoxSizer* bSizer191;
	bSizer191 = new wxBoxSizer( wxVERTICAL );
	
	m_panel7 = new wxPanel( m_panel6, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_popKeyFrameFuncMenu = new wxMenu();
	wxMenuItem* m_menuItem161;
	m_menuItem161 = new wxMenuItem( m_popKeyFrameFuncMenu, ID_M_MENUITEM161, wxString( wxT("Delete All the Keyframes") ) , wxEmptyString, wxITEM_NORMAL );
	m_popKeyFrameFuncMenu->Append( m_menuItem161 );
	
	m_popKeyFrameFuncMenu->AppendSeparator();
	
	m_menu21 = new wxMenu();
	wxMenuItem* m_menuItem291;
	m_menuItem291 = new wxMenuItem( m_menu21, ID_M_MENUITEM291, wxString( wxT("Constant") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem291 );
	
	wxMenuItem* m_menuItem181;
	m_menuItem181 = new wxMenuItem( m_menu21, ID_M_MENUITEM181, wxString( wxT("Linear") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem181 );
	
	wxMenuItem* m_menuItem191;
	m_menuItem191 = new wxMenuItem( m_menu21, ID_M_MENUITEM191, wxString( wxT("Sinusoidal") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem191 );
	
	wxMenuItem* m_menuItem201;
	m_menuItem201 = new wxMenuItem( m_menu21, ID_M_MENUITEM201, wxString( wxT("Exponential") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem201 );
	
	wxMenuItem* m_menuItem301;
	m_menuItem301 = new wxMenuItem( m_menu21, ID_M_MENUITEM301, wxString( wxT("Inverse Sin") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem301 );
	
	wxMenuItem* m_menuItem211;
	m_menuItem211 = new wxMenuItem( m_menu21, ID_M_MENUITEM211, wxString( wxT("Random") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem211 );
	
	wxMenuItem* m_menuItem221;
	m_menuItem221 = new wxMenuItem( m_menu21, ID_M_MENUITEM221, wxString( wxT("SinSin") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem221 );
	
	wxMenuItem* m_menuItem231;
	m_menuItem231 = new wxMenuItem( m_menu21, ID_M_MENUITEM231, wxString( wxT("ExpLog") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu21->Append( m_menuItem231 );
	
	m_popKeyFrameFuncMenu->Append( -1, wxT("Select Interpolation for all keyframes"), m_menu21 );
	
	m_panel7->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_panel7OnContextMenu ), NULL, this ); 
	
	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer( wxVERTICAL );
	
	
	m_panel7->SetSizer( bSizer201 );
	m_panel7->Layout();
	bSizer201->Fit( m_panel7 );
	bSizer191->Add( m_panel7, 1, wxEXPAND | wxALL, 5 );
	
	
	m_panel6->SetSizer( bSizer191 );
	m_panel6->Layout();
	bSizer191->Fit( m_panel6 );
	bSizer181->Add( m_panel6, 1, wxEXPAND | wxALL, 5 );
	
	
	TRICKY_MENU_PANEL->SetSizer( bSizer181 );
	TRICKY_MENU_PANEL->Layout();
	WxBoxSizer1->Add( TRICKY_MENU_PANEL, 1, wxEXPAND | wxALL, 5 );
	
	
	this->SetSizer( WxBoxSizer1 );
	this->Layout();
	m_mbMainMenu = new wxMenuBar( 0 );
	menu0 = new wxMenu();
	wxMenuItem* menuitem0;
	menuitem0 = new wxMenuItem( menu0, ID_MENU_LOADANIM, wxString( wxT("&Load Actor Anim") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem0 );
	
	wxMenuItem* menuitem1;
	menuitem1 = new wxMenuItem( menu0, ID_MENU_SAVEANIM, wxString( wxT("&Save Actor Anim") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem1 );
	
	wxMenuItem* menuitem11;
	menuitem11 = new wxMenuItem( menu0, ID_MENUI_SAVEASANIM, wxString( wxT("&Save Actor Anim As ...") ) + wxT('\t') + wxT("CTRL+ALT+S"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem11 );
	
	menu0->AppendSeparator();
	
	wxMenuItem* m_menuItem12;
	m_menuItem12 = new wxMenuItem( menu0, ID_MENU_IMPORTSKELETON, wxString( wxT("Import Skeleton") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_menuItem12 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( menu0, ID_MENU_EXPORTSKELETON, wxString( wxT("Export Skeleton") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_menuItem10 );
	
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( menu0, ID_MENU_EXPORT_ANIMATION, wxString( wxT("Export Skeleton Action") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_menuItem11 );
	
	menu0->AppendSeparator();
	
	wxMenuItem* m_moRestartWithSkelDef;
	m_moRestartWithSkelDef = new wxMenuItem( menu0, wxID_RESTART_WITH_SKEL_DEF, wxString( wxT("Restart with new Skeleton Definition") ) , wxEmptyString, wxITEM_NORMAL );
	menu0->Append( m_moRestartWithSkelDef );
	
	menu0->AppendSeparator();
	
	wxMenuItem* menuitem3;
	menuitem3 = new wxMenuItem( menu0, ID_MENU_QUIT, wxString( wxT("&Quit") ) + wxT('\t') + wxT("Ctrl+Q"), wxEmptyString, wxITEM_NORMAL );
	menu0->Append( menuitem3 );
	
	m_mbMainMenu->Append( menu0, wxT("&File") ); 
	
	menu1 = new wxMenu();
	wxMenuItem* m_miUndo;
	m_miUndo = new wxMenuItem( menu1, wxID_UNDO, wxString( wxT("Undo") ) + wxT('\t') + wxT("Ctrl+Z"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( m_miUndo );
	
	wxMenuItem* m_miRedo;
	m_miRedo = new wxMenuItem( menu1, wxID_REDO, wxString( wxT("Redo") ) + wxT('\t') + wxT("Ctrl+Shift+Z"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( m_miRedo );
	
	menu1->AppendSeparator();
	
	wxMenuItem* m_miReloadSprites;
	m_miReloadSprites = new wxMenuItem( menu1, wxID_RELOADMATERIALS, wxString( wxT("Reload Sprites and Textures") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( m_miReloadSprites );
	
	menu1->AppendSeparator();
	
	wxMenuItem* menuitem4;
	menuitem4 = new wxMenuItem( menu1, ID_MENUITEM4, wxString( wxT("Export Current Pose as Standard Pose") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuitem4 );
	
	wxMenuItem* menuitem5;
	menuitem5 = new wxMenuItem( menu1, ID_MENUITEM5, wxString( wxT("Import Here Standard Pose") ) + wxT('\t') + wxT("Ctrl+Shift+P"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuitem5 );
	
	menu1->AppendSeparator();
	
	wxMenuItem* menuitem7;
	menuitem7 = new wxMenuItem( menu1, ID_MENUITEM7, wxString( wxT("Select Background Color") ) + wxT('\t') + wxT("Ctrl+B"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuitem7 );
	
	wxMenuItem* menuitem71;
	menuitem71 = new wxMenuItem( menu1, ID_MENUITEM71, wxString( wxT("Load Background Video or Image") ) + wxT('\t') + wxT("Ctrl+L"), wxEmptyString, wxITEM_NORMAL );
	menu1->Append( menuitem71 );
	
	menu1->AppendSeparator();
	
	wxMenuItem* m_menuItem13;
	m_menuItem13 = new wxMenuItem( menu1, ID_MENUITEM13, wxString( wxT("Reset Animation to Skeleton Pose") ) , wxEmptyString, wxITEM_NORMAL );
	menu1->Append( m_menuItem13 );
	
	m_mbMainMenu->Append( menu1, wxT("&Misc") ); 
	
	m_menu5 = new wxMenu();
	wxMenuItem* mo_About;
	mo_About = new wxMenuItem( m_menu5, ID_MO_ABOUT, wxString( wxT("About") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu5->Append( mo_About );
	
	m_mbMainMenu->Append( m_menu5, wxT("Help") ); 
	
	this->SetMenuBar( m_mbMainMenu );
	
	CActorEditorGUIBAR = this->CreateStatusBar( 1, wxST_SIZEGRIP|wxDOUBLE_BORDER|wxSTATIC_BORDER, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CActorEditorGUIBase::GUIOnClose ) );
	this->Connect( wxEVT_IDLE, wxIdleEventHandler( CActorEditorGUIBase::GUIOnIdle ) );
	m_pnDisplay->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CActorEditorGUIBase::m_pnDisplayOnKeyDown ), NULL, this );
	m_pnDisplay->Connect( wxEVT_KEY_UP, wxKeyEventHandler( CActorEditorGUIBase::m_pnDisplayOnKeyUp ), NULL, this );
	m_pnDisplay->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnLeftDown ), NULL, this );
	m_pnDisplay->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnLeftUp ), NULL, this );
	m_pnDisplay->Connect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMiddleDown ), NULL, this );
	m_pnDisplay->Connect( wxEVT_MIDDLE_UP, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMiddleUp ), NULL, this );
	m_pnDisplay->Connect( wxEVT_MOTION, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMotion ), NULL, this );
	m_pnDisplay->Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMouseWheel ), NULL, this );
	m_pnDisplay->Connect( wxEVT_SIZE, wxSizeEventHandler( CActorEditorGUIBase::m_pnDisplayOnSize ), NULL, this );
	m_edBackXOffset->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackXOffsetOnSpinCtrl ), NULL, this );
	m_edBackYOffset->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackYOffsetOnSpinCtrl ), NULL, this );
	m_edBackXScale->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackXScaleOnSpinCtrl ), NULL, this );
	m_edBackYScale->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackYScaleOnSpinCtrl ), NULL, this );
	m_cbEditMode->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_cbEditModeOnChoice ), NULL, this );
	m_btUndo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_btUndoOnButtonClick ), NULL, this );
	m_btRedo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_btRedoOnButtonClick ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	bt_AddKeyframe->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_AddKeyframeOnButtonClick ), NULL, this );
	bt_DelKeyframe->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_DelKeyframeOnButtonClick ), NULL, this );
	m_gdTimeline->Connect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridCellLeftClick ), NULL, this );
	m_gdTimeline->Connect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridCellRightClick ), NULL, this );
	m_gdTimeline->Connect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridLabelLeftClick ), NULL, this );
	m_gdTimeline->Connect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridLabelRightClick ), NULL, this );
	bt_Play->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::OnPlayAnimationClick ), NULL, this );
	cb_Loop->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::OnLoopAnimationClick ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	m_edAnimTime->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edAnimTimeOnSpinCtrl ), NULL, this );
	m_edBoneXCoord->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBoneXCoordOnSpinCtrl ), NULL, this );
	m_edBoneYCoord->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBoneYCoordOnSpinCtrl ), NULL, this );
	m_btResetBonePos->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_button5OnButtonClick ), NULL, this );
	m_poBoneNodes->Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( CActorEditorGUIBase::m_poBoneNodesOnTreeSelChanged ), NULL, this );
	m_edNodePosX->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodePosXOnSpinCtrl ), NULL, this );
	m_edNodePosY->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodePosYOnSpinCtrl ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_edNodeDepth->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodeDepthOnSpinCtrl ), NULL, this );
	m_cpSelectColor->Connect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( CActorEditorGUIBase::m_cpSelectColorOnColourChanged ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	bt_LoadSprite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_LoadSpriteOnButtonClick ), NULL, this );
	bt_ClearSprite->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_ClearSpriteOnButtonClick ), NULL, this );
	m_cbSpriteActionList->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_cbSpriteActionListOnChoice ), NULL, this );
	this->Connect( m_menuItem121->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem121OnMenuSelection ) );
	this->Connect( m_menuItem131->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem131OnMenuSelection ) );
	this->Connect( m_menuItem1311->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem1311OnMenuSelection ) );
	this->Connect( m_menuItem14->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem14OnMenuSelection ) );
	this->Connect( m_menuItem141->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem141OnMenuSelection ) );
	this->Connect( m_miPoseSelState->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miPoseSelStateOnMenuSelection ) );
	this->Connect( m_miInsertPoseSel->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miInsertPoseSelOnMenuSelection ) );
	this->Connect( m_miClearPoseSel->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miClearPoseSelOnMenuSelection ) );
	this->Connect( m_miDeletePoseSel->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miDeletePoseSelOnMenuSelection ) );
	this->Connect( m_menuItem17->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem17OnMenuSelection ) );
	this->Connect( m_menuItem16->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem16OnMenuSelection ) );
	this->Connect( m_menuItem29->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem29OnMenuSelection ) );
	this->Connect( m_menuItem18->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem18OnMenuSelection ) );
	this->Connect( m_menuItem19->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem19OnMenuSelection ) );
	this->Connect( m_menuItem20->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem20OnMenuSelection ) );
	this->Connect( m_menuItem30->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem30OnMenuSelection ) );
	this->Connect( m_menuItem21->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem21OnMenuSelection ) );
	this->Connect( m_menuItem22->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem22OnMenuSelection ) );
	this->Connect( m_menuItem23->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem23OnMenuSelection ) );
	this->Connect( m_menuItem161->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem161OnMenuSelection ) );
	this->Connect( m_menuItem291->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem291OnMenuSelection ) );
	this->Connect( m_menuItem181->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem181OnMenuSelection ) );
	this->Connect( m_menuItem191->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem191OnMenuSelection ) );
	this->Connect( m_menuItem201->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem201OnMenuSelection ) );
	this->Connect( m_menuItem301->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem301OnMenuSelection ) );
	this->Connect( m_menuItem211->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem211OnMenuSelection ) );
	this->Connect( m_menuItem221->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem221OnMenuSelection ) );
	this->Connect( m_menuItem231->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem231OnMenuSelection ) );
	this->Connect( menuitem0->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem0OnMenuSelection ) );
	this->Connect( menuitem1->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem1OnMenuSelection ) );
	this->Connect( menuitem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem11OnMenuSelection ) );
	this->Connect( m_menuItem12->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem12OnMenuSelection ) );
	this->Connect( m_menuItem10->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem10OnMenuSelection ) );
	this->Connect( m_menuItem11->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem11OnMenuSelection ) );
	this->Connect( m_moRestartWithSkelDef->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_moRestartWithSkelDefOnMenuSelection ) );
	this->Connect( menuitem3->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem3OnMenuSelection ) );
	this->Connect( m_miUndo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miUndoOnMenuSelection ) );
	this->Connect( m_miRedo->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miRedoOnMenuSelection ) );
	this->Connect( m_miReloadSprites->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::ReloadMaterialsClick ) );
	this->Connect( menuitem4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem4OnMenuSelection ) );
	this->Connect( menuitem5->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem5OnMenuSelection ) );
	this->Connect( menuitem7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem7SelectBackgroundColor ) );
	this->Connect( menuitem71->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem7OnMenuSelection ) );
	this->Connect( m_menuItem13->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem13OnMenuSelection ) );
	this->Connect( mo_About->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::mo_AboutOnMenuSelection ) );
}

CActorEditorGUIBase::~CActorEditorGUIBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CActorEditorGUIBase::GUIOnClose ) );
	this->Disconnect( wxEVT_IDLE, wxIdleEventHandler( CActorEditorGUIBase::GUIOnIdle ) );
	m_pnDisplay->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CActorEditorGUIBase::m_pnDisplayOnKeyDown ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_KEY_UP, wxKeyEventHandler( CActorEditorGUIBase::m_pnDisplayOnKeyUp ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_LEFT_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnLeftDown ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnLeftUp ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_MIDDLE_DOWN, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMiddleDown ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_MIDDLE_UP, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMiddleUp ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_MOTION, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMotion ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( CActorEditorGUIBase::m_pnDisplayOnMouseWheel ), NULL, this );
	m_pnDisplay->Disconnect( wxEVT_SIZE, wxSizeEventHandler( CActorEditorGUIBase::m_pnDisplayOnSize ), NULL, this );
	m_edBackXOffset->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackXOffsetOnSpinCtrl ), NULL, this );
	m_edBackYOffset->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackYOffsetOnSpinCtrl ), NULL, this );
	m_edBackXScale->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackXScaleOnSpinCtrl ), NULL, this );
	m_edBackYScale->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBackYScaleOnSpinCtrl ), NULL, this );
	m_cbEditMode->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_cbEditModeOnChoice ), NULL, this );
	m_btUndo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_btUndoOnButtonClick ), NULL, this );
	m_btRedo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_btRedoOnButtonClick ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	sb_KeyFrame->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::sb_KeyFrameOnScroll ), NULL, this );
	bt_AddKeyframe->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_AddKeyframeOnButtonClick ), NULL, this );
	bt_DelKeyframe->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_DelKeyframeOnButtonClick ), NULL, this );
	m_gdTimeline->Disconnect( wxEVT_GRID_CELL_LEFT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridCellLeftClick ), NULL, this );
	m_gdTimeline->Disconnect( wxEVT_GRID_CELL_RIGHT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridCellRightClick ), NULL, this );
	m_gdTimeline->Disconnect( wxEVT_GRID_LABEL_LEFT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridLabelLeftClick ), NULL, this );
	m_gdTimeline->Disconnect( wxEVT_GRID_LABEL_RIGHT_CLICK, wxGridEventHandler( CActorEditorGUIBase::m_gdTimelineOnGridLabelRightClick ), NULL, this );
	bt_Play->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::OnPlayAnimationClick ), NULL, this );
	cb_Loop->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::OnLoopAnimationClick ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	sld_AnimTime->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::sld_AnimTimeOnCommandScroll ), NULL, this );
	m_edAnimTime->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edAnimTimeOnSpinCtrl ), NULL, this );
	m_edBoneXCoord->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBoneXCoordOnSpinCtrl ), NULL, this );
	m_edBoneYCoord->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edBoneYCoordOnSpinCtrl ), NULL, this );
	m_btResetBonePos->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::m_button5OnButtonClick ), NULL, this );
	m_poBoneNodes->Disconnect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( CActorEditorGUIBase::m_poBoneNodesOnTreeSelChanged ), NULL, this );
	m_edNodePosX->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodePosXOnSpinCtrl ), NULL, this );
	m_edNodePosY->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodePosYOnSpinCtrl ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_sldNodeAngle->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAngleOnCommandScroll ), NULL, this );
	m_edNodeDepth->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( CActorEditorGUIBase::m_edNodeDepthOnSpinCtrl ), NULL, this );
	m_cpSelectColor->Disconnect( wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( CActorEditorGUIBase::m_cpSelectColorOnColourChanged ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	m_sldNodeAlpha->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( CActorEditorGUIBase::m_sldNodeAlphaOnCommandScroll ), NULL, this );
	bt_LoadSprite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_LoadSpriteOnButtonClick ), NULL, this );
	bt_ClearSprite->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CActorEditorGUIBase::bt_ClearSpriteOnButtonClick ), NULL, this );
	m_cbSpriteActionList->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_cbSpriteActionListOnChoice ), NULL, this );
	this->Disconnect( ID_MNU_COPYPOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem121OnMenuSelection ) );
	this->Disconnect( ID_MNU_PASTEPOSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem131OnMenuSelection ) );
	this->Disconnect( ID_MNU_PASTEPOSEFORCE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem1311OnMenuSelection ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem14OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM14, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem141OnMenuSelection ) );
	this->Disconnect( wxID_POSE_SEL_STATE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miPoseSelStateOnMenuSelection ) );
	this->Disconnect( wxID_POSE_SEL_COPY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miInsertPoseSelOnMenuSelection ) );
	this->Disconnect( wxID_POSE_SEL_CLEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miClearPoseSelOnMenuSelection ) );
	this->Disconnect( wxID_POSE_SEL_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miDeletePoseSelOnMenuSelection ) );
	this->Disconnect( ID_MNU_COPYINIVAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem17OnMenuSelection ) );
	this->Disconnect( ID_MNU_DELETEKEYFRAME, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem16OnMenuSelection ) );
	this->Disconnect( ID_MNU_CONSTANT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem29OnMenuSelection ) );
	this->Disconnect( ID_MNU_LINEAR, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem18OnMenuSelection ) );
	this->Disconnect( ID_MNU_SINUSOIDAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem19OnMenuSelection ) );
	this->Disconnect( ID_MNU_EXPONENTIAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem20OnMenuSelection ) );
	this->Disconnect( ID_MNU_INVSIN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem30OnMenuSelection ) );
	this->Disconnect( ID_MNU_RANDOM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem21OnMenuSelection ) );
	this->Disconnect( ID_MNU_SINSIN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem22OnMenuSelection ) );
	this->Disconnect( ID_MNU_EXPLOG, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem23OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM161, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem161OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM291, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem291OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM181, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem181OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM191, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem191OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM201, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem201OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM301, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem301OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM211, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem211OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM221, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem221OnMenuSelection ) );
	this->Disconnect( ID_M_MENUITEM231, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem231OnMenuSelection ) );
	this->Disconnect( ID_MENU_LOADANIM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem0OnMenuSelection ) );
	this->Disconnect( ID_MENU_SAVEANIM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem1OnMenuSelection ) );
	this->Disconnect( ID_MENUI_SAVEASANIM, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem11OnMenuSelection ) );
	this->Disconnect( ID_MENU_IMPORTSKELETON, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem12OnMenuSelection ) );
	this->Disconnect( ID_MENU_EXPORTSKELETON, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem10OnMenuSelection ) );
	this->Disconnect( ID_MENU_EXPORT_ANIMATION, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem11OnMenuSelection ) );
	this->Disconnect( wxID_RESTART_WITH_SKEL_DEF, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_moRestartWithSkelDefOnMenuSelection ) );
	this->Disconnect( ID_MENU_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem3OnMenuSelection ) );
	this->Disconnect( wxID_UNDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miUndoOnMenuSelection ) );
	this->Disconnect( wxID_REDO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_miRedoOnMenuSelection ) );
	this->Disconnect( wxID_RELOADMATERIALS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::ReloadMaterialsClick ) );
	this->Disconnect( ID_MENUITEM4, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem4OnMenuSelection ) );
	this->Disconnect( ID_MENUITEM5, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem5OnMenuSelection ) );
	this->Disconnect( ID_MENUITEM7, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem7SelectBackgroundColor ) );
	this->Disconnect( ID_MENUITEM71, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::menuitem7OnMenuSelection ) );
	this->Disconnect( ID_MENUITEM13, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::m_menuItem13OnMenuSelection ) );
	this->Disconnect( ID_MO_ABOUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CActorEditorGUIBase::mo_AboutOnMenuSelection ) );
	
	delete m_pupmKeyFrameOps; 
	delete m_popKeyFrameMenu; 
	delete m_popKeyFrameFuncMenu; 
}
