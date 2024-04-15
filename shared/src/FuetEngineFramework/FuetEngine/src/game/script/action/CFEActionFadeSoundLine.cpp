// -----------------------------------------------------------------------------
/*! \class CFEActionFadeSoundLine
 *  \brief Fades the sound in a sound line
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par WordsUp License
 */
// -----------------------------------------------------------------------------
#include "CFEActionFadeSoundLine.h"
#include "support/sound/CFESoundMixer.h"
// -----------------------------------------------------------------------------
void CFEActionFadeSoundLine::Init(EFESoundMixerLine _eMixLine,FEReal _rVol,FEReal _rFadeTime,FEBool _bWait)
{
	m_eMixLine	= _eMixLine;
	m_rVol		= _rVol;
	m_rFadeTime = m_rTime = _rFadeTime;
	m_bWait		= _bWait;
}
// -----------------------------------------------------------------------------
FEBool CFEActionFadeSoundLine::bUpdate(FEReal _rDeltaT)
{
	if (m_bWait)
	{
		if (m_rTime == m_rFadeTime)
			// This only happens once
			CFESoundMixer::SetLineLevel(m_eMixLine,m_rVol,m_rFadeTime);

		m_rTime -= _rDeltaT;
		return(m_rTime > _0r);
	}
	else
	{
		CFESoundMixer::SetLineLevel(m_eMixLine,m_rVol,m_rFadeTime);
		return(false);
	}
}
// -----------------------------------------------------------------------------
void CFEActionFadeSoundLine::Reset()
{
	m_rTime = m_rFadeTime;
}
// -----------------------------------------------------------------------------
CFEString CFEActionFadeSoundLine::sGetActionSignature()
{
	return(CFEString::sFormat("CFEActionFadeSoundLine(MixLine=%d,_rVol=%.02f,_rFadeTime=%.02f,_bWait=%s)",m_eMixLine,m_rVol,m_rFadeTime,m_bWait?"true":"false") );
}
// -----------------------------------------------------------------------------
