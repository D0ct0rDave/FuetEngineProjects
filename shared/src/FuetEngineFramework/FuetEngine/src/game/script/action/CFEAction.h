// -----------------------------------------------------------------------------
/*! \class CFEAction
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionH
#define CFEActionH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// -----------------------------------------------------------------------------
class CFEAction
{
public:

	/// Performs an update step in the action logic. When the action is considered totally performed, the function returns false,
	/// true otherwise.
	virtual bool bUpdate(FEReal _rDeltaT);
	
	/// Resets the action to its initial state.
	virtual void Reset();

};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
