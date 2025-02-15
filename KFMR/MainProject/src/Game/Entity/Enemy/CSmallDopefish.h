//-----------------------------------------------------------------------------
/*! \class CSmallDopefish
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CSmallDopefishH
#define CSmallDopefishH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum ESmallDopefishStates
{
	SDS_NONE,

	SDS_INIT,
	SDS_CHASE,	
	SDS_ATTACK,
	SDS_REPOS,	
	SDS_DIE,
	SDS_DEAD,
	SDS_EXIT
};
//-----------------------------------------------------------------------------
class CSmallDopefish : public CEnemy
{
	public:

		CSmallDopefish();
		~CSmallDopefish();
		
		/// Initializes the enemy.
		virtual void Init(uint _uiSkill);

		/// Spawns the enemy.
		virtual void Spawn(const CFEVect2& _oPos);

		/// Think function in case it's needed
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the entity enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the entity exits from a given state.
		virtual void OnExitState(uint _uiState);

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
			return(GE_SMALL_DOPEFISH);
		}

		// Returns if the enemy is attacking (jumping)
		bool bIsAttacking() { return (uiGetState() == SDS_ATTACK); }
    
    protected:

		void Attack();
		void Move(FEReal _rDeltaT, bool _bRepos = false);

		// Specific to the player
		FEReal		m_rSpeed;

		// Hace diferente a cada entidad de este tipo; velocidad, caída, aceleración, etc...
		FEReal		m_rRandomFactor;

		// For exit state
		bool		m_bMoveLeft;

		// Delay the attack
		FEReal		m_rDelayAttackRange;
		FEReal		m_rDelayAttack;
	
		// Jump
		bool		m_bGoingUp;
		FEReal		m_rJumpSpeed;
		FEReal		m_rJumpHeight;

		// Inertia
		bool		m_bGoingLeft;

		// Every Attack
		bool		m_bAttackDone;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
