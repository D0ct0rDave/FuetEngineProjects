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
#include "CPlayer.h"
#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"
#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CGroundMgr.h"
#include "Game/Helpers/CSFXHelper.h"
//-----------------------------------------------------------------------------
// Player actions
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = -1;
const FEReal RIGHT_DIR_MULT = 1;
//-----------------------------------------------------------------------------
const FEReal JUMP_KICK_DAMAGE   = 0.5f;
const FEReal KICK_DAMAGE		= 0.3f;
const FEReal PUNCH_DAMAGE		= 0.2f;

FEHandler m_hWalkFX;
//-----------------------------------------------------------------------------
const int JUMPING_DODGE_THRESHOLD	= -20;
//-----------------------------------------------------------------------------
const uint FACE_ANIM_ATTACK = 1;
const uint FACE_ANIM_DEATH  = 6;
const uint FACE_ANIM_IDLE   = 0;
const uint FACE_ANIM_PAIN   = 4;
const uint FACE_ANIM_HAPPY  = 7;
//-----------------------------------------------------------------------------
static FEReal PLAYER_ENERGY;
//-----------------------------------------------------------------------------
// Jump
//-----------------------------------------------------------------------------
const FEReal PS_JUMP_ANIM_SPEED					= 1.75f;
const FEReal JUMP_SPEED							= 75.0;
const FEReal JUMP_INIT_SPEED					= JUMP_SPEED * 5;
const FEReal JUMP_IMPULSE_TIME					= 0.15/PS_JUMP_ANIM_SPEED;
const FEReal JUMP_INERTIA_MULT					= 0.75/PS_JUMP_ANIM_SPEED;
const int	 JUMP_MULT_INERTIA_IN_IMPULSE		= 10/PS_JUMP_ANIM_SPEED;
const int	 JUMP_MULT_INERTIA_BEFORE_IMPULSE	= 3/PS_JUMP_ANIM_SPEED;

const FEReal TIMER_MAX_INERTIA					= 0.5f;

//-----------------------------------------------------------------------------
// DEBUG
//-----------------------------------------------------------------------------
static CFELogger oLog;
//-----------------------------------------------------------------------------
CPlayer::CPlayer()
{
	CEntityRegistry::uiAddEntity(this,"player",true);
}
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CPlayer::Init(CFEInput* _poInput,const CFEVect2& _oPos)
{
    CAnimHelper::CreateActor("player");
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance( "player" );
	SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    if (poGetActorFace() == NULL)
	{
	    // assert ???
	    goGameGlobals.m_oLogger.Error("Caca!!! no tienes cara!");
	}
	poGetActorFace()->SetAction( FACE_ANIM_IDLE );
    
	// create the body group
	CFESkelAnimNode* poSkeleton = CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst);
	CFESkelAnimNode* poBodyGroup = CFESkelAnimLoader::poLoadNode("data/actors/player/body");
	CAnimHelper::SubstNode(poSkeleton,"B_hip","hip",poBodyGroup);
	
	// Load proper belt
	// goGameGlobals.m_poCurProfile->m_uiCurBelt;
    CAnimHelper::poSubstNodeSprite(m_hAnimInst,"belt","data/objects/belt_red");

	CFESkelAnimInstMgr::Enable(m_hAnimInst);

	// Set input
	m_poInput = _poInput;

	m_oPos = _oPos;
	Reset();

	m_hWalkFX = CSFXHelper::hSpawnPSys(SFX_DUST);
	CFEParticleSysInstMgr::SetDepth(m_hWalkFX,0.5f);
	CFEParticleSysInstMgr::Disable(m_hWalkFX);
}
//-----------------------------------------------------------------------------
void CPlayer::Reset()
{
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(ACTOR_SCALE,ACTOR_SCALE));
	CFESkelAnimInstMgr::Disable(m_hAnimInst);

	// Jump
	m_rJumpHeight = 0;
	m_rJumpInertia = 1;

	// Inertia
	m_rTimerInertia = 0;

	// Bonus
	m_uiBonusLife = 0;
	m_uiBonusOk = 0;
	m_uiBonusBad = 0;

}
//-----------------------------------------------------------------------------
void CPlayer::Spawn(const CFEVect2& _oPos,bool _bLeft)
{
	m_oPos   = _oPos;
	m_rDirMult = _bLeft?LEFT_DIR_MULT:RIGHT_DIR_MULT;

	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
	PlayAction(ACTION_ACTOR_IDLE);
	poGetActorFace()->SetAction( FACE_ANIM_IDLE );

	SetEnergy(1);

	Enable();
	
	ChangeState(PS_SPAWNING);	
}
//-----------------------------------------------------------------------------
bool CPlayer::bCrouch()
{
	return( uiGetState() == PS_CROUCH || (uiGetState() == PS_ATTACK && m_poInput->bPressed(IB_DOWN)));
}
//-----------------------------------------------------------------------------
bool CPlayer::bJumping()
{
	return (m_rJumpHeight < JUMPING_DODGE_THRESHOLD);
}
//-----------------------------------------------------------------------------
void CPlayer::PlayAction(ECharacterAction _eAction)
{
    CAnimHelper::Play(m_hAnimInst,_eAction);
}
//-----------------------------------------------------------------------------
void CPlayer::Attack()
{
	if(bJumping()) return;

	// Crouch attack
	if (m_poInput->bPressed(IB_DOWN))
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			PlayAction(ACTION_ACTOR_CROUCH_KICK);
			m_oAttack.m_rDamage = KICK_DAMAGE;
			m_oAttack.m_eType = (m_rDirMult == LEFT_DIR_MULT)? EA_LEFT_CROUCH_KICK: EA_RIGHT_CROUCH_KICK;
		}
	else if (m_poInput->bPressed(IB_B))
		{	
			// punch
			PlayAction(ACTION_ACTOR_CROUCH_PUNCH);
			m_oAttack.m_rDamage = PUNCH_DAMAGE;
			m_oAttack.m_eType = (m_rDirMult == LEFT_DIR_MULT)? EA_LEFT_CROUCH_PUNCH: EA_RIGHT_CROUCH_PUNCH;
		}
	}
	// Normal attack
