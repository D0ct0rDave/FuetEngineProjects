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
#include "CFEActionPlaySound.h"
#include "support/sound/CFESoundMgr.h"
#include "support/sound/CFESoundPlayer.h"
// -----------------------------------------------------------------------------
void CFEActionPlaySound::Init(FEHandler _hSound,EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rPan,bool _bLoop,bool _bWait)
{
	m_hSound	= _hSound;
	m_eMixLine	= _eMixLine;
	m_rVol		= _rVol;
	m_rPan		= _rPan;
	m_bWait		= _bWait;

	// Loop the sound.
	if (_bLoop)
		CFESoundMgr::bSetSoundProperty(m_hSound,"Loop",(FEPointer)true);
}
// -----------------------------------------------------------------------------
bool CFEActionPlaySound::bUpdate(FEReal _rDeltaT)
{
	CFESoundPlayer::Play(m_hSound,m_eMixLine,m_rVol,m_rPan);
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
