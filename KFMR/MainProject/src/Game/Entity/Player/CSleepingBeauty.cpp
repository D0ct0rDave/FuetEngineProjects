//-----------------------------------------------------------------------------
/*! \class CPlayer
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CSleepingBeauty.h"

#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"

#include <FuetEngine.h>
#include "FuetEngineExt.h"
//-----------------------------------------------------------------------------
// Player actions
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1;
const FEReal RIGHT_DIR_MULT = 1;
//-----------------------------------------------------------------------------
const FEReal JUMP_KICK_DAMAGE   = 0.5f;
const FEReal KICK_DAMAGE		= 0.3f;
const FEReal PUNCH_DAMAGE		= 0.2f;
//-----------------------------------------------------------------------------
const uint FACE_ANIM_ATTACK = 0;
const uint FACE_ANIM_DEATH  = 3;
const uint FACE_ANIM_IDLE   = 4;
const uint FACE_ANIM_PAIN   = 5;
//-----------------------------------------------------------------------------
static FEReal PLAYER_ENERGY;
//-----------------------------------------------------------------------------
CSleepingBeauty::CSleepingBeauty()
{
	CEntityRegistry::uiAddEntity(this,"player",true);
}
//-----------------------------------------------------------------------------
CSleepingBeauty::~CSleepingBeauty()
{
	CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CSleepingBeauty::Init(CFEInput* _poInput,const CFEVect2& _oPos)
{
	m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("data/actors/actor/actor");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
	if (poGetActorFace() == NULL)
	{
	    // assert ???
	    goGameGlobals.m_oLogger.Error("Caca!!! no tienes cara!");
	}
		
	CAnimHelper::poSubstNodeSprite(m_hAnimInst,"head","data/actors/player/head");
	CAnimHelper::poSubstNodeSprite(m_hAnimInst,"body","data/actors/player/body");
	
	CFESkelAnimInstMgr::Enable(m_hAnimInst);

	// Set input
	m_poInput = _poInput;
	
	m_oPos = _oPos;
	Reset();
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::Reset()
{
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(ACTOR_SCALE,ACTOR_SCALE));
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::Spawn(const CFEVect2& _oPos,bool _bLeft)
{
	m_oPos   = _oPos;
	m_rDirMult = _bLeft?LEFT_DIR_MULT:RIGHT_DIR_MULT;

	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);
	poGetActorFace()->SetAction(FACE_ANIM_IDLE);

	SetEnergy(1);

	Enable();
	
	ChangeState(PS_SPAWNING);	
}
//-----------------------------------------------------------------------------
bool CSleepingBeauty::bCrouch()
{
	return( m_poInput->bPressed(IB_DOWN) );
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::Attack()
{
	// Crouch attack
	if (m_poInput->bPressed(IB_DOWN))
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			m_rDamage = KICK_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_CROUCH_KICK);
		}
	else if (m_poInput->bPressed(IB_B))
		{	
			// punch
			m_rDamage = PUNCH_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_CROUCH_PUNCH);
		}
	}

	// Jump attack
else if (m_poInput->bPressed(IB_UP))
	{
		// kick always
		m_rDamage = JUMP_KICK_DAMAGE;
		CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_JUMP_KICK);
	}

	// Normal attack
else
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			m_rDamage = KICK_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_KICK);
		}
	else if (m_poInput->bPressed(IB_B))
		{
			// punch
			m_rDamage = PUNCH_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_PUNCH);
		}		
	}

	// Perform sound
	if (m_poInput->bPressed(IB_UP))
		CSoundHelper::hPlay(SND_PLAYER_JUMP_ATTACK,1,0.5f + m_rDirMult*0.2f);
	else
		CSoundHelper::hPlay(SND_PLAYER_ATTACK,1,0.5f + m_rDirMult*0.2f);

	// Change facial anim 
	poGetActorFace()->SetAction(CFEMath::iRand(FACE_ANIM_ATTACK,FACE_ANIM_ATTACK+2));


	// m_rStateTime = 0.5;

	// Send message to the game
	CGameMsgMgr::SendMsg(GMSG_PLAYER_ATTACK,this,(FEPointer)&m_rDamage);
}
///-----------------------------------------------------------------------------
void CSleepingBeauty::Update(FEReal _rDeltaT)
{
	m_rStateTime -= _rDeltaT;

	switch ( uiGetState() )
	{
		case PS_SPAWNING:
		{
			/*
			m_rSpeed += (-9.8)*_rDeltaT * 500;
			m_oPos.y -= (m_rSpeed*_rDeltaT);
	
			if (m_oPos.y >= GROUND_HEIGHT)
			{
				m_oPos.y = GROUND_HEIGHT;
				CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

				ChangeState(PS_IDLE);
			}
			else
				CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
			*/
			ChangeState(PS_IDLE);
		}
		break;

		case PS_NONE:		
		break;
		
		// ------------------------
		// IDLE State
		// ------------------------
		case PS_IDLE:
		{		
			if (m_poInput->bPressed(IB_LEFT) || m_poInput->bPressed(IB_RIGHT))
			{
				// Control the limits of the map
				if (m_poInput->bPressed(IB_LEFT ) && (m_oPos.x > m_rLeftLimit))
				{
					m_rDirMult = LEFT_DIR_MULT;
					ChangeState(PS_WALK);
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
				}
			else if (m_poInput->bPressed(IB_RIGHT) && (m_oPos.x < m_rRightLimit))
				{	
					m_rDirMult = RIGHT_DIR_MULT;
					ChangeState(PS_WALK);
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
				}
			}
		else if (m_poInput->bPressed(IB_DOWN))
			{
				ChangeState(PS_CROUCH);
			}
		else if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
			{
				ChangeState(PS_ATTACK);
			}
		}
		break;

		// ------------------------
		// WALK STATE
		// ------------------------
		case PS_WALK:
		{
			if (m_poInput->bUp(IB_LEFT) || m_poInput->bUp(IB_RIGHT))
			{
				ChangeState(PS_IDLE);
			}
		else if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
			{
				ChangeState(PS_ATTACK);
			}
		else
			{
				if (
					   (m_poInput->bPressed(IB_LEFT ) && (m_oPos.x < m_rLeftLimit))
					|| (m_poInput->bPressed(IB_RIGHT) && (m_oPos.x > m_rRightLimit))
					)
				{
					ChangeState(PS_IDLE);
				}
				else
				{
					if (m_poInput->bPressed(IB_LEFT) || m_poInput->bPressed(IB_RIGHT))
					{
						m_oPos.x += m_rDirMult*ACTOR_SPEED*_rDeltaT;
						CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
					}
				}
			}
		}
		break;
		
		// ------------------------
		// ATTACK STATE
		// ------------------------
		case PS_ATTACK:
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
			{
				if (m_poInput->bPressed(IB_DOWN))
					ChangeState(PS_CROUCH);
				else
					ChangeState(PS_IDLE);
			}
		}
		break;

		// ------------------------
		// CROUCHIDLE STATE
		// ------------------------
		case PS_CROUCH:
		{
			if (m_poInput->bPressed(IB_LEFT))
			{
				m_rDirMult = LEFT_DIR_MULT;
				CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
			}
	   else if (m_poInput->bPressed(IB_RIGHT))
			{
				m_rDirMult = RIGHT_DIR_MULT;
				CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
			}		
			if (m_poInput->bReleased(IB_DOWN))
			{
				ChangeState(PS_IDLE);
			}
		else if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
			{
				ChangeState( PS_ATTACK );
			}
		}
		break;
		
		// ------------------------
		// PAIN STATE
		// ------------------------
		case PS_PAIN:		
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
			{
				if (m_poInput->bPressed(IB_DOWN))
					ChangeState(PS_CROUCH);
				else
					ChangeState(PS_IDLE);
			}			
		}
		break;

		// ------------------------
		// DEAD STATE
		// ------------------------
		case PS_DYING:		
		{
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
				ChangeState(PS_DEAD);
		}
		break;

		// ------------------------
		// DEAD STATE
		// ------------------------
		case PS_DEAD:
		{
			/*
			if (! CFESkelAnimInstMgr::bIsPlaying(m_hAnimInst))
			{
				if (m_poInput->bPressed(IB_DOWN))
					ChangeState(PS_CROUCH);
				else
					ChangeState(PS_IDLE);
			}
			*/
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		case PS_NONE:
		break;

		case PS_SPAWNING:
		{
			// Add this entity to the entity manager
			CEntityMgr::iAddEntity(this);
		}
		break;

		case PS_IDLE:
		{
			poGetActorFace()->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_IDLE);
		}
		break;

		case PS_WALK:
		{
			poGetActorFace()->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
		}
		break;

		case PS_CROUCH:
		{
			poGetActorFace()->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_CROUCH_IDLE);
		}
		break;
	
		case PS_ATTACK:
		{
			Attack();
		}
		break;

		case PS_PAIN:
		{
			poGetActorFace()->SetAction(CFEMath::iRand(FACE_ANIM_PAIN,FACE_ANIM_PAIN+1));
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_PAIN);

			PLAYER_ENERGY = rGetEnergy();
			CGameMsgMgr::SendMsg(GMSG_PLAYER_DAMAGED,this,(FEPointer)&PLAYER_ENERGY);

			CSoundHelper::hPlay(SND_PLAYER_PAIN,1);
		}
		break;
		
		case PS_DYING:
		{
			PLAYER_ENERGY = 0;
			CGameMsgMgr::SendMsg(GMSG_PLAYER_DAMAGED,this,(FEPointer)&PLAYER_ENERGY);

			poGetActorFace()->SetAction(FACE_ANIM_DEATH);
			CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_DEATH);
			
			CSoundHelper::hPlay(SND_PLAYER_DEATH,1);
		}
		break;

		case PS_DEAD:
		{
			// Delete this entity from the entity manager
			CEntityMgr::DeleteEntity(this);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
void CSleepingBeauty::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
			CFESpriteInstMgr::Disable(m_hAnimInst);
			Disable();
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if ((uiGetState() == PS_DYING) || (uiGetState() == PS_PAIN)) return;

			DecEnergy( *((FEReal*)_pData) );
			if (rGetEnergy() > _0r)
			{
				ChangeState(PS_PAIN);
			}
			else
			{
				ChangeState(PS_DYING);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
