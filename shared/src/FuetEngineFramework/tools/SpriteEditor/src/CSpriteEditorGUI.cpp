#include "CSpriteEditorGUI.h"

CSpriteEditorGUI::CSpriteEditorGUI( wxWindow* parent )
	:
	CSpriteEditorGUIBase( parent )
{
	// clipboard management.
	CClipBoard::Init();

	SetIcon(globals.m_oIconScheme.m_oAppLogo);

	// Initialize in the application directory
	if ( (globals.m_sExecCmdLine != "") && ( !IsDebuggerPresent() ) ) // Windows API function call: requires Windows XP.
	{
		wxFileName sFilename(globals.m_sExecCmdLine);
		SetCurrentDirectory( sFilename.GetPath().char_str() );
	}

	// Includes are found under CSpriteGUI file properties because wxFormBuilder
	// deletes them after code generation.
	globals.GUI = this;

	// Hook mouse move function...
	pn_Display->HookMouseMove(DisplayPanelMouseMove);
	pn_Display->HookMouseDown(DisplayPanelMouseDown);
	pn_Display->HookMouseUp(DisplayPanelMouseUp);
	pn_Display->HookMouseMiddleDown(DisplayPanelMouseMiddleDown);
	pn_Display->HookMouseMiddleUp(DisplayPanelMouseMiddleUp);

	// m_pnFrameSeq->HookMouseMove(FrameSeqPanelMouseMove);
	m_pnPrevDisplay->HookMouseDown(FrameSeqPanelMouseDown);

	// Hook mouse move function...
	// m_pnFrameSeq->HookMouseMove(FrameSeqPanelMouseMove);
	m_pnFrameSeq->HookMouseDown(FrameSeqPanelMouseDown);
	// pn_Display->HookKeyDown(DisplayPanelKeyDown);

	// Initialize the renderer.

	SCREEN_WIDTH = pn_Display->GetSize().x;
	SCREEN_HEIGHT = pn_Display->GetSize().y;

	globals.m_poRenderer = new CFERenderer();
	globals.m_poRenderer->Init( (void*)pn_Display->GetHandle() );
	globals.m_poRenderer->SetScreenVWidth(SCREEN_WIDTH);
	globals.m_poRenderer->SetScreenVHeight(SCREEN_HEIGHT);
	globals.m_poRenderer->bSetProperty("PRIMITIVE_SORTING",(FEPointer)true);

	globals.m_poFrameSeqRenderer = new CFERenderer();
	globals.m_poFrameSeqRenderer->Init( (void*)m_pnFrameSeq->GetHandle() );
	globals.m_poFrameSeqRenderer->SetScreenVWidth(m_pnFrameSeq->GetSize().x * 2.0);
	globals.m_poFrameSeqRenderer->SetScreenVHeight(m_pnFrameSeq->GetSize().y * 2.0);
	globals.m_poFrameSeqRenderer->bSetProperty("PRIMITIVE_SORTING",(FEPointer)true);

	globals.m_poPrevRenderer = new CFERenderer();
	globals.m_poPrevRenderer->Init( (void*)m_pnPrevDisplay->GetHandle() );
	globals.m_poPrevRenderer->SetScreenVWidth(m_pnPrevDisplay->GetSize().x * 2.0);
	globals.m_poPrevRenderer->SetScreenVHeight(m_pnPrevDisplay->GetSize().y * 2.0);
	globals.m_poPrevRenderer->bSetProperty("PRIMITIVE_SORTING",(FEPointer)true);

	// Load the default font.
	globals.m_poDefaultFont = CFEFontMgr::I()->poLoad("small");

	// Load a default sprite.
	CFESpriteInstMgr::I()->Init(250);

	// ------
	// Sprite Action grid
	m_gdSpriteActions->SetFont( wxFont( 8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Andale Mono") ) );
	m_gdSpriteActions->SetDefaultColSize(16);
	m_gdSpriteActions->SetDefaultRowSize(24);
	m_gdSpriteActions->SetRowLabelSize(0);
	m_gdSpriteActions->SetColLabelSize(24);

	m_gdSpriteActions->SetColLabelValue(0,"#");
	m_gdSpriteActions->SetColLabelValue(1,"W");
	m_gdSpriteActions->SetColLabelValue(2,"Blend");
	m_gdSpriteActions->SetColLabelValue(3,"RST");
	m_gdSpriteActions->SetColLabelValue(4,"Action Name");

	m_gdSpriteActions->SetColSize(0,24);    // action number
	m_gdSpriteActions->SetColSize(1,50);    // wrap icon
	m_gdSpriteActions->SetColSize(2,90);    // BlendMode
	m_gdSpriteActions->SetColSize(3,50);    // random start time
	m_gdSpriteActions->SetColSize(4,185);   // action name column

	m_gdSpriteActions->ClearGrid();
	m_gdSpriteActions->DisableCellEditControl();
	m_gdSpriteActions->EnableEditing(false);

	m_gdSpriteActions->SetCellHighlightColour(globals.m_oColorScheme.m_oGridLabelBase);
	m_gdSpriteActions->SetLabelBackgroundColour(globals.m_oColorScheme.m_oGridLabelBase);
	m_gdSpriteActions->SetGridLineColour(globals.m_oColorScheme.m_oTLGridLine);
	m_gdSpriteActions->SetDefaultRenderer(new CActionGridRenderer);

	m_gdSpriteActions->DisableDragColSize();
	m_gdSpriteActions->DisableDragRowSize();
	m_gdSpriteActions->EnableGridLines(false);

	wxFont oFont = m_gdSpriteActions->GetLabelFont();
	oFont.SetWeight(wxFONTWEIGHT_LIGHT);
	m_gdSpriteActions->SetLabelFont(oFont);
	m_gdSpriteActions->SetLabelTextColour(globals.m_oColorScheme.m_oTLLabels);
	m_gdSpriteActions->Enable(false);

	// do not allow the horizontal scrollbar appear...
	// m_pnFrameProps->SetScrollRate( 0, 5 );
	// m_pnFrameProps->SetScrollbar(wxHORIZONTAL, 0, 0, 0);

	// / If there is an input file to load during startup, then load it...
	/*
	   wxMessageBox(globals.m_sInputFilename,
	            "Confirm",
	            wxYES_NO,this);
	 */
	SetTitle( APP_NAME  );

	bool bCreateSprite = true;
	if ( globals.m_sInputFilename != "" )
	{
		wxFileName sInputFilename(globals.m_sInputFilename);
		SetCurrentDirectory( sInputFilename.GetPath().char_str() );
		globals.m_sWorkingDir = sInputFilename.GetPath();

		wxString sFile = sInputFilename.GetName();
		if ( bLoadSprite( CFEString( sFile.char_str() ) ) )
		{
			SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
			bCreateSprite = false;
		}
	}

	// Create sprite in case of need.
	if ( bCreateSprite == true )
	{
		globals.m_poSprite = new CFESprite;
		globals.m_poSprite->SetName("nonamed sprite");

		globals.m_hCurSprInst = CFESpriteInstMgr::I()->hGetInstance( globals.m_poSprite );
		CFESpriteInstMgr::I()->Enable(globals.m_hCurSprInst);
		CFESpriteInstMgr::I()->SetPos( globals.m_hCurSprInst,CFEVect2::ZERO() );

		globals.m_iSelectedAction = -1;
		globals.m_iSelectedFrame = -1;
	}

	// Setup controls according to sprite information.
	SetupControls();
}

void CSpriteEditorGUI::CSpriteEditorGUIBaseOnClose( wxCloseEvent& event )
{
	if ( wxMessageBox("Are you sure you want to exit?\n"
	                  "All data not saved will be lost",
	                  "Confirm",
	                  wxYES_NO,this) != wxYES )
	{
		event.Veto();
		return;
	}

	globals.m_bFinishingApp = true;

	// To avoid a crash at the end of the app?
	pn_Display->Disconnect( wxID_ANY, wxEVT_SIZE);
	CClipBoard::Finish();

	Destroy();
}

void CSpriteEditorGUI::CSpriteEditorGUIBaseOnIdle( wxIdleEvent& event )
{
	if ( globals.m_bFinishingApp ) return;
	RenderMain(globals.m_poRenderer);
	RenderFrameSeq(globals.m_poFrameSeqRenderer);
	RenderPreview(globals.m_poPrevRenderer);

	if ( globals.m_bPlaying )
		bt_Play->SetLabel("Stop");
	else
		bt_Play->SetLabel("Play");
	event.RequestMore();
}

void CSpriteEditorGUI::m_gdSpriteActionsOnGridCellLeftClick( wxGridEvent& event )
{
	// TODO: Implement m_gdSpriteActionsOnGridCellLeftClick
	if ( event.GetRow() < globals.m_poSprite->m_oActions.size() )
		SelectAction( event.GetRow() );
}

void CSpriteEditorGUI::m_gdSpriteActionsOnGridCellLeftDClick( wxGridEvent& event )
{
	if ( globals.m_iSelectedAction < 0 ) return;
	if ( event.GetRow() >= globals.m_poSprite->m_oActions.size() ) return;
	switch ( event.GetCol() )
	{
		// wrap loop mode
		case 1:
		{
			switch ( globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_ePlayMode )
			{
				case SFSPM_ONESHOT:
				globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_ePlayMode = SFSPM_LOOP;
				break;

				case SFSPM_LOOP:
				globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_ePlayMode = SFSPM_ONESHOT;
				break;
			}

			m_gdSpriteActions->Refresh();
		};
		break;

		// Blend Mode
		case 2:
		{
			EFEBlendMode NextBlendMode[BM_NUM_MODES] =
			{
				BM_COPY, // BM_NONE,
				BM_ALPHA, // BM_COPY,
				BM_ADD, // BM_ALPHA,
				BM_ALPHAADD, // BM_ADD,
				BM_MULT, // BM_ALPHAADD,
				BM_FOG, // BM_MULT
				BM_FOGADD, // BF_FOG
				BM_MAGICMARKER, // BM_FOGADD,
				BM_LIGHTMARKER, // BM_MAGICMARKER,
				BM_LIGHTSABER, // BM_LIGHTMARKER,
				BM_REVEAL, // BM_LIGHTSABER,
				BM_LUMISHADE_REVEAL, // BM_REVEAL,
				BM_COPY, // BM_LUMISHADE_REVEAL,
			};

			EFEBlendMode eBlendMode = globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_eBlendMode;
			globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_eBlendMode = NextBlendMode[eBlendMode];
			m_gdSpriteActions->Refresh();
		}
		break;

		// Random Start Time
		case 3:
		{
			float fOutput = globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_rRandStartTime;
			if ( bSelectFloat("Select Random Start Time:",fOutput,0.0f,20.0f,&fOutput) )
			{
				globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_rRandStartTime = fOutput;
				m_gdSpriteActions->Refresh();
			}
		}
		break;

		// Rename action
		case 4:
		{
			wxTextEntryDialog oInputBox(this,
			                            _T("Rename action to:"),
			                            _T("Enter the new name for the action:"),
			                            _T( globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->sGetName().szString() ),
			                            wxOK | wxCANCEL);

			if ( oInputBox.ShowModal() == wxID_OK )
			{
				globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->SetName( CFEString ( oInputBox.GetValue().char_str() ) );
				m_gdSpriteActions->Refresh();
			}
		}
		break;
	}
}

void CSpriteEditorGUI::m_gdSpriteActionsOnGridCellRightClick( wxGridEvent& event )
{
	// TODO: Implement m_gdSpriteActionsOnGridCellRightClick
}

void CSpriteEditorGUI::m_btNewActionOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btNewActionOnButtonClick
	CFESpriteAction oSprAction;

	CFEString sName;
	sName.Format("SpriteAction%d",globals.m_uiCurAction++);

	oSprAction.SetName(sName);
	oSprAction.m_eBlendMode = BM_ALPHA;
	oSprAction.m_ePlayMode = SFSPM_LOOP;
	oSprAction.m_rActionTime = 0.0f;
	oSprAction.m_rRandStartTime = 0.0f;

	globals.m_poSprite->m_oActions.push_back(oSprAction);

	// globals.GUI->m_gdSpriteActions->DeleteRows(0,globals.GUI->m_gdSpriteActions->GetRows());
	// globals.GUI->m_gdSpriteActions->AppendRows(globals.m_poSprite->m_oActions.size(),false);
	globals.GUI->m_gdSpriteActions->AppendRows(1,false);
	m_gdSpriteActions->Enable(true);

	SelectAction(globals.m_poSprite->m_oActions.size() - 1);
}

void CSpriteEditorGUI::m_btActionUpOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction > 0 )
	{
		// Move action up
		CFESpriteAction oOld = globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
		globals.m_poSprite->m_oActions[globals.m_iSelectedAction  ] = globals.m_poSprite->m_oActions[globals.m_iSelectedAction - 1];
		globals.m_poSprite->m_oActions[globals.m_iSelectedAction - 1] = oOld;

		globals.m_iSelectedAction = globals.m_iSelectedAction - 1;
		SelectAction(globals.m_iSelectedAction);
	}
}

