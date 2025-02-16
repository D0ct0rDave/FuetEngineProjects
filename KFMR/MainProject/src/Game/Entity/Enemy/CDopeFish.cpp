//-----------------------------------------------------------------------------
#include "CDopefish.h"

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
const FEReal LEFT_DIR_MULT  = -1.0f;
const FEReal RIGHT_DIR_MULT = 1.0f;
//-----------------------------------------------------------------------------
// Idle time after attack
const FEReal ENEMY_GROUND_HEIGHT    = 320.0f;
const FEReal WATER_IDLE_TIME		= 7.0f;
const FEReal WATER_IDLE_SPEED 	    = 100.0f;

const FEReal PREPARE_ATTACK_TIME	= 7.0f;
const FEReal PREPARE_ATTACK_XSPEED	= 200.0f;
const FEReal PREPARE_ATTACK_YSPEED	= 500.0f;
const FEReal PREPARE_ATTACK_ACCEL   = 100.0f;

const FEReal JUMP_TO_BRIDGE_TIME    = 2.0f;
const FEReal JUMP_TO_BRIDGE_YSPEED  = 600.0f;
const FEReal JUMP_TO_BRIDGE_ACCEL   = 100.0f;
const FEReal JUMP_TO_BRIDGE_PERTURB_TIME = 1.5;
const FEReal JUMP_TO_BRIDGE_PERTURB_MAG  = 20.0;

const FEReal RETURN_TO_WATER_TIME    = 2.0f;
const FEReal RETURN_TO_WATER_YSPEED  = 500.0f;
const FEReal RETURN_TO_WATER_ACCEL   = 100.0f;

const FEReal JUMP_OVER_BRIDGE_XSPEED = 300.0;
const FEReal JUMP_OVER_BRIDGE_YSPEED = 300.0;
const FEReal JUMP_OVER_BRIDGE_ACCEL  = 200.0;
const FEReal JUMP_OVER_BRIDGE_TIME   = 5.0;

const FEReal JUMP_OVER_BRIDGE_PERTURB_TIME = 0.15;
const FEReal JUMP_OVER_BRIDGE_PERTURB_MAG  = 10.0;

const FEReal BRIDGE_IDLE_TIME		= 1.0f;
const FEReal WHILE_IDLE_RETURN_TO_WATER_PROB = 1.0f/3.0f;

const FEReal VULNERABLE_STATE1_TIME = 3.0f;
const FEReal VULNERABLE_STATE2_TIME = 2.0f;

// Enemy damage
static FEReal ENEMY_DAMAGE = 0.05f;
//-----------------------------------------------------------------------------
// dopefish faces
//-----------------------------------------------------------------------------
const uint DF_IDLE_FACE  = 0;
const uint DF_PAIN_FACE  = 2;
const uint DF_DEAD_FACE  = 5;
//-----------------------------------------------------------------------------
const FEReal	DYING_TIME = 1.0f;

