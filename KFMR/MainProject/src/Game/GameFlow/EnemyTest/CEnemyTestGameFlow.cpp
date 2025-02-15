//-----------------------------------------------------------------------------
#include <FuetEngine.h>

#include "CEnemyTestGameFlow.h"
#include "CEnemyTestEnemySpawner.h"

#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"
#include "Game/Entity/Enemy/CChinoCudeiro.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/Camera/CCamera.h"
#include "Game/Entity/Player/CPlayer.h"
#include "Game/Helpers/CEnemyExiter.h"

#include "Game/Helpers/CHUDMgr.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CSFXHelper.h"
#include "Game/Map/CMapMgr.h"
//-----------------------------------------------------------------------------
enum
{
	ETGFS_NONE,
	ETGFS_INITIAL_CINEMATIC,
	ETGFS_GAME,
	ETGFS_BOSS_INITIAL_CINEMATIC,
	ETGFS_BOSS_FIGHT,
	ETGFS_BOSS_DEAD_CINEMATIC,
	ETGFS_ENDING,
	ETGFS_END,
	ETGFS_FINISHED
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

        // Chino Cudeiro actions.
	    ACTION_CHINO_CUDEIRO_WALK,
        ACTION_CHINO_CUDEIRO_RWALK,
        ACTION_CHINO_CUDEIRO_IDLE,
        ACTION_CHINO_CUDEIRO_ATTACK,

        ACTION_CHINO_CUDEIRO_SIT,
        ACTION_CHINO_CUDEIRO_WAKE,
        ACTION_CHINO_CUDEIRO_PAIN,
        ACTION_CHINO_CUDEIRO_DEATH,

	    ACTION_DOPEFISH_IDLE,
	    ACTION_DOPEFISH_JUMP_UP,
	    ACTION_DOPEFISH_JUMP_DOWN,