void CSpriteEditorGUI::m_btActionDownOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction < (globals.m_poSprite->m_oActions.size() - 1) )
	{
		// Move action down
		CFESpriteAction oOld = globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
		globals.m_poSprite->m_oActions[globals.m_iSelectedAction  ] = globals.m_poSprite->m_oActions[globals.m_iSelectedAction + 1];
		globals.m_poSprite->m_oActions[globals.m_iSelectedAction + 1] = oOld;

		globals.m_iSelectedAction = globals.m_iSelectedAction + 1;
		SelectAction(globals.m_iSelectedAction);
	}
}

void CSpriteEditorGUI::m_btDeleteActionOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btDeleteActionOnButtonClick
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( wxMessageBox("Are you sure you want to delete this action?",
	                  "Confirm",
	                  wxYES_NO,this) != wxYES ) return;
	globals.m_poSprite->m_oActions.Delete( globals.m_iSelectedAction );

	if ( globals.m_iSelectedAction >= globals.m_poSprite->m_oActions.size() )
	{
		if ( globals.m_poSprite->m_oActions.size() > 0 )
			globals.m_iSelectedAction = globals.m_poSprite->m_oActions.size() - 1;
		else
		{
			globals.m_iSelectedAction = -1;
			globals.m_iSelectedFrame = -1;
		}
	}

	SelectAction(globals.m_iSelectedAction);
}

