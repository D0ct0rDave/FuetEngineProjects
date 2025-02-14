#include "CActorEditorGUI.h"
/*
#include "wx/treeListCtrl.h"
#include "wx/things/spinctld.h"
#include "wx/msw/colordlg.h"

#include "CAppGlobals.h"
#include "CActorEditorGUIFuncs.h"
#include "CActorEditorGUI_GridRenderers.h"
#include "CActorEditorGUI_TreeBuilder.h"
#include "CActorEditorGUIFileOps.h"

#include "dialogs/CEditorAboutBox.h"

#include "EdUtils.h"

#include "FileOps/CAEAFileOps.h"
#include "FileOps/CSKAFileOps.h"
#include "FileOps/CSKEFileOps.h"
#include "FileOps/CPoseFileOps.h"

#include "Render.h"
#include <process.h>
*/
CActorEditorGUI::CActorEditorGUI( wxWindow* parent )
	:
	CActorEditorGUIBase( parent )
{
	SetIcon(globals.m_oIconScheme.m_oAppLogo);

	// Initialize in the application directory
	if ( (globals.m_sExecCmdLine != "") && ( !IsDebuggerPresent() ) ) // Windows API function call: requires Windows XP.
	{
		wxFileName sFilename(globals.m_sExecCmdLine);
		SetCurrentDirectory( sFilename.GetPath().char_str() );
	}

	// Includes are found under CActorGUI file properties because wxFormBuilder
	// deletes them after code generation.
	globals.GUI = this;

	// Initialize the renderer.
	SCREEN_WIDTH = m_pnDisplay->GetClientSize().GetX();
	SCREEN_HEIGHT = m_pnDisplay->GetClientSize().GetY();
	INITIAL_AIMPOS = CFEVect2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f);

	globals.m_poRenderer = new CFERenderer();
	globals.m_poRenderer->Init( (void*)m_pnDisplay->GetHandle() );
	globals.m_poRenderer->SetScreenVWidth(SCREEN_WIDTH);
	globals.m_poRenderer->SetScreenVHeight(SCREEN_HEIGHT);
	globals.m_poRenderer->bSetProperty("PRIMITIVE_SORTING",(FEPointer)true);

	globals.m_poPrevRenderer = new CFERenderer();
	globals.m_poPrevRenderer->Init( (void*)pn_PreviewDisplay->GetHandle() );
	globals.m_poPrevRenderer->SetScreenVWidth(pn_PreviewDisplay->GetClientSize().GetX() * 2);     // to keep aspect ratio
	globals.m_poPrevRenderer->SetScreenVHeight(pn_PreviewDisplay->GetClientSize().GetY() * 2);    // to keep aspect ratio
	globals.m_poPrevRenderer->bSetProperty("PRIMITIVE_SORTING",(FEPointer)true);

	// Load the default font.
	globals.m_poDefaultFont = CFEFontMgr::I()->poLoad("small");

	// Load a default sprite.
	CFESpriteInstMgr::I()->Init(1024);
	CFESkelAnimInstMgr::I()->Init(10);

	SetupSkeleton( globals.m_sSkeletonDefinition );

	// Make an image list containing small icons
	const uint ICON_WIDTH = 20;
	const uint ICON_HEIGHT = 20;
	wxImageList* images = new wxImageList(ICON_WIDTH,ICON_HEIGHT,true);

	images->Add(globals.m_oIconScheme.m_oGroupNode);    // 0
	images->Add(globals.m_oIconScheme.m_oSpriteNode);   // 1
	images->Add(globals.m_oIconScheme.m_oBoneNode);     // 2
	images->Add(globals.m_oIconScheme.m_oMeshNode);     // 3

	images->Add(globals.m_oIconScheme.m_oShowLayer);    // 4
	images->Add(globals.m_oIconScheme.m_oHideLayer);    // 5

	m_poBoneNodes->AddColumn("",30);
	m_poBoneNodes->AddColumn("",m_poBoneNodes->GetSize().GetX() - 30,wxALIGN_LEFT,-1,true,false);
	m_poBoneNodes->SetMainColumn(1);
	m_poBoneNodes->AssignImageList(images);
	m_poBoneNodes->AddRoot("Root");

	// ------
	// Time line grid
	for ( uint i = 0; i < 1; i++ )
	{
		wxGrid* poGrid;

		if ( i == 0 )
			poGrid = m_gdTimeline;
		poGrid->SetFont( wxFont( 8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Andale Mono") ) );
		poGrid->SetDefaultColSize(16);
		poGrid->SetDefaultRowSize(16);
		poGrid->SetColLabelSize(16);
		poGrid->SetRowLabelSize(0);

		poGrid->SetCellHighlightColour(globals.m_oColorScheme.m_oGridLabelBase);
		poGrid->SetLabelBackgroundColour(globals.m_oColorScheme.m_oGridLabelBase);
		poGrid->SetGridLineColour(globals.m_oColorScheme.m_oTLGridLine);

		if ( i == 0 )
			poGrid->SetDefaultRenderer(new CActionTimelineGridRenderer); /*
																		    else
																		    poGrid->SetDefaultRenderer(new CActionTimelineHeaderGridRenderer);
																		  */
		poGrid->SetColSize(0,140);  // label column
		poGrid->DisableDragColSize();
		poGrid->DisableDragRowSize();
		poGrid->EnableGridLines(false);

		for ( uint j = 0; j < poGrid->GetCols(); j++ )
		{
			poGrid->SetColMinimalWidth(j,10);
			poGrid->SetReadOnly(0,j,false);
		}

		wxFont oFont = poGrid->GetLabelFont();
		oFont.SetWeight(wxFONTWEIGHT_LIGHT);
		poGrid->SetLabelFont(oFont);
		poGrid->SetLabelTextColour(globals.m_oColorScheme.m_oTLLabels);
		poGrid->DisableCellEditControl();
		poGrid->EnableEditing(false);
		poGrid->Enable(false);
	}

	// / If there is an input file to load during startup, then load it...
	/*
	   wxMessageBox(globals.m_sInputFilename,
	            "Confirm",
	            wxYES_NO,this);
	 */

	SetTitle( APP_NAME );

	if ( globals.m_sInputFilename != "" )
	{
		wxFileName sInputFilename(globals.m_sInputFilename);
		SetCurrentDirectory( sInputFilename.GetPath().char_str() );
		globals.m_sWorkingDir = sInputFilename.GetPath(true);

		if ( bLoadAnimation( CFEString( globals.m_sInputFilename.char_str() ) ) )
		{
			// Trim just in case...
			globals.TrimAnimation(globals.m_uiNumFrames);

			// Update instanced nodes
			CFESkelAnimInstMgr::I()->Update(globals.m_poSkeleton->m_hInst,_0r);

			if ( globals.m_iSelEditBone > globals.m_poSkeleton->m_oBone.size() )
				globals.m_iSelEditBone = 0;
			SetupControls();
			SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
		}
	}
	else
	{
		globals.m_uiNumFrames++;
		SetupControls();
	}
}

void CActorEditorGUI::GUIOnClose( wxCloseEvent& event )
{
	if ( wxMessageBox("Are you sure you want to exit?\n"
	                  "All data not saved will be lost",
	                  "Confirm",
	                  wxYES_NO,this) != wxYES )
	{
		event.Veto();
		return;
	}

	// clipboard management.
	Destroy();
}

void CActorEditorGUI::GUIOnIdle( wxIdleEvent& event )
{
	RenderPreview(globals.m_poPrevRenderer);
	RenderMain(globals.m_poRenderer);

	if ( globals.m_bPlaying )
	{
		// WARNING: Este código es necesario!!! 
		// Debería activarse / desactivarse mediante opción de menú.
		if (m_cbUpdateScrollBar->IsChecked())
		{
			uint uiNewFrame = (globals.m_uiNumFrames - 1) * (globals.m_fCurAnimTime / globals.m_fTotalAnimTime);
			if ( uiNewFrame != globals.m_uiCurFrame )
			{
				// Show cursor over timeline.
				globals.m_uiCurFrame = uiNewFrame;
				sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
				m_gdTimeline->Refresh(false);
			}
		}
	}
	else
		bt_Play->SetLabel("Play");

	event.RequestMore();
}

void CActorEditorGUI::m_pnDisplayOnKeyDown( wxKeyEvent& event )
{
	DisplayPanelOnKeyDown(event);
}

void CActorEditorGUI::m_pnDisplayOnKeyUp( wxKeyEvent& event )
{
	DisplayPanelOnKeyUp(event);
}

void CActorEditorGUI::m_pnDisplayOnLeftDown( wxMouseEvent& event )
{
	DisplayPanelOnMouseDown(event);
}

void CActorEditorGUI::m_pnDisplayOnLeftUp( wxMouseEvent& event )
{
	DisplayPanelOnMouseUp(event);
}

void CActorEditorGUI::m_pnDisplayOnMiddleDown( wxMouseEvent& event )
{
	// TODO: Implement m_pnDisplayOnMiddleDown
	DisplayPanelOnMiddleMouseDown(event);
}

void CActorEditorGUI::m_pnDisplayOnMiddleUp( wxMouseEvent& event )
{
	// TODO: Implement m_pnDisplayOnMiddleUp
	DisplayPanelOnMiddleMouseUp(event);
}

void CActorEditorGUI::m_pnDisplayOnMotion( wxMouseEvent& event )
{
	DisplayPanelOnMouseMove(event);
}

void CActorEditorGUI::m_pnDisplayOnMouseWheel( wxMouseEvent& event )
{
	DisplayPanelOnMouseWheel(event);
}

void CActorEditorGUI::m_pnDisplayOnSize( wxSizeEvent& event )
{
	// TODO: Implement m_pnDisplayOnSize
	SCREEN_WIDTH = event.GetSize().x;
	SCREEN_HEIGHT = event.GetSize().y;
	INITIAL_AIMPOS = CFEVect2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f);

	globals.m_poRenderer->SetScreenVWidth(SCREEN_WIDTH);
	globals.m_poRenderer->SetScreenVHeight(SCREEN_HEIGHT);

	globals.m_poRenderer->bSetProperty("ScreenWidth",(FEPointer)event.GetSize().x);
	globals.m_poRenderer->bSetProperty("ScreenHeight",(FEPointer)event.GetSize().y);

	event.Skip();
}

