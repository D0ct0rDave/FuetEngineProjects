// ----------------------------------------------------------------------------
/*! \class CFESoundPlayer
 *  \brief Sound Class Definition
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
FEHandler CFESoundPlayer::hPlay(FEHandler _hSoundResource,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
    return( CFECore::Sound::hPlaySound(_hSoundResource,_eSoundLine,_rVol,_rPan) );
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
FEBool CFESoundPlayer::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return( CFECore::Sound::bGetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
FEBool CFESoundPlayer::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return( CFECore::Sound::bSetProperty(_sProperty,_pParam) );
}
// ----------------------------------------------------------------------------
