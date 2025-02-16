//-----------------------------------------------------------------------------
#include "CGameLoop.h"

#include "FuetEngineExt.h"
#include "CGameGlobals.h"
#include "CActorTest.h"
#include "CCamera.h"

#include "Game/CAnimHelper.h"

#include <stdlib.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
CFERenderer* CGameLoop::m_poRnd		= NULL;
CFEInput*	 CGameLoop::m_poInput	= NULL;

static CFEMapLayer*		m_poForeLayer;
static CFEFont*			m_poFont;

static FEHandler		m_hBGM;

static FEHandler		m_hSprID;
static FEReal			m_rTime;
static FEHandler		m_hBackground;

const CFEVect2 PLAYER_POS(700,GROUND_HEIGHT);

static bool				m_bGamePaused = false;
CFELogger*				m_poSysLog;
//-----------------------------------------------------------------------------
void CGameLoop::Init(CFERenderer* _poRnd,CFEInput* _poInput)
{
	m_poSysLog = new CFELogger(32*1024);
	
	m_poRnd		= _poRnd;
	m_poInput	= _poInput;

	m_poFont = CFEFontMgr::poLoad("data/fonts/arial");

	goGameGlobals.m_poInput = _poInput;

 	// Create a "safe" area.
	goGameGlobals.m_rScreenMargin = 0.5f * m_poRnd->uiGetScreenVWidth() / CAMERA_ZOOM;
	m_hBackground = CFEMaterialMgr::hLoad("data/background");

	// Load the game level

	// proto level
	#ifndef NOACTOR
    /// Set of actions to load in this level...
    ECharacterAction eActions[] =
    {
        ACTION_ACTOR_WALK,

	    ACTION_ACTOR_IDLE,
	    ACTION_ACTOR_KICK,
	    ACTION_ACTOR_PUNCH,

	    ACTION_ACTOR_CROUCH_IDLE,
	    ACTION_ACTOR_CROUCH_KICK,
	    ACTION_ACTOR_CROUCH_PUNCH,

	    ACTION_ACTOR_JUMP_PUNCH,
	    ACTION_ACTOR_JUMP_KICK,

	    ACTION_ACTOR_PAIN,
	    ACTION_ACTOR_DEATH,

	    ACTION_ACTOR_JUMP,
	    ACTION_ACTOR_JUMP_UP,
	    ACTION_ACTOR_JUMP_DOWN,

        // Generic enemy
	    ACTION_ENEMY_DEATH,

	    // Thrower actions
	    ACTION_ACTOR_THROW_HIGH,
	    ACTION_ACTOR_THROW_LOW,

        // Chino Cudeiro actions.
	    ACTION_CHINO_CUDEIRO_WALK,
        ACTION_CHINO_CUDEIRO_RWALK,
        ACTION_CHINO_CUDEIRO_IDLE,
        ACTION_CHINO_CUDEIRO_ATTACK,

        ACTION_CHINO_CUDEIRO_SIT,
        ACTION_CHINO_CUDEIRO_WAKE,
        ACTION_CHINO_CUDEIRO_PAIN,
        ACTION_CHINO_CUDEIRO_DEATH,

        ACTION_NONE
    };

	CAnimHelper::Init(eActions);
	#ifndef DISABLE_SOUND
	CSoundHelper::Init(0);
	#endif
	#endif

	// Create the camera
	goGameGlobals.m_poCamera= new CCamera;
	goGameGlobals.m_poCamera->Init(_poRnd);
	goGameGlobals.m_poCamera->Reset();
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	
	// Create the player
	goGameGlobals.m_poPlayer = new CActorTest;
	goGameGlobals.m_poPlayer->Init(_poInput,PLAYER_POS);
	// goGameGlobals.m_poPlayer->SetActionLimits(100,2000); // goGameGlobals.m_poMap->m_oBV.m_oIni.x,goGameGlobals.m_poMap->m_oBV.m_oEnd.x);
	goGameGlobals.m_poPlayer->Reset();
	goGameGlobals.m_poPlayer->Spawn(PLAYER_POS,false);

	// Dirty trick to avoid having the first DeltaT > 0
	for (uint i=0;i<2;i++)
	{
		m_poRnd->BeginScene();
		m_poRnd->EndScene();
	}
}
//-----------------------------------------------------------------------------
void CGameLoop::Finish()
{
}
//-----------------------------------------------------------------------------
void CGameLoop::Loop()
{
	Update( m_poRnd->rGetDeltaT() );
	Render();
}
//-----------------------------------------------------------------------------
void CGameLoop::Update(FEReal _rDeltaT)
{
	if (m_poInput->bDown(IB_MIG))
		m_bGamePaused = !m_bGamePaused;
	
	if (m_bGamePaused) return;

	m_rTime += _rDeltaT;

	CFESpriteInstMgr::Update(_rDeltaT);
	CFESkelAnimInstMgr::Update(_rDeltaT);
	
	// Update Screen FX
	CFEScreenFX::Update(_rDeltaT);
	
	goGameGlobals.m_poCamera->Update(_rDeltaT);	
	goGameGlobals.m_poPlayer->Update(_rDeltaT);
}
//-----------------------------------------------------------------------------
void RenderMap(CFERenderer* _poRnd,CFEMap*_poMap,bool _bRenderFore)
{
	for (uint j=0;j<goGameGlobals.m_poMap->m_poLayers.size();j++)
	{
	    CFEMapLayer* poLayer = goGameGlobals.m_poMap->m_poLayers[j];
	    if (poLayer == NULL) return;

		if (poLayer->m_sName |= "Foreground")
			poLayer->m_bVisible = _bRenderFore;
	    else
			poLayer->m_bVisible = ! _bRenderFore;
    }

	CFEMapRenderer::Render(_poMap, _poRnd );
}
//-----------------------------------------------------------------------------
/*
void RenderMap(CFERenderer* _poRnd,CFEMap*_poMap,uint _uiLayer)
{
	for (uint j=0;j<goGameGlobals.m_poMap->m_poLayers.size();j++)
	{
	    CFEMapLayer* poLayer = goGameGlobals.m_poMap->m_poLayers[j];
	    if (poLayer == NULL) return;

		if (j == _uiLayer)		
			poLayer->m_bVisible = true;
	    else
			poLayer->m_bVisible = false;
    }

	CFEMapRenderer::Render(_poMap, _poRnd );
}
*/
//-----------------------------------------------------------------------------
void CGameLoop::Render()
{
	m_poRnd->BeginScene();

		m_poRnd->SetMaterial(m_hBackground);
		m_poRnd->ResetCamera();	
		m_poRnd->RenderQuad(0,0,m_poRnd->uiGetScreenVWidth(),m_poRnd->uiGetScreenVHeight(),0,0,1,1,CFEColor(1,1,1,1));

        const FEReal CAMERA_ZOOM = 1.0;
		m_poRnd->TranslateCamera(goGameGlobals.m_poPlayer->oGetPos().x - (m_poRnd->uiGetScreenVWidth()/2)/CAMERA_ZOOM,goGameGlobals.m_poPlayer->oGetPos().y - (m_poRnd->uiGetScreenVHeight()/CAMERA_ZOOM) + 40);
		m_poRnd->ZoomCamera(CAMERA_ZOOM);
		    

		// 
		// RenderMap(m_poRnd, goGameGlobals.m_poMap,false);

			CFESkelAnimInstMgr::Render( m_poRnd );
			CFESpriteInstMgr::Render( m_poRnd );

		// RenderMap(m_poRnd, goGameGlobals.m_poMap,true);

		m_poRnd->SetFont(m_poFont);

        // Render DEBUG info
        static uint uiQuads = 0;

        // Put it here to not take into account debug display quads
        m_poRnd->bGetProperty("RENDERED_QUADS",(FEPointer)&uiQuads);
		m_poRnd->SetFont(m_poFont);

		m_poRnd->ResetCamera();
		m_poRnd->ZoomCamera(0.75f);
    	char szStr[1024];

    	FEReal rTime  = m_poRnd->rGetDeltaT();
		if (rTime > 0.0f)
		{
			sprintf(szStr,"FPS: %.1f", (float)(1.0f / rTime));
			m_poRnd->RenderText(szStr,10,32,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);

			sprintf(szStr,"Milis: %.1f", (float)(1000 * rTime));
			m_poRnd->RenderText(szStr,10,48,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);
		}

		sprintf(szStr,"Quads: %d",uiQuads);
		m_poRnd->RenderText(szStr,10,64,CFEColor(1,1,1,1),THAM_LEFT,TVAM_CENTER);

		m_poRnd->ResetCamera();
		if (m_bGamePaused)
			m_poRnd->RenderText("PAUSE",0.5f * m_poRnd->uiGetScreenVWidth(),0.5f * m_poRnd->uiGetScreenVHeight(),CFEColor(1,1,1,1),THAM_CENTER,TVAM_CENTER);	
		
		m_poRnd->ZoomCamera(0.5f);
		if (m_poInput->bPressed(IB_C))
			m_poRnd->RenderText(m_poSysLog->szGetLog(),0,0,CFEColor(1,1,0,1),THAM_LEFT,TVAM_CENTER);

        //
        m_poRnd->ResetCamera();
		CFEScreenFX::Render(m_poRnd);

	m_poRnd->EndScene();
}
//-----------------------------------------------------------------------------
