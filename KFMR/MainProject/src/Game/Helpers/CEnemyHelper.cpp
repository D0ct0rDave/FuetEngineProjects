//-----------------------------------------------------------------------------
/*! \class CEnemyHelper
 *  \brief Helper class to perform specific processes over enemies.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CEnemyHelper.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CSFXHelper.h"
//-----------------------------------------------------------------------------
void CEnemyHelper::GenericDie(CEnemy* _poEnemy,TEntityAttack* _poReceivedAttack)
{
	CSoundHelper::hPlay(SND_ENEMY_DEATH,1);
	CAnimHelper::Play(_poEnemy->hGetAnimInst(),ACTION_ENEMY_DEATH);

	if (_poEnemy->poGetActorFace() != NULL)
	    _poEnemy->poGetActorFace()->SetAction(ENEMY_DIE_FACE);

	// create random vector
	FEReal rLowLimit = _0r;
	FEReal rUpLimit  = _0r;
	FEReal rMult     = _1r;

	switch (_poReceivedAttack->m_eType)
	{
	    case EA_RIGHT_JUMP_KICK:
	    case EA_LEFT_JUMP_KICK:
	    rLowLimit = 1.0f / 3.0f;
	    rUpLimit  = 2.0f / 4.0f;
	    rMult     = (_poReceivedAttack->m_eType == EA_LEFT_JUMP_KICK)?-_1r:_1r;
	    break;

	    case EA_RIGHT_CROUCH_KICK:
	    case EA_LEFT_CROUCH_KICK:
	    rLowLimit = _0r;
	    rUpLimit  = 1.0f / 4.0f;
	    rMult     = (_poReceivedAttack->m_eType == EA_LEFT_CROUCH_KICK)?-_1r:_1r;
	    break;

        case EA_RIGHT_KICK:
        case EA_LEFT_KICK:
        rLowLimit = _0r;
	    rUpLimit  = 1.0f / 3.0f;
	    rMult     = (_poReceivedAttack->m_eType == EA_LEFT_KICK)?-_1r:_1r;
	    break;

	    case EA_LEFT_CROUCH_PUNCH:
	    case EA_LEFT_PUNCH:
	    rMult     = -_1r;
	    break;
	}

    FEReal rAngle = CFEMath::rRand(rLowLimit,rUpLimit)*_PIr;
    _poEnemy->m_oSpeed = CFEVect2::X();
    _poEnemy->m_oSpeed.Rotate(rAngle);
    _poEnemy->m_oSpeed *= 20 * _poReceivedAttack->m_rDamage;
    _poEnemy->m_oSpeed.x *= rMult;

    // Simulated Depth
    _poEnemy->m_rZSpeed  = CFEMath::rRand(0.0,0.8);
    _poEnemy->m_rZSpeed *= _poEnemy->m_rZSpeed;      // exponential growing.
    _poEnemy->m_rZ = _1r;

    // Launch onomatopeia FX
    CSFXHelper::SpawnFollowerFX(SFX_ONOMATOPEIA,_poEnemy,CFEVect2::ZERO());
}
//-----------------------------------------------------------------------------
void CEnemyHelper::GenericDieUpdate(FEReal _rDeltaT,CEnemy* _poEnemy)
{
    _poEnemy->m_oSpeed.y += (+9.8f)*_rDeltaT;
	_poEnemy->SetPos( _poEnemy->oGetPos() + (_poEnemy->m_oSpeed*100.0*_rDeltaT) );

	CFESkelAnimInstMgr::SetPos(_poEnemy->hGetAnimInst(), _poEnemy->oGetPos());

	// CFEVect2 oScale = CFESkelAnimInstMgr::oGetScale( _poEnemy->hGetAnimInst() );

    _poEnemy->m_rZ += _poEnemy->m_rZSpeed*_rDeltaT;
    _poEnemy->rGetScale();

	// oScale *= _poEnemy->m_rZ;
    // CFESkelAnimInstMgr::SetScale(_poEnemy->hGetAnimInst(),oScale);
}
//-----------------------------------------------------------------------------
