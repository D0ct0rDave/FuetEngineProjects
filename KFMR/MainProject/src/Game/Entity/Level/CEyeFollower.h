//-----------------------------------------------------------------------------
/*! \class CEyeFollower
 *  \brief The class that manages objects thrown by ThrowerEnemy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEyeFollowerH
#define CEyeFollowerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CEntity.h"
//-----------------------------------------------------------------------------
class CEyeFollower : public CEntity
{ 
	public:

		CEyeFollower();
		~CEyeFollower();

		/// Initializes the enemy.
		void Init();

		/// Spawns the enemy.
		virtual void Spawn(const CFEVect2& _oPos);

		// 
		virtual void Reset();

		// 
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the entity enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the entity exits from a given state.
		virtual void OnExitState(uint _uiState, uint _uiNewState);

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData);

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

		/// Returns the type of entity
		virtual EGameEntity eGetType()
		{
			return(GE_EYE_FOLLOWER);
		}
        
	protected:
        
        /// Sets the height of the entity
        void SetHeight(FEReal _rHeight);
        
		virtual void Kill();	
        
        FEReal      m_rCurHeight;
        FEHandler   m_hEye[2];
		FEHandler	m_hAnimInst;				
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
