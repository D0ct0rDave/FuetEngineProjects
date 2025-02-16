//-----------------------------------------------------------------------------
#include "CThrowerEnemy.h"

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
const FEReal THIS_ACTOR_SPEED = 300.0f;
const FEReal THROWN_OBJ_HEIGHT[2] = {-100.0f, -100.0f};
//-----------------------------------------------------------------------------
// Idle time after attack
const FEReal IDLE_TIME		= 2.0f;

// Distance to attack the player
const FEReal DIST_THRESHOLD = 280.0f;

// Enemy damage
static FEReal ENEMY_DAMAGE = 0.1f;

//-----------------------------------------------------------------------------
const FEReal	DYING_TIME = 1.0f;
//-----------------------------------------------------------------------------
CThrowerEnemy::CThrowerEnemy()
{
    CEntityRegistry::uiAddEntity(this,"ThrowerEnemy",true);
    m_rResistanceFact = _05r;
}
//-----------------------------------------------------------------------------
CThrowerEnemy::~CThrowerEnemy()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CThrowerEnemy::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    m_hAnimInst = CAnimHelper::hGetReskinedActorInstance("thrower");
	CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst,1.5f);
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	// Create object to throw
	m_oObj.Init();
	
	Reset();
}
///-----------------------------------------------------------------------------
void CThrowerEnemy::Reset()
{
	m_oPos = CFEVect2::oZERO();
    m_rZ = _1r;

	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);

	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

	SetScale( ACTOR_SCALE * CFEMath::rRand(ENE_SCALE_MIN, ENE_SCALE_MAX) );
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,rGetScale());

	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	SetEnergy(1);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    
	Disable();
	
	// Reset object also
	m_oObj.Reset();
}
///-----------------------------------------------------------------------------
void CThrowerEnemy::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	m_rZ = _1r;

    CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	ChangeState(TES_INIT);
}
///-----------------------------------------------------------------------------
void CThrowerEnemy::Throw()
{
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_THROW_HIGH);
	poGetActorFace()->SetAction(ENEMY_ATTACK_FACE);

	// Throw object.
	CFEVect2 oPos = m_oPos;
	oPos.y += THROWN_OBJ_HEIGHT[ CFEMath::uiRand(1) ]*rGetScale();
	m_oObj.Spawn(oPos,m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x);
}
///-----------------------------------------------------------------------------
void CThrowerEnemy::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;

	m_rStateTime -= _rDeltaT;

	// Set proper height
	if (uiGetState() != TES_DIE)
        m_oPos.y = CGroundMgr::rGetHeight( m_oPos.x );

    // Manage states
    switch (uiGetState())
	{
		case TES_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case TES_INIT:
		{
			ChangeState(TES_APPROACH);
		}
		break;

		// ------------------------
		// APPROACH STATE
		// ------------------------
		case TES_APPROACH:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < DIST_THRESHOLD)
			{
				//	Change to idle state
				ChangeState(TES_IDLE);
			}
			else
			{
				if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				{
				    m_rDirMult = RIGHT_DIR_MULT;   
					m_oPos.x += THIS_ACTOR_SPEED*_rDeltaT;
				}
				else
				{
				    m_rDirMult = LEFT_DIR_MULT;
					m_oPos.x -= THIS_ACTOR_SPEED*_rDeltaT;
				}
			}
		}
		break;
		
		// ------------------------
		// IDLE STATE
		// ------------------------
		case TES_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(TES_THROW);
			}
		}
		break;

		// ------------------------
		// THROW STATE
		// ------------------------
		case TES_THROW:
		{
			// Lan
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
			{
				if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < DIST_THRESHOLD)
					ChangeState(TES_ESCAPE);
				else
					ChangeState(TES_APPROACH);
			}
		}
		break;
		
		// ------------------------
		// THROW STATE
		// ------------------------
		case TES_ESCAPE:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) >= DIST_THRESHOLD)
			{
				//	Change to idle state
				m_rDirMult *= -_1r;
				ChangeState(TES_IDLE);
			}
			else
			{
				if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				{
					m_oPos.x -= THIS_ACTOR_SPEED*_rDeltaT;
					m_rDirMult = LEFT_DIR_MULT;
				}
				else
				{
					m_oPos.x  += THIS_ACTOR_SPEED*_rDeltaT;
					m_rDirMult = RIGHT_DIR_MULT;
				}
			}
		}
		break;
		
		// ------------------------
		// DIE STATE
		// ------------------------
		case TES_DIE:
		{
			if (! CGameHelper::bVisibleOnScreen(m_oPos,100))
			{
				ChangeState(TES_DEAD);
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
		case TES_EXIT:
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
void CThrowerEnemy::OnEnterState(uint _uiState)
{
	if (uiGetState() == _uiState) return;
	
	switch (_uiState)
	{
		case TES_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;

		case TES_IDLE:
		{
			// Face the player
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);
			m_rStateTime = IDLE_TIME;
		}
		break;

		case TES_APPROACH:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
		}
		break;

		case TES_THROW:
		{
			// Throw object ...
			Throw();
		}
		break;

		case TES_ESCAPE:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
		}
		break;

		case TES_DIE:
		{
		    CEnemyHelper::GenericDie(this,m_poReceivedAttack);
		}
		break;

		case TES_DEAD:
		{
			// Delete the entity from the enetity manager.
			CEntityMgr::DeleteEntity(this);

			// Disable me.
			Disable();

			CFESkelAnimInstMgr::Disable(m_hAnimInst);

			// Send message announcing the defunction of this entity.
			CGameMsgMgr::SendMsg(GMSG_ENEMY_DIED,this,0);
		}
		break;

		case TES_EXIT:
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
void CThrowerEnemy::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
void CThrowerEnemy::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
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
			DecEnergy( m_poReceivedAttack->m_rDamage/m_rResistanceFact);

			if (rGetEnergy() <= _0r)
			{
				if (uiGetState() != TES_DIE)
					ChangeState(TES_DIE);
			}
			else
				CSoundHelper::hPlay(SND_ENEMY_PAIN,1);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
