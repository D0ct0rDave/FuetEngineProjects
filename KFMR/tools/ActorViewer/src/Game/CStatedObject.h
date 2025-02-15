//-----------------------------------------------------------------------------
/*! \class CStatedObject
 *  \brief A class to derive from for objects with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CStatedObjectH
#define CStatedObjectH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
class CStatedObject
{
	public:

		CStatedObject();
		~CStatedObject();

		/// Specific code to perform when the object enters to a given state.
		virtual void OnEnterState(uint _uiState) {};
		
		/// Specific code to perform when the object exits from a given state.
		virtual void OnExitState(uint _uiState) {};

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
			m_uiPrevState = m_uiState;
			m_uiState = _uiState;
		}

		/// Changes the object to a given state, performing Exit and Enter events.
		void ChangeState(uint _uiState)
		{
			if ((!m_bAllowReentrance) && (_uiState == m_uiState)) return;

			OnExitState(m_uiState);
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
		
	private:

		// Current object state
		uint		m_uiState;

		// Previous object state
		uint		m_uiPrevState;

		// Allow state reentrance?
		bool		m_bAllowReentrance;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