void CSpriteEditorGUI::OnPlayAnimationClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction < 0 ) return;
	if ( globals.m_hCurSprInst == NULL )
	{
		return; // Restart animation
	}
	CFESpriteInstMgr::I()->SetAction(globals.m_hCurSprInst,globals.m_iSelectedAction);
}

void CSpriteEditorGUI::m_btReloadMaterialsOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btReloadMaterialsOnButtonClick
	CFEMaterialMgr::I()->Reload();
	CFESpriteMgr::I()->Reload();
}

void CSpriteEditorGUI::m_btCopyFrameOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btCopyFrameOnButtonClick
	if ( (globals.m_iSelectedFrame > -1) && (globals.m_poSprite->m_oActions.size() > 0) )
	{
		m_btPasteFrame->Enable();

		CFESpriteFrame* poSF = new CFESpriteFrame(globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame]);
		CFESpriteFrame* poOldSE = (CFESpriteFrame*)CClipBoard::pSetData("FUETENGINE_SPRITEEDITOR_SPRITEFRAME",poSF);

		if ( poOldSE != NULL )
			delete poOldSE;
	}
}

void CSpriteEditorGUI::m_btPasteFrameOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btPasteFrameOnButtonClick
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( !CClipBoard::bExistData("FUETENGINE_SPRITEEDITOR_SPRITEFRAME") ) return;
	CFESpriteFrame* poSF = (CFESpriteFrame*)CClipBoard::pGetData("FUETENGINE_SPRITEEDITOR_SPRITEFRAME");

	CFESpriteAction* poAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction);

	if ( poAction->m_oSeq.size() == 0 )
	{
		// The action hasn't any frame, so we should create a new one and then paste it
		CFESpriteFrame oSprFrame;
		poAction->m_oSeq.push_back(oSprFrame);
		globals.m_iSelectedFrame = 0;
	}

	memcpy( &globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame],poSF,sizeof(CFESpriteFrame) );
	RecalcFrameTimes( &globals.m_poSprite->m_oActions[globals.m_iSelectedAction] );
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::sb_KeyFrameOnScroll( wxScrollEvent& event )
{
	// TODO: Implement sb_KeyFrameOnScroll
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( globals.m_iSelectedFrame == -1 )
	{
		return; // globals.m_iFirstVisFrame = event.GetPosition();
	}
	globals.m_iFirstVisFrame = sb_KeyFrame->GetThumbPosition();
	// sliders produce OnScrollEvents, and produce weird behaviour, so it's safer to not use the event data here.
	// globals.m_iFirstVisFrame = event.GetPosition();
}

