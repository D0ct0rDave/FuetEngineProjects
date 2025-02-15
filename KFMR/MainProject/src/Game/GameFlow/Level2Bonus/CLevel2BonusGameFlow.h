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
#ifndef CLevel2BonusGameFlowH
#define CLevel2BonusGameFlowH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/GameFlow/CGameFlow.h"
//-----------------------------------------------------------------------------
class CLevel2BonusGameFlow : public CGameFlow
{
	public:

		/// Default constructor of the level game flow.		
		CLevel2BonusGameFlow(uint _uiSkill);

		/// Specific destructor for this class.
		virtual ~CLevel2BonusGameFlow();

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
    
    protected:

		///
		void GenericGameLoop(FEReal _rDeltaT);
 
 		CFECinematic*	m_poCinematic;

		FEReal	m_rTimer;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

