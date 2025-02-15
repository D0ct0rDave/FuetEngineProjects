//-----------------------------------------------------------------------------
/*! \class CChinoCudeiro
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CChinoCudeiroH
#define CChinoCudeiroH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum EChinoCudeiroStates
{
	CCS_NONE,
	CCS_SIT,
	CCS_WAKE,
	CCS_INIT,
	CCS_IDLE,
	CCS_MINI_IDLE,
	CCS_CHASE,
	CCS_GOBACK,
	CCS_ATTACK,
	CCS_LOTTERY,
	CCS_PAIN_0,
	CCS_PAIN_1,
	CCS_PAIN_2,
	CCS_DYING,
	CCS_DEAD
};
//-----------------------------------------------------------------------------
class CChinoCudeiro : public CEnemy
{
	public:

		CChinoCudeiro();
		~CChinoCudeiro();
		
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
		
		/// To tell if the enemy is currently in pain.
		bool bInPain()
		{
			return ((uiGetState() >=CCS_PAIN_0) && (uiGetState()<=CCS_PAIN_2));
		}

		/// Returns the type of entity
		virtual EGameEntity eGetType()
		{
			return(GE_CHINO_CUDEIRO);
		}

	protected:

		void Attack();

		// Specific to the player
		FEReal		m_rSpeed;

		// Specific to the player
		FEReal		m_rAttackTime;
		
		CFESkelAnimSprite* m_poHandSpr;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
