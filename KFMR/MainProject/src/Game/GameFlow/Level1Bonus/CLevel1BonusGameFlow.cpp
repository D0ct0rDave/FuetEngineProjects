//-----------------------------------------------------------------------------
#include <FuetEngine.h>

#include "CLevel1BonusGameFlow.h"
#include "CLevel1BonusEnemySpawner.h"
#include "Level1BonusGlobals.h"

#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/Camera/CCamera.h"
#include "Game/Entity/Player/CPlayerBonus.h"
#include "Game/Entity/Player/CSleepingBeauty.h"

#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CSFXHelper.h"
#include "Game/Helpers/CHUDMgr.h"
#include "Game/Map/CMapMgr.h"

//-----------------------------------------------------------------------------
const uint L1B_MAX_BAD_OBJECTS = 3;
const uint L1B_TIME_MAX_TO_END = 60;
//-----------------------------------------------------------------------------
enum
{
	// Level1BonusGameFlowState
	L1CGFS_NONE,
	L1CGFS_INITIAL_CINEMATIC,
	L1CGFS_GAME,
	L1CGFS_BOSS_INITIAL_CINEMATIC,
	L1CGFS_BOSS_FIGHT,
	L1CGFS_BOSS_DEAD_CINEMATIC,
	L1CGFS_FINISH
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

	    ACTION_ACTOR_GRAB,
	    ACTION_ACTOR_GRAB_CROUCH,

        ACTION_NONE
    
    };

	CAnimHelper::Init(eActions);
}
//-----------------------------------------------------------------------------
static void InitHUD()
{
    CHUDMgr::Init(0x1c);
    CHUDMgr::SetProgressFace(0);
    CHUDMgr::SetLevelTitle("The Forest: Bonus");
}
//-----------------------------------------------------------------------------
CLevel1BonusGameFlow::CLevel1BonusGameFlow(uint _uiSkill)
{
    // Load common actor and animations
    InitAnimations();

	// Load common sounds
	InitSounds();
	
	// Load common sfx
    InitFX();

    // 
    InitHUD();

	// Load the game level
	CMapMgr::Init("data/levels/level1/level");	// ToDo: Change graphics for the level 0

	// Create the camera
	goGameGlobals.m_poCamera= new CCamera;
	goGameGlobals.m_poCamera->Init(goGameGlobals.m_poRenderer[0]);

	// Create the player
	const CFEVect2 PLAYER_POS(700,0);
	goGameGlobals.m_poPlayer = new CPlayerBonus;
	goGameGlobals.m_poPlayer->Init(goGameGlobals.m_poInput,PLAYER_POS);

	// Create the enemy spawner
	m_poEnemySpawner = new CLevel1BonusEnemySpawner(_uiSkill);
	m_poCinematic = CFECinematicLoader::poLoad("data/cinematics/level1c");

	// Load level bgm
    CSoundHelper::LoadBGM("bonus");
	
	// Set initial state
	SetState(0);
}
//-----------------------------------------------------------------------------
CLevel1BonusGameFlow::~CLevel1BonusGameFlow()
{
}
//-----------------------------------------------------------------------------
void CLevel1BonusGameFlow::Reset()
{
	CEntityMgr::Reset();

	goGameGlobals.m_poPlayer->Reset();
	goGameGlobals.m_poCamera->Reset();

	m_poEnemySpawner->Reset();
    
    CMapMgr::Reset();
	ChangeState(0);

	m_rTimer = L1B_TIME_MAX_TO_END;
}
//-----------------------------------------------------------------------------
/// Function to be called the first time a level is to be played.
void CLevel1BonusGameFlow::Start()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(0,370));

	ChangeState(L1CGFS_INITIAL_CINEMATIC);
}
//-----------------------------------------------------------------------------
/// Function to be called upon level restart.
void CLevel1BonusGameFlow::Restart()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(420,370));

	ChangeState(L1CGFS_GAME);
}
//-----------------------------------------------------------------------------
bool CLevel1BonusGameFlow::bFinished()
{
    return( uiGetState() == L1CGFS_FINISH );
}
//-----------------------------------------------------------------------------
void CLevel1BonusGameFlow::GenericGameLoop(FEReal _rDeltaT)
{
	if (! goGameGlobals.m_poPlayer->bDead())
	{
		// Update the entity manager.
		CEntityMgr::Update(_rDeltaT);
		
		// Update the enemy spawner.
		m_poEnemySpawner->Update(_rDeltaT);

		// Set the player progress
		CHUDMgr::SetPlayerProgress( (goGameGlobals.m_poPlayer->oGetPos().x - goGameGlobals.m_poMap->m_oBV.m_oIni.x) / (goGameGlobals.m_poMap->m_oBV.m_oEnd.x - goGameGlobals.m_poMap->m_oBV.m_oIni.x) );

		// Update the timer to end the game
		m_rTimer -= _rDeltaT;
		if(m_rTimer <= 0) m_rTimer = 0;

		// End game
		if(goGameGlobals.m_poPlayer->uiGetBadObjects() >= L1B_MAX_BAD_OBJECTS || m_rTimer <= 0)
		{
			ChangeState(L1CGFS_FINISH);
		}
		if(m_rTimer < 10)
		{
			goGameGlobals.m_oLogger.Print("------> Tiempo restante: %.1f seconds\n",(float)m_rTimer);
		}

	}
}
//-----------------------------------------------------------------------------
void CLevel1BonusGameFlow::Update(FEReal _rDeltaT)
{
	switch (uiGetState())
	{
		case L1CGFS_NONE:
		{
			ChangeState(L1CGFS_INITIAL_CINEMATIC);
		}
		break;

		case L1CGFS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(L1CGFS_GAME);
			}
		}
		break;

		case L1CGFS_GAME:
		{
			// Update the gameplay
			GenericGameLoop(_rDeltaT);

			if (goGameGlobals.m_poPlayer->oGetPos().x >= BOSS_AREA_LIMIT)
			{
				// Present boss
				ChangeState(L1CGFS_BOSS_INITIAL_CINEMATIC);
			}
		}
		break;

		case L1CGFS_BOSS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(L1CGFS_BOSS_FIGHT);
			}
		}
		break;

		case L1CGFS_BOSS_FIGHT:
		{
			// Update the gameplay
			CEntityMgr::Update(_rDeltaT);
		}
		break;

		case L1CGFS_BOSS_DEAD_CINEMATIC:
		{
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CLevel1BonusGameFlow::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		case L1CGFS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Lock();

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(0);
		}
		break;

		case L1CGFS_GAME:
		{
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poCamera->oGetPos().x,goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin * 2.0f);
			//CGameMsgMgr::SendMsg(GMSG_PRESENT_HUD,NULL);
			CGameMsgMgr::SendMsg(GMSG_START_GAME_LOOP,NULL);
		}
		break;

		case L1CGFS_FINISH:
		break;
	}
}
//-----------------------------------------------------------------------------
void CLevel1BonusGameFlow::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case L1CGFS_INITIAL_CINEMATIC:
		{
			//goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
