//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"

#include "CPlayer.h"
#include "Game/CGameGlobals.h"
#include "Game/CSoundHelper.h"
#include "Game/CAnimHelper.h"

//-----------------------------------------------------------------------------
// Player actions
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1.0f;
const FEReal RIGHT_DIR_MULT = 1.0f;
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
CPlayer::CPlayer()
{
}
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
}
///-----------------------------------------------------------------------------
void CPlayer::Init(CFEInput* _poInput,const CFEVect2& _oPos)
{
	#ifndef NOACTOR
	m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("data/actors/actor/actor");

	m_poFaceSpr = CAnimHelper::poSubstNode(m_hAnimInst,"face","data/actors/actor/face",FACE_ANIM_IDLE);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	#endif

	// Set input
	m_poInput = _poInput;
	
	m_oPos = _oPos;
	Reset();
}
//-----------------------------------------------------------------------------
void CPlayer::Reset()
{
	#ifndef NOACTOR
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(ACTOR_SCALE,ACTOR_SCALE));
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	#endif
}
//-----------------------------------------------------------------------------
void CPlayer::Spawn(const CFEVect2& _oPos,bool _bLeft)
{
	m_oPos   = _oPos;
	m_rDirMult = _bLeft?LEFT_DIR_MULT:RIGHT_DIR_MULT;

	#ifndef NOACTOR
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
	CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_IDLE);
	m_poFaceSpr->SetAction(FACE_ANIM_IDLE);
	#endif
	
	SetEnergy(1.0f);

	Enable();
	
	ChangeState(PS_SPAWNING);	
}
//-----------------------------------------------------------------------------
bool CPlayer::bCrouch()
{
	return( m_poInput->bPressed(IB_DOWN) );
}
//-----------------------------------------------------------------------------
void CPlayer::Attack()
{
	#ifndef NOACTOR
	// Crouch attack
	if (m_poInput->bPressed(IB_DOWN))
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			m_rDamage = KICK_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_CROUCH_KICK);
		}
	else if (m_poInput->bPressed(IB_B))
		{	
			// punch
			m_rDamage = PUNCH_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_CROUCH_PUNCH);
		}
	}

	// Jump attack
else if (m_poInput->bPressed(IB_UP))
	{
		// kick always
		m_rDamage = JUMP_KICK_DAMAGE;
		CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_JUMP_KICK);
	}

	// Normal attack
else
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			m_rDamage = KICK_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_KICK);
		}
	else if (m_poInput->bPressed(IB_B))
		{
			// punch
			m_rDamage = PUNCH_DAMAGE;
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_PUNCH);
		}		
	}

	// Perform sound
	#ifndef DISABLE_SOUND
	if (m_poInput->bPressed(IB_UP))
		CSoundHelper::Play(SND_PLAYER_JUMP_ATTACK,1.0,0.5 + m_rDirMult*0.2f);
	else
		CSoundHelper::Play(SND_PLAYER_ATTACK,1.0,0.5 + m_rDirMult*0.2f);
	#endif

	// Change facial anim 
	m_poFaceSpr->SetAction(CFEMath::iRand(FACE_ANIM_ATTACK,FACE_ANIM_ATTACK+2));


	// m_rStateTime = 0.5;
	#endif
}
///-----------------------------------------------------------------------------
void CPlayer::Update(FEReal _rDeltaT)
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
					
					#ifndef NOACTOR
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
					#endif
				}
			else if (m_poInput->bPressed(IB_RIGHT) && (m_oPos.x < m_rRightLimit))
				{	
					m_rDirMult = RIGHT_DIR_MULT;
					ChangeState(PS_WALK);

					#ifndef NOACTOR
					CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
					#endif
				}
			}

			#ifndef NOACTOR
		else if (m_poInput->bPressed(IB_DOWN))
			{
				ChangeState(PS_CROUCH);
			}
		else if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
			{
				ChangeState(PS_ATTACK);
			}
			#endif
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
			
			#ifndef NOACTOR
		else if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
			{
				ChangeState(PS_ATTACK);
			}
			#endif
			
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

						#ifndef NOACTOR
						CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
						#endif
					}
				}
			}
		}
		break;
		
		#ifndef NOACTOR		
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
		#endif
	}	
}
//-----------------------------------------------------------------------------
void CPlayer::OnEnterState(uint _uiState)
{
	#ifndef NOACTOR
	switch (_uiState)
	{
		case PS_NONE:
		break;

		case PS_SPAWNING:
		{
		}
		break;

		case PS_IDLE:
		{
			m_poFaceSpr->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_IDLE);
		}
		break;

		case PS_WALK:
		{
			m_poFaceSpr->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_WALK);
		}
		break;
		
		case PS_CROUCH:
		{
			m_poFaceSpr->SetAction(FACE_ANIM_IDLE);
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_CROUCH_IDLE);
		}
		break;
	
		case PS_ATTACK:
		{
			Attack();
		}
		break;

		case PS_PAIN:
		{
			m_poFaceSpr->SetAction(CFEMath::iRand(FACE_ANIM_PAIN,FACE_ANIM_PAIN+1));
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_PAIN);

			PLAYER_ENERGY = rGetEnergy();

			#ifndef DISABLE_SOUND
			CSoundHelper::Play(SND_PLAYER_PAIN,1.0);
			#endif
		}
		break;
		
		case PS_DYING:
		{
			PLAYER_ENERGY = 0;
			m_poFaceSpr->SetAction(FACE_ANIM_DEATH);
			CAnimHelper::Play(m_hAnimInst,ANIM_ACTOR_DEATH);
			
			#ifndef DISABLE_SOUND
			CSoundHelper::Play(SND_PLAYER_DEATH,1.0);
			#endif
		}
		break;

		case PS_DEAD:
		{
		}
		break;
	}
	#endif	
}
//-----------------------------------------------------------------------------
void CPlayer::OnExitState(uint _uiState)
{
}
//-----------------------------------------------------------------------------
void CPlayer::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
}
//-----------------------------------------------------------------------------
