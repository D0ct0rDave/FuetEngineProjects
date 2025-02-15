#include "CKFRMActorEditorGUI.h"



CKFRMActorEditorGUI::CKFRMActorEditorGUI( wxWindow* parent )
:
CKFRMActorEditorGUIBase( parent )
{
	// Create the Renderers and initialize them.
	globals = new CAppGlobals;
	globals->m_poScreen->Init((void*)pn_Display->GetHandle());

	globals->m_poScreen->SetScreenVWidth (pn_Display->GetSize().x);
	globals->m_poScreen->SetScreenVHeight(pn_Display->GetSize().y);

	// Create input
	globals->m_oInput.Init((void*)pn_Display->GetHandle());

	// Initialize managers
	CFESkelAnimInstMgr::Init(64);
	CFESpriteInstMgr::Init(1024);

	// Initialize the game.
	CGameLoop::Init( globals->m_poScreen, &globals->m_oInput);
    // pn_Display->HookKeyDown(DisplayPanelKeyDown);
}

void CKFRMActorEditorGUI::CKFRMActorEditorGUIBaseOnClose( wxCloseEvent& event )
{
// TODO: Implement CKFRMActorEditorGUIBaseOnClose
    Destroy();
}

void CKFRMActorEditorGUI::CKFRMActorEditorGUIBaseOnIdle( wxIdleEvent& event )
{
// TODO: Implement CKFRMActorEditorGUIBaseOnIdle
{
	if (globals->m_poScreen == NULL) return;

	FEReal rDeltaT = globals->m_poScreen->rGetDeltaT();
	globals->m_oInput.Update(rDeltaT);
	
	CGameLoop::Loop();
}
    event.RequestMore();

}

void CKFRMActorEditorGUI::sb_KeyFrameOnScroll( wxScrollEvent& event )
{
// TODO: Implement sb_KeyFrameOnScroll
}

void CKFRMActorEditorGUI::cb_ShowBackgroundOnCheckBox( wxCommandEvent& event )
{
// TODO: Implement cb_ShowBackgroundOnCheckBox
    goGameGlobals.m_bShowBackground = cb_ShowBackground->GetValue();
}

void CKFRMActorEditorGUI::cb_ShowLabelsOnCheckBox( wxCommandEvent& event )
{
// TODO: Implement cb_ShowLabelsOnCheckBox
}

void CKFRMActorEditorGUI::ReloadMaterialsClick( wxCommandEvent& event )
{
// TODO: Implement ReloadMaterialsClick
    CFEMaterialMgr::Reload();
    CFESpriteMgr::Reload();
}

void CKFRMActorEditorGUI::OnPlayAnimationClick( wxCommandEvent& event )
{
// TODO: Implement OnPlayAnimationClick
}

void CKFRMActorEditorGUI::sld_AnimTimeOnCommandScroll( wxCommandEvent& event )
{
// TODO: Implement sld_AnimTimeOnCommandScroll
}

void CKFRMActorEditorGUI::sld_HScaleOnCommandScroll( wxCommandEvent& event )
{
// TODO: Implement sld_HScaleOnCommandScroll
}

void CKFRMActorEditorGUI::sld_VScaleOnCommandScroll( wxCommandEvent& event )
{
// TODO: Implement sld_VScaleOnCommandScroll
}

void CKFRMActorEditorGUI::bt_LoadSpriteOnButtonClick( wxCommandEvent& event )
{
// TODO: Implement bt_LoadSpriteOnButtonClick
}

void CKFRMActorEditorGUI::cbSpriteActionOnCombobox( wxCommandEvent& event )
{
// TODO: Implement cbSpriteActionOnCombobox
}

void CKFRMActorEditorGUI::sld_OfsOnCommandScroll( wxCommandEvent& event )
{
// TODO: Implement sld_OfsOnCommandScroll
}

void CKFRMActorEditorGUI::sld_YOfsOnCommandScroll( wxCommandEvent& event )
{
// TODO: Implement sld_YOfsOnCommandScroll
}