void CActorEditorGUI::m_edBackXOffsetOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edBackXOffsetOnSpinCtrl
}

void CActorEditorGUI::m_edBackYOffsetOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edBackYOffsetOnSpinCtrl
}

void CActorEditorGUI::m_edBackXScaleOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edBackXScaleOnSpinCtrl
}

void CActorEditorGUI::m_edBackYScaleOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edBackYScaleOnSpinCtrl
}

void CActorEditorGUI::m_cbEditModeOnChoice( wxCommandEvent& event )
{
	globals.m_poSkeleton->SetEditMode( m_cbEditMode->GetSelection() );

	// Update the instance
	CFESkelAnimInstMgr::I()->SetAction( globals.m_poSkeleton->m_hInst, m_cbEditMode->GetSelection() ); // WARNING: this puts the animation instance to the 0 sec pose
	CFESkelAnimInstMgr::I()->SetTime(globals.m_poSkeleton->m_hInst,globals.m_fTime);

	for ( uint i = 0; i < globals.m_poSkeleton->m_oBone.size(); i++ )
	{
		if ( globals.m_poSkeleton->m_oBone[i].m_poBone != NULL )
		{
			globals.m_poSkeleton->m_oBone[i].m_poBone->SetAngle(0);
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poBone);
		}

		/*
		   if (globals.m_poSkeleton->m_oBone[i].m_poBoneJoint != NULL)
		   {
		   globals.m_poSkeleton->m_oBone[i].m_poBoneJoint->SetAngle(0);
		   globals.m_poSkeleton->m_oBone[i].m_poBoneJoint->SetPos(CFEVect2::ZERO());
		   UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poBoneJoint);
		   }

		   if (globals.m_poSkeleton->m_oBone[i].m_poChildJoint != NULL)
		   {
		   globals.m_poSkeleton->m_oBone[i].m_poChildJoint->SetAngle(0);
		   globals.m_poSkeleton->m_oBone[i].m_poChildJoint->SetPos(CFEVect2::ZERO());
		   UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poChildJoint);
		   }
		 */

		if ( globals.m_poSkeleton->m_oBone[i].m_poParentJoint != NULL )
		{
			globals.m_poSkeleton->m_oBone[i].m_poParentJoint->SetAngle(0);
			globals.m_poSkeleton->m_oBone[i].m_poParentJoint->SetPos( CFEVect2::ZERO() );
			UpdateNodeInstance(globals.m_poSkeleton->m_oBone[i].m_poParentJoint);
		}
	}

	SetupBoneControls();
}

void CActorEditorGUI::m_btUndoOnButtonClick( wxCommandEvent& event )
{
	#ifndef DISABLE_UNDO_REDO
	if ( !m_btUndo->IsEnabled() ) return;
	globals.EditUndo();

	if ( !globals.m_oUndoStack.bCanPop() )
	{
		m_btUndo->Disable();
		m_mbMainMenu->Enable(wxID_UNDO,false);
	}

	m_mbMainMenu->Enable(wxID_REDO,true);
	m_btRedo->Enable();

	SetupControls();

	// Show cursor over timeline.
	m_gdTimeline->Refresh(false);
	#endif
}

void CActorEditorGUI::m_btRedoOnButtonClick( wxCommandEvent& event )
{
	#ifndef DISABLE_UNDO_REDO
	if ( !m_btRedo->IsEnabled() ) return;
	globals.EditRedo();

	if ( !globals.m_oUndoStack.bCanUnpop() )
	{
		m_btRedo->Disable();
		m_mbMainMenu->Enable(wxID_REDO,false);
	}

	m_btUndo->Enable();
	m_mbMainMenu->Enable(wxID_UNDO,true);

	SetupControls();

	// Show cursor over timeline.
	m_gdTimeline->Refresh(false);
	#endif
}

void CActorEditorGUI::sb_KeyFrameOnScroll( wxScrollEvent& event )
{
	// Avoid events from sliders ... !?!
	if ( event.GetEventObject() == sb_KeyFrame )
	{
		globals.m_uiCurFrame = sb_KeyFrame->GetThumbPosition();
		globals.m_fTime = (float)globals.m_uiCurFrame * SECS_PER_TICK;

		SetupBoneControls();
		
		
		// Refresh render to allow scroll the keyframe scroll bar be used for animation previewing purposes.
		RenderPreview(globals.m_poPrevRenderer);
		RenderMain(globals.m_poRenderer);

		// Show cursor over timeline.
		m_gdTimeline->MakeCellVisible((globals.m_iSelEditBone!=-1)?globals.m_iSelEditBone:0,globals.m_uiCurFrame+1);
		m_gdTimeline->Refresh(false);
	}
}

