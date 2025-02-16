//-----------------------------------------------------------------------------
#include "CEnanoSaltarin.h"

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
#include "Game/Helpers/CCameraMgr.h"

#include <FuetEngine.h>
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT			= -1.0f;
const FEReal RIGHT_DIR_MULT			= 1.0f;

const FEReal THIS_ACTOR_SPEED		= 300.0f;
const FEReal ENEMY_GROUND_HEIGHT    = 370.0f;

const FEReal START_ATTACK_THRESHOLD = 300;
const FEReal STOP_ATTACK_THRESHOLD  = START_ATTACK_THRESHOLD + 10;
const FEReal LOOKING_TIMER			= 1.5f;
//-----------------------------------------------------------------------------
const FEReal JUMP_X_OFFSET	= 50;
const FEReal JUMP_Y_OFFSET	= 150;
const FEReal JUMP_TIME		= 0.5f;
const FEReal JUMP_YSPEED	= 500.0f;
const FEReal JUMP_ACCEL		= 100.0f;

// Enemy damage
static FEReal ENEMY_DAMAGE		= 0.05f;
const FEReal ATTACK_THRESHOLD	= 50;
//-----------------------------------------------------------------------------
const FEReal	DYING_TIME = 1.0f;

//-----------------------------------------------------------------------------
CEnanoSaltarin::CEnanoSaltarin()
{
    CEntityRegistry::uiAddEntity(this,"EnanoSaltarin",true);
}
//-----------------------------------------------------------------------------
CEnanoSaltarin::~CEnanoSaltarin()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CEnanoSaltarin::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    CAnimHelper::CreateActor("enano_saltarin");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("enano_saltarin");
    
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
	Reset();
}
///-----------------------------------------------------------------------------
void CEnanoSaltarin::Reset()
{
	m_rInitialSpeed = 0;
	m_rSpeed = 0;

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
}
///-----------------------------------------------------------------------------
void CEnanoSaltarin::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	m_oPos.y = ENEMY_GROUND_HEIGHT;
	m_rZ = _1r;

    CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	ChangeState(ENS_INIT);
}
///-----------------------------------------------------------------------------
void CEnanoSaltarin::UpdatePhysics(FEReal _rDeltaT)
{
    FEReal rOldSpeed = m_rSpeed;
    m_rSpeed += (_rDeltaT*9.8f*m_rAccelMultiplier);

    if ((m_rSpeed*rOldSpeed) < 0)
    {
		// Go Down!
		CAnimHelper::Play(m_hAnimInst, ACTION_ACTOR_CROUCH_IDLE);	// ToDo: Animación del tío dando vueltas (como Sonic)
    }

    m_oPos.y += m_rSpeed*_rDeltaT;

    // llegamos al suelo?
    if ((m_rSpeed >= _0r) && (m_oPos.y >= m_rDestinationY))
    {                
		if(m_bStopAttacking)
		{
			ChangeState(ENS_APPROACHING);
		}
		else
		{
			m_bBounce = true;
			m_bAttackDone = false;
			m_rSpeed = m_rInitialSpeed;

			// Go up!
			CAnimHelper::Play(m_hAnimInst, ACTION_ACTOR_CROUCH_IDLE);	// ToDo: Animación del tío dando vueltas (como Sonic)
		}
        m_oPos.y = m_rDestinationY;
    }
    else
	{
        m_bBounce = false;
	}
}
///-----------------------------------------------------------------------------
void CEnanoSaltarin::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;
	
	m_rStateTime -= _rDeltaT;

    // Manage states
	switch ( uiGetState() )
	{
		case ENS_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case ENS_INIT:
		{
			ChangeState(ENS_APPROACHING);
		}
		break;

		// ------------------------
		// APPROACHING STATE
		// ------------------------
		case ENS_APPROACHING:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < START_ATTACK_THRESHOLD)
			{
				ChangeState(ENS_LOOKING);
			}
			else
			{
				if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				{
					m_rDirMult = RIGHT_DIR_MULT;   
				}
				else
				{
					m_rDirMult = LEFT_DIR_MULT;
				}

				m_oPos.x += THIS_ACTOR_SPEED*m_rDirMult*_rDeltaT;
			}
		}
		break;

		// ------------------------
		// LOOKING STATE
		// ------------------------
		case ENS_LOOKING:
		{
			if(m_rStateTime < 0 || CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < (START_ATTACK_THRESHOLD / 2.0f))
			{
				ChangeState(ENS_ATTACKING);
			}
		}
		break;

		// ------------------------
		// ATTACKING STATE
		// ------------------------
		case ENS_ATTACKING:
		{
			// Movement
			m_oPos.x += THIS_ACTOR_SPEED*m_rDirMult*_rDeltaT;
			UpdatePhysics(_rDeltaT);

			// Come back to look the player
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) > START_ATTACK_THRESHOLD)
			{
				m_bStopAttacking = true;
			}		

			// Do damage
			if ((CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < ATTACK_THRESHOLD)
				&& (goGameGlobals.m_poPlayer->uiGetState() != PS_PAIN)
				&& (goGameGlobals.m_poPlayer->uiGetState() != PS_DYING)
				&& (goGameGlobals.m_poPlayer->uiGetState() != PS_DEAD)
				&& !m_bAttackDone)
			{
				m_bAttackDone = true;
				m_oAttack.m_eType = (rGetDir() <_0r)?EA_LEFT_PUNCH:EA_RIGHT_PUNCH;
				m_oAttack.m_rDamage = ENEMY_DAMAGE;
				CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);
			}
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case ENS_DIE:
		{
			if (! CGameHelper::bVisibleOnScreen(m_oPos,100))
			{
				ChangeState(ENS_DEAD);
			}
			else
			{
			    CEnemyHelper::GenericDieUpdate(_rDeltaT,this);
			}
		}
		break;
	}

    CFEVect2 oScale = CFEVect2( m_rDirMult * m_rScale * m_rZ, m_rScale * m_rZ);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,oScale);

	SetPos(m_oPos);	
}
//-----------------------------------------------------------------------------
void CEnanoSaltarin::OnEnterState(uint _uiState)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;

	if (uiGetState() == _uiState) return;

	switch (_uiState)
	{
		case ENS_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;
		
		case ENS_APPROACHING:
		{
			CAnimHelper::Play(m_hAnimInst, ACTION_ACTOR_WALK);	// ToDo: Ponerle un run
		}
		break;

		case ENS_LOOKING:
		{
			m_rStateTime = LOOKING_TIMER;
			CAnimHelper::Play(m_hAnimInst, ACTION_ACTOR_IDLE);
		}
		break;

		case ENS_ATTACKING:
		{
			m_bStopAttacking = false;
			m_bAttackDone = false;
			CAnimHelper::Play(m_hAnimInst, ACTION_ACTOR_CROUCH_IDLE);	// ToDo: Animación del tío dando vueltas (como Sonic)

			// Juuump!
			m_rInitialX     = m_oPos.x;
			m_rInitialY     = m_oPos.y;
			m_rDestinationX = m_oPos.x + JUMP_X_OFFSET;
			m_rDestinationY = m_oPos.y;
			m_rStateTime    =  JUMP_TIME;
			m_rSpeed        = -JUMP_YSPEED;
			m_rAccelMultiplier = JUMP_ACCEL;
			m_rInitialSpeed = m_rSpeed;

			// Look direction
			if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
			{
				m_rDirMult = RIGHT_DIR_MULT;   
			}
			else
			{
				m_rDirMult = LEFT_DIR_MULT;
			}
		}
		break;

		case ENS_DIE:
		{
            CEnemyHelper::GenericDie(this,m_poReceivedAttack);
		}
		break;

		case ENS_DEAD:
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
	}
}
//-----------------------------------------------------------------------------
void CEnanoSaltarin::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
    }
}
//-----------------------------------------------------------------------------
void CEnanoSaltarin::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if (uiGetState() != ENS_DIE)
			{
			    m_poReceivedAttack = (TEntityAttack*)_pData;
				ChangeState(ENS_DIE);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
