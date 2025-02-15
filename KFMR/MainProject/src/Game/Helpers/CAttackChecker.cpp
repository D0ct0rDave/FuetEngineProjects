//-----------------------------------------------------------------------------
#include "CAttackChecker.h"

#include "Game/Entity/Player/CPlayer.h"

#include "Game/Entity/Enemy/CSimpleEnemy.h"
#include "Game/Entity/Enemy/CThrowerEnemy.h"
#include "Game/Entity/Enemy/CThrownObject.h"
#include "Game/Entity/Enemy/CChinoCudeiro.h"
#include "Game/Entity/Enemy/CDiscChinoCudeiro.h"
#include "Game/GameFlow/Level1Bonus/CLevel1BonusThrownObject.h"
#include "Game/Entity/Enemy/CSmallDopefish.h"
#include "Game/Entity/Enemy/CDopefish.h"
#include "Game/Entity/Enemy/CEnanoSaltarin.h"


#include "Game/Entity/CEntityMgr.h"
#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CCameraMgr.h"
//-----------------------------------------------------------------------------
const FEReal KICK_THRESHOLD			= 150;
const FEReal PUNCH_THRESHOLD		= 75;
const FEReal ENEMY_DIST_THRESHOLD   = 50;
const FEReal OBJECT_TEST_MARGIN		= 30;
const FEReal DOPEFISH_RADIUS        = 125;
//-----------------------------------------------------------------------------
void CAttackChecker::Check(CEntity* _poEnt)
{
	_poEnt->Accept(this);
}
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CEntity* _poEnt)
{
}
//-----------------------------------------------------------------------------
inline FEReal rGetThreshold(const TEntityAttack& _oAttack)
{
    switch (_oAttack.m_eType)
    {
        case EA_RIGHT_PUNCH:
        case EA_LEFT_PUNCH:
        case EA_RIGHT_CROUCH_PUNCH:
        case EA_LEFT_CROUCH_PUNCH:
        return(PUNCH_THRESHOLD);
        break;
        
        case EA_RIGHT_KICK:
        case EA_RIGHT_CROUCH_KICK:
        case EA_RIGHT_JUMP_KICK:
        case EA_LEFT_JUMP_KICK:
        case EA_LEFT_KICK:
        case EA_LEFT_CROUCH_KICK:
        return(KICK_THRESHOLD);
        break;

        default:
        return(PUNCH_THRESHOLD);
        break;
    }
}
//-----------------------------------------------------------------------------
inline bool bPlayerReachsTarget(CPlayer* _poPlayer,const CFEVect2& _oPos,const TEntityAttack& _oAttack)
{
    FEReal rThreshold = rGetThreshold(_oAttack);
    FEReal rD = (_oPos.x - _poPlayer->oGetPos().x) * _poPlayer->rGetDir();
    return( CFEMath::bBetween(_0r, rThreshold, rD) );
}
//-----------------------------------------------------------------------------
// Player Attack
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CPlayer* _poEnt)
{

	for (uint e=0;e<MAX_ENTITIES;e++)
	{
		CEntity* poEnt = CEntityMgr::poGetEntity(e);
		if ((poEnt != NULL) && (poEnt != _poEnt))
		{
			switch(poEnt->eGetType())
			{
				case GE_DISC_CHINO_CUDEIRO:
				{
					CCDiscipleAttack(_poEnt, (CDiscChinoCudeiro*) poEnt);
				}
				break;

				case GE_SMALL_DOPEFISH:
				{
					if(((CSmallDopefish*) poEnt)->bIsAttacking())
					{
						DefaultAttack(_poEnt, poEnt);
					}
				}
				break;
                
				case GE_DOPEFISH:
				{
				    DopefishAttack(_poEnt,(CDopefish*)poEnt);
				}
				break;
				
				default:
				{
					DefaultAttack(_poEnt, poEnt);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
// Player attacks to default enemy
//-----------------------------------------------------------------------------
void CAttackChecker::DefaultAttack(CPlayer* _poPlayer, CEntity* _poEnemy)
{
	if (_poPlayer->bAttacking() && bPlayerReachsTarget(_poPlayer,_poEnemy->oGetPos(),m_oAttack))
	{
        CSoundHelper::hPlay(SND_PLAYER_HIT, _1r);
		_poEnemy->ReceiveMessage(GMSG_DAMAGE_ENTITY, _poPlayer, (FEPointer)&m_oAttack);
	}
}

//-----------------------------------------------------------------------------
// Player attacks (or try) to ChinoCudeiro Disciple
//-----------------------------------------------------------------------------
void CAttackChecker::CCDiscipleAttack(CPlayer* _poPlayer, CDiscChinoCudeiro* _poEnemy)
{
	if (_poPlayer->bAttacking())
	{
		if (bPlayerReachsTarget(_poPlayer,_poEnemy->oGetPos(),m_oAttack) && (!_poPlayer->bJumping()))
		{
			switch(_poEnemy->uiGetState())
			{
				case DCCS_PREATTACK_DOWN:
				{
					_poEnemy->PlayerAttacks();
				}
				break;
				default:
				{
					CSoundHelper::hPlay(SND_PLAYER_HIT, _1r);
					_poEnemy->ReceiveMessage(GMSG_DAMAGE_ENTITY, _poPlayer, (FEPointer)&m_oAttack);
				}
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
// Player attacks the Big Dopefish
// Sólo es valido cuando el player le da al diente. Es decir si está de frente al jugador, y en el puente.
void CAttackChecker::DopefishAttack(CPlayer* _poPlayer, CDopefish* _poEnemy)
{
    if (
           (    
                (_poEnemy->uiGetState() == DFS_IDLE) 
             || (_poEnemy->uiGetState() == DFS_JUMP_OVER_BRIDGE)
             || (_poEnemy->uiGetState() == DFS_VULNERABLE_STATE1)
             || (_poEnemy->uiGetState() == DFS_VULNERABLE_STATE2)
            )

            && (_poPlayer->bAttacking())
            && (_poEnemy->rGetDir() != _poPlayer->rGetDir())
            && (
                // player and dopefish are facing mutually
                ((_poEnemy->rGetDir() == -_1r) && (_poEnemy->oGetPos().x > _poPlayer->oGetPos().x))
                ||
                ((_poEnemy->rGetDir() ==  _1r) && (_poEnemy->oGetPos().x < _poPlayer->oGetPos().x))
                )
        )
    {
        FEReal rThreshold = rGetThreshold(m_oAttack);
        FEReal rD = (_poEnemy->oGetPos().x - _poPlayer->oGetPos().x) * _poPlayer->rGetDir();

        if (CFEMath::bBetween(_0r, rThreshold*3.0f, rD) )
        {
		    CSoundHelper::hPlay(SND_PLAYER_HIT, _1r);
		    _poEnemy->ReceiveMessage(GMSG_DAMAGE_ENTITY, _poPlayer, (FEPointer)&m_oAttack);
		}
    }
}
//-----------------------------------------------------------------------------
// Simple Enemy Attack
//-----------------------------------------------------------------------------
// Control if the attack of the simple enemy reachs player
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CSimpleEnemy* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

	if ( (CFEMath::rAbs(rD) < PUNCH_THRESHOLD) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
		goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
	}
}
//-----------------------------------------------------------------------------
// Enano Saltarín Attack
//-----------------------------------------------------------------------------
// Control if the attack of the Enano Saltarín reachs player
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CEnanoSaltarin* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

	if ( (CFEMath::rAbs(rD) < ENEMY_DIST_THRESHOLD) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
		goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
	}
}
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CThrowerEnemy* _poEnt)
{
}
//-----------------------------------------------------------------------------
// Thrown object attack.
//-----------------------------------------------------------------------------
// Control if the attack of the simple enemy reachs player
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CThrownObject* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	// mirar si el objeto colisiona con el player:
	CFEVect2 oPlayerPos = goGameGlobals.m_poPlayer->oGetPos();

	if (CFEMath::bBetween(oPlayerPos.x - OBJECT_TEST_MARGIN,oPlayerPos.x + OBJECT_TEST_MARGIN,_poEnt->oGetPos().x))
	{
		// estamos en la zona de test.

		// Thrown object por arriba
		if(_poEnt->oGetPos().y < 300)
		{
			// un test sencillo podría ser si el player esta agachado ..
			if (! goGameGlobals.m_poPlayer->bCrouch())
			{
				// toque manoque !
				CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);

				goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
				_poEnt->ReceiveMessage(GMSG_KILL_ENTITY,NULL,NULL);
			}
		}
		else	// Thrown object por abajo
		{
			// un test sencillo podría ser si el player esta saltando ..
			if (! goGameGlobals.m_poPlayer->bJumping())
			{
				// toque manoque !
				CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);

				goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
				_poEnt->ReceiveMessage(GMSG_KILL_ENTITY,NULL,NULL);
			}
		}

	}
}
//-----------------------------------------------------------------------------
// Thrown object bonus.
//-----------------------------------------------------------------------------
// Control if the bonus reachs player
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CLevel1BonusThrownObject* _poEnt)
{
	// mirar si el objeto colisiona con el player:
	CFEVect2 oPlayerPos = goGameGlobals.m_poPlayer->oGetPos();

	if (CFEMath::bBetween(oPlayerPos.x - OBJECT_TEST_MARGIN,oPlayerPos.x + OBJECT_TEST_MARGIN,_poEnt->oGetPos().x))
	{
		// estamos en la zona de test.

		// Thrown object por arriba
		if(_poEnt->oGetPos().y < 300)
		{
			// un test sencillo podría ser si el player esta agachado ..
			if (! goGameGlobals.m_poPlayer->bCrouch())
			{
				if(_poEnt->bIsGood())	// Good object
				{
					if(goGameGlobals.m_poPlayer->bAttacking())
					{
						// Pilla Bonus
						CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);			// ToDo: Change sound for good object
						goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BONUS_OBJ,_poEnt,0);
					}
					else
					{
						// toque manoque !
						CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
						goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BAD_BONUS_OBJ,_poEnt,0);

					}
				}
				else					// Bad object
				{
					// toque manoque !
					CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
					goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BAD_BONUS_OBJ,_poEnt,0);
				}
				_poEnt->ReceiveMessage(GMSG_KILL_ENTITY,NULL,NULL);
			}
		}
		else	// Thrown object por abajo
		{
			// un test sencillo podría ser si el player esta saltando ..
			if (! goGameGlobals.m_poPlayer->bJumping())
			{
				if(_poEnt->bIsGood())	// Good object
				{
					if(goGameGlobals.m_poPlayer->bAttacking())
					{
						// Pilla Bonus
						CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);			// ToDo: Change sound for good object
						goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BONUS_OBJ,_poEnt,0);
					}
					else
					{
						// toque manoque !
						CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
						goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BAD_BONUS_OBJ,_poEnt,0);
					}
				}
				else					// Bad object
				{
					// toque manoque !
					CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
					goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_GET_BAD_BONUS_OBJ,_poEnt,0);
				}
				_poEnt->ReceiveMessage(GMSG_KILL_ENTITY,NULL,NULL);
			}
		}

	}
}
//-----------------------------------------------------------------------------
// ChinoCudeiro Attack
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CChinoCudeiro* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

    // assume matamoscas has the same traversal as a player kick
	if ( (CFEMath::rAbs(rD) < KICK_THRESHOLD) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		if (
			 (_poEnt->bInPain())
		 || ((! _poEnt->bInPain()) && (!goGameGlobals.m_poPlayer->bCrouch()) )
		   )
		{
			// toque manoque!
			CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
			goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
            CCameraMgr::Perturb(2.0,10.0);
		}
	}
}
//-----------------------------------------------------------------------------
// ChinoCudeiro disciple Attack
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CDiscChinoCudeiro* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

	if ( (CFEMath::rAbs(rD) < PUNCH_THRESHOLD) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		if(_poEnt->bIsAttackingDown())
		{
			if(!goGameGlobals.m_poPlayer->bJumping())
			{
				CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
				goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
			}
		}
		else
		{
			if(!goGameGlobals.m_poPlayer->bCrouch())
			{
				CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
				goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
			}
		}
	}
}
//-----------------------------------------------------------------------------
// Small Dope Fish Attack
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CSmallDopefish* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

	if ( (CFEMath::rAbs(rD) < PUNCH_THRESHOLD) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
		goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
	}
}
//-----------------------------------------------------------------------------
// Dopefish Attack
//-----------------------------------------------------------------------------
void CAttackChecker::Visit(CDopefish* _poEnt)
{
	if ((goGameGlobals.m_poPlayer->uiGetState() == PS_DYING) || (goGameGlobals.m_poPlayer->uiGetState() == PS_PAIN)) return;

	FEReal rD = _poEnt->oGetPos().x - goGameGlobals.m_poPlayer->oGetPos().x;

	if ( (CFEMath::rAbs(rD) < DOPEFISH_RADIUS) && (! goGameGlobals.m_poPlayer->bDead()))
	{
		CSoundHelper::hPlay(SND_ENEMY_HIT,_1r);
		goGameGlobals.m_poPlayer->ReceiveMessage(GMSG_DAMAGE_ENTITY,_poEnt,(FEPointer)&m_oAttack);
	}
}
//-----------------------------------------------------------------------------