void CActorEditorGUI::bt_AddKeyframeOnButtonClick( wxCommandEvent& event )
{
	globals.EditBegin();

	globals.InsertFrameAt(globals.m_uiCurFrame);
	globals.SyncSprites();
	SetupControls();

	sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
	m_gdTimeline->MakeCellVisible((globals.m_iSelEditBone!=-1)?globals.m_iSelEditBone:0,globals.m_uiCurFrame+1);

	globals.EditEnd();
}

void CActorEditorGUI::bt_DelKeyframeOnButtonClick( wxCommandEvent& event )
{
	if ( (globals.m_uiNumFrames > 1) && (globals.m_uiCurFrame > 0) )
	{
		globals.EditBegin();

		globals.DeleteFrameAt(globals.m_uiCurFrame);
		globals.SyncSprites();
		SetupControls();

		sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
		m_gdTimeline->MakeCellVisible((globals.m_iSelEditBone!=-1)?globals.m_iSelEditBone:0,globals.m_uiCurFrame+1);

		globals.EditEnd();
	}
}

void CActorEditorGUI::m_gdTimelineOnGridCellLeftClick( wxGridEvent& event )
{
	if ( globals.m_uiNumFrames == 0 ) return;
	int row = event.GetRow();
	int col = event.GetCol();

	// Select another bone?
	if ( (row >= 0) && (row != globals.m_iSelEditBone) )
	{
		globals.m_iSelEditBone = row;
		SetupBoneControls();
	}

	if ( col > 0 )
	{
		// Move the cursor to the picked position.
		globals.m_uiCurFrame = col - 1;
		sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
		globals.m_fTime = (float)(col - 1) * SECS_PER_TICK;

		// Since there is a change in the time we need to setup everything in the skeleton according to the new time!!!!
		CFESkelAnimInstMgr::I()->SetTime(globals.m_poSkeleton->m_hInst,globals.m_fTime);

		SetupBoneControls();
	}
	else if ( row > 0 )
		// primera columna
		if ( event.GetPosition().x > 95 )
		{
			// Set the bone as non editable / non visible
			CFESkelAnimNodeAction* poAction = globals.m_poSkeleton->m_oBone[row].m_poPJointAction;
			poAction->Show( !poAction->bIsVisible() );
		} // Show cursor over timeline.
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_gdTimelineOnGridCellRightClick( wxGridEvent& event )
{
	if ( globals.m_uiNumFrames == 0 ) return;
	int row = event.GetRow();
	int col = event.GetCol();

	if ( row >= 0 )
	{
		globals.m_iSelEditBone = row;
		SetupBoneControls();

		CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];
		if ( col > 0 )
		{
			globals.m_uiCurFrame = col - 1;

			int iKeyFrame = globals.iGetKeyFrame( poBone, globals.m_uiCurFrame,0x01);
			CFEKeyFrame* poKF = NULL;

			if ( iKeyFrame != -1 )
				poKF = poBone->m_poPJointAction->m_rAngleFunc.poGetKeyFrame(iKeyFrame);
			else
			{
				iKeyFrame = globals.iGetKeyFrame( poBone, globals.m_uiCurFrame,0x02);
				if ( iKeyFrame != -1 )
					poKF = poBone->m_poBJointAction->m_oPosFunc.poGetKeyFrame(iKeyFrame);
			}

			if ( poKF != NULL )
			{
				// Retrieve the current keyframe function:
				m_popKeyFrameMenu->Check( ID_MNU_CONSTANT,   (poKF->eGetLerpFunc() == KFLF_CONSTANT) );
				m_popKeyFrameMenu->Check( ID_MNU_LINEAR,     (poKF->eGetLerpFunc() == KFLF_LERP) );
				m_popKeyFrameMenu->Check( ID_MNU_SINUSOIDAL, (poKF->eGetLerpFunc() == KFLF_SIN) );
				m_popKeyFrameMenu->Check( ID_MNU_EXPONENTIAL,(poKF->eGetLerpFunc() == KFLF_EXP) );
				m_popKeyFrameMenu->Check( ID_MNU_INVSIN,     (poKF->eGetLerpFunc() == KFLF_ISIN) );
				m_popKeyFrameMenu->Check( ID_MNU_RANDOM,     (poKF->eGetLerpFunc() == KFLF_RAND) );
				m_popKeyFrameMenu->Check( ID_MNU_SINSIN,     (poKF->eGetLerpFunc() == KFLF_SINSIN) );
				m_popKeyFrameMenu->Check( ID_MNU_EXPLOG,     (poKF->eGetLerpFunc() == KFLF_EXPLOG) );

				// m_popKeyFrameMenu->Enable(ID_MNU_DELETEKEYFRAME,false);
				PopupMenu(m_popKeyFrameMenu);
			}
		}
		else
			// Functions that affect all the keyframes
			PopupMenu(m_popKeyFrameFuncMenu);
	}
	else
	{
	}
}

void CActorEditorGUI::m_gdTimelineOnGridLabelLeftClick( wxGridEvent& event )
{
	int col = event.GetCol();
	if ( col > 0 )
	{
		// Move the cursor to the picked position.
		globals.m_uiCurFrame = col - 1;
		sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
		globals.m_fTime = (float)(col - 1) * SECS_PER_TICK;

		SetupBoneControls();

		// Show cursor over timeline.
		m_gdTimeline->Refresh(false);
	}
}

void CActorEditorGUI::m_gdTimelineOnGridLabelRightClick( wxGridEvent& event )
{
	int col = event.GetCol();

	// Row 0 pop up menu.
	// copy / paste / etc ...
	if ( col > 0 )
	{
		globals.m_uiBlockCurCol = (col - 1); // First col is for the labels

		// Check paste
		m_pupmKeyFrameOps->Enable(ID_MNU_PASTEPOSE,globals.m_poCopyPose != NULL);
		m_pupmKeyFrameOps->Enable(ID_MNU_PASTEPOSEFORCE,globals.m_poCopyPose != NULL);

		// handle selection options
		switch ( globals.m_uiBlockSelState )
		{
			// no selection
			case 0:
			{
				m_pupmKeyFrameOps->SetLabel(wxID_POSE_SEL_STATE,"Start Selection of Poses");
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_COPY,false);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_CLEAR,false);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_DELETE,false);
			}
			break;

			// no selection still
			case 1:
			{
				m_pupmKeyFrameOps->SetLabel(wxID_POSE_SEL_STATE,"End Selection of Poses");
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_COPY,false);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_CLEAR,false);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_DELETE,false);
			}
			break;

			// Make available selection options
			case 2:
			{
				m_pupmKeyFrameOps->SetLabel(wxID_POSE_SEL_STATE,"Start Selection of Poses");
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_COPY,true);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_CLEAR,true);
				m_pupmKeyFrameOps->Enable(wxID_POSE_SEL_DELETE,true);
			}
			break;
		}

		PopupMenu(m_pupmKeyFrameOps);
	}
}