else
	{
		if (m_poInput->bDown(IB_A))
		{
			// kick
			PlayAction(ACTION_ACTOR_KICK);
			m_oAttack.m_rDamage = KICK_DAMAGE;
			m_oAttack.m_eType = (m_rDirMult == LEFT_DIR_MULT)? EA_LEFT_KICK: EA_RIGHT_KICK;
		}
	else if (m_poInput->bPressed(IB_B))
		{
			// punch
			PlayAction(ACTION_ACTOR_PUNCH);
			m_oAttack.m_rDamage = PUNCH_DAMAGE;
			m_oAttack.m_eType = (m_rDirMult == LEFT_DIR_MULT)? EA_LEFT_PUNCH: EA_RIGHT_PUNCH;
		}		
	}

	// Perform sound
	CSoundHelper::hPlay(SND_PLAYER_ATTACK,1,0.5f + m_rDirMult*0.2f);

	// Change facial anim 
	poGetActorFace()->SetAction( CFEMath::iRand(FACE_ANIM_ATTACK,FACE_ANIM_ATTACK+2) );

	// Send message to the game
	CGameMsgMgr::SendMsg(GMSG_PLAYER_ATTACK,this,(FEPointer)&m_oAttack);
}
///-----------------------------------------------------------------------------
void CPlayer::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;
	// m_bUpdateInCinem && !CFECinematicMgr::bPlaneFinished() && uiGetState() != PS_JUMP) return;

	m_rStateTime -= _rDeltaT;

	// Set proper height
    CFEVect2 oNewPos( oGetPos().x, m_rJumpHeight + CGroundMgr::rGetHeight( oGetPos().x) );
	SetPos( oNewPos );

	// Falling
	if((uiGetState() != PS_JUMP) && ((m_rJumpHeight < 0) || (m_rJumpSpeed > 0)) )
	{
		m_rJumpSpeed += (-9.8f)*_rDeltaT * JUMP_SPEED;
		m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);

		// On the ground
		if ((m_rJumpHeight > 0) && (m_rJumpSpeed < 0))
		{
			m_rJumpInertia = JUMP_INERTIA_MULT;
		}
	}

	// Cheats
#ifdef _DEBUG
	if(m_poInput->bPressed(IB_C))
	{
		if(m_poInput->bPressed(IB_DOWN))
		{
			IncEnergy(0.5);
		}		
		if (m_poInput->bPressed(IB_LEFT))
		{
			m_oPos.x -= ACTOR_SPEED*20*_rDeltaT;
			CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

			m_rDirMult = LEFT_DIR_MULT;
			CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
		}
		if (m_poInput->bPressed(IB_RIGHT))
		{
			m_oPos.x += ACTOR_SPEED*20*_rDeltaT;
			CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

			m_rDirMult = RIGHT_DIR_MULT;
			CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*ACTOR_SCALE,ACTOR_SCALE));
		}
			
	}
