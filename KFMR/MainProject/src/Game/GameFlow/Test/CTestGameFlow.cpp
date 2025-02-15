//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"
#include "Game/Entity/CEntityMgr.h"

#include "CTestGameFlow.h"

#include "Game/Entity/Player/CActorTest.h"
#include "Game/Entity/Camera/CCamera.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CSFXHelper.h"
#include "Game/Helpers/CHUDMgr.h"
//-----------------------------------------------------------------------------
enum
{
	// Level0BonusGameFlowState
	LTESTGFS_NONE,
	LTESTGFS_GAME,
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

        // bonus player actions
	    ACTION_ACTOR_GRAB,
	    ACTION_ACTOR_GRAB_CROUCH,

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
}
//-----------------------------------------------------------------------------
CTestGameFlow::CTestGameFlow(uint _uiSkill)
{
    ///
    InitAnimations();
    ///
    InitSounds();
    ///
    InitFX();
    
    // Load the game level
 	goGameGlobals.m_poMap = CFEMapLoader::poLoad("data/levels/level1/level");
 	goGameGlobals.m_poMap->m_oBV.m_oIni.x += goGameGlobals.m_rScreenMargin;
 	goGameGlobals.m_poMap->m_oBV.m_oEnd.x -= goGameGlobals.m_rScreenMargin;

	// Create the camera
	goGameGlobals.m_poCamera= new CCamera;
	goGameGlobals.m_poCamera->Init(goGameGlobals.m_poRenderer[0]);

	// Create the player
	const CFEVect2 PLAYER_POS(700,0);
	goGameGlobals.m_poPlayer = new CActorTest;
	goGameGlobals.m_poPlayer->Init(goGameGlobals.m_poInput,PLAYER_POS);

	// Set initial state
	SetState(0);
}
//-----------------------------------------------------------------------------
CTestGameFlow::~CTestGameFlow()
{
}
//-----------------------------------------------------------------------------
void CTestGameFlow::Reset()
{
	CEntityMgr::Reset();

	goGameGlobals.m_poPlayer->Reset();
	goGameGlobals.m_poCamera->Reset();

	ChangeState(0);
}
//-----------------------------------------------------------------------------
/// Function to be called the first time a level is to be played.
void CTestGameFlow::Start()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(400,370));

	ChangeState(LTESTGFS_GAME);
}
//-----------------------------------------------------------------------------
/// Function to be called upon level restart.
void CTestGameFlow::Restart()
{
	goGameGlobals.m_poCamera->Spawn(CFEVect2(100,0));
	goGameGlobals.m_poPlayer->Spawn(CFEVect2(420,370));

	ChangeState(LTESTGFS_GAME);
}
//-----------------------------------------------------------------------------
bool CTestGameFlow::bFinished()
{
    return(false);
}
//-----------------------------------------------------------------------------
void CTestGameFlow::Update(FEReal _rDeltaT)
{
	switch (uiGetState())
	{
		case LTESTGFS_NONE:
		{
			ChangeState(LTESTGFS_GAME);
		}
		break;

		case LTESTGFS_GAME:
		{
	        // Update the entity manager.
	        CEntityMgr::Update(_rDeltaT);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CTestGameFlow::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		case LTESTGFS_GAME:
		{
			goGameGlobals.m_poCamera->Unlock();
			goGameGlobals.m_poPlayer->ChangeState(PS_IDLE);
			goGameGlobals.m_poPlayer->SetActionLimits(goGameGlobals.m_poMap->m_oBV.m_oIni.x,goGameGlobals.m_poMap->m_oBV.m_oEnd.x);
			CGameMsgMgr::SendMsg(GMSG_PRESENT_HUD,NULL);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CTestGameFlow::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