        ACTION_NONE
    };

	CAnimHelper::Init(eActions);
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
static void InitHUD()
{
    CHUDMgr::Init(0x1b);
    CHUDMgr::SetProgressFace(0);
    CHUDMgr::SetLevelTitle("Enemy Test");
}
//-----------------------------------------------------------------------------
CEnemyTestGameFlow::CEnemyTestGameFlow(uint _uiSkill)
{
     // Load common actor and animations
    InitAnimations();

	// Load common sounds
	InitSounds();

    // Load FX
    InitFX();

    // 
    InitHUD();

	// Load the game level
	CMapMgr::Init("data/levels/level2/level2");	// ToDo: Change graphics for the level 0
	goGameGlobals.m_oLogger.Print("------> Level Loaded: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Create the camera
	goGameGlobals.m_poCamera= new CCamera;
	goGameGlobals.m_poCamera->Init(goGameGlobals.m_poRenderer[0]);

	// Create the player
	const CFEVect2 PLAYER_POS(700,0);
	goGameGlobals.m_poPlayer = new CPlayer;
	goGameGlobals.m_poPlayer->Init(goGameGlobals.m_poInput,PLAYER_POS);
    goGameGlobals.m_oLogger.Print("------> Player created: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Create the enemy spawner
	m_poEnemySpawner = new CEnemyTestEnemySpawner(_uiSkill);
	goGameGlobals.m_oLogger.Print("------> Enemy Spawner created: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());
	
	m_poCinematic = CFECinematicLoader::poLoad("data/cinematics/level1b");
    goGameGlobals.m_oLogger.Print("------> Cinematics intialized: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Load level bgm
	CSoundHelper::LoadBGM("bgm_1");

    goGameGlobals.m_oLogger.Print("------> End Gameflow Init: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Set initial state
	SetState(0);
}
//-----------------------------------------------------------------------------
CEnemyTestGameFlow::~CEnemyTestGameFlow()
{
}
//-----------------------------------------------------------------------------
void CEnemyTestGameFlow::Reset()
{
	CEntityMgr::Reset();

	goGameGlobals.m_poPlayer->Reset();
	goGameGlobals.m_poCamera->Reset();

	m_poEnemySpawner->Reset();

    CMapMgr::Reset();
	ChangeState(0);
}
//-----------------------------------------------------------------------------
/// Function to be called the first time a level is to be played.
void CEnemyTestGameFlow::Start()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(0,370));

	ChangeState(ETGFS_GAME);
}
//-----------------------------------------------------------------------------
/// Function to be called upon level restart.
void CEnemyTestGameFlow::Restart()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(420,370));

	ChangeState(ETGFS_GAME);
}
//-----------------------------------------------------------------------------
bool CEnemyTestGameFlow::bFinished()
{
    return(uiGetState() == ETGFS_FINISHED);
}
//-----------------------------------------------------------------------------
void CEnemyTestGameFlow::GenericGameLoop(FEReal _rDeltaT)
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
void CEnemyTestGameFlow::Update(FEReal _rDeltaT)
{
	switch (uiGetState())
	{
		case ETGFS_NONE:
		{
			ChangeState(ETGFS_INITIAL_CINEMATIC);
		}
		break;

		case ETGFS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(ETGFS_GAME);
			}
		}
		break;

		case ETGFS_GAME:
		{
			// Update the gameplay
			GenericGameLoop(_rDeltaT);
		}
		break;

		case ETGFS_END:
		{
		    // Ponemos esto aquí pues a lo mejor es interesante hacer distintas
		    // cortinillas en función de si terminas un level, o un final boss, etc ...
			if(!CHUDMgr::bPlayingAction(HA_SHOW_CORTINILLAIN))
			{
				ChangeState(ETGFS_FINISHED);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CEnemyTestGameFlow::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
        /*
        case ETGFS_INITIAL_CINEMATIC:
		{
		    goGameGlobals.m_poPlayer->ChangeState(ACTOR_CINEMATIC_STATE);
			goGameGlobals.m_poCamera->Lock();

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(0);
		}
		break;
        */

		case ETGFS_GAME:
		{
    		goGameGlobals.m_poCamera->Lock();
    		goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poCamera->oGetPos().x,goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin * 2.0f);
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			CGameMsgMgr::SendMsg(GMSG_START_GAME_LOOP,NULL);
		}
		break;

		case ETGFS_BOSS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Lock();
			goGameGlobals.m_poPlayer->ChangeState(ACTOR_CINEMATIC_STATE);

            // Bosses always start in cinematic state
            // goGameGlobals.m_poBoss->ChangeState(ACTOR_CINEMATIC_STATE);

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(1);

			CEnemyExiter::DoExit();
		}
		break;

		case ETGFS_BOSS_FIGHT:
		{
			//
			goGameGlobals.m_poBoss->ChangeState(CCS_IDLE);
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poCamera->oGetPos().x,goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin * 2.0f);
			goGameGlobals.m_poCamera->Lock();

			// Send message announcing the boss want to fight
			CGameMsgMgr::SendMsg(GMSG_BOSS_STARTS,NULL,0);
		}
		break;

		case ETGFS_BOSS_DEAD_CINEMATIC:
		{
			CHUDMgr::Play(HA_HIDE_ENEMY_HUD);
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
		}
		break;

		case ETGFS_ENDING:
		{
			CHUDMgr::Play(HA_SHOW_STAGE_CLEAR);
		}
		break;

		case ETGFS_END:
		{
			CHUDMgr::Play(HA_SHOW_CORTINILLAIN);
			CFESoundMixer::FadeOut(2.0);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CEnemyTestGameFlow::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case ETGFS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;

		case ETGFS_BOSS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CEnemyTestGameFlow::ReceiveMessage(uint _uiMessage/*,CEntity* _poSrcEntity,FEPointer _pData*/)
{
	if(_uiMessage == GMSG_BOSS_DIED)
	{
		ChangeState(ETGFS_BOSS_DEAD_CINEMATIC);		// End level
	}
	
}
//-----------------------------------------------------------------------------
