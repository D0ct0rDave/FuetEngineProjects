//-----------------------------------------------------------------------------
/*! \class CGameFlow
 *  \brief The class responsible of the game flow in the level.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CLevel2BGameFlowH
#define CLevel2BGameFlowH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/GameFlow/CGameFlow.h"
//-----------------------------------------------------------------------------
class CLevel2BGameFlow : public CGameFlow
{
	public:

		/// Default constructor of the level game flow.		
		CLevel2BGameFlow(uint _uiSkill);

		/// Specific destructor for this class.
		virtual ~CLevel2BGameFlow();

		/// Resets the gameflow to its initial state.
		virtual void Reset();

		/// Performs an update step over the game flow logic.
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the game enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the game exits from a given state.
		virtual void OnExitState(uint _uiState, uint _uiNewState);

		/// Function to be called the first time a level is to be played.
		virtual void Start();

		/// Function to be called upon level restart.
		virtual void Restart();

		/// Retrieves whether the gameflow is already done or not.
		virtual bool bFinished();

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage/*,CEntity* _poSrcEntity,FEPointer _pData*/);

	protected:

		///
		void GenericGameLoop(FEReal _rDeltaT);
        

		CFECinematic*	m_poCinematic;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