#endif

    // Manage states
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
			if(m_poInput->bPressed(IB_UP))
			{
				ChangeState(PS_JUMP);
			}
			if (m_poInput->bPressed(IB_DOWN))
			{
				ChangeState(PS_CROUCH);
			}
			if (m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B))
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
		else if(m_poInput->bPressed(IB_UP))
			{
				ChangeState(PS_JUMP);
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
						m_oPos.x += m_rDirMult*m_rJumpInertia*ACTOR_SPEED*_rDeltaT;
						CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
						
						// Restart inertia
						if(m_rJumpInertia < 1) 
						{
							m_rJumpInertia += JUMP_MULT_INERTIA_BEFORE_IMPULSE * _rDeltaT;
							if(m_rJumpInertia > 1) m_rJumpInertia = 1;
						}

						// Increase the inertia time
						if(m_rTimerInertia < TIMER_MAX_INERTIA)
						{
							m_rTimerInertia += _rDeltaT;
							if(m_rTimerInertia > TIMER_MAX_INERTIA) m_rTimerInertia = TIMER_MAX_INERTIA;
						}

			            // Spawn polvillo
						CFEVect2 FXPos = m_oPos;
						FXPos.x += (m_rDirMult == LEFT_DIR_MULT)? 20: -20;
			            CFEParticleSysInstMgr::SetPos(m_hWalkFX, FXPos);
                        // CSFXHelper::SpawnParticle(SFX_DUST,m_oPos);
					}
					else
					{
						m_rJumpInertia = 1;
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
		// JUMP STATE
		// ------------------------
		case PS_JUMP:
		{
			// Attack
			if ((m_poInput->bDown(IB_A) || m_poInput->bDown(IB_B)) && !m_bJumpAttack)
			{
				m_bJumpAttack = true;


				// Change facial anim 
				poGetActorFace()->SetAction( CFEMath::iRand(FACE_ANIM_ATTACK,FACE_ANIM_ATTACK+2));

				PlayAction(ACTION_ACTOR_JUMP_KICK);
				CSoundHelper::hPlay(SND_PLAYER_JUMP_ATTACK,1,0.5f + m_rDirMult*0.2f);

				// Send message to the game
				m_oAttack.m_rDamage = JUMP_KICK_DAMAGE;
				m_oAttack.m_eType = (m_rDirMult == LEFT_DIR_MULT)? EA_LEFT_JUMP_KICK: EA_RIGHT_JUMP_KICK;
				CGameMsgMgr::SendMsg(GMSG_PLAYER_ATTACK,this,(FEPointer)&m_oAttack);
			}

			// Inertia
			if(m_rJumpInertia != 0)
			{
				m_oPos.x += m_rJumpInertia*ACTOR_SPEED*_rDeltaT;

				// Check limits 
				if(m_oPos.x < m_rLeftLimit)
				{
					m_oPos.x = m_rLeftLimit;
				}
				else if(m_oPos.x > m_rRightLimit)
				{
					m_oPos.x = m_rRightLimit;
				}
				CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);

				FEReal rMultImpulse;
				if(m_rStateTime > 0)
				{
					rMultImpulse = JUMP_MULT_INERTIA_IN_IMPULSE;
				}
				else
				{
					rMultImpulse = 1;
					if(m_rJumpSpeed == JUMP_INIT_SPEED)
					{
						m_rJumpInertia *= JUMP_MULT_INERTIA_BEFORE_IMPULSE;
					}
				}

				if(m_rJumpInertia > 0)
				{
					m_rJumpInertia -= JUMP_INERTIA_MULT * _rDeltaT * rMultImpulse;
				}
				else
				{
					m_rJumpInertia += JUMP_INERTIA_MULT * _rDeltaT * rMultImpulse;
				}
			}

			// Wait the impulse
			if(m_rStateTime > 0) 
			{
				return;
			}
			else if(m_rJumpSpeed == JUMP_INIT_SPEED)
			{					
				poGetActorFace()->SetAction( CFEMath::iRand(FACE_ANIM_ATTACK,FACE_ANIM_ATTACK+2));
				
			}

			m_rJumpSpeed += (-9.8f)*_rDeltaT * JUMP_SPEED;
			m_rJumpHeight -= (m_rJumpSpeed * _rDeltaT);

			// Fall without attacks
			if(m_rJumpSpeed < 0 && m_bGoingUp && !m_bJumpAttack)	
			{
				m_bGoingUp = false;
			    PlayAction(ACTION_ACTOR_JUMP_DOWN);
				poGetActorFace()->SetAction( FACE_ANIM_IDLE);
			}

			// On the ground
			if(m_rJumpHeight > 0 && m_rJumpSpeed < 0) 
			{
				ChangeState(PS_IDLE);
				m_rJumpInertia = JUMP_INERTIA_MULT;
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
void CPlayer::OnEnterState(uint _uiState)
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
		    poGetActorFace()->SetAction( FACE_ANIM_IDLE);
			PlayAction(ACTION_ACTOR_IDLE);
		}
		break;

		case PS_WALK:
		{
		    CFEParticleSysInstMgr::Enable(m_hWalkFX);
		    poGetActorFace()->SetAction( FACE_ANIM_IDLE);
			PlayAction(ACTION_ACTOR_WALK);
		}
		break;

		case PS_CROUCH:
		{
		    poGetActorFace()->SetAction( FACE_ANIM_IDLE);
			PlayAction(ACTION_ACTOR_CROUCH_IDLE);
		}
		break;
	
		case PS_ATTACK:
		{
			Attack();
		}
		break;
	
		case PS_JUMP:
		{
		    poGetActorFace()->SetAction( FACE_ANIM_IDLE);
			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, PS_JUMP_ANIM_SPEED);
			PlayAction(ACTION_ACTOR_JUMP_UP);

			// Reset vars			
			m_rStateTime = JUMP_IMPULSE_TIME;
			m_rJumpSpeed = JUMP_INIT_SPEED;
			m_bJumpAttack = false;
			m_bGoingUp = true;
			m_rJumpHeight = 0;

			// Horizontal inertia
			if (m_poInput->bPressed(IB_LEFT))
			{
				m_rJumpInertia = LEFT_DIR_MULT;
			} 
			else if(m_poInput->bPressed(IB_RIGHT))
			{
				m_rJumpInertia = RIGHT_DIR_MULT;
			}
			else
			{
				m_rJumpInertia = 0;
			}

			// Inertia mult (dependiente de cuanto tiempo hace que estabas pulsando el botón de izquierda o derecha)
			m_rJumpInertia *= (m_rTimerInertia / TIMER_MAX_INERTIA);
			m_rTimerInertia = 0; 
			
			// Spawn cinect FX
			// CSFXHelper::SpawnPSys(SFX_CINECT,m_oPos,0.25f);
			CSFXHelper::SpawnParticle(SFX_CINECT_UP,m_oPos);
		}
		break;

		case PS_PAIN:
		{
		    poGetActorFace()->SetAction( CFEMath::iRand(FACE_ANIM_PAIN,FACE_ANIM_PAIN+1));
			PlayAction(ACTION_ACTOR_PAIN);

			PLAYER_ENERGY = rGetEnergy();
			CGameMsgMgr::SendMsg(GMSG_PLAYER_DAMAGED,this,(FEPointer)&PLAYER_ENERGY);

			CSoundHelper::hPlay(SND_PLAYER_PAIN,1);
		}
		break;
		
		case PS_DYING:
		{
			PLAYER_ENERGY = 0;
			CGameMsgMgr::SendMsg(GMSG_PLAYER_DAMAGED,this,(FEPointer)&PLAYER_ENERGY);

			poGetActorFace()->SetAction( FACE_ANIM_DEATH);
			PlayAction(ACTION_ACTOR_DEATH);
			
			CSoundHelper::hPlay(SND_PLAYER_DEATH,1);
		}
		break;

		case PS_DEAD:
		{
			// Delete this entity from the entity manager
			CEntityMgr::DeleteEntity(this);
		}
		break;

		case ACTOR_CINEMATIC_STATE:
		{
            CFEParticleSysInstMgr::Disable(m_hWalkFX);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CPlayer::OnExitState(uint _uiState, uint _uiNewState)
{	
	switch (_uiState)
	{
		case PS_WALK:
        {
			m_rJumpInertia = 1;

			if(_uiNewState != PS_JUMP)
			{
				m_rTimerInertia = 0; 
			}

			CFEParticleSysInstMgr::Disable(m_hWalkFX);
		}
		break;
		
		case PS_JUMP:
		{
			m_rTimerInertia = 0; 
			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, 1);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CPlayer::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
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
            
            m_poReceivedAttack = (TEntityAttack*)_pData;
			DecEnergy( m_poReceivedAttack->m_rDamage );

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

		case GMSG_GET_BONUS_OBJ:
		{
			m_uiBonusLife++;
			m_uiBonusOk++;

			poGetActorFace()->SetAction( FACE_ANIM_HAPPY );

			//oLog.Print("\nPilla bonus: %d", m_uiBonusLife);
		}
		break;
		case GMSG_GET_BAD_BONUS_OBJ:
		{
			if ((uiGetState() == PS_DYING) || (uiGetState() == PS_PAIN)) return;
			
			ChangeState(PS_PAIN);
			m_uiBonusLife--;
			m_uiBonusBad++;

			// TODO (Bonus): Control de fin de juego (si has llegado al límite de triste)

			//oLog.Print("\nPilla bad bonus: %d", m_uiBonusLife);
			oLog.Print("\n--> Bad object (%i/3)", m_uiBonusBad);
		}
		break;
	}
}
//-----------------------------------------------------------------------------
