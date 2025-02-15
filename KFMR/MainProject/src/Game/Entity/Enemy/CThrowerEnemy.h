//-----------------------------------------------------------------------------
/*! \class CThrowerEnemy
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CThrowerEnemyH
#define CThrowerEnemyH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
#include "CThrownObject.h"
//-----------------------------------------------------------------------------
typedef enum EThrowerEnemyStates
{
	TES_NONE,

	TES_INIT,
	TES_IDLE,
	TES_APPROACH,	
	TES_THROW,
	TES_ESCAPE,
	TES_DIE,
	TES_DEAD,
	TES_EXIT
};
//-----------------------------------------------------------------------------
class CThrowerEnemy : public CEnemy
{
	public:

		CThrowerEnemy();
		~CThrowerEnemy();
		
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
			return(GE_THROWER);
		}

	protected:

		void Throw();

		// Specific to the player
		FEReal				m_rSpeed;
		
		//
		CThrownObject		m_oObj;

		// For exit state
		bool		        m_bMoveLeft;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
