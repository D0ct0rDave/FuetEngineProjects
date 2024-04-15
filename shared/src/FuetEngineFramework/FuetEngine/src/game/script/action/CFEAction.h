// -----------------------------------------------------------------------------
/*! \class CFEAction
 *  \brief The base class for script actions.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionH
#define CFEActionH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
// -----------------------------------------------------------------------------
class CFEAction
{
public:
	
	/// The virtual destructor is needed because when the script deletes all the actions,
	/// it doesn't know what destructor to call, so it just calls ~CFEAction(). Specific attributes
	/// for every other action type, are not deleted never.
	virtual ~CFEAction() {};

	/// Performs an update step in the action logic. When the action is considered totally performed, the function returns false,
	/// true otherwise.
	virtual FEBool bUpdate(FEReal _rDeltaT);
	
	/// Resets the action to its initial state.
	virtual void Reset();
	
	/// Forces the finish the action being played.
	virtual void ForceFinish();

	/// Retrieves the action signature.
	virtual CFEString sGetActionSignature();
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