void CActorEditorGUI::OnPlayAnimationClick( wxCommandEvent& event )
{
	PlayStopAnimation();
}

void CActorEditorGUI::OnLoopAnimationClick( wxCommandEvent& event )
{
	// DMC WARNinG: Si descomentamos este código, hace imposible mover huesos en el último frame de la animación, incluso
	// el PIVOT. Esto podría ser deseable !?!?

	/*
	   for (uint i=0;i<globals.m_poSkeleton->m_oBone.size();i++)
	   {
	   if (cb_Loop->GetValue())
	   {
	   globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.SetWrapMode(KFBFWM_LOOP);
	   }
	   else
	   {
	   globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.SetWrapMode(KFBFWM_FINALVALUE);
	   }
	   }
	 */

	m_gdTimeline->Refresh();
}

void CActorEditorGUI::sld_AnimTimeOnCommandScroll( wxScrollEvent& event )
{
	FEReal rFact = (FEReal)sld_AnimTime->GetValue() / (FEReal)sld_AnimTime->GetMax();

	// kind of logarithmic scale...
	// V = value, M = MaxValue, T = TotalAnimTime, C = MAX_ANIM_TIME constant
	// T = C*((V/M)^4);

	// globals.m_fTotalAnimTime = MAX_ANIM_TIME * (rFact * rFact * rFact * rFact);
	globals.m_fTotalAnimTime = MAX_ANIM_TIME * pow(rFact,4);

	m_edAnimTime->SetValue( globals.m_fTotalAnimTime * 1000 );
	globals.SyncSprites();
}

void CActorEditorGUI::m_edAnimTimeOnSpinCtrl( wxSpinEvent& event )
{
	FEReal rFact = (FEReal)m_edAnimTime->GetValue() / (FEReal)m_edAnimTime->GetMax();
	globals.m_fTotalAnimTime = MAX_ANIM_TIME * rFact;

	// V = value, M = MaxValue, T = TotalAnimTime, C = MAX_ANIM_TIME constant

	// T = C*((V/M)^4);
	// T/C = (V/M)^4
	// (T/C)^1/4 = V/M
	// M*((T/C)^1/4) = V
	rFact = pow(globals.m_fTotalAnimTime / MAX_ANIM_TIME,0.25);
	sld_AnimTime->SetValue( rFact * sld_AnimTime->GetMax() );

	globals.SyncSprites();
}

void CActorEditorGUI::m_edBoneXCoordOnSpinCtrl( wxSpinEvent& event )
{
	// in animation mode, this control only does translation no rotation
	int iEditMode = m_cbEditMode->GetSelection();
	if ( iEditMode > 0 ) iEditMode = 2;
	globals.EditBegin();

	CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];

	CFEVect2 oDisplayPos;
	oDisplayPos.x = globals.GUI->m_edBoneXCoord->GetValue();
	oDisplayPos.y = globals.GUI->m_edBoneYCoord->GetValue();

	globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone,globals.m_fTime,oDisplayPos,globals.m_oOldBonePos,globals.m_oOldParentPos,globals.m_fOldAngle,iEditMode);

	globals.EditEnd();
	RefreshBoneControls();
	m_gdTimeline->Refresh();
}

void CActorEditorGUI::m_edBoneYCoordOnSpinCtrl( wxSpinEvent& event )
{
	// in animation mode, this control only does translation no rotation
	int iEditMode = m_cbEditMode->GetSelection();
	if ( iEditMode > 0 ) iEditMode = 2;
	globals.EditBegin();

	CFEVect2 oDisplayPos;
	oDisplayPos.x = globals.GUI->m_edBoneXCoord->GetValue();
	oDisplayPos.y = globals.GUI->m_edBoneYCoord->GetValue();
	globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone,globals.m_fTime,oDisplayPos,globals.m_oOldBonePos,globals.m_oOldParentPos,globals.m_fOldAngle,iEditMode);

	globals.EditEnd();
	RefreshBoneControls();
	m_gdTimeline->Refresh();
}

void CActorEditorGUI::m_button5OnButtonClick( wxCommandEvent& event )
{
	// in animation mode, this control only does translation no rotation
	int iEditMode = m_cbEditMode->GetSelection();
	if ( iEditMode > 0 )
	{
		iEditMode = 2;

		// Reset bone to skeleton position.
		globals.EditBegin();

		EFEKFLerpFunc eLerpFunc = KFLF_LERP;
		CFEKeyFrame* poKF = globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_poBJointAction->m_oPosFunc.poGetKeyFrameAtTime(globals.m_fTime);
		if ( poKF != NULL ) eLerpFunc = poKF->eGetLerpFunc();
		globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone].m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),globals.m_fTime,eLerpFunc);

		globals.EditEnd();

		// Update instanced nodes
		RefreshBoneControls();
		m_gdTimeline->Refresh();
	}
}

void CActorEditorGUI::m_poBoneNodesOnTreeSelChanged( wxTreeEvent& event )
{
	// insert your code here

	// don't want to handle this event if we're setting the bone nodes controls
	if ( globals.m_bSettingBoneControls ) return;
	int iCol = event.GetInt();
	if ( iCol < 0 ) return;
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() )
	{
		event.Veto();
		return;
	}

	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	switch ( iCol )
	{
		// toggle visibility
		case 0:
		{
			// Show/Hide the object
			poBoneNode->m_poObj->Show( !poBoneNode->m_poObj->bIsVisible() );
			UpdateNodeInstance(poBoneNode->m_poObj);

			// Set image appropriatelly.
			m_poBoneNodes->SetItemImage(oID,0,poBoneNode->m_poObj->bIsVisible()?IMG_VISIBLE:IMG_INVISIBLE);

			event.Veto();
		}
		break;

		// handle element selection.
		case 1:
		{
			SetupBoneNodeControls(poBoneNode);
			event.Veto();
		}
		break;
	}
}

void CActorEditorGUI::m_edNodePosXOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edNodePosXOnSpinCtrl
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	CFEVect2 oPos(m_edNodePosX->GetValue(),poBoneNode->m_poObj->oGetPos().y);
	poBoneNode->m_poObj->SetPos(oPos);
	UpdateNodeInstance(poBoneNode->m_poObj);
}

void CActorEditorGUI::m_edNodePosYOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edNodePosYOnSpinCtrl
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	CFEVect2 oPos( poBoneNode->m_poObj->oGetPos().x,m_edNodePosY->GetValue() );
	poBoneNode->m_poObj->SetPos(oPos);
	UpdateNodeInstance(poBoneNode->m_poObj);
}

void CActorEditorGUI::m_sldNodeAngleOnCommandScroll( wxScrollEvent& event )
{
	// TODO: Implement m_sldNodeAngleOnCommandScroll
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	poBoneNode->m_poObj->SetAngle( _PIr * (float)m_sldNodeAngle->GetValue() / 180.0f );
	UpdateNodeInstance(poBoneNode->m_poObj);
}

void CActorEditorGUI::m_edNodeDepthOnSpinCtrl( wxSpinEvent& event )
{
	// TODO: Implement m_edNodeDepthOnSpinCtrl
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	poBoneNode->m_poObj->SetDepth( m_edNodeDepth->GetValue() / 100.0f );
	UpdateNodeInstance(poBoneNode->m_poObj);
}

