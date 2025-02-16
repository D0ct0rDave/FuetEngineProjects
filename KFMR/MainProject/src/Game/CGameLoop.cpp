//-----------------------------------------------------------------------------
#include "CGameLoop.h"
#include "CGame.h"

#include "CGameGlobals.h"

#include "Game/Helpers/CHUDMgr.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CCameraMgr.h"

#include "Game/CinematicActions/CKFRMActionLoader.h"

#include <stdlib.h>
#include <stdio.h>

#include "Extensions/CMusicTrack.h"
#include "Extensions/CMusicTracker.h"
#include "Extensions/CMusicTrackLoader.h"
//-----------------------------------------------------------------------------
static CMusicTrack*		m_poMT;
//-----------------------------------------------------------------------------
static CFEMapLayer*		m_poForeLayer;
static CFEFont*			m_poFont;

static FEHandler		m_hSprID;
static FEReal			m_rTime;

static bool				m_bGamePaused = false;

CFELogger				m_oSysLog(10000);

//-----------------------------------------------------------------------------
void CGameLoop::Init(CFERenderer* _poRnd0,CFERenderer* _poRnd1,CFEInput* _poInput)
{
    goGameGlobals.m_oChrono.Start();

    goGameGlobals.m_oLogger.Print("------> Loading debug font\n");
	m_poFont = CFEFontMgr::poLoad("data/fonts/arial");
    
    goGameGlobals.m_poRenderer[0]   = _poRnd0;
    goGameGlobals.m_poRenderer[1]   = _poRnd1;
	goGameGlobals.m_poInput         = _poInput;

 	// Create a "safe" area.
	goGameGlobals.m_rScreenMargin = 0.5f * goGameGlobals.m_poRenderer[0]->uiGetScreenVWidth() / CAMERA_ZOOM;

	goGameGlobals.SlowMotionReset();

	// Sets the action loader for this game
	CFEScriptLoader::SetActionLoader(new CKFRMActionLoader);
    
	// Game Init
	CGame::Init(goGameGlobals.m_poRenderer[0],_poInput);

	// Dirty trick to avoid having the first DeltaT > 0
	for (uint i=0;i<2;i++)
	{
		goGameGlobals.m_poRenderer[0]->BeginScene();
		goGameGlobals.m_poRenderer[0]->EndScene();

		if (goGameGlobals.m_poRenderer[1])
		{
		    goGameGlobals.m_poRenderer[1]->BeginScene();
		    goGameGlobals.m_poRenderer[1]->EndScene();
        }
	}

    /*
	m_poMT = CMusicTrackLoader::poLoad("data/sound/bgm/final_boss/final_boss");
    CMusicTracker::Init(m_poMT);
    CMusicTracker::Play();
	*/

	goGameGlobals.m_oChrono.Stop();
	goGameGlobals.m_oLogger.Print("------> Total initialization time: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());
}
//-----------------------------------------------------------------------------
void CGameLoop::Finish()
{
    CGame::Finish();
}
//-----------------------------------------------------------------------------
void CGameLoop::Loop()
{
	Update( goGameGlobals.m_poRenderer[0]->rGetDeltaT() );
	// Update( 0.01 );
	Render();
}
//-----------------------------------------------------------------------------
void CGameLoop::Update(FEReal _rDeltaT)
{
	// SlowMotion
	if(goGameGlobals.bSlowMotionNeedsUpdate())
	{
		goGameGlobals.SlowMotionUpdate();
		_rDeltaT *= goGameGlobals.m_fSlowMotionFactor;
	}

    if (goGameGlobals.m_poInput->bPressed(IPB_A))
        _rDeltaT *= 0.025f;

    if (goGameGlobals.m_poInput->bDown(IPB_B))
    {
        CCameraMgr::Perturb(10.0,5.0);
    }

	if (goGameGlobals.m_poInput->bDown(IB_MIG))
		m_bGamePaused = !m_bGamePaused;
	
	if (m_bGamePaused) return;

	CFESpriteInstMgr::Update(_rDeltaT);
	CFESkelAnimInstMgr::Update(_rDeltaT);
	CFEParticleSysInstMgr::Update(_rDeltaT);
	m_rTime += _rDeltaT;

	// Update the game.
	CGame::Update(_rDeltaT);
	
	// Update Screen FX
	CFEScreenFX::Update(_rDeltaT);
	
	/// Update camera manager.
	CCameraMgr::Update(_rDeltaT);
	
    /// Update music tracker.
	CMusicTracker::Update(_rDeltaT);	
}
//-----------------------------------------------------------------------------
void RenderMap(CFERenderer* _poRnd,CFEMap*_poMap,bool _bRenderFore)
{
	for (uint j=0;j<goGameGlobals.m_poMap->m_poLayers.size();j++)
	{
	    CFEMapLayer* poLayer = goGameGlobals.m_poMap->m_poLayers[j];
	    if (poLayer == NULL) return;

		if (poLayer->m_sName.SubString(0,4) |= "Fore")
			poLayer->m_bVisible = _bRenderFore;
	    else
			poLayer->m_bVisible = ! _bRenderFore;
    }

	CFEMapRenderer::Render(_poMap, _poRnd );
}
//-----------------------------------------------------------------------------
void CGameLoop::Render()
{
    CFERenderer* poRnd = NULL;

    //-----------------------------------------------------------------------------
    // Render main screen
    //-----------------------------------------------------------------------------
    poRnd = goGameGlobals.m_poRenderer[0];
	poRnd->BeginScene();

		poRnd->SetMaterial(NULL);

		poRnd->ResetCamera();
        poRnd->SetDepth(1.0f);
		poRnd->RenderQuad(0,0,poRnd->uiGetScreenVWidth(),poRnd->uiGetScreenVHeight(),0,0,1,1,CFEColor(0.6f,0.6f,0.6f,1));

		// goGameGlobals.m_poCamera->oGetPos().x = -200;
		poRnd->TranslateCamera(CCameraMgr::oGetPos().x,CCameraMgr::oGetPos().y + CAMERA_YOFS);
		poRnd->ZoomCamera(CCameraMgr::rGetZoom());

		// 
		RenderMap(poRnd, goGameGlobals.m_poMap,false);

			CFESkelAnimInstMgr::Render( poRnd );
			CFESpriteInstMgr::Render( poRnd );
		    CFEParticleSysInstMgr::Render( poRnd );
        // 
		RenderMap(poRnd, goGameGlobals.m_poMap,true);

        // Render HUD
        poRnd->SetDepth(_0r);
		CHUDMgr::Render(poRnd,0);

        // Render DEBUG info
        #ifndef MASTER_RELEASE_VERSION
        poRnd->SetFont(m_poFont);
		poRnd->ResetCamera();
		// Render Game Info.
		CGame::Render();

        // Retrieve stats
        uint uiQuads = 0;
        poRnd->bGetProperty("RENDERED_QUADS",(FEPointer)&uiQuads); 
		poRnd->SetFont(m_poFont);

    	char szStr[1024];
		poRnd->ResetCamera();
		poRnd->ZoomCamera(0.75f);

    	FEReal rTime  = poRnd->rGetDeltaT();
		if (rTime > 0.0f)
		{
			sprintf(szStr,"FPS: %.1f", (float)(1.0f / rTime));
			poRnd->RenderText(szStr,10,200,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);

			sprintf(szStr,"Milis: %.1f", (float)(1000 * rTime));
			poRnd->RenderText(szStr,10,216,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);
		}

		sprintf(szStr,"Quads: %d",uiQuads);
		poRnd->RenderText(szStr,10,232,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);
        #endif

        //
        poRnd->SetDepth(_0r);
        poRnd->ResetCamera();        
		CFEScreenFX::Render(poRnd);

		poRnd->ResetCamera();
		poRnd->ZoomCamera(0.75f);
        
		if (goGameGlobals.m_poInput->bDown(IB_C))
			poRnd->RenderText(m_oSysLog.szGetLog(),0,0,CFEColor(1,1,0,1),THAM_LEFT,TVAM_CENTER);
		
		if (m_bGamePaused)
			poRnd->RenderText("PAUSE",0.5f * poRnd->uiGetScreenVWidth(),0.5f * poRnd->uiGetScreenVHeight(),CFEColor(1,1,1,1),THAM_CENTER,TVAM_CENTER);

		// ÑAPA: Draw black quad if at the end of the game
		poRnd->ResetCamera();
		if (	
			    ( (CGame::uiGetState() == GS_GAME_OVER) && (!CHUDMgr::bPlayingAction(HA_SHOW_GAMEOVER)) )
			 || ( CGame::uiGetState() == GS_FADE_IN)
			 || ( CGame::uiGetState() == GS_FINISH)
			 || ( CGame::uiGetState() == GS_STAGE_FINISH)
			 || ( CGame::uiGetState() == GS_NEXT_STAGE)
			)
			CFEScreenUtils::FSRender(poRnd,NULL,CFEColor(0,0,0,1));

	poRnd->EndScene();

    //-----------------------------------------------------------------------------
    // Render sub screen
    //-----------------------------------------------------------------------------
    if (goGameGlobals.m_poRenderer[1] != NULL)
    {
        poRnd = goGameGlobals.m_poRenderer[1];
	    poRnd->BeginScene();

            poRnd->SetDepth(_0r);
		    CHUDMgr::Render(poRnd,1);

        poRnd->EndScene();
    }
}
//-----------------------------------------------------------------------------
