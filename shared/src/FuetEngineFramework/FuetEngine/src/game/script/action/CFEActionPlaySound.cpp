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
#include "CFEActionPlaySound.h"
#include "support/sound/CFESoundMgr.h"
#include "support/sound/CFESoundPlayer.h"

#include "CFESoundAction.h"
// -----------------------------------------------------------------------------
void CFEActionPlaySound::Init(FEHandler _hSoundResource,EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rPan,FEBool _bLoop,FEBool _bWait)
{
	m_hSoundResource = _hSoundResource;
	m_eMixLine	= _eMixLine;
	m_rVol		= _rVol;
	m_rPan		= _rPan;
	m_bWait		= _bWait;

	// Loop the sound.
	if (_bLoop)
		CFESoundMgr::I()->bSetSoundProperty(m_hSoundResource,"Loop",(FEPointer)true);
}
// -----------------------------------------------------------------------------
FEBool CFEActionPlaySound::bUpdate(FEReal _rDeltaT)
{
	FEHandler hSound = CFESoundPlayer::hPlay(m_hSoundResource,m_eMixLine,m_rVol,m_rPan);

	// Queue the sound...
	CFESoundActionQueue::I()->Queue(m_hSoundResource,hSound);

	if (m_bWait)
	{
		// return( CFESoundPlayer::IsPlaying(m_hSound) ) ;
		return(false);
	}
	else
		return(false);
}
// -----------------------------------------------------------------------------
void CFEActionPlaySound::Reset()
{
}
// -----------------------------------------------------------------------------
CFEString CFEActionPlaySound::sGetActionSignature()
{
	return( CFEString::sFormat("CFEActionPlaySound(_hSoundResource=0x%x,_eMixLine=%d,_rVol=%.02f,_rPan=%.02f,_bLoop=%s,_bWait=%s)",m_hSoundResource,m_eMixLine,m_rVol,m_rPan,"?",m_bWait?"true":"false") );
}
// -----------------------------------------------------------------------------
