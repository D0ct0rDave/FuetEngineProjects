//-----------------------------------------------------------------------------
#include <FuetEngine.h>

#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"
#include "Game/Entity/CEntityMgr.h"

#include "CLevel1AGameFlow.h"
#include "CLevel1AEnemySpawner.h"
#include "Level1AGlobals.h"

#include "Game/Entity/Camera/CCamera.h"
#include "Game/Entity/Player/CPlayer.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CSFXHelper.h"
#include "Game/Helpers/CHUDMgr.h"
#include "Game/Map/CMapMgr.h"
//-----------------------------------------------------------------------------
enum
{
	// Level1AGameFlowState
	L1AGFS_NONE,
	L1AGFS_INITIAL_CINEMATIC,
	L1AGFS_GAME,
	L1AGFS_ENDING,
	L1AGFS_END,
	L1AGFS_FINISHED,
};
//-----------------------------------------------------------------------------
static void InitFX()
{
    /// Set of actions to load in this level...
    EGameSFX eSFX[] =
    {
        SFX_ONOMATOPEIA,
        SFX_LEVEL_1_SMOKE,
        
        SFX_DUST,
        SFX_CINECT_UP,
        SFX_CINECT_DOWN,

        SFX_NONE,	
    };

    CSFXHelper::Init(eSFX);
}
//-----------------------------------------------------------------------------
static void InitSounds()
{
    /// Set of actions to load in this level...
    EGameSounds eSounds[] =
    {
	    SND_PLAYER_ATTACK,
	    SND_PLAYER_JUMP_ATTACK,
	    SND_PLAYER_PAIN,
	    SND_PLAYER_HIT,
	    SND_PLAYER_DEATH,

	    SND_ENEMY_ATTACK,
	    SND_ENEMY_PAIN,
	    SND_ENEMY_HIT,
	    SND_ENEMY_DEATH,

	    SND_THROWN,
	    SND_OBJECT_HIT,

	    SND_CHINOCUDEIRO_ATTACK,
        SND_NONE,	
    };

    CSoundHelper::Init(eSounds);
}
//-----------------------------------------------------------------------------
static void InitAnimations()
{
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

        ACTION_NONE
    
    };

	CAnimHelper::Init(eActions);
}
//-----------------------------------------------------------------------------
static void InitHUD()
{
    CHUDMgr::Init(0x1a);
    CHUDMgr::SetProgressFace(0);
    CHUDMgr::SetLevelTitle("The Forest: Part I");
}
//-----------------------------------------------------------------------------
CLevel1AGameFlow::CLevel1AGameFlow(uint _uiSkill)
{
    // Load common actor and animations
    InitAnimations();

	// Load common sounds
	InitSounds();
    
    // Load common SFX
    InitFX();

    // HUDMgr
	InitHUD();

	// Load the game level
	CMapMgr::Init("data/levels/level1/level");	// ToDo: Change graphics for the level 0

	// Create the camera
	goGameGlobals.m_poCamera= new CCamera;
	goGameGlobals.m_poCamera->Init(goGameGlobals.m_poRenderer[0]);

	// Create the player
	const CFEVect2 PLAYER_POS(700,0);
	goGameGlobals.m_poPlayer = new CPlayer;
	goGameGlobals.m_poPlayer->Init(goGameGlobals.m_poInput,PLAYER_POS);

	// Create the enemy spawner
	m_poEnemySpawner = new CLevel1AEnemySpawner(_uiSkill);
	m_poCinematic = CFECinematicLoader::poLoad("data/cinematics/level1a");

	// Load level bgm
	CSoundHelper::LoadBGM("level");

    /// Add level specific SFX
    CFEVect2 oOfs(goGameGlobals.m_rScreenMargin,goGameGlobals.m_poRenderer[0]->uiGetScreenVHeight() / CAMERA_ZOOM);
    CSFXHelper::SpawnFollowerFX(SFX_LEVEL_1_SMOKE,goGameGlobals.m_poCamera,oOfs);

	// Set initial state
	SetState(0);
}
//-----------------------------------------------------------------------------
CLevel1AGameFlow::~CLevel1AGameFlow()
{
}
//-----------------------------------------------------------------------------
void CLevel1AGameFlow::Reset()
{
	CEntityMgr::Reset();

	goGameGlobals.m_poPlayer->Reset();
	goGameGlobals.m_poCamera->Reset();

	m_poEnemySpawner->Reset();

    CMapMgr::Reset();

    CHUDMgr::Reset();
    // CHUDMgr::SetPlayerLives( m_iPlayerLives );

	ChangeState(0);
}
//-----------------------------------------------------------------------------
/// Function to be called the first time a level is to be played.
void CLevel1AGameFlow::Start()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(0,370));

	ChangeState(L1AGFS_INITIAL_CINEMATIC);
}
//-----------------------------------------------------------------------------
/// Function to be called upon level restart.
void CLevel1AGameFlow::Restart()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(420,370));

	ChangeState(L1AGFS_GAME);
}
//-----------------------------------------------------------------------------
bool CLevel1AGameFlow::bFinished()
{
    return(uiGetState() == L1AGFS_FINISHED);
}
//-----------------------------------------------------------------------------
void CLevel1AGameFlow::GenericGameLoop(FEReal _rDeltaT)
{
	if (! goGameGlobals.m_poPlayer->bDead())
	{
		// Update the entity manager.
		CEntityMgr::Update(_rDeltaT);
		
		// Update the enemy spawner.
		m_poEnemySpawner->Update(_rDeltaT);
		
		// Set the player progress
		CHUDMgr::SetPlayerProgress( (goGameGlobals.m_poPlayer->oGetPos().x - goGameGlobals.m_oStartPos.x) / (goGameGlobals.m_oEndPos.x - goGameGlobals.m_oStartPos.x) );
	}
}
//-----------------------------------------------------------------------------
void CLevel1AGameFlow::Update(FEReal _rDeltaT)
{
    // update level particle system.
    /*
    CFEVect2 oPos = goGameGlobals.m_poCamera->oGetPos();
    oPos.x += goGameGlobals.m_rScreenMargin;
    oPos.y += (goGameGlobals.m_poRenderer->uiGetScreenVHeight() / CAMERA_ZOOM);
    CFEParticleSysInstMgr::SetPos(m_hPS,oPos);
    */

    // Update gameflow
	switch (uiGetState())
	{
		case L1AGFS_NONE:
		{
			ChangeState(L1AGFS_INITIAL_CINEMATIC);
		}
		break;

		case L1AGFS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(L1AGFS_GAME);
			}
		}
		break;

		case L1AGFS_GAME:
		{
			// Update the gameplay
			GenericGameLoop(_rDeltaT);

			if (goGameGlobals.m_poPlayer->oGetPos().x >= goGameGlobals.m_oEndPos.x)
			{
				ChangeState(L1AGFS_ENDING);		// Ending level
			}
		}
		break;

		case L1AGFS_ENDING:
		{
			if(CFECinematicMgr::bPlaneFinished() && !CHUDMgr::bPlayingAction(HA_SHOW_STAGE_CLEAR))
			{
				ChangeState(L1AGFS_END);		// End level
			}
		}
		break;

		case L1AGFS_END:
		{
		    // Ponemos esto aquí pues a lo mejor es interesante hacer distintas
		    // cortinillas en función de si terminas un level, o un final boss, etc ...
			if(!CHUDMgr::bPlayingAction(HA_SHOW_CORTINILLAIN))
			{
				ChangeState(L1AGFS_FINISHED);
			}
		}
		break;
    }
}
//-----------------------------------------------------------------------------
void CLevel1AGameFlow::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		case L1AGFS_INITIAL_CINEMATIC:
		{
		    goGameGlobals.m_poPlayer->ChangeState(ACTOR_CINEMATIC_STATE);
			goGameGlobals.m_poCamera->Lock();

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(0);
		}
		break;

		case L1AGFS_GAME:
		{
    		goGameGlobals.m_poCamera->Unlock();
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poMap->m_oBV.m_oIni.x,goGameGlobals.m_poMap->m_oBV.m_oEnd.x);
			CGameMsgMgr::SendMsg(GMSG_START_GAME_LOOP,NULL);
		}
		break;

		case L1AGFS_ENDING:
		{
		    CHUDMgr::SetProgressFace(7);
			goGameGlobals.m_poPlayer->SetState(ACTOR_CINEMATIC_STATE);
			CHUDMgr::Play(HA_SHOW_STAGE_CLEAR);

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(1);
		}		
		break;

		case L1AGFS_END:
		{
			CHUDMgr::Play(HA_SHOW_CORTINILLAIN);
			CFESoundMixer::FadeOut(2.0);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CLevel1AGameFlow::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case L1AGFS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;

		case L1AGFS_GAME:
		{
			goGameGlobals.m_poCamera->Lock();
			goGameGlobals.m_poPlayer->Disable();
			CHUDMgr::Play(HA_HIDE_PLAYER_HUD);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
