//-----------------------------------------------------------------------------
/*! \class CChinoCudeiro
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CChinoCudeiro.h"

#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CGroundMgr.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// BEGIN: Gameplay Vars
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
const FEReal THIS_ACTOR_FSPEED = 175.0f;

// Initial idle time
const FEReal IDLE_TIME		= 2.0f;

// Idle time after attack
const FEReal MINI_IDLE_TIME = 0.5f;

// time reverse walking
const FEReal GO_BACK_TIME = 1.5f;

// pain
const FEReal GO_DOWN_TIME = 0.5f;
const FEReal DOWN_TIME    = 1.0f;
const FEReal GROUND_ATTACK_INTERVAL = 0.5f;	// time between in ground atacks

// Distance to attack the player
const FEReal DIST_THRESHOLD = 100.0f;
const FEReal THIS_ACTOR_SCALE = 1.1f * ACTOR_SCALE;

const FEReal THIS_ACTOR_BSPEED = 100.0f;
const FEReal PLAYER_DISTURBING_DISTANCE = 320.0f;

// Enemy damage
static FEReal ENEMY_DAMAGE = 0.22f;
static FEReal ENEMY_ENERGY;

const FEReal PLAYER_DAMAGE_MULTIPLIER = 0.5f;

// Enemy Dying
const FEReal DYING_TIME = 1.0f;
// -----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// END: Gameplay Vars
//-----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1.0f;
const FEReal RIGHT_DIR_MULT = 1.0f;
//-----------------------------------------------------------------------------
CChinoCudeiro::CChinoCudeiro()
{
	CEntityRegistry::uiAddEntity(this,"chinocudeiro",true);
	goGameGlobals.m_poBoss = this;	
	m_rResistanceFact = _1r;
}
//-----------------------------------------------------------------------------
CChinoCudeiro::~CChinoCudeiro()
{
	CEntityRegistry::DeleteEntity(this);
	// no more boss :_(
    goGameGlobals.m_poBoss = NULL;
}
///-----------------------------------------------------------------------------
void CChinoCudeiro::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;
    
    CAnimHelper::CreateActor("chinocudeiro");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("chinocudeiro");

	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst);
	m_poHandSpr = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(poNode,"rhand");
	if (m_poHandSpr != NULL) m_poHandSpr->SetAction(0);

	Reset();
}
///-----------------------------------------------------------------------------
void CChinoCudeiro::Reset()
{
	SetEnergy(1);
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	Disable();
}
///-----------------------------------------------------------------------------
void CChinoCudeiro::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;

	CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_SIT);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(LEFT_DIR_MULT*THIS_ACTOR_SCALE,THIS_ACTOR_SCALE));
	
	SetEnergy(1);
	Enable();
    
	ChangeState(CCS_INIT);
}
///-----------------------------------------------------------------------------
void CChinoCudeiro::Attack()
{
	CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_ATTACK);
	CSoundHelper::hPlay(SND_CHINOCUDEIRO_ATTACK);
}
///-----------------------------------------------------------------------------
void CChinoCudeiro::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;
	
	m_rStateTime -= _rDeltaT;

	// Set proper height
    CFEVect2 oNewPos( oGetPos().x,CGroundMgr::rGetHeight( oGetPos().x) );
	SetPos( oNewPos );

    // Manage states
	switch ( uiGetState() )
	{
		case CCS_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case CCS_INIT:
		{
			ChangeState(ACTOR_CINEMATIC_STATE);
		}
		break;

		// ------------------------
		// IDLE STATE
		// ------------------------
		case CCS_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(CCS_CHASE);
			}
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case CCS_CHASE:
		{
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < DIST_THRESHOLD)
			{
				//	Change to attack state
				ChangeState(CCS_ATTACK);
			}
			else
			{
				if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				{
					m_oPos.x += THIS_ACTOR_FSPEED*_rDeltaT;
					CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(RIGHT_DIR_MULT*THIS_ACTOR_SCALE,THIS_ACTOR_SCALE));
				}
				else
				{
					m_oPos.x -= THIS_ACTOR_FSPEED*_rDeltaT;
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(LEFT_DIR_MULT*THIS_ACTOR_SCALE,THIS_ACTOR_SCALE));
					CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
				}
			}
		}
		break;
		
		case CCS_ATTACK:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				ChangeState(CCS_LOTTERY);
		}
		break;
		
		// ------------------------
		// MINI IDLE STATE
		// ------------------------
		case CCS_MINI_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(CCS_ATTACK);
			}
		}
		break;
		
		// ------------------------
		// LOTTERY STATE
		// ------------------------
		case CCS_LOTTERY:
		{
			uint uiTicket = CFEMath::uiRand(5);
			if (uiTicket == 1)
				// win lottery!
				ChangeState(CCS_MINI_IDLE);
			else
				// lose :(
				ChangeState(CCS_GOBACK);
		}
		break;
		
		// ------------------------
		// GO BACKWARDS STATE
		// ------------------------
		case CCS_GOBACK:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(CCS_CHASE);
			}
			else
			{
				m_oPos.x -= m_rSpeed*THIS_ACTOR_BSPEED*_rDeltaT;
				CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
				CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rSpeed*THIS_ACTOR_SCALE,THIS_ACTOR_SCALE));
			}
		}
		break;

		// ------------------------
		// PAIN STATE
		// ------------------------
		case CCS_PAIN_0:
		{
			if (m_rStateTime <= _0r)
				ChangeState(CCS_PAIN_1);
		}
		break;

		case CCS_PAIN_1:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(CCS_PAIN_2);
			}
			else
			{
				m_rAttackTime -= _rDeltaT;
				if (m_rAttackTime <= _0r)
				{
					// chino launches attacks.
					CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&ENEMY_DAMAGE);

					//
					CSoundHelper::hPlay(SND_CHINOCUDEIRO_ATTACK);

					// plan next attack
					m_rAttackTime = GROUND_ATTACK_INTERVAL;
				}
			}				
		}
		break;

		case CCS_PAIN_2:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				ChangeState(CCS_CHASE);
		}
		break;

		// ------------------------
		// DYING STATE
		// ------------------------
		case CCS_DYING:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				ChangeState(CCS_DEAD);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CChinoCudeiro::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		case CCS_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;
		
		case CCS_SIT:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_SIT);
		}
		break;

		case CCS_WAKE:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_WAKE);
		}
		break;

		case CCS_IDLE:
		{
		    // show matamoscas
		    if (m_poHandSpr != NULL) m_poHandSpr->SetAction(1);
		    CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_IDLE);
			m_rStateTime = IDLE_TIME;
		}
		break;
		
		case CCS_CHASE:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_WALK);
		}
		break;
		
		case CCS_ATTACK:
		{
			// Send message announcing the defunction of this entity.
			m_oAttack.m_eType = EA_MATAMOSCAZOS;
			m_oAttack.m_rDamage = ENEMY_DAMAGE;
			CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);
			Attack();
		}
		break;
		
		case CCS_MINI_IDLE:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_IDLE);
			m_rStateTime = MINI_IDLE_TIME;
		}
		break;

		case CCS_GOBACK:
		{
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_RWALK);
			m_rStateTime = GO_BACK_TIME;

			if (m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x)
				m_rSpeed = RIGHT_DIR_MULT;
			else
				m_rSpeed = LEFT_DIR_MULT;
		}
		break;

		case CCS_PAIN_0:
		{
			CSoundHelper::hPlay(SND_ENEMY_PAIN,1);

			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_PAIN);
			m_rStateTime = GO_DOWN_TIME;			

			// Send message announcing the boss has been damaged

			ENEMY_ENERGY = rGetEnergy();
			CGameMsgMgr::SendMsg(GMSG_BOSS_DAMAGED,this,(FEPointer)&ENEMY_ENERGY);
		}
		break;

		case CCS_PAIN_1:
		{			
			m_rStateTime = DOWN_TIME;
			m_rAttackTime = 0;
		}
		break;

		case CCS_PAIN_2:
		{
		}
		break;

		case CCS_DYING:
		{
			goGameGlobals.SlowMotion();

			m_rSpeed = 0;
			m_rStateTime = DYING_TIME;
			CSoundHelper::hPlay(SND_ENEMY_DEATH,1);
			CAnimHelper::Play(m_hAnimInst,ACTION_CHINO_CUDEIRO_DEATH);

			// hide matamoscas
			if (m_poHandSpr != NULL) m_poHandSpr->SetAction(0);
		}
		break;

		case CCS_DEAD:
		{	
			// Delete the entity from the enetity manager.
			CEntityMgr::DeleteEntity(this);

			// Disable me.
			Disable();

			CFESkelAnimInstMgr::Disable(m_hAnimInst);

			ENEMY_ENERGY = 0;
			CGameMsgMgr::SendMsg(GMSG_BOSS_DAMAGED,this,(FEPointer)&ENEMY_ENERGY);

			// Send message announcing the defunction of this entity.
			CGameMsgMgr::SendMsg(GMSG_BOSS_DIED,this,0);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CChinoCudeiro::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
void CChinoCudeiro::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if  ((uiGetState() == CCS_DYING) || (bInPain())) return;

		    m_poReceivedAttack = (TEntityAttack*)_pData;
			DecEnergy( m_poReceivedAttack->m_rDamage/m_rResistanceFact);

			if (rGetEnergy() > _0r)
			{
				ChangeState(CCS_PAIN_0);
			}
			else
			{
				ChangeState(CCS_DYING);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
