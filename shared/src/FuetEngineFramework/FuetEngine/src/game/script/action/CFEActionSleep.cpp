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
#include "CFEActionSleep.h"
// -----------------------------------------------------------------------------
void CFEActionSleep::Init(FEReal _rTime)
{
	m_rSleepTime = _rTime;
	m_rTime		 = _rTime;
}
// -----------------------------------------------------------------------------
void CFEActionSleep::Reset()
{
	m_rTime = m_rSleepTime;
}
// -----------------------------------------------------------------------------
FEBool CFEActionSleep::bUpdate(FEReal _rDeltaT)
{
	m_rTime -= _rDeltaT;
	return(m_rTime > _0r);
}
// -----------------------------------------------------------------------------
void CFEActionSleep::ForceFinish()
{
	m_rTime = _0r;
	return;
}
// -----------------------------------------------------------------------------
CFEString CFEActionSleep::sGetActionSignature()
{
	return( CFEString::sFormat("CFEActionSleep(_rTime=%.02f)",m_rSleepTime) );
}
// -----------------------------------------------------------------------------
