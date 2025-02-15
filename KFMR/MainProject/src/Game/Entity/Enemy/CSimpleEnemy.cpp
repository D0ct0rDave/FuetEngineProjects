//-----------------------------------------------------------------------------
#include "CSimpleEnemy.h"

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

#include <FuetEngine.h>
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1.0f;
const FEReal RIGHT_DIR_MULT = 1.0f;
//-----------------------------------------------------------------------------
// Idle time after attack
const FEReal IDLE_TIME		= 0.5f;

// Distance to attack the player
const FEReal DIST_THRESHOLD = 50;

const FEReal THIS_ACTOR_SPEED = 220.0f;

// Enemy damage
static FEReal ENEMY_DAMAGE = 0.05f;

// Timers Min-Max for the delay on attack
const FEReal SES_DELAY_ATTACK_MIN = 0;
const FEReal SES_DELAY_ATTACK_MAX = 0.75f;
//-----------------------------------------------------------------------------
const FEReal	DYING_TIME = 1.0f;
//-----------------------------------------------------------------------------
CSimpleEnemy::CSimpleEnemy()
{
    CEntityRegistry::uiAddEntity(this,"SimpleEnemy",true);
    m_rResistanceFact = _1r;
}
//-----------------------------------------------------------------------------
CSimpleEnemy::~CSimpleEnemy()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    m_hAnimInst = CAnimHelper::hGetReskinedActorInstance("simpleenemy");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	Reset();
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::Reset()
{
	m_oPos = CFEVect2::ZERO();
    m_rZ = _1r;

	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);

	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    SetScale( ACTOR_SCALE * CFEMath::rRand(ENE_SCALE_MIN, ENE_SCALE_MAX) );

	CFESkelAnimInstMgr::SetScale(m_hAnimInst,rGetScale());
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	SetEnergy(1);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    
	Disable();
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	m_rZ = _1r;

    CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	ChangeState(SES_INIT);
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::DoAttack(FEReal _rDeltaT)
{
	if(m_rDelayAttack == -1)
	{
		m_rDelayAttack = 0;
		m_rDelayAttackRange = CFEMath::rRand(SES_DELAY_ATTACK_MIN, SES_DELAY_ATTACK_MAX);		// Set the delay for the attack
	}
	else				// Wait for attack
	{
		// If player is "painning" or jumping don't attack
		if(    
		        (goGameGlobals.m_poPlayer->uiGetState() != PS_PAIN)
		     && (goGameGlobals.m_poPlayer->uiGetState() != PS_DYING)
		     && (goGameGlobals.m_poPlayer->uiGetState() != PS_DEAD)
		     && (!goGameGlobals.m_poPlayer->bJumping())
		     )
		{
			m_rDelayAttack  += _rDeltaT;
			if(m_rDelayAttack > m_rDelayAttackRange)
			{
				//	Change to attack state
				ChangeState(SES_ATTACK);
			}
		}
	}
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::Attack()
{
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_PUNCH);
	poGetActorFace()->SetAction(ENEMY_ATTACK_FACE);
	m_rDelayAttack = -1;
}
///-----------------------------------------------------------------------------
void CSimpleEnemy::Update(FEReal _rDeltaT)
{
	m_rStateTime -= _rDeltaT;

	// Set proper height
	if (uiGetState() != SES_DIE)
        m_oPos.y = CGroundMgr::rGetHeight( m_oPos.x );

    // Manage states
	switch ( uiGetState() )
	{
		case SES_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case SES_INIT:
		{
			ChangeState(SES_CHASE);
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case SES_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
				if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) > DIST_THRESHOLD)
					ChangeState(SES_CHASE);
				else
					DoAttack(_rDeltaT);
			}
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case SES_CHASE:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < DIST_THRESHOLD)
			{
				if(m_rDelayAttack == -1)
				{
					CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);			// Play idle anim
				}
				DoAttack(_rDeltaT);
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
		
		case SES_ATTACK:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				ChangeState(SES_IDLE);
		}
		break;
		
		// ------------------------
		// CHASE STATE
		// ------------------------
		case SES_DIE:
		{
			if (! CGameHelper::bVisibleOnScreen(m_oPos,100))
			{
				ChangeState(SES_DEAD);
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
		case SES_EXIT:
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
void CSimpleEnemy::OnEnterState(uint _uiState)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;

	if (uiGetState() == _uiState) return;

	switch (_uiState)
	{
		case SES_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;

		case SES_IDLE:
		{
			m_rDelayAttack = -1;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);
			m_rStateTime = IDLE_TIME;
		}
		break;

		case SES_CHASE:
		{
			m_rDelayAttack = -1;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
		}
		break;

		case SES_DIE:
		{
            CEnemyHelper::GenericDie(this,m_poReceivedAttack);
		}
		break;

		case SES_DEAD:
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

		case SES_ATTACK:
		{
			// Send message announcing the defunction of this entity.
			m_oAttack.m_eType = (rGetDir() <_0r)?EA_LEFT_PUNCH:EA_RIGHT_PUNCH;
			m_oAttack.m_rDamage = ENEMY_DAMAGE;
			CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);
			Attack();
		}
		break;

		case SES_EXIT:
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
void CSimpleEnemy::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case SES_ATTACK:
		{
		    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
		}
		break;
    }
}
//-----------------------------------------------------------------------------
void CSimpleEnemy::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if (uiGetState() != SES_DIE)
			{
			    m_poReceivedAttack = (TEntityAttack*)_pData;
				ChangeState(SES_DIE);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
