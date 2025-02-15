//-----------------------------------------------------------------------------
/*! \class CSimpleEnemy
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CSimpleEnemyH
#define CSimpleEnemyH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum ESimpleEnemyStates
{
	SES_NONE,

	SES_INIT,
	SES_IDLE,
	SES_CHASE,	
	SES_ATTACK,
	SES_LOCK_PLAYER,
	SES_DIE,
	SES_DEAD,
	SES_EXIT
};
//-----------------------------------------------------------------------------
class CSimpleEnemy : public CEnemy
{
	public:

		CSimpleEnemy();
		~CSimpleEnemy();
		
		/// Initializes the enemy.
		virtual void Init(uint _uiSkill);

		/// Spawns the enemy.
		virtual void Spawn(const CFEVect2& _oPos);

		/// Think function in case it's needed
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the entity enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the entity exits from a given state.
		virtual void OnExitState(uint _uiState, uint _uiNewState);

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData);

		/// Resets the entity to a given initial state
		virtual void Reset();

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

		/// Returns the type of entity
		virtual EGameEntity eGetType()
		{
			return(GE_SIMPLE_ENEMY);
		}
    
    protected:

		void DoAttack(FEReal _rDeltaT);	// Check if is possible attack
		void Attack();

		// For exit state
		bool		    m_bMoveLeft;

		// Delay the attack
		FEReal		    m_rDelayAttackRange;
		FEReal		    m_rDelayAttack;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
