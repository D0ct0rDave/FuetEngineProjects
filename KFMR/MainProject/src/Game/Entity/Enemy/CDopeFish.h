//-----------------------------------------------------------------------------
/*! \class CDopefish
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CDopefishH
#define CDopefishH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum EDopeFishStates
{
	DFS_NONE,

	DFS_INIT,
	DFS_WATER_IDLE,
	DFS_PREPARE_ATTACK,
	DFS_JUMP_TO_BRIDGE,
	DFS_IDLE,
	DFS_LOTTERY,
	DFS_JUMP_OVER_BRIDGE,
	DFS_RETURN_TO_WATER,
	DFS_VULNERABLE_STATE1,
	DFS_VULNERABLE_STATE2,
	DFS_DIE,
	DFS_DEAD,
};
//-----------------------------------------------------------------------------
class CDopefish : public CEnemy
{
	public:

		CDopefish();
		~CDopefish();
		
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
			return(GE_DOPEFISH);
		}
    
    protected:

        void UpdatePhysics(FEReal _rDeltaT);
		void DoAttack(FEReal _rDeltaT);	// Check if is possible attack
		void Attack();
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
