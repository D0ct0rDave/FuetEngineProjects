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
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
void CFESoundPlayer::Init(FEHandler _hParam)
{
    CFESystem::Sound::hInit(_hParam);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Play(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
    CFESystem::Sound::PlaySound(_hSound,_eSoundLine,_rVol,_rPan);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Stop(FEHandler _hSound)
{
	CFESystem::Sound::StopSound(_hSound);
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::StopSounds()
{
    CFESystem::Sound::StopSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::PauseSounds()
{
	CFESystem::Sound::PauseSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::ResumeSounds()
{
	CFESystem::Sound::ResumeSounds();
}
// ----------------------------------------------------------------------------
void CFESoundPlayer::Update(FEReal _rDeltaT)
{
	CFESystem::Sound::Update();
}
// ----------------------------------------------------------------------------