FEReal      m_rInitialSpeed = 0;
FEReal      m_rSpeed = 0;
FEReal      m_rAccelMultiplier;
CFESmoother m_oSmoother;
FEReal      m_rDestinationX;
FEReal      m_rDestinationY;
FEReal      m_rInitialX;
FEReal      m_rInitialY;
bool        m_bBounce;
uint        m_uiPainCounter;
//-----------------------------------------------------------------------------
CDopefish::CDopefish()
{
    CEntityRegistry::uiAddEntity(this,"Dopefish",true);
    goGameGlobals.m_poBoss = this;	

    m_oAttack.m_eType = EA_DOPEFISH_ATTACK;    
    m_oAttack.m_rDamage = 0.125;
    m_rResistanceFact   = 4.0;
}
//-----------------------------------------------------------------------------
CDopefish::~CDopefish()
{
    CEntityRegistry::DeleteEntity(this);
}
///-----------------------------------------------------------------------------
void CDopefish::Init(uint _uiSkill)
{
	m_uiSkill = _uiSkill;

    CAnimHelper::CreateActor("dopefish");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("dopefish");
    
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(DF_IDLE_FACE);
	Reset();
	
	m_oSmoother.Init(0.65f);
}
///-----------------------------------------------------------------------------
void CDopefish::Reset()
{
	m_oPos = CFEVect2::oZERO();
    m_rZ = _1r;

	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);

	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	CFESkelAnimInstMgr::Disable(m_hAnimInst);
	SetEnergy(1);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    
    m_uiPainCounter = 0;
	Disable();
}
///-----------------------------------------------------------------------------
void CDopefish::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	m_oPos.y = ENEMY_GROUND_HEIGHT;
	m_rZ = _1r;

    CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_WALK);
	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

	SetEnergy(1);
	Enable();
    
	ChangeState(DFS_INIT);
}
///-----------------------------------------------------------------------------
void CDopefish::DoAttack(FEReal _rDeltaT)
{
    /*
	if(m_rDelayAttack == -1)
	{
		m_rDelayAttack = 0;
		m_rDelayAttackRange = CFEMath::rRand(DFS_DELAY_ATTACK_MIN, DFS_DELAY_ATTACK_MAX);		// Set the delay for the attack
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
				ChangeState(DFS_ATTACK);
			}
		}
	} 
	*/
}
///-----------------------------------------------------------------------------
void CDopefish::Attack()
{
	/*
	CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_PUNCH);
	poGetActorFace()->SetAction(ENEMY_ATTACK_FACE);
	m_rDelayAttack = -1;
	*/
}
///-----------------------------------------------------------------------------
void CDopefish::UpdatePhysics(FEReal _rDeltaT)
{
    FEReal rOldSpeed = m_rSpeed;
    m_rSpeed += (_rDeltaT*9.8f*m_rAccelMultiplier);

    if ((m_rSpeed*rOldSpeed) < 0)
    {
        // cambio de signo!
        CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_DOWN);
    }

    m_oPos.y += m_rSpeed*_rDeltaT;
    
    FEReal rAngle = _0r;
    if (m_rDirMult == 1.0)
        rAngle =  -(m_rSpeed / m_rInitialSpeed) * 0.3;
    else
        rAngle =   (m_rSpeed / m_rInitialSpeed) * 0.3;

    CFESkelAnimInstMgr::SetAngle( m_hAnimInst, m_oSmoother.rGetValue(rAngle) );

    // llegamos al suelo?
    if ((m_rSpeed >= _0r) && (m_oPos.y >= m_rDestinationY))
    {                 
        m_bBounce = true;
        m_rSpeed = m_rInitialSpeed;

        // cambio de signo!
        CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_UP);
        m_oPos.y = m_rDestinationY;
    }
    else
        m_bBounce = false;
}
///-----------------------------------------------------------------------------
void CDopefish::Update(FEReal _rDeltaT)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;
	
	m_rStateTime -= _rDeltaT;

    // Manage states
	switch ( uiGetState() )
	{
		case DFS_NONE:
		{
		}
		break;

		// ------------------------
		// INIT State
		// ------------------------
		case DFS_INIT:
		{
			ChangeState(DFS_WATER_IDLE);
		}
		break;

		// ------------------------
		// WATER_IDLE STATE
		// ------------------------
		case DFS_WATER_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
                ChangeState(DFS_PREPARE_ATTACK);
			}
			else
			{
			    // ir de un lado a otro idle.

                // goint to the left
                if (m_rDirMult == LEFT_DIR_MULT)
                {
                    if (m_oPos.x < goGameGlobals.m_poCamera->oGetPos().x)
                    {
                        m_rDirMult *= -1.0;
                    }
                    else
                    {

                    }
                }
                // goint to the right
           else {
                    if (m_oPos.x > (goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin*2))
                    {
                        m_rDirMult *= -1.0;
                    }
                    else
                    {
                    }
                }

                m_oPos.x += (WATER_IDLE_SPEED*_rDeltaT*m_rDirMult);
			}
		}
		break;

		// ------------------------
		// PREPARE ATTACK STATE
		// ------------------------
		case DFS_PREPARE_ATTACK:
		{
		    // ir de un lado a otro idle.

            // goint to the left
            if (m_rDirMult == LEFT_DIR_MULT)
            {
                if (m_oPos.x < goGameGlobals.m_poCamera->oGetPos().x)
                {
                    m_rDirMult *= -1.0;
                }
                else
                {

                }
            }
            // goint to the right
       else {
                if (m_oPos.x > (goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin*2))
                {
                    m_rDirMult *= -1.0;
                }
                else
                {
                }
            }
            m_oPos.x += (PREPARE_ATTACK_XSPEED*_rDeltaT*m_rDirMult);

            UpdatePhysics(_rDeltaT);

            // llegamos al suelo?
            if ((m_bBounce) && (m_rStateTime <= _0r))
            {                    
                ChangeState(DFS_JUMP_TO_BRIDGE);
            }
		}
		break;

		// ------------------------
		// JUMP TO BRIDGE STATE
		// ------------------------
		case DFS_JUMP_TO_BRIDGE:
		{
            m_oPos.x = CFEMath::rLerp(m_rDestinationX,m_rInitialX, m_rStateTime / JUMP_TO_BRIDGE_TIME);
            UpdatePhysics(_rDeltaT);

            // llegamos al suelo?
            if (m_bBounce)
            {
                ChangeState(DFS_IDLE);
            }
		}
		break;
		
		// ------------------------
		// IDLE STATE
		// ------------------------
		case DFS_IDLE:
		{
			if (m_rStateTime <= _0r)
			{
			    // Do lottery !
			    if (CFEMath::rRand() <= WHILE_IDLE_RETURN_TO_WATER_PROB)
				    ChangeState(DFS_RETURN_TO_WATER);
				else
			        ChangeState(DFS_JUMP_OVER_BRIDGE);
			}
			else
			    CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);
		}
		break;

		// ------------------------
		// JUMP OVER BRIDGE STATE
		// ------------------------
		case DFS_JUMP_OVER_BRIDGE:
		{
		    // ir de un lado a otro idle.

            // goint to the left
            if (m_rDirMult == LEFT_DIR_MULT)
            {
                if (m_oPos.x < goGameGlobals.m_poCamera->oGetPos().x)
                {
                    m_rDirMult *= -1.0;
                }
                else
                {

                }
            }
            // goint to the right
       else {
                if (m_oPos.x > (goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin*2))
                {
                    m_rDirMult *= -1.0;
                }
                else
                {
                }
            }
            m_oPos.x += (PREPARE_ATTACK_XSPEED*_rDeltaT*m_rDirMult);

            UpdatePhysics(_rDeltaT);

            // llegamos al suelo?
            if (m_bBounce)                         
            {   
                CCameraMgr::Perturb(JUMP_OVER_BRIDGE_PERTURB_TIME,JUMP_OVER_BRIDGE_PERTURB_MAG);

                if (m_rStateTime <= _0r)
                    ChangeState(DFS_RETURN_TO_WATER);                
            }

		    CGameMsgMgr::SendMsg(GMSG_ENEMY_ATTACK,this,(FEPointer)&m_oAttack);            
		}
		break;

		// ------------------------
		// VULNERABLE STATE 1
		// ------------------------
		case DFS_VULNERABLE_STATE1:
		{
			if (m_rStateTime <= _0r)
			{
				//
				ChangeState(DFS_VULNERABLE_STATE2);
			}
		}
		break;

		// ------------------------
		// VULNERABLE STATE 2
		// ------------------------
		case DFS_VULNERABLE_STATE2:
		{
			if (m_rStateTime <= _0r)
			{
				//
				ChangeState(DFS_RETURN_TO_WATER);
			}
		}
		break;

		// ------------------------
		// DFS_RETURN_TO_WATER STATE
		// ------------------------
		case DFS_RETURN_TO_WATER:
		{
            m_oPos.x = CFEMath::rLerp(m_rDestinationX,m_rInitialX, m_rStateTime / RETURN_TO_WATER_TIME);
            UpdatePhysics(_rDeltaT);

            // llegamos al suelo?
            if (m_bBounce)
            {
                ChangeState(DFS_WATER_IDLE);
            }
		}
		break;

		// ------------------------
		// CHASE STATE
		// ------------------------
		case DFS_DIE:
		{
			if (! CGameHelper::bVisibleOnScreen(m_oPos,100))
			{
				ChangeState(DFS_DEAD);
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
void CDopefish::OnEnterState(uint _uiState)
{
	// No update while a cinematic is playing
	if(uiGetState() == ACTOR_CINEMATIC_STATE) return;

	if (uiGetState() == _uiState) return;

	switch (_uiState)
	{
		case DFS_INIT:
		{
			CEntityMgr::iAddEntity(this);
		}
		break;

		case DFS_WATER_IDLE:
		{
			m_rStateTime = WATER_IDLE_TIME;

			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _1r);
			CFESkelAnimInstMgr::SetAngle(m_hAnimInst, _0r);
			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_IDLE);
		}
		break;

		case DFS_PREPARE_ATTACK:
		{
			m_rDestinationY = ENEMY_GROUND_HEIGHT;
			m_rStateTime = PREPARE_ATTACK_TIME;
			m_rSpeed = -PREPARE_ATTACK_YSPEED;
			m_rInitialSpeed = m_rSpeed;
			m_rAccelMultiplier = PREPARE_ATTACK_ACCEL;

			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_UP);
		}
		break;

		case DFS_JUMP_TO_BRIDGE:
		{
		    m_rInitialX = m_oPos.x;
		    m_rInitialY = m_oPos.y;
		    m_rDestinationX = goGameGlobals.m_poPlayer->oGetPos().x;
		    m_rDestinationY = goGameGlobals.m_poPlayer->oGetPos().y;
			m_rStateTime    = JUMP_TO_BRIDGE_TIME;
            m_rSpeed        = -JUMP_TO_BRIDGE_YSPEED;
            m_rInitialSpeed = m_rSpeed;
            m_rAccelMultiplier = JUMP_TO_BRIDGE_ACCEL;

			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _2r);
			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_UP);
		}
		break;

		case DFS_IDLE:
		{
			m_rStateTime    = BRIDGE_IDLE_TIME;

            CFESkelAnimInstMgr::SetAngle(m_hAnimInst, _0r);
			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_IDLE);    						
		}
		break;

		case DFS_JUMP_OVER_BRIDGE:
		{
		    m_rInitialX     = m_oPos.x;
		    m_rInitialY     = m_oPos.y;
		    m_rDestinationX = goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin;
		    m_rDestinationY = m_oPos.y;
			m_rStateTime    =  JUMP_OVER_BRIDGE_TIME;
            m_rSpeed        = -JUMP_OVER_BRIDGE_YSPEED;
            m_rAccelMultiplier = JUMP_OVER_BRIDGE_ACCEL;
            m_rInitialSpeed = m_rSpeed;

			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _2r);
			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_UP);
		}
		break;

		case DFS_RETURN_TO_WATER:
		{
		    m_rInitialX     = m_oPos.x;
		    m_rInitialY     = m_oPos.y;
		    m_rDestinationX = goGameGlobals.m_poCamera->oGetPos().x + goGameGlobals.m_rScreenMargin;
		    m_rDestinationY = ENEMY_GROUND_HEIGHT;
			m_rStateTime    =  RETURN_TO_WATER_TIME;
            m_rSpeed        = -RETURN_TO_WATER_YSPEED;
            m_rInitialSpeed = m_rSpeed;
            m_rAccelMultiplier = RETURN_TO_WATER_ACCEL;

			CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _2r);
			CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_JUMP_UP);
			poGetActorFace()->SetAction(DF_IDLE_FACE);
		}
		break;

        case DFS_VULNERABLE_STATE1:
        {
            CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, 3.0f);
            CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_IDLE);
            m_rStateTime    =  VULNERABLE_STATE1_TIME;
        }
        break;

        case DFS_VULNERABLE_STATE2:
        {
            CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, 6.0f);
            CAnimHelper::Play(m_hAnimInst,ACTION_DOPEFISH_IDLE);
            m_rStateTime    =  VULNERABLE_STATE2_TIME;
            
        }
        break;

		case DFS_DIE:
		{
            goGameGlobals.SlowMotion();

            CEnemyHelper::GenericDie(this,m_poReceivedAttack);
            poGetActorFace()->SetAction(DF_DEAD_FACE);
		}
		break;

		case DFS_DEAD:
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
void CDopefish::OnExitState(uint _uiState, uint _uiNewState)
{
	switch (_uiState)
	{
		case DFS_PREPARE_ATTACK:
		{
            CFESkelAnimInstMgr::SetAngle(m_hAnimInst, _0r);
		}
		break;

		case DFS_JUMP_TO_BRIDGE:
		{
    		CCameraMgr::Perturb(JUMP_TO_BRIDGE_PERTURB_TIME,JUMP_TO_BRIDGE_PERTURB_MAG);

    		CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _1r);
    		CFESkelAnimInstMgr::SetAngle(m_hAnimInst, _0r);
        }
        break;

        case DFS_JUMP_OVER_BRIDGE:
        {
            CFESkelAnimInstMgr::SetSpeedMult(m_hAnimInst, _1r);
        }
        break;
    }
}
//-----------------------------------------------------------------------------
void CDopefish::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
		}
		break;

		case GMSG_DAMAGE_ENTITY:
		{
			if (uiGetState() == DFS_DIE) return;

		    m_poReceivedAttack = (TEntityAttack*)_pData;
			DecEnergy( m_poReceivedAttack->m_rDamage/m_rResistanceFact);

			goGameGlobals.m_oLogger.Print("Energy: %.2f\n",rGetEnergy());
			m_uiPainCounter++;
			poGetActorFace()->SetAction( DF_PAIN_FACE + (m_uiPainCounter & 0x01) );

			if (rGetEnergy() > _0r)
			{
			    switch (uiGetState())
			    {
			        case DFS_VULNERABLE_STATE1:
			        break;
			        case DFS_VULNERABLE_STATE2:
			        break;

			        default:
                        ChangeState(DFS_VULNERABLE_STATE1);
			    }
            }
			else
			{
				ChangeState(DFS_DIE);
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
