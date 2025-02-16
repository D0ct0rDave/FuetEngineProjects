// -----------------------------------------------------------------------------
/*! \class CFEActionSleep
 *  \brief An action to perform a non active wait over the action queue.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionSleepH
#define CFEActionSleepH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
// -----------------------------------------------------------------------------
class CFEActionSleep : public CFEAction
{
	public:

		/// Initializes the action with the given parameters.
		void Init(FEReal _rTime);
		
		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);
		
		/// Resets the action to its initial state.
		virtual void Reset();

	private:

		FEReal			m_rSleepTime;
		FEReal			m_rTime;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------