void CKFRMActorEditorGUI::m_menuItem12OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem12OnMenuSelection
}

void CKFRMActorEditorGUI::m_menuItem10OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem10OnMenuSelection
}

void CKFRMActorEditorGUI::menuitem3OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement menuitem3OnMenuSelection
    if (wxMessageBox("Are you sure you want to exit?\n"
				    "All data not saved will be lost",
				    "Confirm",
				    wxYES_NO,this) != wxYES) return;

    Close();
}
/*
#include "CActorEditorGUI.h"

CActorEditorGUI::CActorEditorGUI( wxWindow* parent )
:
CActorEditorGUIBase( parent )
{
    //
    // Includes are found under CActorGUI file properties because wxFormBuilder
    // deletes them after code generation.
    globals.GUI = this;

    // Hook mouse move function...
    pn_Display->HookMouseMove(DisplayPanelMouseMove);
    pn_Display->HookMouseDown(DisplayPanelMouseDown);
    pn_Display->HookMouseUp(DisplayPanelMouseUp);
    // pn_Display->HookKeyDown(DisplayPanelKeyDown);

    // Initialize the renderer.
    globals.m_poRenderer = new CFERenderer();
    globals.m_poRenderer->Init((void*)pn_Display->GetHandle());
    globals.m_poRenderer->SetScreenVWidth(pn_Display->GetSize().x);
    globals.m_poRenderer->SetScreenVHeight(pn_Display->GetSize().y);

    globals.m_poPrevRenderer = new CFERenderer();
    globals.m_poPrevRenderer->Init((void*)pn_PreviewDisplay->GetHandle());
    globals.m_poPrevRenderer->SetScreenVWidth(pn_Display->GetSize().x); // same as main
    globals.m_poPrevRenderer->SetScreenVHeight(pn_Display->GetSize().y); // same as main

    // Load the default font.
    globals.m_poDefaultFont = CFEFontMgr::poLoad("small");

    // Load a default sprite.
    CFESpriteInstMgr::Init(250);
    CFESkelAnimInstMgr::Init(10);

    SetupSkeleton();

    // ------
    // Time line grid
	m_gdTimeline->SetFont(wxFont(8, wxSWISS, wxNORMAL, wxNORMAL, false, wxT("Andale Mono")));
	m_gdTimeline->SetDefaultColSize(16);
	m_gdTimeline->SetDefaultRowSize(16);
    m_gdTimeline->SetColLabelSize(0);

    m_gdTimeline->SetCellHighlightColour(globals.m_oColorScheme.m_oGridLabelBase);
    m_gdTimeline->SetLabelBackgroundColour(globals.m_oColorScheme.m_oGridLabelBase);
    m_gdTimeline->SetGridLineColour(globals.m_oColorScheme.m_oTLGridLine);
    m_gdTimeline->SetDefaultRenderer(new CActionTimelineGridRenderer);
    m_gdTimeline->SetColSize(0,50);	// label column
    m_gdTimeline->DisableDragColSize();
	m_gdTimeline->DisableDragRowSize();
    m_gdTimeline->EnableGridLines(false);

    m_gdTimeline->SetRowLabelSize(60);
    m_gdTimeline->SetRowLabelValue(0,"");
    for (uint i=1;i<m_gdTimeline->GetRows();i++)
        m_gdTimeline->SetRowLabelValue(i,BONE_NAMES[i-1]);

	for (uint i=0;i<m_gdTimeline->GetCols();i++)
	{
        m_gdTimeline->SetColMinimalWidth(i,10);
		m_gdTimeline->SetReadOnly(0,i,false);
	}

	wxFont oFont = m_gdTimeline->GetLabelFont();
    oFont.SetWeight(wxFONTWEIGHT_LIGHT);
	m_gdTimeline->SetLabelFont(oFont);
    m_gdTimeline->SetLabelTextColour(globals.m_oColorScheme.m_oTLLabels);
    m_gdTimeline->DisableCellEditControl();
    m_gdTimeline->EnableEditing(false);
    m_gdTimeline->Enable(false);

}

void CActorEditorGUI::CActorEditorGUIBaseOnClose( wxCloseEvent& event )
{
    Destroy();
}

void CActorEditorGUI::CActorEditorGUIBaseOnIdle( wxIdleEvent& event )
{
    RenderPreview(globals.m_poPrevRenderer);
    RenderMain(globals.m_poRenderer);

    if (globals.m_bPlaying)
        bt_Play->SetLabel("Stop");
    else
        bt_Play->SetLabel("Play");

    event.RequestMore();
}

void CActorEditorGUI::sb_KeyFrameOnScroll( wxScrollEvent& event )
{
    globals.m_uiCurFrame = sb_KeyFrame->GetThumbPosition();
    globals.m_fTime = ((float)globals.m_uiCurFrame / ((float)globals.m_oFrames.size()-1));

    // Show cursor over timeline.
    m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::cb_ShowLabelsOnCheckBox( wxCommandEvent& event )
{
    globals.m_bShowLabels = cb_ShowLabels->GetValue();
}

void CActorEditorGUI::cb_ShowSpritesOnCheckBox( wxCommandEvent& event )
{
    globals.m_bShowSprites = cb_ShowSprites->GetValue();
}

void CActorEditorGUI::m_gdTimelineOnGridCellLeftClick( wxGridEvent& event )
{
    if (globals.m_oFrames.size() == 0 ) return;
    if (event.GetCol() == 0) return;

	// Move the cursor to the picked position.
	globals.m_uiCurFrame = event.GetCol()-1;
	sb_KeyFrame->SetThumbPosition(globals.m_uiCurFrame);
    globals.m_fTime = ((float)globals.m_uiCurFrame / ((float)globals.m_oFrames.size()-1));

    if (globals.m_iSelEditBone = event.GetRow()>0)
    {
        globals.m_iSelEditBone = event.GetRow()-1;
        SetupBoneControls();
    }

    // Show cursor over timeline.
    m_gdTimeline->Refresh(false);
}

void CActorEditorGUI::m_gdTimelineOnGridCellRightClick( wxGridEvent& event )
{
}

void CActorEditorGUI::sld_HScaleOnCommandScroll( wxCommandEvent& event )
{
    globals.m_fHScale = (FEReal)sld_HScale->GetValue()/100.0;
    globals.m_poSkeleton->RescaleSkeleton(640.0f*globals.m_fHScale,480.0f*globals.m_fVScale);
}

void CActorEditorGUI::sld_VScaleOnCommandScroll( wxCommandEvent& event )
{
    globals.m_fVScale = (FEReal)sld_VScale->GetValue()/100.0;
    globals.m_poSkeleton->RescaleSkeleton(640.0f*globals.m_fHScale,480.0f*globals.m_fVScale);
}

void CActorEditorGUI::ShowBonesClick( wxCommandEvent& event )
{
    globals.m_bShowPrevBones = cb_ShowPrevBones->GetValue();
}

void CActorEditorGUI::cb_ShowGridOnCheckBox( wxCommandEvent& event )
{
    globals.m_bDisplayGrid = cb_ShowGrid->GetValue();
}

void CActorEditorGUI::ReloadMaterialsClick( wxCommandEvent& event )
{
    CFEMaterialMgr::Reload();
    CFESpriteMgr::Reload();
}

void CActorEditorGUI::OnPlayAnimationClick( wxCommandEvent& event )
{
    if (globals.m_bPlaying == false)
    {
        globals.m_bPlaying = true;
        bt_Play->SetLabel("Stop");
        globals.m_fCurAnimTime = 0.0f;
    }
    else
    {
        globals.m_bPlaying = false;
        bt_Play->SetLabel("Play");
        globals.m_fCurAnimTime = 0.0f;
    }
}

void CActorEditorGUI::OnLoopAnimationClick( wxCommandEvent& event )
{
    globals.m_bLoop = cb_Loop->GetValue();
    for (uint i=0;i<MAX_BONES;i++)
    {
        if (globals.m_bLoop)
            globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.SetWrapMode(KFBFWM_LOOP);
        else
            globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.SetWrapMode(KFBFWM_FINALVALUE);
    }

    m_gdTimeline->Refresh();
}

void CActorEditorGUI::sld_AnimTimeOnCommandScroll( wxCommandEvent& event )
{
	globals.m_fTotalAnimTime = (float)sld_AnimTime->GetValue() / 100.0f;
}

void CActorEditorGUI::bt_LoadSpriteOnButtonClick( wxCommandEvent& event )
{
    if (globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_eBoneType == BT_SPRITE)
    {
        wxFileDialog dlgLoadFile(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.spr;*.tga;*.png"), wxOPEN | wxCHANGE_DIR);
        if (dlgLoadFile.ShowModal() == wxID_OK)
        {
            wxFileName sFilename( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
            wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
            sFullFilename.Replace("\\","/");
	        wxString sCurDir		= sFilename.GetPath(true);

            CFEString sSpriteFile = CFEString( sFullFilename.char_str() );
            FEHandler hInst = CFESpriteInstMgr::hGetInstance(sSpriteFile);
            if (hInst != NULL)
            {
                globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->SetSprite(hInst);

                // Setup the instanced sprite
                CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::poGetInstancedNode(globals.m_poSkeleton->m_hInst);
                CFESkelAnimNode* poNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->sGetName());
                if (poNode != NULL)
                    ((CFESkelAnimSprite*)poNode)->SetSprite(hInst);

                // globals.m_poPrevSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->SetSprite(hInst);
                CFESpriteInstMgr::Enable(hInst);

                globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_sExternalFile = sSpriteFile;

                edIconSprite->SetValue(sFullFilename);
                FillSpriteActionList(sFullFilename);

                cbSpriteAction->Select(0);
                globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->SetAction(0);
                // globals.m_poPrevSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->SetAction(0);
            }
        }
    }
    else if(globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_eBoneType != BT_CONTROL)
    {
        wxFileDialog dlgLoadFile(this, wxT("Choose a file"), wxT(""), wxT(""), wxT("*.tga;*.png"), wxOPEN | wxCHANGE_DIR);
        if (dlgLoadFile.ShowModal() == wxID_OK)
        {
            wxFileName sFilename( dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
            wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
            sFullFilename.Replace("\\","/");
	        wxString sCurDir		= sFilename.GetPath(true);

            CFEString sSpriteFile = CFEString( sFullFilename.char_str() );
            FEHandler hInst = CFEMaterialMgr::hLoad( sSpriteFile );

            if (hInst != NULL)
            {
                if (globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poMesh != NULL)
                {
                    globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poMesh->SetMaterial( hInst );
                    // globals.m_poPrevSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poMesh->SetMaterial( hInst );


                    // Setup the instanced sprite
                    CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::poGetInstancedNode(globals.m_poSkeleton->m_hInst);
                    CFESkelAnimNode* poNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poMesh->sGetName());
                    if (poNode != NULL)
                        ((CFESkelAnimMesh*)poNode)->SetMaterial( hInst );
                }

                edIconSprite->SetValue(sFullFilename);
            }
        }
    }
}

void CActorEditorGUI::cbSpriteActionOnCombobox( wxCommandEvent& event )
{
    // TODO: Implement cbSpriteActionOnCombobox
}

void CActorEditorGUI::sld_OfsOnCommandScroll( wxCommandEvent& event )
{
    if (globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr == NULL) return;

    CFEVect2 oOfs;
    oOfs.x = (FEReal)sld_XOfs->GetValue();
    oOfs.y = globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs.y;

    // globals.m_poCurrentLayer->SetAngle( ));
    globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs = oOfs;

    CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::poGetInstancedNode(globals.m_poSkeleton->m_hInst);
    CFESkelAnimNode* poNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->sGetName());

    if (poNode != NULL)
        ((CFESkelAnimSprite*)poNode)->SetPos( oOfs );
}

void CActorEditorGUI::sld_YOfsOnCommandScroll( wxCommandEvent& event )
{
    if (globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr == NULL) return;

    CFEVect2 oOfs;
    oOfs.y = (FEReal)sld_YOfs->GetValue();
    oOfs.x = globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs.x;

    // globals.m_poCurrentLayer->SetAngle( ));
    globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_oOfs = oOfs;

    CFESkelAnimNode* poRoot = CFESkelAnimInstMgr::poGetInstancedNode(globals.m_poSkeleton->m_hInst);
    CFESkelAnimNode* poNode = CFESkelAnimNodeLocator::poLocateNode(poRoot,globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poSpr->sGetName());

    if (poNode != NULL)
        ((CFESkelAnimSprite*)poNode)->SetPos( oOfs );
}

void CActorEditorGUI::m_menuItem17OnMenuSelection( wxCommandEvent& event )
{
    float fTime = ((float)globals.m_uiCurFrame / ((float)globals.m_oFrames.size()-1));
    globals.m_poSkeleton->MoveBoneTo(globals.m_iSelEditBone ,globals.m_fTime,globals.m_poSkeleton->m_oBone[ globals.m_iSelEditBone ].m_poNodeAction->m_oPosFunc.oGetValue(0.0));
}

void CActorEditorGUI::m_menuItem16OnMenuSelection( wxCommandEvent& event )
{
}

void CActorEditorGUI::m_menuItem18OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem18OnMenuSelection (linear interpolation)
}

void CActorEditorGUI::m_menuItem19OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem19OnMenuSelection (sinusoidal interpolation)
}

void CActorEditorGUI::m_menuItem20OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem20OnMenuSelection (exponential interpolation)
}

void CActorEditorGUI::m_menuItem21OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem21OnMenuSelection (random interpolation)
}

void CActorEditorGUI::m_menuItem22OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem22OnMenuSelection (sinsin interpolation)
}

void CActorEditorGUI::m_menuItem23OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem23OnMenuSelection (explog interpolation)
}

void CActorEditorGUI::m_menuItem121OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem121OnMenuSelection (wrap to final value)
}

void CActorEditorGUI::m_menuItem131OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem131OnMenuSelection (wrap to initial value)
}

void CActorEditorGUI::m_menuItem14OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem14OnMenuSelection (wrap loop)
}

void CActorEditorGUI::m_menuItem15OnMenuSelection( wxCommandEvent& event )
{
// TODO: Implement m_menuItem15OnMenuSelection (wrap ping pong)
}

void CActorEditorGUI::menuitem0OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgLoadFile(this, wxT("Load animation"), wxT(""), wxT(""), wxT("*")+AE_ANIMATION_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
    {
        wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
        // globals.m_sFilename     = sFullFilename;
        globals.m_sWorkingDir   = sFilename.GetPath(true);

        // globals.m_sCurFilename	= sFilename.GetName();
        if (bLoadAnimation(CFEString(sFullFilename.char_str()) + AE_ANIMATION_FILE_EXT))
        {
            // Invertir coordenadas del pivot.
            // InvertPivotCoords();
            SetupControls();
        }
    }
}

void CActorEditorGUI::menuitem1OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgSaveFile(this, wxT("Save animation"), wxT(""), wxT(""), wxT("*")+AE_ANIMATION_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    globals.m_sWorkingDir   = sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();

        // InvertPivotCoords();
	        SaveAnimation(CFEString(sFullFilename.char_str())+AE_ANIMATION_FILE_EXT);
	    // InvertPivotCoords();
    }
}

void CActorEditorGUI::m_menuItem12OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgLoadFile(this, wxT("Load skeleton"), wxT(""), wxT(""), wxT("*")+FE_SKELETON_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    // globals.m_sCurDir		= sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();
	    wxString sCurDir		= sFilename.GetPath(true);

	    LoadSkeleton(CFEString(sFullFilename.char_str())+FE_SKELETON_FILE_EXT,globals.m_poSkeleton);
	    SetupControls();
    }
}

void CActorEditorGUI::m_menuItem10OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgSaveFile(this, wxT("Save skeleton"), wxT(""), wxT(""), wxT("*")+FE_SKELETON_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    globals.m_sWorkingDir   = sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();

	    SaveSkeleton(CFEString(sFullFilename.char_str())+FE_SKELETON_FILE_EXT,globals.m_poSkeleton);
    }
}

void CActorEditorGUI::m_menuItem11OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgSaveFile(this, wxT("Save skeleton action"), wxT(""), wxT(""), wxT("*")+FE_SKELACTION_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    globals.m_sWorkingDir   = sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();

        // Create a special skeleton to save the action.
        // CSkeleton* poSkel = poCreateSkeletonForFE(globals.m_poSkeleton,globals.m_fHScale,globals.m_fVScale,globals.m_fTotalAnimTime);

        // Save the action
	    SaveSkeletonAction(CFEString(sFullFilename.char_str())+FE_SKELACTION_FILE_EXT,globals.m_poSkeleton);

	    // free resources...
	    // delete poSkel;
    }
}

void CActorEditorGUI::menuitem3OnMenuSelection( wxCommandEvent& event )
{
    if (wxMessageBox("Are you sure you want to exit?\n"
				    "All data not saved will be lost",
				    "Confirm",
				    wxYES_NO,this) != wxYES) return;

    Close();
}

void CActorEditorGUI::menuitem4OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgSaveFile(this, wxT("Save as standard pose"), wxT(""), wxT(""), wxT("*")+AE_POSE_FILE_EXT, wxSAVE | wxCHANGE_DIR);

    if (dlgSaveFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgSaveFile.GetDirectory(),dlgSaveFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    // globals.m_sCurDir		= sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();

	    SavePoseFile(CFEString(sFullFilename.char_str())+AE_POSE_FILE_EXT,globals.m_poSkeleton,globals.m_fTime);
    }
}

void CActorEditorGUI::menuitem5OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgLoadFile(this, wxT("Load standard pose"), wxT(""), wxT(""), wxT("*")+AE_POSE_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName();
	    // globals.m_sFilename     = sFullFilename;
	    // globals.m_sCurDir		= sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();

	    LoadPoseFile(CFEString(sFullFilename.char_str())+AE_POSE_FILE_EXT,globals.m_poSkeleton,globals.m_fTime);
    }
}

void CActorEditorGUI::menuitem7OnMenuSelection( wxCommandEvent& event )
{
    wxFileDialog dlgLoadFile(this, wxT("Load frame sequence list"), wxT(""), wxT(""), wxT("*")+AE_FRAMELIST_FILE_EXT, wxOPEN | wxCHANGE_DIR);

    if (dlgLoadFile.ShowModal() == wxID_OK)
    {
	    wxFileName sFilename(dlgLoadFile.GetDirectory(),dlgLoadFile.GetFilename());
        wxString sFullFilename  = sFilename.GetPath() + wxString( sFilename.GetPathSeparator() ) + sFilename.GetName() + AE_FRAMELIST_FILE_EXT;
	    // globals.m_sFilename     = sFullFilename;
	    // globals.m_sCurDir		= sFilename.GetPath(true);
	    // globals.m_sCurFilename	= sFilename.GetName();
	    wxString sCurDir		= sFilename.GetPath(true);

	    if (chdir(sCurDir.char_str()) == 0)
	    {
		    if (bLoadFrameSequence( CFEString(sFullFilename.char_str())))
		    {
		        globals.m_sFrameFile = CFEString( sRelativizePath(sFullFilename,globals.m_sWorkingDir).char_str() );
	            globals.m_uiCurFrame = 0;
	            SetupControls();
            }
        }
    }
}

void CActorEditorGUI::menuitem8OnMenuSelection( wxCommandEvent& event )
{
    if (globals.m_oFrames.size() == 0) return;

    for (uint i=0;i<MAX_BONES;i++)
    {
        // Add key frames
        uint k;
        CFEKFBFunc<CFEVect2> oNewPos;
        for (k=0;k<globals.m_poSkeleton->m_oBone [i].m_poNodeAction->m_oPosFunc.uiGetNumKeyFrames();k++)
        {
            CFETypedKeyFrame<CFEVect2>* poKF = globals.m_poSkeleton->m_oBone [i].m_poNodeAction->m_oPosFunc.poGetKeyFrameData(k);
            float fOldTime = poKF->rGetKeyTime();
            CFEVect2 oPos  = poKF->m_oKey;
            EFEKFLerpFunc eLerpFunc = poKF->eGetLerpFunc();

            int iFrame  = fOldTime * ((float)globals.m_oFrames.size()-1);
            float fTime = (float)iFrame / ((float)globals.m_oFrames.size()-1);

            oNewPos.InsertKeyFrame(oPos,fTime,eLerpFunc);
        }

        // Rebuild array.
        globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.Reset();
        globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_rAngleFunc.Reset();

        for (k=0;k<oNewPos.uiGetNumKeyFrames();k++)
        {
            CFEKeyFrame* poKF = oNewPos.poGetKeyFrame(k);
            float fTime = poKF->rGetKeyTime();
            CFEVect2 oPos = oNewPos.oGetValue(fTime);
            EFEKFLerpFunc eLerpFunc = poKF->eGetLerpFunc();

            globals.m_poSkeleton->MoveBoneTo(i,fTime,oPos);
        }
    }
}

void CActorEditorGUI::m_menuItem25OnMenuSelection( wxCommandEvent& event )
{
    /// Traverse all nodeactions and divide the coords by (640,480)
    for (uint b=0;b<MAX_BONES;b++)
        for (uint k=0;k<globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_oPosFunc.uiGetNumKeyFrames();k++)
        {
            CFETypedKeyFrame<CFEVect2>* poTKF = globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_oPosFunc.poGetKeyFrameData(k);
            poTKF->m_oKey *= CFEVect2(1.0f/640.0f,1.0f/480.0f);
        }
}

void CActorEditorGUI::m_menuItem251OnMenuSelection( wxCommandEvent& event )
{
    /// Delete keyframes at this moment in time.
    if (globals.m_fTime== 0.0) return;

    for (uint b=0;b<MAX_BONES;b++)
    {
        globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_oPosFunc.DeleteKeyFrameAtTime(globals.m_fTime);
        globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_rAngleFunc.DeleteKeyFrameAtTime(globals.m_fTime);
    }
}

void CActorEditorGUI::m_menuItem26OnMenuSelection( wxCommandEvent& event )
{
    // Recompute orientations
    for (uint b=0;b<MAX_BONES;b++)
        globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_rAngleFunc.Reset();

    for (uint b=0;b<MAX_BONES;b++)
    {
        for (uint k=0;k<globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_oPosFunc.uiGetNumKeyFrames();k++)
        {
            CFETypedKeyFrame<CFEVect2>* poKF = globals.m_poSkeleton->m_oBone[b].m_poNodeAction->m_oPosFunc.poGetKeyFrameData(k);
            globals.m_poSkeleton->MoveBoneTo(b,poKF->rGetKeyTime(),poKF->m_oKey);
        }
    }
}

void CActorEditorGUI::m_menuItem13OnMenuSelection( wxCommandEvent& event )
{
    for (uint i=0;i<MAX_BONES;i++)
    {
        while (globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.uiGetNumKeyFrames()>1)
            globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oPosFunc.DeleteKeyFrame(1);

        while (globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oScaleFunc.uiGetNumKeyFrames()>1)
            globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_oScaleFunc.DeleteKeyFrame(1);

        while (globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_rAngleFunc.uiGetNumKeyFrames()>1)
            globals.m_poSkeleton->m_oBone[i].m_poNodeAction->m_rAngleFunc.DeleteKeyFrame(1);
    }
}
*/