// -----------------------------------------------------------------------------
/*! \class CActionPlaySound
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEActionStopSound.h"
#include "support/sound/CFESoundPlayer.h"
// -----------------------------------------------------------------------------
void CFEActionStopSound::Init(FEHandler _hSound)
{
	m_hSound = _hSound;
}
// -----------------------------------------------------------------------------
bool CFEActionStopSound::bUpdate(FEReal _rDeltaT)
{
	CFESoundPlayer::Stop(m_hSound);
	return(false);
}
// -----------------------------------------------------------------------------
void CFEActionStopSound::Reset()
{
}
// -----------------------------------------------------------------------------