void CActorEditorGUI::m_cpSelectColorOnColourChanged( wxColourPickerEvent& event )
{
	// TODO: Implement m_cpSelectColorOnColourChanged
	#ifdef USE_SKELANIM_SCALE_COLOR
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	CFEColor oColor = WXColor2FEColor( event.GetColour() );
	CFEColor oOriginalColor = poBoneNode->m_poObj->oGetColor();
	oColor.a = oOriginalColor.a;

	poBoneNode->m_poObj->SetColor(oColor);
	UpdateNodeInstance(poBoneNode->m_poObj);
	#endif
}

void CActorEditorGUI::m_sldNodeAlphaOnCommandScroll( wxScrollEvent& event )
{
	// TODO: Implement m_sldNodeAlphaOnCommandScroll
	#ifdef USE_SKELANIM_SCALE_COLOR
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	CFEColor oColor = poBoneNode->m_poObj->oGetColor();
	oColor.a = (float)m_sldNodeAlpha->GetValue() / 255.0f;

	poBoneNode->m_poObj->SetColor(oColor);
	UpdateNodeInstance(poBoneNode->m_poObj);
	#endif
}

void CActorEditorGUI::bt_LoadSpriteOnButtonClick( wxCommandEvent& event )
{
	// / Estamos aqu� porque el panel de propiedades de sprite est� activado, por lo que asumimos que el nodo es de tipo sprite.
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	CFEString sString = poBoneNode->sGetExternalFile();

	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Choose a file"), sFilename.GetPath(true), sFilename.GetName(),wxT("*.spr;*.tga;*.png"), wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		sFullFilename.Replace("\\","/");
		wxString sCurDir = sFilename.GetPath(true);

		CFEString sSpriteFile = CFEString( sFullFilename.char_str() );
		FEHandler hInst = CFESpriteInstMgr::I()->hGetInstance(sSpriteFile);

		globals.EditBegin();

		AssignSpriteToBoneNode(poBoneNode,hInst);
		globals.SyncSprites();

		globals.EditEnd();
	}
}

void CActorEditorGUI::bt_ClearSpriteOnButtonClick( wxCommandEvent& event )
{
	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	globals.EditBegin();

	AssignSpriteToBoneNode(poBoneNode,NULL);

	globals.EditEnd();
}

void CActorEditorGUI::m_cbSpriteActionListOnChoice( wxCommandEvent& event )
{
	// / Estamos aqu porque el panel de propiedades de sprite est� activado, por lo que asumimos que el nodo es de tipo sprite.
	uint uiSel = globals.GUI->m_cbSpriteActionList->GetSelection();

	wxTreeItemId oID = m_poBoneNodes->GetSelection();
	if ( !oID.IsOk() ) return;
	CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
	if ( poBoneNode == NULL ) return;
	globals.EditBegin();

	AssignSpriteActionToBoneNode(poBoneNode,uiSel);

	globals.EditEnd();
}

void CActorEditorGUI::m_menuItem121OnMenuSelection( wxCommandEvent& event )
{
	if ( globals.m_poCopyPose != NULL ) delete globals.m_poCopyPose;
	globals.m_poCopyPose = globals.poGetCurrentPose();
}

