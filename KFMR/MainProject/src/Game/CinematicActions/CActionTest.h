// -----------------------------------------------------------------------------
/*! \class CActionTest
 *  \brief The base class for script actions.
 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CActionTestH
#define CActionTestH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
// -----------------------------------------------------------------------------
class CActionTest : public CFEAction
{
	public:

		void Init(CFEInput* _poInput);
		
		/// Performs an update step in the action logic.
		virtual bool bUpdate(FEReal _rDeltaT);

		/// Resets the action to its initial state.
		virtual void Reset();
	
	protected:

		CFEInput* m_poInput;
};

// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------