// -----------------------------------------------------------------------------
/*! \class CFEActionSleep
 *  \brief An action to perform a non active wait over the action queue.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEActionWaitInput.h"
// -----------------------------------------------------------------------------
void CFEActionWaitInput::Init()
{
	m_bExit = false;
}
// -----------------------------------------------------------------------------
void CFEActionWaitInput::Reset()
{
	m_bExit = false;
}
// -----------------------------------------------------------------------------
FEBool CFEActionWaitInput::bUpdate(FEReal _rDeltaT)
{
	return(m_bExit == false);
}
// -----------------------------------------------------------------------------
void CFEActionWaitInput::ForceFinish()
{
	m_bExit = true;
}
// -----------------------------------------------------------------------------
CFEString CFEActionWaitInput::sGetActionSignature()
{
	return( "CFEActionWaitInput()" );
}
// -----------------------------------------------------------------------------
