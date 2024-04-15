// -----------------------------------------------------------------------------
/*! \class CActionPlaySound
 *  \brief The base class for script actions.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEActionStopSound.h"
#include "support/sound/CFESoundPlayer.h"

#include "CFESoundAction.h"
// -----------------------------------------------------------------------------
void CFEActionStopSound::Init(FEHandler _hSoundResource)
{
	m_hSoundResource = _hSoundResource;
}
// -----------------------------------------------------------------------------
FEBool CFEActionStopSound::bUpdate(FEReal _rDeltaT)
{
	FEHandler hHnd = CFESoundActionQueue::I()->hGetSound(m_hSoundResource);
	if (hHnd != NULL)
	{
		CFESoundPlayer::Stop(hHnd);
		CFESoundActionQueue::I()->Dequeue(m_hSoundResource);
	}

	return(false);
}
// -----------------------------------------------------------------------------
void CFEActionStopSound::Reset()
{
}
// -----------------------------------------------------------------------------
CFEString CFEActionStopSound::sGetActionSignature()
{
	return( CFEString::sFormat("CFEActionStopSound(_hSoundResource=0x%x)",m_hSoundResource) );
}
// -----------------------------------------------------------------------------
