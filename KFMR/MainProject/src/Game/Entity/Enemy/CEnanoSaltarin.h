//-----------------------------------------------------------------------------
/*! \class CEnanoSaltarin
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEnanoSaltarinH
#define CEnanoSaltarinH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEnemy.h"
//-----------------------------------------------------------------------------
typedef enum EEnanoSaltarinStates
{
	ENS_NONE,

	ENS_INIT,
	ENS_APPROACHING,
	ENS_LOOKING,
	ENS_ATTACKING,
	ENS_DIE,
	ENS_DEAD,
};
//-----------------------------------------------------------------------------
class CEnanoSaltarin : public CEnemy
{
	public:

		CEnanoSaltarin();
		~CEnanoSaltarin();
		
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
			return(GE_ENANO_SALTARIN);
		}
    
    protected:

        void UpdatePhysics(FEReal _rDeltaT);

		FEReal      m_rInitialSpeed;
		FEReal      m_rSpeed;
		FEReal      m_rAccelMultiplier;
		FEReal      m_rDestinationX;
		FEReal      m_rDestinationY;
		FEReal      m_rInitialX;
		FEReal      m_rInitialY;
		bool        m_bBounce;
		bool		m_bStopAttacking;
		bool		m_bAttackDone;		// Only one attack every jump
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
