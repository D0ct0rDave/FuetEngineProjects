//-----------------------------------------------------------------------------
#include <FuetEngine.h>

#include "CLevel2BGameFlow.h"
#include "CLevel2BEnemySpawner.h"
#include "Level2BGlobals.h"

#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"
#include "Game/Entity/Enemy/CDopefish.h"

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
	// Level0GameFlowState
	L2BGFS_NONE,
	L2BGFS_INITIAL_CINEMATIC,
	L2BGFS_GAME,
	L2BGFS_BOSS_INITIAL_CINEMATIC,
	L2BGFS_BOSS_FIGHT,
	L2BGFS_BOSS_DEAD_CINEMATIC,
	L2BGFS_ENDING,
	L2BGFS_END,
	L2BGFS_FINISHED
};
//-----------------------------------------------------------------------------
static void InitFX()
{
    /// Set of actions to load in this level...
    EGameSFX eSFX[] =
    {
        SFX_ONOMATOPEIA,
        // SFX_LEVEL_1_SMOKE,

        // SFX_DUST,
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

        // Dopefish actions.
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
    CHUDMgr::Init(0x2b,"dopefish");
    CHUDMgr::SetProgressFace(0);
    CHUDMgr::SetLevelTitle("The Bridge: Part II");
}
//-----------------------------------------------------------------------------
CLevel2BGameFlow::CLevel2BGameFlow(uint _uiSkill)
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
	m_poEnemySpawner = new CLevel2BEnemySpawner(_uiSkill);
	goGameGlobals.m_oLogger.Print("------> Enemy Spawner created: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	m_poCinematic = CFECinematicLoader::poLoad("data/cinematics/level2b");
    goGameGlobals.m_oLogger.Print("------> Cinematics intialized: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Load level bgm
	CSoundHelper::LoadBGM("bgm_1");
    goGameGlobals.m_oLogger.Print("------> End Gameflow Init: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());

	// Set initial state
	SetState(0);
}
//-----------------------------------------------------------------------------
CLevel2BGameFlow::~CLevel2BGameFlow()
{
}
//-----------------------------------------------------------------------------
void CLevel2BGameFlow::Reset()
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
void CLevel2BGameFlow::Start()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(0,370));

	ChangeState(L2BGFS_INITIAL_CINEMATIC);
}
//-----------------------------------------------------------------------------
/// Function to be called upon level restart.
void CLevel2BGameFlow::Restart()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(420,370));

	ChangeState(L2BGFS_GAME);
}
//-----------------------------------------------------------------------------
bool CLevel2BGameFlow::bFinished()
{
    return(uiGetState() == L2BGFS_FINISHED);
}
//-----------------------------------------------------------------------------
void CLevel2BGameFlow::GenericGameLoop(FEReal _rDeltaT)
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
void CLevel2BGameFlow::Update(FEReal _rDeltaT)
{
	switch (uiGetState())
	{
		case L2BGFS_NONE:
		{
			ChangeState(L2BGFS_INITIAL_CINEMATIC);
		}
		break;

		case L2BGFS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(L2BGFS_GAME);
			}
		}
		break;

		case L2BGFS_GAME:
		{
			// Update the gameplay
			GenericGameLoop(_rDeltaT);

			if (goGameGlobals.m_poPlayer->oGetPos().x >= BOSS_AREA_LIMIT)
			{
				// Present boss
				ChangeState(L2BGFS_BOSS_INITIAL_CINEMATIC);
			}
		}
		break;

		case L2BGFS_BOSS_INITIAL_CINEMATIC:
		{
			if (CFECinematicMgr::bPlaneFinished())
			{
				ChangeState(L2BGFS_BOSS_FIGHT);
			}
			// Update the entity manager.
			CEntityMgr::Update(_rDeltaT);
		}
		break;

		case L2BGFS_BOSS_FIGHT:
		{
			// Update the gameplay
			CEntityMgr::Update(_rDeltaT);			
		}
		break;

		case L2BGFS_BOSS_DEAD_CINEMATIC:
		{
			if(!CHUDMgr::bPlayingAction(HA_HIDE_ENEMY_HUD))
			{
				ChangeState(L2BGFS_ENDING);
			}
		}
		break;

		case L2BGFS_ENDING:
		{
			if(CFECinematicMgr::bPlaneFinished() && !CHUDMgr::bPlayingAction(HA_SHOW_STAGE_CLEAR))
			{
				ChangeState(L2BGFS_END);		// End level
			}
		}
		break;

		case L2BGFS_END:
		{
		    // Ponemos esto aquí pues a lo mejor es interesante hacer distintas
		    // cortinillas en función de si terminas un level, o un final boss, etc ...
			if(!CHUDMgr::bPlayingAction(HA_SHOW_CORTINILLAIN))
			{
				ChangeState(L2BGFS_FINISHED);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define CASE(STATE)\
    case STATE:\
    goGameGlobals.m_oLogger.Print(" Gameflow Entering %s\n",#STATE);
#else
    #define CASE(STATE)\
        case STATE:
#endif
//-----------------------------------------------------------------------------
void CLevel2BGameFlow::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
        CASE(L2BGFS_INITIAL_CINEMATIC)
		{
		    goGameGlobals.m_poPlayer->ChangeState(ACTOR_CINEMATIC_STATE);
			goGameGlobals.m_poCamera->Lock();

			CFECinematicMgr::SetCinematic(m_poCinematic);
			CFECinematicMgr::SetPlane(0);
		}
		break;

		CASE(L2BGFS_GAME)
		{
			goGameGlobals.m_poCamera->Unlock();
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poMap->m_oBV.m_oIni.x,goGameGlobals.m_poMap->m_oBV.m_oEnd.x);
			CGameMsgMgr::SendMsg(GMSG_START_GAME_LOOP,NULL);
		}
		break;

		CASE(L2BGFS_BOSS_INITIAL_CINEMATIC)
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

		CASE(L2BGFS_BOSS_FIGHT)
		{
			//
			goGameGlobals.m_poBoss->ChangeState(DFS_IDLE);
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poCamera->oGetPos().x,goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin * 2.0f);
			goGameGlobals.m_poCamera->Lock();

			// Send message announcing the boss want to fight
			CGameMsgMgr::SendMsg(GMSG_BOSS_STARTS,NULL,0);
		}
		break;

		CASE(L2BGFS_BOSS_DEAD_CINEMATIC)
		{
			CHUDMgr::Play(HA_HIDE_ENEMY_HUD);
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
		}
		break;

		CASE(L2BGFS_ENDING)
		{
			CHUDMgr::Play(HA_SHOW_STAGE_CLEAR);
		}
		break;

		CASE(L2BGFS_END)
		{
			CHUDMgr::Play(HA_SHOW_CORTINILLAIN);
			CFESoundMixer::FadeOut(2.0);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CLevel2BGameFlow::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case L2BGFS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;

		case L2BGFS_BOSS_INITIAL_CINEMATIC:
		{
			goGameGlobals.m_poCamera->Unlock();
			CFECinematicMgr::SetCinematic(NULL);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CLevel2BGameFlow::ReceiveMessage(uint _uiMessage/*,CEntity* _poSrcEntity,FEPointer _pData*/)
{
	if(_uiMessage == GMSG_BOSS_DIED)
	{
		ChangeState(L2BGFS_BOSS_DEAD_CINEMATIC);		// End level
	}
}
//-----------------------------------------------------------------------------
