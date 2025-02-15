//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"

#include "CDiscChinoCudeiro.h"

#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CGroundMgr.h"
#include "Game/Helpers/CGameHelper.h"
#include "Game/Helpers/CEnemyHelper.h"
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1.0f;
const FEReal RIGHT_DIR_MULT = 1.0f;
//-----------------------------------------------------------------------------
// Idle time after attack
const FEReal IDLE_TIME		= 0.5f;

// Distance to attack the player
const FEReal DIST_THRESHOLD = 80;

const FEReal THIS_ACTOR_SPEED = 300.0f;	// 220

// Timer to attack for down or jump
const FEReal DCCS_PREATTACK_TIMER = 0.75f;

const FEReal JUMP_SPEED			= 75.0;
const FEReal JUMP_INIT_SPEED	= JUMP_SPEED * 5;

// Enemy damage
static FEReal ENEMY_DAMAGE = 0.05f;

// Timers Min-Max for the delay on attack
const FEReal DCCS_DELAY_ATTACK_MIN = 0;
const FEReal DCCS_DELAY_ATTACK_MAX = 0.75f;
const FEReal THIS_ENEMY_SCALE_MIN = 1.0; // 0.6f;
const FEReal THIS_ENEMY_SCALE_MAX = 1.0; // 0.7f;
//-----------------------------------------------------------------------------
const FEReal	DYING_TIME = 1.0f;
//-----------------------------------------------------------------------------
CDiscChinoCudeiro::CDiscChinoCudeiro()
{
    CEntityRegistry::uiAddEntity(this,"disc_chinocudeiro",true);
    m_rResistanceFact = _1r;
}
//-----------------------------------------------------------------------------
CDiscChinoCudeiro::~CDiscChinoCudeiro()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    CAnimHelper::CreateActor("dis_chinocudeiro");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("dis_chinocudeiro");
    
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
	Reset();
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::Reset()
{
	m_oPos = CFEVect2::ZERO();
    m_rZ = _1r;
	m_bAttackDown = true;
	m_rJumpHeight = 0;

	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

	SetScale( ACTOR_SCALE * CFEMath::rRand(THIS_ENEMY_SCALE_MIN, THIS_ENEMY_SCALE_MAX) );
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,rGetScale());
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	SetEnergy(1);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    
	Disable();
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	m_rZ = _1r;

    CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	ChangeState(DCCS_INIT);
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::Attack()
{
	m_rDelayAttack = -1;
	if(m_bAttackDown)
	{
		CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_PUNCH);
	}
	else
	{
		CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_PUNCH);
	}
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::DoAttack(FEReal _rDeltaT)
{
	if(m_rDelayAttack == -1)
	{
		m_rDelayAttack = 0;
	}
	else
	{		
		//	Change to attack state
		if(m_bAttackDown)
		{
			ChangeState(DCCS_ATTACK_DOWN);
		}
		else
		{
			if(m_bJumpWithAttack)
			{
				ChangeState(DCCS_ATTACK_UP);
			}
			else
			{
				ChangeState(DCCS_FALLING);
			}

			m_rJumpSpeed = JUMP_INIT_SPEED;
			m_rJumpHeight = 0;
			m_bGoingUp = true;
		}
	}
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::PlayerAttacks()
{
	// Attack if the player is not jumping
	if(!goGameGlobals.m_poPlayer->bJumping())
	{
		m_rStateTime = 0;		// Attack inmediatly
		m_rDelayAttack = 0;
		m_bAttackDown = false;	// The enemy will attack for up side

		if(goGameGlobals.m_poPlayer->bCrouch())
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_UP);
			m_bJumpWithAttack = false;
		}
		else
		{
			m_bJumpWithAttack = true;
		}
		
		DoAttack(0);
	}
}
///-----------------------------------------------------------------------------
void CDiscChinoCudeiro::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;

	m_rStateTime -= _rDeltaT;

	// Set proper height
	if (uiGetState() != DCCS_DIE)
        m_oPos.y = m_rJumpHeight + CGroundMgr::rGetHeight( m_oPos.x );

	// Jump & Fall
	if((m_rJumpHeight < 0) || (m_rJumpSpeed > 0))
	{
		m_rJumpSpeed += (-9.8f)*_rDeltaT * JUMP_SPEED;
		m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);
	}

    // Manage states
	switch ( uiGetState() )
	{
		case DCCS_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case DCCS_INIT:
		{
			ChangeState(DCCS_CHASE);
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case DCCS_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
				if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) > DIST_THRESHOLD)
					ChangeState(DCCS_CHASE);
				else
					ChangeState(DCCS_PREATTACK_DOWN);
			}
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case DCCS_CHASE:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < DIST_THRESHOLD)
			{
				if(m_rDelayAttack == -1)
				{
					CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);			// Play idle anim
				}
				ChangeState(DCCS_PREATTACK_DOWN);
				//DoAttack(_rDeltaT);
			}
			else
			{
				if(m_rDelayAttack != -1)
				{
					CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
					m_rDelayAttack = -1;
				}
				if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				{
					m_oPos.x += THIS_ACTOR_SPEED*_rDeltaT;
					m_rDirMult = RIGHT_DIR_MULT;
				}
				else
				{
					m_oPos.x -= THIS_ACTOR_SPEED*_rDeltaT;
					m_rDirMult = LEFT_DIR_MULT;
				}
			}
		}
		break;
		
		case DCCS_PREATTACK_DOWN:
		{
			if(m_rStateTime <= 0)
			{
				DoAttack(_rDeltaT);
			}
		}
		break;
		
		case DCCS_ATTACK_DOWN:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
			{
				ChangeState(DCCS_IDLE);
				m_bAttackDown = true;
			}
		}
		break;
		
		case DCCS_ATTACK_UP:
		{
			// Starts the fall
			if(m_rJumpSpeed < 0 && m_bGoingUp)
			{
				m_bGoingUp = false;

				// Send message announcing the defunction of this entity.
				m_oAttack.m_rDamage = ENEMY_DAMAGE;
				m_oAttack.m_eType   = EA_MATAMOSCAZOS;
				CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&ENEMY_DAMAGE);
				Attack();
			}
			if(!m_bGoingUp)
			{
				if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				{
					ChangeState(DCCS_IDLE);
					m_bAttackDown = true;
				}
			}
		}
		break;

		case DCCS_FALLING:
		{
			if(m_rJumpSpeed < 0 && m_bGoingUp)
			{
				m_bGoingUp = false;
				CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_DOWN);
			}
			if(m_rJumpHeight > 0)
			{
				m_rJumpHeight = 0;
				m_bJumpWithAttack = true;
				m_bAttackDown = true;
				ChangeState(DCCS_IDLE);
			}
		}
		break;
		
		// ------------------------
		// DIE STATE
		// ------------------------
		case DCCS_DIE:
		{
			if (! CGameHelper::bVisibleOnScreen(m_oPos,100))
			{
				ChangeState(DCCS_DEAD);
			}
			else
			{
			    CEnemyHelper::GenericDieUpdate(_rDeltaT,this);
			}
		}
		break;
		
		// ------------------------
		// CHASE EXIT
		// ------------------------
		case DCCS_EXIT:
		{
			if(m_bMoveLeft)
			{
				m_oPos.x -= THIS_ACTOR_SPEED*_rDeltaT;
				m_rDirMult = LEFT_DIR_MULT;
			}
			else
			{
				m_oPos.x += THIS_ACTOR_SPEED*_rDeltaT;
				m_rDirMult = RIGHT_DIR_MULT;
			}
		}
		break;
	}

    CFEVect2 oScale = CFEVect2( m_rDirMult * m_rScale * m_rZ, m_rScale * m_rZ);

	CFESkelAnimInstMgr::SetScale(m_hAnimInst,oScale);
	SetPos(m_oPos);	
}
//-----------------------------------------------------------------------------
void CDiscChinoCudeiro::OnEnterState(uint _uiState)
{
	if (uiGetState() == _uiState) return;

	switch (_uiState)
	{
		case DCCS_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;

		case DCCS_IDLE:
		{
			m_rDelayAttack = -1;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);
			m_rStateTime = IDLE_TIME;
		}
		break;

		case DCCS_CHASE:
		{
			m_rDelayAttack = -1;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
		}
		break;

		case DCCS_DIE:
		{
		    CEnemyHelper::GenericDie(this,m_poReceivedAttack);
		}
		break;

		case DCCS_DEAD:
		{
			// Delete the entity from the enetity manager.
			CEntityMgr::DeleteEntity(this);

			// Disable me.
			Disable();
			
			// Disable the animation
			CFESkelAnimInstMgr::Disable(m_hAnimInst);

			// Send message announcing the defunction of this entity.
			CGameMsgMgr::SendMsg(GMSG_ENEMY_DIED,this,0);
		}
		break;

		case DCCS_PREATTACK_DOWN:
		{
			m_rStateTime = DCCS_PREATTACK_TIMER;
		}
		break;

		case DCCS_ATTACK_UP:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_UP);
		}
		break;

		case DCCS_ATTACK_DOWN:
		{
			// Send message announcing the defunction of this entity.
			m_oAttack.m_eType   = EA_MATAMOSCAZOS; // (rGetDir() <_0r)?EA_MATAMOSCAZOS:EA_RIGHT_PUNCH;
			m_oAttack.m_rDamage = ENEMY_DAMAGE;
			CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);
			Attack();
		}
		break;

		case DCCS_FALLING:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_DOWN);
		}
		break;

		case DCCS_EXIT:
		{
			// Play walk anim
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);

			// Set direction
			if(goGameGlobals.m_poPlayer->oGetPos().x > m_oPos.x)
			{
				m_bMoveLeft = true;
			}
			else
			{
				m_bMoveLeft = false;
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CDiscChinoCudeiro::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
void CDiscChinoCudeiro::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
		    m_poReceivedAttack = (TEntityAttack*)_pData;

			if (uiGetState() != DCCS_DIE)
			{
				ChangeState(DCCS_DIE);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
