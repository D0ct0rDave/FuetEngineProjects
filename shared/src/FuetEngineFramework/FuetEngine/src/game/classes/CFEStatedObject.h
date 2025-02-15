//-----------------------------------------------------------------------------
/*! \class CFEStatedObject
 *  \brief A class to derive from for objects with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#ifndef CFEStatedObjectH
#define CFEStatedObjectH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
//-----------------------------------------------------------------------------
class CFEStatedObject
{
	public:

		CFEStatedObject();
		~CFEStatedObject();

		/// Specific code to perform when the object enters to a given state.
		virtual void OnEnterState(uint _uiState);
		
		/// Specific code to perform when the object exits from a given state.
		virtual void OnExitState(uint _uiState, uint _uiNewState);

		/// Retrieve the previous state of the object
		uint uiGetPrevState()
		{
			return(m_uiPrevState);
		}

		/// Retrieve the state of the object
		uint uiGetState()
		{
			return(m_uiState);
		}

		/// Force the object to a given state: Should not be called directly unless strictly necessary.
		void SetState(uint _uiState)
		{			
			m_uiPrevState	= m_uiState;
			m_uiState		= _uiState;
		}

		/// Changes the object to a given state, performing Exit and Enter events.
		void ChangeState(uint _uiState)
		{
			if ((!m_bAllowReentrance) && (_uiState == m_uiState)) return;

			OnExitState(m_uiState, _uiState);
			m_rStateTime = _0r;
			OnEnterState(_uiState);

			SetState(_uiState);
		}

		/// Enables or disables whether the object allows reentering to the current state.
		void AllowReentrance(bool _bAllowReentrance = true)
		{
			m_bAllowReentrance = _bAllowReentrance;
		}

		/// Retrieves if the object allows reentering to the current state.
		bool bAllowReentrance()
		{
			return(m_bAllowReentrance);
		}
		
		/// Sets up the initial time of the state.
		void SetStateTime(FEReal _rStateTime)
		{
			m_rStateTime = _rStateTime; 
		}

		/// Retrieves the current state time.
		FEReal rGetStateTime()
		{
			return(m_rStateTime);
		}

		/// Updates the time of the current state. Returns true if the state time has not yet reached 0.
		bool bUpdateStateTime(FEReal _rDeltaT)
		{
			m_rStateTime -= _rDeltaT;
			return(m_rStateTime>_0r);
		}

	private:

		// Current object state
		uint		m_uiState;

		// Previous object state
		uint		m_uiPrevState;

		// Allow state reentrance?
		bool		m_bAllowReentrance;

        /// The time of the current state.
		FEReal	    m_rStateTime;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
