//-----------------------------------------------------------------------------
#include "CPlayer.h"
#include <FuetEngine.h>
#include "FuetEngineExt.h"
//-----------------------------------------------------------------------------
// Player actions

/*
#define PA_WALK			1
#define PA_JUMP			0
#define PA_CROUCHING	10
#define PA_CROUCH_IDLE	5
#define PA_CROUCH_KICK	6
#define PA_IDLE			4
#define PA_JUMP_KICK	7
#define PA_KICK			3
*/
//-----------------------------------------------------------------------------
const FEReal PLAYER_SCALE = 1.0; // 0.3;
const FEReal PLAYER_SPEED = 200.0;
const FEReal JUMP_KICK_ANIM_TIME= 0.3;
const FEReal JUMP_TIME			= 1.0;
const FEReal JUMP_UP_TIME		= JUMP_TIME*0.5;

const FEReal JUMP_MULTIPLIER	= 40.0;
const FEReal JUMP_SPEED			= 5.5;
const FEReal GROUND_HEIGHT		= 320;
//-----------------------------------------------------------------------------

static FEReal	m_rDirMult	= 1.0;

//-----------------------------------------------------------------------------

static int PA_WALK			=1;
static int PA_JUMP_UP		=0;
static int PA_JUMP_DOWN		=0;
static int PA_CROUCHING		=10;
static int PA_CROUCH_IDLE	=5;
static int PA_IDLE			=4;

#define MAX_CROUCH_KICKS 1
static int PA_CROUCH_KICK[MAX_CROUCH_KICKS];

#define MAX_JUMP_KICKS 2
static int PA_JUMP_KICK[MAX_JUMP_KICKS];

#define MAX_KICKS 2
static int PA_KICK[MAX_KICKS];

#define MAX_CROUCH_PUNCHES 1
static int PA_CROUCH_PUNCH[MAX_CROUCH_PUNCHES];

#define MAX_JUMP_PUNCHES 1
static int PA_JUMP_PUNCH[MAX_JUMP_PUNCHES];

#define MAX_PUNCHES 1
static int PA_PUNCH[MAX_PUNCHES];

