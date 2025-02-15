//-----------------------------------------------------------------------------
/*! \class CEnemy
 *  \brief The base class that all enemys share
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEnemyH
#define CEnemyH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CKFRMActor.h"
//-----------------------------------------------------------------------------
const FEReal ENE_SCALE_MIN = 0.9f;
const FEReal ENE_SCALE_MAX = 1.1f;
//-----------------------------------------------------------------------------
const uint ENEMY_IDLE_FACE   = 0;
const uint ENEMY_ATTACK_FACE = 1;
const uint ENEMY_DIE_FACE    = 2;
//-----------------------------------------------------------------------------
class CEnemy : public CKFRMActor
{
	public:

		CEnemy();
		~CEnemy();

		/// Initializes the enemy.
		virtual void Init(uint _uiSkill) { m_uiSkill = _uiSkill; };

		/// Spawns the enemy.
		virtual void Spawn(const CFEVect2& _oPos) = 0;

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	public:

		// Specific to the enemy
		CFEVect2        m_oSpeed;
        FEReal          m_rZSpeed;
        FEReal          m_rZ;
        FEReal          m_rResistanceFact;

	protected:

        //
		uint	        m_uiSkill;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