void CActorEditorGUI::m_menuItem131OnMenuSelection( wxCommandEvent& event )
{
	if ( globals.m_poCopyPose == NULL ) return;
	globals.EditBegin();

	globals.PastePose(globals.m_poCopyPose,false);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_menuItem1311OnMenuSelection( wxCommandEvent& event )
{
	if ( globals.m_poCopyPose == NULL ) return;
	globals.EditBegin();

	globals.PastePose(globals.m_poCopyPose,true);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_menuItem14OnMenuSelection( wxCommandEvent& event )
{
	// / Clear keyframes at this moment in time.
	if ( globals.m_fTime == 0.0 ) return;
	globals.EditBegin();

	globals.ClearFrameAt(globals.m_uiCurFrame);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_menuItem141OnMenuSelection( wxCommandEvent& event )
{
	globals.EditBegin();

	// Reset current position to skeleton pose.
	CSkelPose oPose;
	for ( uint b = 0; b < globals.m_poSkeleton->m_oBone.size(); b++ )
	{
		oPose.m_oPos.push_back( CFEVect2::ZERO() );
		oPose.m_rAngle.push_back(_0r);
	}

	globals.PastePose(&oPose);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_miPoseSelStateOnMenuSelection( wxCommandEvent& event )
{
	// handle selection
	switch ( globals.m_uiBlockSelState )
	{
		// start selection
		case 0:
		{
			globals.m_uiBlockStartFrame = globals.m_uiBlockCurCol;
			globals.m_uiBlockSelState++;
		}
		break;

		// finish selection
		case 1:
		{
			if ( globals.m_uiBlockCurCol < globals.m_uiBlockStartFrame )
			{
				globals.m_uiBlockEndFrame = globals.m_uiBlockStartFrame;
				globals.m_uiBlockStartFrame = globals.m_uiBlockCurCol;
			}
			else
				globals.m_uiBlockEndFrame = globals.m_uiBlockCurCol;
			globals.m_uiBlockSelState++;
		}
		break;

		// start selection
		case 2:
		{
			globals.m_uiBlockStartFrame = globals.m_uiBlockCurCol;
			globals.m_uiBlockSelState = 1;
		}
		break;
	}

	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_miInsertPoseSelOnMenuSelection( wxCommandEvent& event )
{
	// TODO: Implement m_miClearPoseSelOnMenuSelection
	if ( (globals.m_uiCurFrame >= globals.m_uiBlockStartFrame) && (globals.m_uiCurFrame <= globals.m_uiBlockEndFrame) )
		// cannot insert the selected block inside the selected block
		return;
	globals.EditBegin();

	int iNumBlocks = globals.m_uiBlockEndFrame - globals.m_uiBlockStartFrame + 1;

	// -----------------------------------
	// make space at the given position...
	for ( uint i = 0; i < iNumBlocks; i++ )
	{
		globals.InsertFrameAt(globals.m_uiCurFrame + i); // -----------------------------------
	}
	// selection block has been displaced ?
	if ( globals.m_uiCurFrame < globals.m_uiBlockStartFrame )
	{
		globals.m_uiBlockStartFrame += iNumBlocks;
		globals.m_uiBlockEndFrame += iNumBlocks;
	}

	// -----------------------------------
	// / copy and paste the first key frame...
	float fOldTime = globals.m_fTime;
	globals.m_fTime = globals.m_uiBlockStartFrame * SECS_PER_TICK;

	// copy pose at globals.m_uiBlockStartFrame+i
	CSkelPose* poPose = globals.poGetCurrentPose();

	// paste pose at globals.m_uiCurFrame+i
	globals.m_fTime = globals.m_uiCurFrame * SECS_PER_TICK;
	globals.PastePose(poPose);

	delete poPose;
	globals.m_fTime = fOldTime;

	// -----------------------------------
	// copy the rest of keyframes
	for ( uint i = 1; i < iNumBlocks; i++ )
		globals.CopyKeyFramesFromTo(globals.m_uiBlockStartFrame + i,globals.m_uiCurFrame + i);
	globals.EditEnd();

	// Update instanced nodes
	SetupControls();
	sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
	m_gdTimeline->MakeCellVisible((globals.m_iSelEditBone!=-1)?globals.m_iSelEditBone:0,globals.m_uiCurFrame+1);
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_miClearPoseSelOnMenuSelection( wxCommandEvent& event )
{
	// TODO: Implement m_miClearPoseSelOnMenuSelection
	globals.EditBegin();

	// clear all the keyframes between selection marks...
	for ( uint i = globals.m_uiBlockStartFrame; i <= globals.m_uiBlockEndFrame; i++ )
		globals.ClearFrameAt(i);
	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_miDeletePoseSelOnMenuSelection( wxCommandEvent& event )
{
	globals.EditBegin();

	// delete all the keyframes between selection marks...
	for ( uint i = globals.m_uiBlockEndFrame; i >= globals.m_uiBlockStartFrame; i-- )
	{
		globals.DeleteFrameAt(i); // / as the block has been deleted, invalidate the selection
	}
	globals.m_uiBlockSelState = 0;

	globals.EditEnd();

	SetupControls();
	sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
	m_gdTimeline->MakeCellVisible((globals.m_iSelEditBone!=-1)?globals.m_iSelEditBone:0,globals.m_uiCurFrame+1);
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_menuItem17OnMenuSelection( wxCommandEvent& event )
{
	globals.EditBegin();

	CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];

	poBone->m_poPJointAction->m_rAngleFunc.InsertKeyFrame(poBone->m_poPJointAction->m_rAngleFunc.oGetValue(_0r),globals.m_fTime,KFLF_LERP);
	poBone->m_poBJointAction->m_oPosFunc.InsertKeyFrame(poBone->m_poBJointAction->m_oPosFunc.oGetValue(_0r),globals.m_fTime,KFLF_LERP);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
}

void CActorEditorGUI::m_menuItem16OnMenuSelection( wxCommandEvent& event )
{
	globals.EditBegin();

	CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];

	poBone->m_poPJointAction->m_rAngleFunc.DeleteKeyFrameAtTime(globals.m_fTime);
	poBone->m_poBJointAction->m_oPosFunc.DeleteKeyFrameAtTime(globals.m_fTime);

	globals.EditEnd();

	// Update instanced nodes
	RefreshBoneControls();
	m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_menuItem29OnMenuSelection( wxCommandEvent& event )
{
	// (Constant interpolation)
	EFEKFLerpFunc eLF = KFLF_CONSTANT;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem18OnMenuSelection( wxCommandEvent& event )
{
	// (linear interpolation)
	EFEKFLerpFunc eLF = KFLF_LERP;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem19OnMenuSelection( wxCommandEvent& event )
{
	// (sinusoidal interpolation)
	EFEKFLerpFunc eLF = KFLF_SIN;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem20OnMenuSelection( wxCommandEvent& event )
{
	// (exponential interpolation)
	EFEKFLerpFunc eLF = KFLF_EXP;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem30OnMenuSelection( wxCommandEvent& event )
{
	// (inv sin interpolation)
	EFEKFLerpFunc eLF = KFLF_ISIN;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem21OnMenuSelection( wxCommandEvent& event )
{
	// (random interpolation)
	EFEKFLerpFunc eLF = KFLF_RAND;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem22OnMenuSelection( wxCommandEvent& event )
{
	// (sinsin interpolation)
	EFEKFLerpFunc eLF = KFLF_SINSIN;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem23OnMenuSelection( wxCommandEvent& event )
{
	// (explog interpolation)
	EFEKFLerpFunc eLF = KFLF_EXPLOG;
	globals.SetKeyFrameInterpolation(globals.m_iSelEditBone,globals.m_uiCurFrame,eLF);
}

void CActorEditorGUI::m_menuItem161OnMenuSelection( wxCommandEvent& event )
{
	if ( wxMessageBox("Are you sure you want to delete all the key frames of this bone?\n",
	                  "Confirm",
	                  wxYES_NO,this) != wxYES )
	{
		return;                   // delete all the key frames in the current bone
	}
	CBone* poBone = &globals.m_poSkeleton->m_oBone[globals.m_iSelEditBone];

	poBone->m_poBJointAction->m_oPosFunc.Reset();
	poBone->m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),_0r,KFLF_LERP);
	poBone->m_poBJointAction->m_oPosFunc.Invalidate();

	poBone->m_poPJointAction->m_rAngleFunc.Reset();
	poBone->m_poPJointAction->m_rAngleFunc.InsertKeyFrame(_0r,_0r,KFLF_LERP);
	poBone->m_poPJointAction->m_rAngleFunc.Invalidate();

	m_gdTimeline->Refresh();
}

void CActorEditorGUI::m_menuItem291OnMenuSelection( wxCommandEvent& event )
{
	// (Constant interpolation)
	EFEKFLerpFunc eLF = KFLF_CONSTANT;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem181OnMenuSelection( wxCommandEvent& event )
{
	EFEKFLerpFunc eLF = KFLF_LERP;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem191OnMenuSelection( wxCommandEvent& event )
{
	// (sin interpolation)
	EFEKFLerpFunc eLF = KFLF_SIN;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem201OnMenuSelection( wxCommandEvent& event )
{
	// (exp interpolation)
	EFEKFLerpFunc eLF = KFLF_EXP;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem301OnMenuSelection( wxCommandEvent& event )
{
	// (inv sin interpolation)
	EFEKFLerpFunc eLF = KFLF_ISIN;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem211OnMenuSelection( wxCommandEvent& event )
{
	// (random interpolation)
	EFEKFLerpFunc eLF = KFLF_RAND;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem221OnMenuSelection( wxCommandEvent& event )
{
	// (sinsin interpolation)
	EFEKFLerpFunc eLF = KFLF_SINSIN;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::m_menuItem231OnMenuSelection( wxCommandEvent& event )
{
	// (explog interpolation)
	EFEKFLerpFunc eLF = KFLF_EXPLOG;
	globals.SetKeyFrameFuncInterpolation(globals.m_iSelEditBone,eLF);
}

void CActorEditorGUI::menuitem0OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Load animation"),sFilename.GetPath(true), sFilename.GetName(),wxT("*") + AE_ANIMATION_FILE_EXT, wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath(true);
		globals.m_sInputFilename = sFullFilename + AE_ANIMATION_FILE_EXT;

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
			globals.m_sWorkingDir = sInputFilename.GetPath() ;
			if ( !SetCurrentDirectory( wxString(globals.m_sWorkingDir + "\\").char_str() ) )
			{
				int iLastError = GetLastError();
				printf("%d",iLastError);
			}

			// Setup controls according to sprite information.
			if ( bLoadAnimation( CFEString( globals.m_sInputFilename.char_str() ) ) )
			{
				// Trim just in case...
				globals.TrimAnimation(globals.m_uiNumFrames);

				// Update instanced nodes
				CFESkelAnimInstMgr::I()->Update(globals.m_poSkeleton->m_hInst,_0r);

				SetupControls();
				SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
			}
		}
	}
}

void CActorEditorGUI::menuitem1OnMenuSelection( wxCommandEvent& event )
{
	if ( globals.m_sInputFilename == "" )
		// Save as ...
		menuitem11OnMenuSelection(event);
	else
	{
		if ( wxMessageBox("Current file contents will be overwritten?\n"
		                  "Are you sure?",
		                  "Confirm",
		                  wxYES_NO,this) != wxYES ) return;
		wxFileName sFilename(globals.m_sInputFilename);
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath(true);

		// set working directory as the current one
		if ( !SetCurrentDirectory( wxString(globals.m_sWorkingDir + "\\").char_str() ) )
		{
			int iLastError = GetLastError();
			printf("%d",iLastError);
		}

		// Trim just in case...
		globals.TrimAnimation(globals.m_uiNumFrames);

		// Save
		if ( !bSaveAnimation(CFEString( sFullFilename.char_str() ) + AE_ANIMATION_FILE_EXT) )
			ShowFileErrorMessage(sFullFilename + AE_ANIMATION_FILE_EXT);
		else
		// save skeleton action
		if ( !bSaveSkeletonAction( CFEString( sFullFilename.char_str() ) + FE_SKELACTION_FILE_EXT,globals.m_poSkeleton,m_cbAnimatePivot->GetValue() ) )
			ShowFileErrorMessage(sFullFilename + FE_SKELACTION_FILE_EXT); // save skeleton
		                                                                 // wxString sSkeletonFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + "actor";
		                                                                 // SaveSkeleton(CFEString(sSkeletonFilename.char_str())+FE_SKELETON_FILE_EXT,globals.m_poSkeleton);
	}
}

void CActorEditorGUI::menuitem11OnMenuSelection( wxCommandEvent& event )
{
	// -----------
	// SAVE AS ...
	// -----------
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgSaveFile(this, wxT("Save animation"),sFilename.GetPath(true), sFilename.GetName(), wxT("*") + AE_ANIMATION_FILE_EXT, wxSAVE | wxCHANGE_DIR);

	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		// globals.m_sFilename     = sFullFilename;
		globals.m_sWorkingDir = sFilename.GetPath(true);
		// globals.m_sCurFilename	= sFilename.GetName();

		// set working directory as the current one
		if ( !SetCurrentDirectory( wxString(globals.m_sWorkingDir + "\\").char_str() ) )
		{
			int iLastError = GetLastError();
			printf("%d",iLastError);
		}

		// Trim just in case...
		globals.TrimAnimation(globals.m_uiNumFrames);

		if ( !bSaveAnimation(CFEString( sFullFilename.char_str() ) + AE_ANIMATION_FILE_EXT) )
			ShowFileErrorMessage(sFullFilename + AE_ANIMATION_FILE_EXT);
		else
		{
			// save skeleton action
			if ( !bSaveSkeletonAction( CFEString( sFullFilename.char_str() ) + FE_SKELACTION_FILE_EXT,globals.m_poSkeleton,m_cbAnimatePivot->GetValue() ) )
				ShowFileErrorMessage(sFullFilename + FE_SKELACTION_FILE_EXT);
			else
			{
				globals.m_sInputFilename = sFullFilename + AE_ANIMATION_FILE_EXT;
				SetTitle( APP_NAME + wxString(" - ") + globals.m_sInputFilename );
			}
		}

		// save also skeleton
		// WARNING: Doesn't work properly!!!!
		// wxString sSkeletonFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + "actor";
		// SaveSkeleton(CFEString(sSkeletonFilename.char_str())+FE_SKELETON_FILE_EXT,globals.m_poSkeleton);
	}
}

void CActorEditorGUI::m_menuItem12OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Load skeleton"), sFilename.GetPath(true), wxT("actor") + FE_SKELETON_FILE_EXT, wxT("*") + FE_SKELETON_FILE_EXT, wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName ( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();

		globals.EditBegin();

		LoadSkeleton(CFEString( sFullFilename.char_str() ) + FE_SKELETON_FILE_EXT,globals.m_poSkeleton);

		globals.EditEnd();

		SetupControls();
	}
}

void CActorEditorGUI::m_menuItem10OnMenuSelection( wxCommandEvent& event )
{
	// -----------------------------------------
	// Prevent saving incomplete skeleton info ...
	// -----------------------------------------
	wxString sBones = "";
	for ( uint i = 0; i < globals.m_poSkeleton->m_oBone.size(); i++ )
		if ( ( !globals.m_poSkeleton->m_oBone[i].bVisible() ) && ( globals.m_poSkeleton->m_oBone[i].bEditable() ) )
			sBones += wxString( globals.m_poSkeleton->m_oBone[i].sGetName().szString() ) + wxString(", ");
	if ( sBones != "" )
	{
		wxString sMessage = wxString("There are non directional bones:\n\n")
		                    + sBones
		                    + wxString(
		    "\n\n"
		    "that are currently hidden.\n\n"
		    "These will not be saved on the Skeleton File, but if they are needed you will experience\n"
		    "bad behaviour on your game.\n\n"
		    "In that case, making them visible, assigning them a sprite, and reexporting the skeleton\n"
		    "will be enough to solve the problem.\n\n"
		    "Are you sure this is what you want to export?"
		    );

		if ( wxMessageBox(sMessage,
		                  "Confirm",
		                  wxYES_NO,this) != wxYES ) return;
	}

	// -----------------------------------------
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgSaveFile(this, wxT("Save skeleton"), sFilename.GetPath(true), "actor.ske", wxT("*") + FE_SKELETON_FILE_EXT, wxSAVE | wxCHANGE_DIR);

	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();

		SaveSkeleton(CFEString( sFullFilename.char_str() ) + FE_SKELETON_FILE_EXT,globals.m_poSkeleton);
	}
}

void CActorEditorGUI::m_menuItem11OnMenuSelection( wxCommandEvent& event )
{
	// save the skeleton file.
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgSaveFile(this, wxT("Save skeleton action"), sFilename.GetPath(true), sFilename.GetName(), wxT("*") + FE_SKELACTION_FILE_EXT, wxSAVE | wxCHANGE_DIR);

	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath(true);

		// Save the action
		if ( !bSaveSkeletonAction( CFEString( sFullFilename.char_str() ) + FE_SKELACTION_FILE_EXT,globals.m_poSkeleton, m_cbAnimatePivot->GetValue() ) )
			ShowFileErrorMessage(sFullFilename + FE_SKELACTION_FILE_EXT);
	}
}

void CActorEditorGUI::m_moRestartWithSkelDefOnMenuSelection( wxCommandEvent& event )
{
	// Restart application with new skeleton definition
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Choose skeletion definition file"),sFilename.GetPath(true), sFilename.GetName(),wxT("*") + AE_SKELETON_DEFINITION_EXT, wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		globals.m_sWorkingDir = sFilename.GetPath(true);
		globals.m_sSkeletonDefinition = sFullFilename + AE_SKELETON_DEFINITION_EXT;

		if ( !IsDebuggerPresent() ) // Windows API function call: requires Windows XP.
		{
			wxString sArguments = "-sd=" + globals.m_sSkeletonDefinition;
			_execl(globals.m_sExecCmdLine.char_str(),globals.m_sExecCmdLine.char_str(),sArguments.char_str(),NULL);
		}
		else
		{
			// ----------------------
			// FOR DEBUGGING PURPOSES
			// ----------------------
			SetupSkeleton(globals.m_sSkeletonDefinition);

			wxFileName sInputFilename(globals.m_sSkeletonDefinition);
			globals.m_sWorkingDir = sInputFilename.GetPath() ;
			if ( !SetCurrentDirectory( wxString(globals.m_sWorkingDir + "\\").char_str() ) )
			{
				int iLastError = GetLastError();
				printf("%d",iLastError);
			}

			// Update instanced nodes
			CFESkelAnimInstMgr::I()->Update(globals.m_poSkeleton->m_hInst,_0r);

			SetupControls();
			SetTitle( APP_NAME );
		}
	}
}

void CActorEditorGUI::menuitem3OnMenuSelection( wxCommandEvent& event )
{
	// quit
	Close();
}

void CActorEditorGUI::m_miUndoOnMenuSelection( wxCommandEvent& event )
{
	m_btUndoOnButtonClick(event);
}

void CActorEditorGUI::m_miRedoOnMenuSelection( wxCommandEvent& event )
{
	m_btRedoOnButtonClick(event);
}

void CActorEditorGUI::ReloadMaterialsClick( wxCommandEvent& event )
{
	CFEMaterialMgr::I()->Reload();
	CFESpriteMgr::I()->Reload();
	globals.SyncSprites(true);
}

void CActorEditorGUI::menuitem4OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgSaveFile(this, wxT("Save as standard pose"), sFilename.GetPath(true), wxT("stdpos") + AE_POSE_FILE_EXT, wxT("*") + AE_POSE_FILE_EXT, wxSAVE | wxCHANGE_DIR);

	if ( dlgSaveFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
		SavePoseFile(CFEString( sFullFilename.char_str() ) + AE_POSE_FILE_EXT,globals.m_poSkeleton,globals.m_fTime);
	}
}

void CActorEditorGUI::menuitem5OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Load standard pose"), sFilename.GetPath(true), wxT("stdpos") + AE_POSE_FILE_EXT, wxT("*") + AE_POSE_FILE_EXT, wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();

		globals.EditBegin();

		LoadPoseFile(CFEString( sFullFilename.char_str() ) + AE_POSE_FILE_EXT,globals.m_poSkeleton,globals.m_fTime);

		globals.EditEnd();
		m_gdTimeline->Refresh(false);
	}
}

void CActorEditorGUI::menuitem7SelectBackgroundColor( wxCommandEvent& event )
{
	// Select background color.
	wxColourData data;
	data.SetChooseFull(true);
	data.SetColour( FEColor2WXColor(globals.m_oColorScheme.m_oFrameBackground) );

	wxColourDialog dialog(this, &data);
	if (dialog.ShowModal() == wxID_OK)
		globals.m_oColorScheme.m_oFrameBackground = WXColor2FEColor( dialog.GetColourData().GetColour() );
}

void CActorEditorGUI::menuitem7OnMenuSelection( wxCommandEvent& event )
{
	CFEString sString = globals.m_sInputFilename;
	wxFileName sFilename( wxString( sString.szString() ) );
	wxFileDialog dlgLoadFile(this, wxT("Load background video"), sFilename.GetPath(true), sFilename.GetName() + AE_FRAMELIST_FILE_EXT, wxT("*") + AE_FRAMELIST_FILE_EXT + wxT(";*.lst;*.jpg;*.tga;*.png"), wxOPEN | wxCHANGE_DIR | wxFD_FILE_MUST_EXIST);

	if ( dlgLoadFile.ShowModal() == wxID_OK )
	{
		sFilename = wxFileName( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename() );
		wxString sFullFilename = sFilename.GetFullPath();
		wxString sCurDir = sFilename.GetPath(true);

		if ( chdir( sCurDir.char_str() ) == 0 )
		{
			if ( bLoadFrameSequence( CFEString( sFullFilename.char_str() ) ) )
			{
				globals.m_sFrameFile = CFEString( sFullFilename.char_str() );
				globals.m_uiCurFrame = 0;

				// WARNING: Don't do that. If we load a single image, we loose all other frames...
				// globals.m_uiNumFrames = globals.m_oFrames.size()>1);
				globals.SyncSprites();

				SetupControls();
			}
			else
			{
			}
		}
	}
}

void CActorEditorGUI::m_menuItem13OnMenuSelection( wxCommandEvent& event )
{
	// Reset animation to the first keyframe with squeleton positions
	globals.EditBegin();

	globals.TrimAnimation(1);

	for ( uint i = 0; i < globals.m_poSkeleton->m_oBone.size(); i++ )
	{
		globals.m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.InsertKeyFrame(CFEVect2::ZERO(),_0r,KFLF_LERP);
		globals.m_poSkeleton->m_oBone[i].m_poBJointAction->m_oPosFunc.Invalidate();

		globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.InsertKeyFrame(_0r,_0r,KFLF_LERP);
		globals.m_poSkeleton->m_oBone[i].m_poPJointAction->m_rAngleFunc.Invalidate();
	}

	globals.m_uiNumFrames = 1;
	globals.m_uiCurFrame = 0;
	globals.m_fTime = _0r;

	globals.EditEnd();
	globals.SyncSprites();
	SetupControls();

	m_gdTimeline->Refresh();
}

void CActorEditorGUI::mo_AboutOnMenuSelection( wxCommandEvent& event )
{
	CEditorAboutBox oAbout(this);
	if ( oAbout.ShowModal() == wxID_OK )
	{
	}
}

/*
   void CActorEditorGUI::m_edNodeScaleXOnSpinCtrl( wxSpinEvent& event )


   {
    // TODO: Implement m_edNodeScaleXOnSpinCtrl
    wxTreeItemId oID = m_poBoneNodes->GetSelection();
    if (! oID.IsOk()) return;

    CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
    if (poBoneNode == NULL) return;

    CFEVect2 oScale(m_edNodeScaleX->GetValue(),poBoneNode->m_poObj->oGetScale().y);
    poBoneNode->m_poObj->SetScale(oScale);
    UpdateNodeInstance(poBoneNode->m_poObj);


   }

   void CActorEditorGUI::m_edNodeScaleYOnSpinCtrl( wxSpinEvent& event )


   {
   // TODO: Implement m_edNodeScaleYOnSpinCtrl
    wxTreeItemId oID = m_poBoneNodes->GetSelection();
    if (! oID.IsOk()) return;

    CBoneSkelNode* poBoneNode = (CBoneSkelNode* )m_poBoneNodes->GetItemData(oID);
    if (poBoneNode == NULL) return;

    CFEVect2 oScale(poBoneNode->m_poObj->oGetScale().x,m_edNodeScaleY->GetValue());
    poBoneNode->m_poObj->SetScale(oScale);
    UpdateNodeInstance(poBoneNode->m_poObj);


   }

   void CActorEditorGUI::m_pnTimeLineHeaderOnPaint( wxPaintEvent& event )


   {
   // TODO: Implement m_pnTimeLineHeaderOnPaint
    wxPaintDC dc(m_pnTimeLineHeader);

    // get first visible col ...
   //   poGrid->SetColSize(0,140);	// label column

   // int r = m_gdTimeline->GetColRight(0);
   //int l = m_gdTimeline->GetColLeft(0);

    int x, y;
    m_gdTimeline->CalcUnscrolledPosition(0, 0, &x, &y);
    int iFirstCol = m_gdTimeline->XToCol(x,true);


    for (int i=iFirstCol;i<iFirstCol+100;i++)
    {
        wxRect re((i)*16,0,16,16);
        DrawTimelineHeader(dc,re,0,i,false);
    }


   }
 */
