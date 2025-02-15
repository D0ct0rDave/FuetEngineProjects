//-----------------------------------------------------------------------------
#include "CSmallDopefish.h"

#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CGroundMgr.h"

#include <FuetEngine.h>
//-----------------------------------------------------------------------------
const  FEReal LEFT_DIR_MULT  = -1.0f;
const  FEReal RIGHT_DIR_MULT = 1.0f;

const  FEReal THIS_ACTOR_SPEED	= 220.0f;
const  FEReal SD_SPEED_INCR		= 200.0f;
const  FEReal SD_SPEED_DECR		= SD_SPEED_INCR * 3;

static FEReal ENEMY_DAMAGE		= 0.05f;
const  FEReal DYING_TIME		= 1.0f;
const  int	  HEIGHT_OFFSET		= -50;

const FEReal  SD_JUMP_SPEED		= 100.0;
const FEReal  SD_JUMP_INIT_SPEED= SD_JUMP_SPEED * 5;
const FEReal  SD_JUMP_SUMERGIR  = 40;

const  FEReal SD_DIST_THRESHOLD = 150;	// Distance to attack the player
const  FEReal DIST_CHASE_AGAIN	= 250;

const	FEReal SD_RANDOM_FACT_MIN = 0.4f;
const	FEReal SD_RANDOM_FACT_MAX = 1.3f;
//-----------------------------------------------------------------------------
// Small dopefish animations
//-----------------------------------------------------------------------------
const uint SDF_ANIM_DEATH   = 0;
const uint SDF_ANIM_IDLE    = 1;
const uint SDF_ANIM_ATTACK  = 2;
//-----------------------------------------------------------------------------
CSmallDopefish::CSmallDopefish()
{
    CEntityRegistry::uiAddEntity(this,"ThrowerEnemy",true);
    m_rResistanceFact = _1r;
}
//-----------------------------------------------------------------------------
CSmallDopefish::~CSmallDopefish()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CSmallDopefish::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    m_hAnimInst = CFESpriteInstMgr::hGetInstance("data/actors/small_dopefish/small_dopefish");
	Reset();
}
///-----------------------------------------------------------------------------
void CSmallDopefish::Reset()
{
	m_oPos = CFEVect2(CFEVect2(0,0));

	m_rJumpHeight = 0;
	m_rSpeed = 0;
	m_rRandomFactor = CFEMath::rRand(SD_RANDOM_FACT_MIN, SD_RANDOM_FACT_MAX); 
	m_bAttackDone = false;

	m_rScale = ACTOR_SCALE * CFEMath::rRand(ENE_SCALE_MIN, ENE_SCALE_MAX);
    
    CFESpriteInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESpriteInstMgr::SetScale(m_hAnimInst,m_rScale);
	CFESpriteInstMgr::Disable(m_hAnimInst);
	SetEnergy(1);
	
	Disable();
}
///-----------------------------------------------------------------------------
void CSmallDopefish::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;

	CFESpriteInstMgr::SetAction(m_hAnimInst,SDF_ANIM_IDLE);
	CFESpriteInstMgr::Enable(m_hAnimInst);
	CFESpriteInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESpriteInstMgr::SetDepth(m_hAnimInst,0.5);

	ChangeState(SDS_INIT);
}
///-----------------------------------------------------------------------------
void CSmallDopefish::Attack()
{
	CFESpriteInstMgr::SetAction(m_hAnimInst,SDF_ANIM_ATTACK);
}
///-----------------------------------------------------------------------------
void CSmallDopefish::Update(FEReal _rDeltaT)
{
	m_rStateTime -= _rDeltaT;


	// Falling
	if(uiGetState() != SDS_ATTACK)
	{
		if(m_rJumpHeight < 0)
		{
			m_rJumpSpeed += (-9.8f)*_rDeltaT * SD_JUMP_SPEED;
			m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);
		}
		else if(m_rJumpHeight > 0)
		{
			m_rJumpSpeed -= (-9.8f)*_rDeltaT * (SD_JUMP_SPEED / 2);
			m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);

			if(m_rJumpHeight < 0) m_rJumpHeight = 0;
		}
	}

    // Manage states	
	switch ( uiGetState() )
	{
		case SDS_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case SDS_INIT:
		{
			ChangeState(SDS_CHASE);
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case SDS_CHASE:
		{
			Move(_rDeltaT);
			if (CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < SD_DIST_THRESHOLD * m_rRandomFactor)
			{
				ChangeState(SDS_ATTACK);
			}
		}
		break;
		
		// ------------------------
		// ATTACK STATE
		// ------------------------
		case SDS_ATTACK:
		{
			Move(_rDeltaT, true);

			FEReal rLastSpeed = m_rJumpSpeed;
			m_rJumpSpeed += (-9.8f)*_rDeltaT * SD_JUMP_SPEED;
			m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);

			// Attack!
			if(!m_bAttackDone && CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) < SD_DIST_THRESHOLD / 2.0f)
			{
				m_bAttackDone = true;
				m_oAttack.m_eType = (rGetDir() <_0r)?EA_LEFT_PUNCH:EA_RIGHT_PUNCH;
				m_oAttack.m_rDamage = ENEMY_DAMAGE;
				CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack );
			}

			// On the ground
			if(m_rJumpHeight > SD_JUMP_SUMERGIR * m_rRandomFactor ) 
			{
				m_rJumpSpeed = 0;
				m_bAttackDone = false;
				ChangeState(SDS_REPOS);
			}
		}
		break;
		
		// ------------------------
		// REPOSITION STATE
		// ------------------------
		case SDS_REPOS:
		{
			Move(_rDeltaT, true);
		}
		break;
		
		// ------------------------
		// CHASE STATE
		// ------------------------
		case SDS_DIE:
		{
			if (m_rStateTime <= _0r)
			{
				ChangeState(SDS_DEAD);
			}
			else
			{
				m_rSpeed += (-9.8f)*_rDeltaT * 100.0f;
				m_oPos.y -= (m_rSpeed*_rDeltaT);
			}
		}
		
		// ------------------------
		// CHASE EXIT
		// ------------------------
		case SDS_EXIT:
		{
			if(m_bMoveLeft)
			{
				m_oPos.x -= THIS_ACTOR_SPEED*_rDeltaT;
				CFESpriteInstMgr::SetScale(m_hAnimInst,CFEVect2(LEFT_DIR_MULT*m_rScale,m_rScale));
			}
			else
			{
				m_oPos.x += THIS_ACTOR_SPEED*_rDeltaT;
				CFESpriteInstMgr::SetScale(m_hAnimInst,CFEVect2(RIGHT_DIR_MULT*m_rScale,m_rScale));
			}			
		}
		break;
	}
	
	// Set proper height
    CFEVect2 oNewPos( oGetPos().x, m_oPos.y + m_rJumpHeight - HEIGHT_OFFSET/* + CGroundMgr::rGetHeight(oGetPos().x) - HEIGHT_OFFSET*/ );

	CFESpriteInstMgr::SetPos(m_hAnimInst,oNewPos);
}
//-----------------------------------------------------------------------------
void CSmallDopefish::OnEnterState(uint _uiState)
{
	if (uiGetState() == _uiState) return;

	switch (_uiState)
	{
		case SDS_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;

		case SDS_CHASE:
		case SDS_REPOS:
		{
		    CFESpriteInstMgr::SetAction(m_hAnimInst,SDF_ANIM_IDLE);
		}
		break;

		case SDS_DIE:
		{
			m_rSpeed = 0;
			m_rStateTime = DYING_TIME;
   			CSoundHelper::hPlay(SND_ENEMY_DEATH,1);
   			CFESpriteInstMgr::SetAction(m_hAnimInst,SDF_ANIM_DEATH);
		}
		break;

		case SDS_DEAD:
		{
			// Delete the entity from the enetity manager.
			CEntityMgr::DeleteEntity(this);

			// Disable me.
			Disable();
			
			// Disable the animation
			CFESpriteInstMgr::Disable(m_hAnimInst);

			// Send message announcing the defunction of this entity.
			CGameMsgMgr::SendMsg(GMSG_ENEMY_DIED,this,0);
		}
		break;

		case SDS_ATTACK:
		{
			Attack();

			m_rJumpSpeed = SD_JUMP_INIT_SPEED * m_rRandomFactor;
			m_rJumpHeight = 0;
		}
		break;

		case SDS_EXIT:
		{
			// Play walk anim
			// CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);

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
void CSmallDopefish::OnExitState(uint _uiState)
{
}
//-----------------------------------------------------------------------------
void CSmallDopefish::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if (uiGetState() != SDS_DIE)
			{
				ChangeState(SDS_DIE);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CSmallDopefish::Move(FEReal _rDeltaT, bool _bRepos)
{
	if(m_rSpeed < THIS_ACTOR_SPEED * m_rRandomFactor)
	{
		m_rSpeed += _rDeltaT * SD_SPEED_INCR * m_rRandomFactor;
		if(m_rSpeed > THIS_ACTOR_SPEED * m_rRandomFactor) m_rSpeed = THIS_ACTOR_SPEED * m_rRandomFactor;
	}

	if ((_bRepos && !m_bGoingLeft) || (!_bRepos && m_oPos.x < goGameGlobals.m_poPlayer->oGetPos().x))
	{
		m_oPos.x += m_rSpeed*_rDeltaT;
        
        CFESpriteInstMgr::SetPos(m_hAnimInst,m_oPos);
        CFESpriteInstMgr::SetScale(m_hAnimInst,CFEVect2(RIGHT_DIR_MULT*m_rScale,m_rScale));	
			
		if(!_bRepos) m_bGoingLeft = false;
	}
	else if((_bRepos && m_bGoingLeft) || !_bRepos)
	{
		m_oPos.x -= m_rSpeed*_rDeltaT;
        CFESpriteInstMgr::SetPos(m_hAnimInst,m_oPos);
        CFESpriteInstMgr::SetScale(m_hAnimInst,CFEVect2(LEFT_DIR_MULT*m_rScale,m_rScale));	

		if(!_bRepos) m_bGoingLeft = true;
	}

	if(_bRepos)
	{
		if(CFEMath::rAbs(m_oPos.x - goGameGlobals.m_poPlayer->oGetPos().x) > DIST_CHASE_AGAIN * m_rRandomFactor)
		{
			// Decrease speed
			if(m_rSpeed > SD_SPEED_INCR/4.0f)
			{
				m_rSpeed -= _rDeltaT * SD_SPEED_DECR * m_rRandomFactor;
			}
			else	// And come back to attack
			{
				ChangeState(SDS_CHASE);
			}
		}
	}
}