//-----------------------------------------------------------------------------
CPlayer::CPlayer()
{
}
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}
///-----------------------------------------------------------------------------&
void CPlayer::Init(FEHandler _hHandler)
{
	// m_hSprInst = CFESpriteInstMgr::hGetInstance("data/actors/player/anims");
	m_hSprInst = CFESkelAnimInstMgr::hGetInstance("data/actors/player/anims");

	CFESkelAnim* poSpr = CFESkelAnimInstMgr::poGetAnim(m_hSprInst);

	// CFESprite* poSpr = CFESpriteInstMgr::poGetSprite(m_hSprInst);

	PA_WALK				= poSpr->poGetAnimActionSet()->iGetActionIdx("walk");
	PA_JUMP_UP			= poSpr->poGetAnimActionSet()->iGetActionIdx("jump_up");
	PA_JUMP_DOWN		= poSpr->poGetAnimActionSet()->iGetActionIdx("jump_down");
	PA_CROUCHING		= poSpr->poGetAnimActionSet()->iGetActionIdx("crouch");
	PA_CROUCH_IDLE		= poSpr->poGetAnimActionSet()->iGetActionIdx("crouch_idle");
	PA_IDLE				= poSpr->poGetAnimActionSet()->iGetActionIdx("idle");

	PA_JUMP_KICK[0]		= poSpr->poGetAnimActionSet()->iGetActionIdx("jump_kick1");
	PA_JUMP_KICK[1]		= poSpr->poGetAnimActionSet()->iGetActionIdx("jump_kick1"); // poSpr->iGetActionIdx("jump_kick2");
	PA_KICK[0]			= poSpr->poGetAnimActionSet()->iGetActionIdx("kick1");
	PA_KICK[1]			= poSpr->poGetAnimActionSet()->iGetActionIdx("kick1"); // poSpr->iGetActionIdx("kick2");
	PA_CROUCH_KICK[0]	= poSpr->poGetAnimActionSet()->iGetActionIdx("crouch_kick1");

	PA_PUNCH[0]			= poSpr->poGetAnimActionSet()->iGetActionIdx("kick1");// poSpr->iGetActionIdx("punch1");
	PA_JUMP_PUNCH[0]	= poSpr->poGetAnimActionSet()->iGetActionIdx("kick1");// poSpr->iGetActionIdx("punch1");
	PA_CROUCH_PUNCH[0]	= poSpr->poGetAnimActionSet()->iGetActionIdx("kick1");// poSpr->iGetActionIdx("punch1");

	/*
	poSpr->poGetAction("walk")->m_ePlayMode = SFSPM_LOOP;
	poSpr->poGetAction("crouch_idle")->m_ePlayMode = SFSPM_LOOP;
	poSpr->poGetAction("idle")->m_ePlayMode = SFSPM_LOOP;
	*/

	CFESpriteInstMgr::Enable(m_hSprInst);
	CFESpriteInstMgr::SetAction(m_hSprInst,PA_IDLE);

	m_oPos = CFEVect2(CFEVect2(0,GROUND_HEIGHT));

	CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
	CFESpriteInstMgr::SetScale(m_hSprInst,PLAYER_SCALE);

	m_oInput.Init(_hHandler);
}
///-----------------------------------------------------------------------------
void CPlayer::UpdateInputs(FEReal _rDeltaT)
{
	m_oInput.Update(_rDeltaT);
}
///-----------------------------------------------------------------------------
void CPlayer::Update(FEReal _rDeltaT)
{
	UpdateInputs(_rDeltaT);
	m_rStateTime -= _rDeltaT;

	switch (m_uiState)
	{
		case PS_NONE:
		ChangeState(PS_IDLE);
		break;
		
		// ------------------------
		// IDLE State
		// ------------------------
		case PS_IDLE:
		{		
			if (m_oInput.bPressed(IB_LEFT))
			{
				ChangeState(PS_WALK);
				m_rDirMult = 1.0;
				CFESpriteInstMgr::SetScale(m_hSprInst,CFEVect2(m_rDirMult*PLAYER_SCALE,PLAYER_SCALE));
			}
	   else if (m_oInput.bPressed(IB_RIGHT))
			{
				ChangeState(PS_WALK);
				m_rDirMult = -1.0;
				CFESpriteInstMgr::SetScale(m_hSprInst,CFEVect2(m_rDirMult*PLAYER_SCALE,PLAYER_SCALE));
			}
		else if (m_oInput.bDown(IB_UP))
			{
				ChangeState(PS_JUMP_UP);
			}
		else if (m_oInput.bPressed(IB_DOWN))
			{
				ChangeState(PS_CROUCHING);
			}
		else if (m_oInput.bDown(IB_A))
			{
				ChangeState(PS_KICK);
			}
		else if (m_oInput.bDown(IB_B))
			{
				ChangeState(PS_PUNCH);
			}
		}
		break;

		// ------------------------
		// WALK STATE
		// ------------------------
		case PS_WALK:
		{
			if (m_oInput.bUp(IB_LEFT))
			{
				ChangeState(PS_IDLE);
			}
		else if (m_oInput.bUp(IB_RIGHT))
			{
				ChangeState(PS_IDLE);
			}
		else if (m_oInput.bDown(IB_A))
			{
				ChangeState(PS_KICK);
			}
		else if (m_oInput.bDown(IB_B))
			{
				ChangeState(PS_PUNCH);
			}
		else
			{
				m_oPos.x += -m_rDirMult*PLAYER_SPEED*_rDeltaT;
				CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
			}
		}
		break;
		
		// ------------------------
		// PUNCH STATE
		// ------------------------
		case PS_PUNCH:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
				ChangeState(PS_IDLE);
		}
		break;

		// ------------------------
		// KICK STATE
		// ------------------------
		case PS_KICK:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
				ChangeState(PS_IDLE);
		}
		break;

		// ------------------------
		// CROUCHING STATE
		// ------------------------
		case PS_CROUCHING:
		{
			// mirar si ha acabado la animación para empezar con el idle.			
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
				ChangeState(PS_CROUCH_IDLE);
		}
		break;

		// ------------------------
		// CROUCHIDLE STATE
		// ------------------------
		case PS_CROUCH_IDLE:
		{
			if (m_oInput.bPressed(IB_LEFT))
			{
				m_rDirMult = 1.0;
				CFESpriteInstMgr::SetScale(m_hSprInst,CFEVect2(m_rDirMult*PLAYER_SCALE,PLAYER_SCALE));
			}
	   else if (m_oInput.bPressed(IB_RIGHT))
			{
				m_rDirMult = -1.0;
				CFESpriteInstMgr::SetScale(m_hSprInst,CFEVect2(m_rDirMult*PLAYER_SCALE,PLAYER_SCALE));
			}		
			if (m_oInput.bReleased(IB_DOWN))
			{
				ChangeState(PS_IDLE);
			}
		else if (m_oInput.bDown(IB_A))
			{
				ChangeState( PS_CROUCH_KICK );
			}
		else if (m_oInput.bDown(IB_B))
			{
				ChangeState(PS_CROUCH_PUNCH);
			}			
		}
		break;

		// ------------------------
		// CROUCH_KICK STATE
		// ------------------------
		case PS_CROUCH_KICK:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
				ChangeState(PS_CROUCH_IDLE);
		}
		break;

		// ------------------------
		// CROUCH_PUNCH STATE
		// ------------------------
		case PS_CROUCH_PUNCH:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
				ChangeState(PS_CROUCH_IDLE);
		}
		break;

		// ------------------------
		// JUMP UP STATE
		// ------------------------
		case PS_JUMP_UP:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst))
			{
				ChangeState(PS_JUMP_DOWN);
			}
			else
			{
				FEReal rT = (1.0 - m_rStateTime);
				m_oPos.y = GROUND_HEIGHT - JUMP_MULTIPLIER * (JUMP_SPEED*rT + 0.5*(-9.8*(rT*rT)));

				CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
			}

			if ( m_oInput.bDown(IB_A) /* && ( m_rStateTime > (JUMP_TIME-JUMP_KICK_ANIM_TIME)) */ )
			{
				ChangeState(PS_JUMP_KICK);
			}
		}
		break;

		// ------------------------
		// JUMP DOWN STATE
		// ------------------------
		case PS_JUMP_DOWN:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst) || (m_rStateTime <= 0.0))
			{
				m_oPos.y = GROUND_HEIGHT;
				CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
				ChangeState(PS_IDLE);
			}
			else
			{
				FEReal rT = (1.0 - m_rStateTime);
				m_oPos.y = GROUND_HEIGHT - JUMP_MULTIPLIER * (JUMP_SPEED*rT + 0.5*(-9.8*(rT*rT)));

				CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
			}

			/*
			if (m_oInput.bDown(IB_A) )
			{
				ChangeState(PS_JUMP_KICK);
			}
			*/
		}
		break;

		// ------------------------
		// JUMP_KICK STATE
		// ------------------------
		case PS_JUMP_KICK:
		{
			if (! CFESpriteInstMgr::bIsPlaying(m_hSprInst)) // (m_rStateTime <= JUMP_UP_TIME)
			{				
				ChangeState(PS_JUMP_DOWN);
			}
			else				
			{
				const FEReal rMultiplier = 50.0;
				FEReal rT = (1.0 - m_rStateTime);
				m_oPos.y = GROUND_HEIGHT - rMultiplier* (JUMP_SPEED*rT + 0.5*(-9.8*(rT*rT)));

				CFESpriteInstMgr::SetPos(m_hSprInst,m_oPos);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CPlayer::OnEnterState(uint _uiState)
{
	if (uiGetState() == _uiState) return;
	
	switch (_uiState)
	{
		case PS_NONE:
		break;
		
		case PS_IDLE:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_IDLE);
		}
		break;

		case PS_WALK:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_WALK);
		}
		break;

		case PS_CROUCHING:
		{
			m_rStateTime = 0.5;
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_CROUCHING);
		}
		break;

		case PS_CROUCH_IDLE:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_CROUCH_IDLE);
		}
		break;

		case PS_JUMP_UP:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_JUMP_UP);
			m_rStateTime = JUMP_TIME;
		}
		break;

		case PS_JUMP_DOWN:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_JUMP_DOWN);
			// m_rStateTime = 0.5;
		}
		break;
	
		case PS_KICK:
		{
			m_rStateTime = 0.5;
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_KICK[CFEMath::iRand(0,MAX_KICKS-1)]);
		}
		break;

		case PS_JUMP_KICK:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_JUMP_KICK[CFEMath::iRand(0,MAX_JUMP_KICKS-1)]);
		}
		break;

		case PS_CROUCH_KICK:
		{
			m_rStateTime = 0.5;
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_CROUCH_KICK[CFEMath::iRand(0,MAX_CROUCH_KICKS-1)]);
		}
		break;
		
		case PS_PUNCH:
		{
			m_rStateTime = 0.5;
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_PUNCH[CFEMath::iRand(0,MAX_PUNCHES-1)]);
		}
		break;

		case PS_JUMP_PUNCH:
		{
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_JUMP_PUNCH[CFEMath::iRand(0,MAX_JUMP_PUNCHES-1)]);
		}
		break;

		case PS_CROUCH_PUNCH:
		{
			m_rStateTime = 0.5;
			CFESpriteInstMgr::SetAction(m_hSprInst,PA_CROUCH_PUNCH[CFEMath::iRand(0,MAX_CROUCH_PUNCHES-1)]);
		}
		break;				
	}
}
//-----------------------------------------------------------------------------
void CPlayer::OnExitState(uint _uiState)
{
}
//-----------------------------------------------------------------------------