void CSpriteEditorGUI::m_btNewFrameOnButtonClick( wxCommandEvent& event )
{
	CFESpriteFrame oSprFrame;

	if ( (globals.m_iSelectedFrame > -1) && (globals.m_poSprite->m_oActions.size() > 0) )
		memcpy( &oSprFrame,&globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame],sizeof(CFESpriteFrame) );
	else
	{
		oSprFrame.m_hMaterial = globals.m_hCurMaterial;
		oSprFrame.m_oPivot = CFEVect2(_05r,_05r);

		oSprFrame.m_oUV.m_oIni = CFEVect2::ZERO();
		oSprFrame.m_oUV.m_oEnd = CFEVect2::ONE();
		oSprFrame.m_bScaleXUEqually = false;
		oSprFrame.m_bScaleYVEqually = false;
		oSprFrame.m_bUWorldCoords = false;
		oSprFrame.m_bVWorldCoords = false;

		SetFrameValues(&oSprFrame,_1r,_1r);

		if ( oSprFrame.m_hMaterial == NULL )
			oSprFrame.m_oSize = CFEVect2::ZERO();
		else
		{
			int w,h;
			CFEMaterialMgr::I()->bGetMaterialProperty(oSprFrame.m_hMaterial,"DiffuseMap.Width",(FEPointer) & w);
			CFEMaterialMgr::I()->bGetMaterialProperty(oSprFrame.m_hMaterial,"DiffuseMap.Height",(FEPointer) & h);
			oSprFrame.m_oSize = CFEVect2(w,h);
		}
	}

	CFESpriteAction* poAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction );
	poAction->m_oSeq.push_back(oSprFrame);

	RecalcFrameTimes(poAction);
	SelectFrame(poAction->m_oSeq.size() - 1);
}

void CSpriteEditorGUI::m_btDeleteFrameOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( globals.m_iSelectedFrame == -1 ) return;
	if ( wxMessageBox(   "Are you sure you want to delete this frame from the sequence?",
	                     "Confirm",
	                     wxYES_NO,this) != wxYES ) return;
	CFESpriteAction* poAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction );
	poAction->m_oSeq.Delete(globals.m_iSelectedFrame);

	if ( globals.m_iSelectedFrame >= poAction->m_oSeq.size() )
	{
		if ( poAction->m_oSeq.size() > 0 )
			globals.m_iSelectedFrame = poAction->m_oSeq.size() - 1;
		else
			globals.m_iSelectedFrame = -1;
		uint FS_VIS_FRAMES = globals.m_poFrameSeqRenderer->uiGetScreenVWidth() / FRAME_WIDTH;
		if ( ( globals.m_iFirstVisFrame + poAction->m_oSeq.size() ) > FS_VIS_FRAMES )
			globals.m_iFirstVisFrame = poAction->m_oSeq.size() - FS_VIS_FRAMES;
	}

	RecalcFrameTimes( poAction );
	SelectFrame(globals.m_iSelectedFrame);

	// Reset animation in case, we're using this frame right now
	CFESpriteInstMgr::I()->SetCurrentActionTime(globals.m_hCurSprInst,_0r);
}

void CSpriteEditorGUI::pn_DisplayOnMouseWheel( wxMouseEvent& event )
{
	// TODO: Implement pn_DisplayOnMouseWheel
	DisplayPanelOnMouseWheel(event);
}

void CSpriteEditorGUI::pn_DisplayOnSize( wxSizeEvent& event )
{
	if ( globals.m_bFinishingApp )
	{
		return;                        // TODO: Implement pn_DisplayOnSize
	}
	SCREEN_WIDTH = event.GetSize().x;
	SCREEN_HEIGHT = event.GetSize().y;

	globals.m_poRenderer->SetScreenVWidth(SCREEN_WIDTH);
	globals.m_poRenderer->SetScreenVHeight(SCREEN_HEIGHT);
	globals.m_poRenderer->bSetProperty( "ScreenWidth", CASTVALUE(FEPointer,event.GetSize().x) );
	globals.m_poRenderer->bSetProperty( "ScreenHeight",CASTVALUE(FEPointer,event.GetSize().y) );

	globals.m_poFrameSeqRenderer->SetScreenVWidth(SCREEN_WIDTH * 2.0);
	globals.m_poFrameSeqRenderer->bSetProperty( "ScreenWidth", CASTVALUE(FEPointer,event.GetSize().x) );
	// globals.m_poFrameSeqRenderer->SetScreenVHeight(m_pnFrameSeq->GetSize().y * 2.0);

	globals.m_poPrevRenderer->SetScreenVWidth(m_pnPrevDisplay->GetSize().x * 2.0);
	globals.m_poPrevRenderer->SetScreenVHeight(m_pnPrevDisplay->GetSize().y * 2.0);
	globals.m_poPrevRenderer->bSetProperty( "ScreenWidth", CASTVALUE(FEPointer,m_pnPrevDisplay->GetSize().x) );
	globals.m_poPrevRenderer->bSetProperty( "ScreenHeight",CASTVALUE(FEPointer,m_pnPrevDisplay->GetSize().y) );

	//
	uint FS_VIS_FRAMES = globals.m_poFrameSeqRenderer->uiGetScreenVWidth() / FRAME_WIDTH;
	if (
	    (globals.m_poSprite != 0)
	    && (globals.m_iSelectedAction > -1)
	    && (globals.m_poSprite->poGetAction(globals.m_iSelectedAction) != NULL)
	    )
	{
		if ( globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq.size() <= FS_VIS_FRAMES ) globals.m_iFirstVisFrame = 0;
		sb_KeyFrame->SetScrollbar(globals.m_iFirstVisFrame,FS_VIS_FRAMES,globals.m_poSprite->poGetAction(globals.m_iSelectedAction)->m_oSeq.size(),FS_VIS_FRAMES,true);
	}
}

void CSpriteEditorGUI::m_cbSnapFactorOnChoice( wxCommandEvent& event )
{
	// TODO: Implement m_cbSnapFactorOnChoice
	globals.m_uiGridSelection = m_cbSnapFactor->GetSelection();
	globals.m_uiGridSize = iGetGridSize();
}

