// ----------------------------------------------------------------------------
/*! \class CFESoundMixer
 *  \brief CFESoundMixer Class Definition
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundMixer.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
static FEReal       m_rUserLines [SML_NUM_LINES]		= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurUserLines[SML_NUM_LINES]		= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurUserDeltas[SML_NUM_LINES]		= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rMixerLines[SML_NUM_LINES]		= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurMixerLines[SML_NUM_LINES]		= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
static FEReal       m_rCurMixerDeltas[SML_NUM_LINES]	= { _1r, _1r, _1r, _1r, _1r , _1r, _1r, _1r, _1r, _1r, _1r, _1r, _1r};
// ----------------------------------------------------------------------------
void CFESoundMixer::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rTime)
{
	if (_rTime <= _0r)
	{
		m_rMixerLines[_eSoundLine] = m_rCurMixerLines[_eSoundLine] = CFEMath::rClamp(_0r,_1r,_rVol);

		// Update the system lines
		CFECore::Sound::SetLineLevel(_eSoundLine,m_rCurMixerLines[_eSoundLine]*m_rCurUserLines[_eSoundLine]);
	}
	else if (_rVol != m_rMixerLines[_eSoundLine])
	{
		m_rCurMixerLines[_eSoundLine] = m_rMixerLines[_eSoundLine];
		m_rMixerLines[_eSoundLine] = _rVol;

		m_rCurMixerDeltas[_eSoundLine] = (_rVol - m_rCurMixerLines[_eSoundLine]) / _rTime;
	}
}
// ----------------------------------------------------------------------------
void CFESoundMixer::SetUserLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rTime)
{
	if (_rTime <= _0r)
	{
		m_rUserLines[_eSoundLine] = m_rCurUserLines[_eSoundLine] = CFEMath::rClamp(_0r,_1r,_rVol);

		// Update the system lines
		CFECore::Sound::SetLineLevel(_eSoundLine,m_rCurMixerLines[_eSoundLine]*m_rCurUserLines[_eSoundLine]);
	}
	else if (_rVol != m_rUserLines[_eSoundLine])
	{
		m_rCurUserLines[_eSoundLine] = m_rUserLines[_eSoundLine];
		m_rUserLines[_eSoundLine] = _rVol;

		m_rCurUserDeltas[_eSoundLine] = (_rVol - m_rCurUserLines[_eSoundLine]) / _rTime;
	}

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
		FEBool bChangeLine = false;

		// fade mixer lines
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

			bChangeLine = true;
		}

		// fade user lines
		if (m_rCurUserLines[i] != m_rUserLines[i])
		{
			m_rCurUserLines[i] += m_rCurUserDeltas[i]*_rDeltaT;

			if (m_rCurUserDeltas[i]<_0r)
			{
				if (m_rCurUserLines[i] < m_rUserLines[i])
					m_rCurUserLines[i] = m_rUserLines[i];
			}
			else
			{
				if (m_rCurUserLines[i] > m_rUserLines[i])
					m_rCurUserLines[i] = m_rUserLines[i];
			}

			bChangeLine = true;
		}

		if (bChangeLine)
			CFECore::Sound::SetLineLevel((EFESoundMixerLine)i,m_rCurMixerLines[i]*m_rCurUserLines[i]);
    }
}
// ----------------------------------------------------------------------------
