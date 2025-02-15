//-----------------------------------------------------------------------------
/*! \class CActorTest
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//----------------
#ifndef CActorTestH
#define CActorTestH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CActor.h"
//-----------------------------------------------------------------------------
class CActorTest : public CActor
{
	public:

		CActorTest();
		~CActorTest();

		/// Initializes the player.
		virtual void Init(CFEInput* _poInput,const CFEVect2& _oPos);

		/// Spawns the player in the map in left or right direction.
		virtual void Spawn(const CFEVect2& _oPos,bool _bLeft);
		
		/// Think function in case it's needed
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the entity enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the entity exits from a given state.
		virtual void OnExitState(uint _uiState, uint _uiNewState);

		/// Accept procedure for processing functionality over entities
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	
	protected:
		uint m_uiAction;
	CFEInput*	    m_poInput;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