void CSpriteEditorGUI::m_btLoadMaterialOnButtonClick( wxCommandEvent& event )
{
	if ( (globals.m_iSelectedAction != -1) && (globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq.size() > 0) )
	{
		wxFileDialog dlgLoadFile(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.tga;*.png"), wxOPEN | wxCHANGE_DIR);
		if ( dlgLoadFile.ShowModal() == wxID_OK )
		{
			wxFileName sFilename( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
			wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
			sFullFilename.Replace("\\","/");
			wxString sCurDir = sFilename.GetPath();

			CFEString sMaterialFile = CFEString( sFullFilename.char_str() );
			globals.m_hCurMaterial = CFEMaterialMgr::I()->hLoad( sMaterialFile );

			globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame].m_hMaterial = globals.m_hCurMaterial;

			int w,h;
			CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Width",(FEPointer) & w);
			CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Height",(FEPointer) & h);

			globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame].m_hMaterial = globals.m_hCurMaterial;
			globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame].m_oSize = CFEVect2(w,h);

			SelectFrame(globals.m_iSelectedFrame);
		}
	}
}

void CSpriteEditorGUI::m_btClearMaterialOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( globals.m_iSelectedFrame == -1 ) return;
	if ( wxMessageBox("Are you sure you want to clear this frame material?",
	                  "Confirm",
	                  wxYES_NO,this) != wxYES ) return;
	CFESpriteAction* poAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction );
	poAction->m_oSeq[globals.m_iSelectedFrame].m_hMaterial = NULL;

	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_cbSWrapModeOnCombobox( wxCommandEvent& event )
{
	// TODO: Implement m_cbSWrapModeOnCombobox
	if ( globals.m_bSkipEvents ) return;
	CFEMaterialMgr::I()->bSetMaterialProperty( globals.m_hCurMaterial,"DiffuseMap.SWrapMode",(FEPointer)event.GetInt() );
}

void CSpriteEditorGUI::m_cbTWrapModeOnCombobox( wxCommandEvent& event )
{
	// TODO: Implement m_cbTWrapModeOnCombobox
	if ( globals.m_bSkipEvents ) return;
	CFEMaterialMgr::I()->bSetMaterialProperty( globals.m_hCurMaterial,"DiffuseMap.TWrapMode",(FEPointer)event.GetInt() );
}

void CSpriteEditorGUI::m_cbFilterOnCombobox( wxCommandEvent& event )
{
	// TODO: Implement m_cbFilterOnCombobox
	if ( globals.m_bSkipEvents ) return;
	CFEMaterialMgr::I()->bSetMaterialProperty( globals.m_hCurMaterial,"DiffuseMap.Filter",(FEPointer)event.GetInt() );
}

void CSpriteEditorGUI::m_btAffectAllFramesOnToggleButton( wxCommandEvent& event )
{
	// TODO: Implement m_btAffectAllFramesOnToggleButton
	if ( m_btAffectAllFrames->GetValue() == true )
		m_btAffectAllFrames->SetLabel("Click to affect current frame");
	else
		m_btAffectAllFrames->SetLabel("Click to affect all frames");
}

void CSpriteEditorGUI::m_sldDelayBlendFactOnCommandScroll( wxScrollEvent& event )
{
	if ( globals.m_bSkipEvents ) return;
	if ( globals.m_iSelectedAction == -1 ) return;
	if ( globals.m_iSelectedFrame == -1 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;
	FEReal rFrameTime = (FEReal)m_sldFrameTime->GetValue() / 1000.0f;
	FEReal rFact = (FEReal)m_sldDelayBlendFact->GetValue() / 100.0f;

	if ( m_btAffectAllFrames->GetValue() == true )
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
			SetFrameValues(&poSprAct->m_oSeq[i],rFrameTime,rFact);
	else
		SetFrameValues(&poSprAct->m_oSeq[ globals.m_iSelectedFrame ],rFrameTime,rFact);
	RecalcFrameTimes(poSprAct);

	// Show frame time vs total anim time
	CFEString sStr;
	sStr.Format("%.3f / %.3f secs",rFrameTime,poSprAct->m_rActionTime);
	m_lbFrameTime->SetLabel( wxString( sStr.szString() ) );
}

void CSpriteEditorGUI::m_sldFrameTimeOnCommandScroll( wxScrollEvent& event )
{
	m_sldDelayBlendFactOnCommandScroll(event);
}

void CSpriteEditorGUI::m_btAutoAdjustTopLeftOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAutoAdjustTopLeftOnButtonClick
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;
	CFEVect2 oPivot = CFEVect2(0.0f,0.0f);
	if ( m_btAffectAllFrames->GetValue() == true )
	{
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
		{
			SetFrameGeometry(&poSprAct->m_oSeq[i],
			                 CFEVect2::ZERO(),
			                 CFEVect2::ONE(),
			                 oPivot);
		}
	}
	else
	{
		SetFrameGeometry(   &poSprAct->m_oSeq[globals.m_iSelectedFrame],
		                    CFEVect2::ZERO(),
		                    CFEVect2::ONE(),
		                    oPivot);
	}
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_btAutoAdjustCenterOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAutoAdjustCenterOnButtonClick
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;
	CFEVect2 oPivot = CFEVect2(0.5f,0.5f);
	if ( m_btAffectAllFrames->GetValue() == true )
	{
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
		{
			SetFrameGeometry(   &poSprAct->m_oSeq[i],
			                    CFEVect2::ZERO(),
			                    CFEVect2::ONE(),
			                    oPivot);
		}
	}
	else
	{
		SetFrameGeometry(&poSprAct->m_oSeq[globals.m_iSelectedFrame],
		                 CFEVect2::ZERO(),
		                 CFEVect2::ONE(),
		                 oPivot);
	}
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_btAutoAdjustBottomCenterOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;
	CFEVect2 oPivot = CFEVect2(0.5f,1.0f);
	if ( m_btAffectAllFrames->GetValue() == true )
	{
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
		{
			SetFrameGeometry(&poSprAct->m_oSeq[i],
			                 CFEVect2::ZERO(),
			                 CFEVect2::ONE(),
			                 oPivot);
		}
	}
	else
	{
		SetFrameGeometry(&poSprAct->m_oSeq[globals.m_iSelectedFrame],
		                 CFEVect2::ZERO(),
		                 CFEVect2::ONE(),
		                 oPivot);
	}
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_btAutoAdjustSizeOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 )
	{
		return; // convert the size in pixels relative to the texture.
	}
	if ( m_btAffectAllFrames->GetValue() == true )
	{
		for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
		{
			CFESpriteFrame* poFrame = &poSprAct->m_oSeq[i];
			SetFrameGeometry( poFrame,
			                  CFEVect2::ZERO(),
			                  CFEVect2::ONE(),
			                  poFrame->m_oPivot);
		}
	}
	else
	{
		CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];
		SetFrameGeometry( poFrame,
		                  CFEVect2::ZERO(),
		                  CFEVect2::ONE(),
		                  poFrame->m_oPivot);
	}

	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_btAdjustSizeManuallyOnButtonClick( wxCommandEvent& event )
{
	if ( globals.m_iSelectedAction < 0 ) return;
	CFESpriteAction* poSprAct = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction];
	if ( poSprAct->m_oSeq.size() <= 0 ) return;
	CFESpriteFrame* poFrame = &poSprAct->m_oSeq[ globals.m_iSelectedFrame ];

	// convert the size in pixels relative to the texture.
	CFEVect2 oSize = poFrame->m_oSize;
	wxSelect2DVector* dlg = new wxSelect2DVector(this,wxT("Select Value"),oSize.x,-65536.0,65536.0,oSize.y,-65536.0,65536.0);
	dlg->ShowModal();

	if ( dlg->bOK() )
	{
		oSize.x = dlg->fGetXValue();
		oSize.y = dlg->fGetYValue();

		if ( m_btAffectAllFrames->GetValue() == true )
		{
			for ( uint i = 0; i < poSprAct->m_oSeq.size(); i++ )
			{
				poFrame = &poSprAct->m_oSeq[i];
				poFrame->m_oSize = oSize;
			}
		}
		else
			poFrame->m_oSize = oSize;
	}

	delete dlg;

	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_cbScaleXUEquallyOnCheckBox( wxCommandEvent& event )
{
	// TODO: Implement m_cbScaleXUEquallyOnCheckBox
	if ( globals.m_bSkipEvents ) return;
	CFESpriteFrame* poFrame = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame];
	poFrame->m_bScaleXUEqually = event.IsChecked();
}

