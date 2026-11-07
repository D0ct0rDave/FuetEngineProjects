// ----------------------------------------------------------------------------
/*! \class CFESoundPlayer
 *  \brief Sound Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESoundPlayer.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
void CFESoundPlayer::Init(FEHandler _hParam)
{
    CFECore::Sound::hInit(_hParam);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Play(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
    CFECore::Sound::PlaySound(_hSound,_eSoundLine,_rVol,_rPan);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Stop(FEHandler _hSound)
{
	CFECore::Sound::StopSound(_hSound);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::StopSounds()
{
    CFECore::Sound::StopSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::PauseSounds()
{
	CFECore::Sound::PauseSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::ResumeSounds()
{
	CFECore::Sound::ResumeSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Update(FEReal _rDeltaT)
{
	CFECore::Sound::Update();
}
// ----------------------------------------------------------------------------
