//-----------------------------------------------------------------------------
/*! \class CDiscChinoCudeiro
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CDiscChinoCudeiroH
#define CDiscChinoCudeiroH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum EDiscCCEnemyStates
{
	DCCS_NONE,

	DCCS_INIT,
	DCCS_IDLE,
	DCCS_CHASE,	
	DCCS_PREATTACK_DOWN,
	DCCS_ATTACK_DOWN,
	DCCS_ATTACK_UP,
	DCCS_FALLING,
	DCCS_LOCK_PLAYER,
	DCCS_DIE,
	DCCS_DEAD,
	DCCS_EXIT
};
//-----------------------------------------------------------------------------
class CDiscChinoCudeiro : public CEnemy
{
	public:

		CDiscChinoCudeiro();
		~CDiscChinoCudeiro();
		
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

		/// Returns the type of entity (default = 0)
		virtual EGameEntity eGetType()
		{
			return(GE_DISC_CHINO_CUDEIRO);
		}

		/// The player is attacking 
		void PlayerAttacks();

		/// Get if the attack if for down
		bool bIsAttackingDown() { return m_bAttackDown; }

	protected:

		void DoAttack(FEReal _rDeltaT);	// Check if is possible attack
		void Attack();

		// Specific to the player
		FEReal		m_rSpeed;
		bool		m_bAttackDown;
		bool		m_bJumpWithAttack;
		FEReal		m_rJumpSpeed;
		FEReal		m_rJumpHeight;
		bool		m_bGoingUp;

		// For exit state
		bool		m_bMoveLeft;

		// Delay the attack
		FEReal		m_rDelayAttackRange;
		FEReal		m_rDelayAttack;
	
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
