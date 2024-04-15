// -----------------------------------------------------------------------------
/*! \class CFEAction
 *  \brief The base class for script actions.
 *  \author David M�rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEAction.h"
// -----------------------------------------------------------------------------
FEBool CFEAction::bUpdate(FEReal _rDeltaT)
{
	return(false);
}
// -----------------------------------------------------------------------------
void CFEAction::Reset()
{
}
// -----------------------------------------------------------------------------
void CFEAction::ForceFinish()
{
	
}
// -----------------------------------------------------------------------------
CFEString CFEAction::sGetActionSignature()
{
	return("CFEAction");
}
// -----------------------------------------------------------------------------
