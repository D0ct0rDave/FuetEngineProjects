// ----------------------------------------------------------------------------
/*! \class CFESoundMixer
 *  \brief CFESoundMixer Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundMixer.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
typedef enum
{
    MS_NONE,

    MS_IDLE,
    MS_FADEIN,
    MS_FADEOUT,

    MS_NUMSTATES

}EMixState;
// ----------------------------------------------------------------------------
static FEReal       m_rUserLines [SML_NUM_LINES] = { _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rMixerLines[SML_NUM_LINES] = { _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurMixerLines[SML_NUM_LINES] = { _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurMixerDeltas[SML_NUM_LINES] = { _1r, _1r, _1r, _1r, _1r};

static FEReal       gsrFadeFact = _1r;
static FEReal       gsrFadeStep = _1r;
static EMixState    gseMixState = MS_IDLE;
// ----------------------------------------------------------------------------
void CFESoundMixer::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rTime)
{
	if (_rTime <= _0r)
	{
		m_rMixerLines[_eSoundLine] = _rVol;
		m_rCurMixerLines[_eSoundLine] = _rVol;

		// Update the system lines
		CFESystem::Sound::SetLineLevel(_eSoundLine,m_rCurMixerLines[_eSoundLine]*m_rUserLines[_eSoundLine]);
	}
	else if (_rVol != m_rMixerLines[_eSoundLine])
	{
		m_rCurMixerLines[_eSoundLine] = m_rMixerLines[_eSoundLine];
		m_rMixerLines[_eSoundLine] = _rVol;

		m_rCurMixerDeltas[_eSoundLine] = (_rVol - m_rCurMixerLines[_eSoundLine]) / _rTime;
	}
}
// ----------------------------------------------------------------------------
void CFESoundMixer::SetUserLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
    // Update the system lines
    m_rUserLines[_eSoundLine] = CFEMath::rClamp(_0r,_1r,_rVol);
    CFESystem::Sound::SetLineLevel(_eSoundLine,m_rCurMixerLines[_eSoundLine]*m_rUserLines[_eSoundLine]);
}
// ----------------------------------------------------------------------------
void CFESoundMixer::FadeIn(FEReal _rTime)
{
	for (uint i=0;i<SML_NUM_LINES;i++)
		SetLineLevel((EFESoundMixerLine)i,_1r,_rTime);
}
// ----------------------------------------------------------------------------
void CFESoundMixer::FadeOut(FEReal _rTime)
{
	for (uint i=0;i<SML_NUM_LINES;i++)
		SetLineLevel((EFESoundMixerLine)i,_0r,_rTime);
}
// ----------------------------------------------------------------------------
void CFESoundMixer::Update(FEReal _rDeltaT)
{
	for (uint i=0;i<SML_NUM_LINES;i++)
    {
		if (m_rCurMixerLines[i] != m_rMixerLines[i])
		{
			m_rCurMixerLines[i] += m_rCurMixerDeltas[i]*_rDeltaT;

			if (m_rCurMixerDeltas[i]<_0r)
			{
				if (m_rCurMixerLines[i] < m_rMixerLines[i])
					m_rCurMixerLines[i] = m_rMixerLines[i];
			}
			else
			{
				if (m_rCurMixerLines[i] > m_rMixerLines[i])
					m_rCurMixerLines[i] = m_rMixerLines[i];
			}

			CFESystem::Sound::SetLineLevel((EFESoundMixerLine)i,m_rCurMixerLines[i]*m_rUserLines[i]);
		}
    }
}
// ----------------------------------------------------------------------------