void CSpriteEditorGUI::m_cbUWorldCoordsOnCheckBox( wxCommandEvent& event )
{
	// TODO: Implement m_cbUWorldCoordsOnCheckBox
	if ( globals.m_bSkipEvents ) return;
	CFESpriteFrame* poFrame = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame];
	poFrame->m_bUWorldCoords = event.IsChecked();
}

void CSpriteEditorGUI::m_cbScaleYVEquallyOnCheckBox( wxCommandEvent& event )
{
	// TODO: Implement m_cbScaleYVEquallyOnCheckBox
	if ( globals.m_bSkipEvents ) return;
	CFESpriteFrame* poFrame = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame];
	poFrame->m_bScaleYVEqually = event.IsChecked();
}

void CSpriteEditorGUI::m_cbVWorldCoordsOnCheckBox( wxCommandEvent& event )
{
	// TODO: Implement m_cbVWorldCoordsOnCheckBox
	if ( globals.m_bSkipEvents ) return;
	CFESpriteFrame* poFrame = &globals.m_poSprite->m_oActions[globals.m_iSelectedAction].m_oSeq[globals.m_iSelectedFrame];
	poFrame->m_bVWorldCoords = event.IsChecked();
}

void CSpriteEditorGUI::m_edPivotImageXCoordOnKeyDown( wxKeyEvent& event )
{
	// TODO: Implement m_edPivotImageXCoordOnKeyDown
	if ( event.GetKeyCode() != WXK_RETURN ) return;
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.x = m_edPivotImageXCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_edPivotImageXCoordOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edPivotImageXCoordOnSpinCtrl
	if ( globals.m_bSkipEvents ) return;
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.x = m_edPivotImageXCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_edPivotImageXCoordOnSpinCtrlText( wxCommandEvent& event )
{
	// TODO: Implement m_edPivotImageXCoordOnSpinCtrlText
	if ( globals.m_bSkipEvents ) return;
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.x = m_edPivotImageXCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_edPivotImageYCoordOnKeyDown( wxKeyEvent& event )
{
	// TODO: Implement m_edPivotImageYCoordOnKeyDown
	if ( event.GetKeyCode() != WXK_RETURN ) return;
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.y = m_edPivotImageYCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_edPivotImageYCoordOnSpinCtrl( wxSpinEvent& event )
{
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.y = m_edPivotImageYCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_edPivotImageYCoordOnSpinCtrlText( wxCommandEvent& event )
{
	// TODO: Implement m_edPivotImageYCoordOnSpinCtrlText
	CFEVect2 oPos = oGetCurrentPivotPos();
	oPos.y = m_edPivotImageYCoord->GetValue();

	SetPivot(oPos);
	SelectFrame(globals.m_iSelectedFrame);
}

void CSpriteEditorGUI::m_btAdjustPivotVerticalTopOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotVerticalTopOnButtonClick
	AdjustPivot( CFEVect2(0.0f,0.0f),false,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotTopLeftOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotTopLeftOnButtonClick
	AdjustPivot( CFEVect2(0.0f,0.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotTopCenterOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotTopCenterOnButtonClick
	AdjustPivot( CFEVect2(0.5f,0.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotTopRightOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotTopRightOnButtonClick
	AdjustPivot( CFEVect2(1.0f,0.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotVerticalCenterOnButtonClick( wxCommandEvent& event )
{
	AdjustPivot( CFEVect2(0.0f,0.5f),false,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotCenterLeftOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotCenterLeftOnButtonClick
	AdjustPivot( CFEVect2(0.0f,0.5f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotCenterCenterOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotCenterCenterOnButtonClick
	AdjustPivot( CFEVect2(0.5f,0.5f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotCenterRightOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotCenterRightOnButtonClick
	AdjustPivot( CFEVect2(1.0f,0.5f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotVerticalBottomOnButtonClick( wxCommandEvent& event )
{
	AdjustPivot( CFEVect2(0.0f,1.0f),false,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotBottomLeftOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotBottomLeftOnButtonClick
	AdjustPivot( CFEVect2(0.0f,1.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotBottomCenterOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotBottomLeftOnButtonClick
	AdjustPivot( CFEVect2(0.5f,1.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotBottomRightOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotBottomRightOnButtonClick
	AdjustPivot( CFEVect2(1.0f,1.0f),true,true,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotHorizontalLeftOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotHorizontalLeftOnButtonClick
	AdjustPivot( CFEVect2(0.0f,0.0f),true,false,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotHorizontalCenterOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotHorizontalCenterOnButtonClick
	AdjustPivot( CFEVect2(0.5f,0.0f),true,false,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::m_btAdjustPivotHorizontalRightOnButtonClick( wxCommandEvent& event )
{
	// TODO: Implement m_btAdjustPivotHorizontalRightOnButtonClick
	AdjustPivot( CFEVect2(1.0f,0.0f),true,false,m_btAffectAllFrames->GetValue() );
}

void CSpriteEditorGUI::menuitem0OnMenuSelection( wxCommandEvent& event )
{
	// Load Sprite
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Load sprite"),sFilename.GetPath(true), sFilename.GetName() + SE_SPRITE_FILE_EXT, wxT("*") + SE_SPRITE_FILE_EXT, wxOPEN | wxCHANGE_DIR);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath();
		globals.m_sInputFilename = sFullFilename + SE_SPRITE_FILE_EXT;

		if ( !IsDebuggerPresent() ) // Windows API function call: requires Windows XP.
		{
			wxString sArguments = "-i=" + globals.m_sInputFilename;
			_execl(globals.m_sExecCmdLine.char_str(),globals.m_sExecCmdLine.char_str(),sArguments.char_str(),NULL);
		}
		else
		{
			// ----------------------
			// FOR DEBUGGING PURPOSES
			// ----------------------
			wxFileName sInputFilename(globals.m_sInputFilename);
			SetCurrentDirectory( sInputFilename.GetPath().char_str() );
			globals.m_sWorkingDir = sInputFilename.GetPath();

			wxString sFile = sInputFilename.GetName();
			if ( bLoadSprite( CFEString( sFile.char_str() ) ) )
				SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename ); // Setup controls according to sprite information.
			SetupControls();
		}
	}
}

void CSpriteEditorGUI::menuitem1OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );

	wxFileDialog dlgSaveFile(this, wxT("Save Sprite as ..."),sFilename.GetPath(true), sFilename.GetName(), wxT("*") + SE_SPRITE_FILE_EXT, wxSAVE | wxCHANGE_DIR);
	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath();
		globals.m_sInputFilename = sFullFilename + SE_SPRITE_FILE_EXT;

		if ( !bSaveSprite( globals.m_poSprite, CFEString( globals.m_sInputFilename.char_str() ) ) )
			ShowFileErrorMessage(globals.m_sInputFilename);
		else
			SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
	}
}

void CSpriteEditorGUI::menuitem11OnMenuSelection( wxCommandEvent& event )
{
	// Save sprite as animated gif ...
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );

	wxFileDialog dlgSaveFile(this, wxT("Save Sprite Animated GIF as ..."),sFilename.GetPath(true), sFilename.GetName(), wxT("*.gif"), wxSAVE | wxCHANGE_DIR);
	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetFullName();

		if ( !bSaveSpriteAsAnimatedGIF( globals.m_poSprite, CFEString( sFullFilename.char_str() ) ) )
			ShowFileErrorMessage(globals.m_sInputFilename);
		else
			SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
	}
}

void CSpriteEditorGUI::menuitem3OnMenuSelection( wxCommandEvent& event )
{
	// insert your code here
	Close();
}

void CSpriteEditorGUI::mo_GenerateHScrollOnMenuSelection( wxCommandEvent& event )
{
	// / Using the current material, creates a new action that performs a full scroll over the material.
	if ( globals.m_poSprite == NULL ) return;
	if ( globals.m_hCurMaterial == NULL ) return;
	if ( globals.m_iSelectedAction<0) return;

	CFESpriteAction* poSprAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction);
	if (poSprAction == NULL) return;
	if (poSprAction->m_oSeq.size()==0) return;

	long lSteps = wxGetNumberFromUser("Please enter the number of steps to perform a full horizontal scroll.","Steps","Sprite Editor",1,1,250,this);
	if ( lSteps == -1 ) return;

	// Retrieve the first sprite frame
	CFESpriteFrame oOriSprFrame = poSprAction->m_oSeq[0];

	// CFESpriteAction oSprAction;
	// oSprAction.m_eBlendMode = BM_ALPHA;
	// oSprAction.SetName("Horizontal Scroll");
	poSprAction->m_ePlayMode = SFSPM_LOOP;
	poSprAction->m_rActionTime = _1r;
	poSprAction->m_rRandStartTime = 0.0f;	

	int w,h;
	CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Width",(FEPointer) & w);
	CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Height",(FEPointer) & h);
	CFEMaterialMgr::I()->bSetMaterialProperty( globals.m_hCurMaterial,"DiffuseMap.SWrapMode", CASTVALUE(FEPointer,1) );

	FEReal rTimeStep = poSprAction->m_rActionTime / lSteps;
	FEReal rCurTime = _0r;
	FEReal rStep = _1r / (FEReal)lSteps;
	FEReal rCurPos = _0r;

	// Create the frame and intialize invariant values.
	CFESpriteFrame oFrame;

	oFrame.m_bScaleXUEqually = false;
	oFrame.m_bScaleYVEqually = false;
	oFrame.m_bUWorldCoords	= false;
	oFrame.m_bVWorldCoords	= false;

	oFrame.m_hMaterial		= globals.m_hCurMaterial;

	oFrame.m_oSize			= oOriSprFrame.m_oSize;
	oFrame.m_o1OverDims		= CFEVect2(_1r / oFrame.m_oSize.x,_1r / oFrame.m_oSize.y);

	oFrame.m_oPivot.x		= oOriSprFrame.m_oPivot.x;
	oFrame.m_oPivot.y		= oOriSprFrame.m_oPivot.y;
	oFrame.m_oUV.m_oIni.y	= oOriSprFrame.m_oUV.m_oIni.y;
	oFrame.m_oUV.m_oEnd.y	= oOriSprFrame.m_oUV.m_oEnd.y;

	oFrame.m_rDelay			= _0r;
	oFrame.m_rBlendTime		= rTimeStep;
	oFrame.m_rFrameTime		= oFrame.m_rDelay + oFrame.m_rBlendTime;

	// clean original sequence
	poSprAction->m_oSeq.clear();

	// create the scroll frames
	for ( uint i = 0; i < lSteps; i++ )
	{
		oFrame.m_oUV.m_oIni.x = oOriSprFrame.m_oUV.m_oIni.x + rCurPos;
		oFrame.m_oUV.m_oEnd.x = oOriSprFrame.m_oUV.m_oEnd.x + rCurPos;
		oFrame.m_rStartTime   = rCurTime;

		rCurPos += rStep;
		rCurTime += rTimeStep;

		// add the frame
		poSprAction->m_oSeq.push_back(oFrame);
	}

	SelectAction(globals.m_iSelectedAction);
}

void CSpriteEditorGUI::mo_GenerateVScrollOnMenuSelection( wxCommandEvent& event )
{
	if ( globals.m_poSprite == NULL ) return;
	if ( globals.m_hCurMaterial == NULL ) return;
	if ( globals.m_iSelectedAction<0) return;

	CFESpriteAction* poSprAction = globals.m_poSprite->poGetAction(globals.m_iSelectedAction);
	if (poSprAction == NULL) return;
	if (poSprAction->m_oSeq.size()==0) return;

	long lSteps = wxGetNumberFromUser("Please enter the number of steps to perform a full vertical scroll.","Steps","Sprite Editor",1,1,250,this);
	if ( lSteps == -1 ) return;

	// Retrieve the first sprite frame
	CFESpriteFrame oOriSprFrame = poSprAction->m_oSeq[0];

	// CFESpriteAction oSprAction;
	// oSprAction.m_eBlendMode = BM_ALPHA;
	// oSprAction.SetName("Horizontal Scroll");
	poSprAction->m_ePlayMode = SFSPM_LOOP;
	poSprAction->m_rActionTime = _1r;
	poSprAction->m_rRandStartTime = 0.0f;	

	int w,h;
	CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Width",(FEPointer) & w);
	CFEMaterialMgr::I()->bGetMaterialProperty(globals.m_hCurMaterial,"DiffuseMap.Height",(FEPointer) & h);
	CFEMaterialMgr::I()->bSetMaterialProperty( globals.m_hCurMaterial,"DiffuseMap.TWrapMode", CASTVALUE(FEPointer,1) );

	FEReal rTimeStep = poSprAction->m_rActionTime / lSteps;
	FEReal rCurTime = _0r;
	FEReal rStep = _1r / (FEReal)lSteps;
	FEReal rCurPos = _0r;

	// Create the frame and intialize invariant values.
	CFESpriteFrame oFrame;

	oFrame.m_bScaleXUEqually = false;
	oFrame.m_bScaleYVEqually = false;
	oFrame.m_bUWorldCoords	= false;
	oFrame.m_bVWorldCoords	= false;

	oFrame.m_hMaterial		= globals.m_hCurMaterial;
	oFrame.m_oSize			= oOriSprFrame.m_oSize;
	oFrame.m_o1OverDims		= CFEVect2(_1r / oFrame.m_oSize.x,_1r / oFrame.m_oSize.y);

	oFrame.m_oPivot.x		= oOriSprFrame.m_oPivot.x;
	oFrame.m_oPivot.y		= oOriSprFrame.m_oPivot.y;
	oFrame.m_oUV.m_oIni.x	= oOriSprFrame.m_oUV.m_oIni.x;
	oFrame.m_oUV.m_oEnd.x	= oOriSprFrame.m_oUV.m_oEnd.x;

	oFrame.m_rDelay			= _0r;
	oFrame.m_rBlendTime		= rTimeStep;
	oFrame.m_rFrameTime		= oFrame.m_rDelay + oFrame.m_rBlendTime;

	// clean original sequence
	poSprAction->m_oSeq.clear();

	// create the scroll frames
	for ( uint i = 0; i < lSteps; i++ )
	{
		oFrame.m_oUV.m_oIni.y = oOriSprFrame.m_oUV.m_oIni.y - rCurPos;
		oFrame.m_oUV.m_oEnd.y = oOriSprFrame.m_oUV.m_oEnd.y - rCurPos;
		oFrame.m_rStartTime = rCurTime;

		rCurPos += rStep;
		rCurTime += rTimeStep;

		// add the frame
		poSprAction->m_oSeq.push_back(oFrame);
	}
	
	SelectAction(globals.m_iSelectedAction);
}

void CSpriteEditorGUI::mo_AboutOnMenuSelection( wxCommandEvent& event )
{
	CEditorAboutBox oAbout(this);
	if ( oAbout.ShowModal() == wxID_OK )
	{
	}
}
