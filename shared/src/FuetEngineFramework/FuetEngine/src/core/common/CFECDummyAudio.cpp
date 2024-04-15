// ----------------------------------------------------------------------------
/*! \class
 *  \brief DummyAudio Sound Implementation.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
 // ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (SOUND == USE_DUMMYAUDIO)
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEDefs.h"
#include "../CFECore.h"

// ----------------------------------------------------------------------------
/// Initializes the sound system.
FEHandler CFECore::Sound::hInit(FEHandler _hParam)
{
    return 0; // Return a dummy handle
}
// ----------------------------------------------------------------------------
/// Finalizes the sound system.
void CFECore::Sound::Finish()
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
/// Performs an update step over the sound system, once per application frame.
void CFECore::Sound::Update()
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
/// Sets the volume of the given Line to a given level.
void CFECore::Sound::SetLineLevel(EFESoundMixerLine _eSoundLine, FEReal _rVol)
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
/// Loads a sound indicating if we want to treat it as a BGM or not.
FEHandler CFECore::Sound::hLoadSound(const CFEString& _sFilename, FEBool _bBGM)
{
    return 0; // Return a dummy handle
}
// ----------------------------------------------------------------------------
/// Deletes a given sound.
void CFECore::Sound::DeleteSound(FEHandler _hSound)
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
/// Retrieves a sound system property if it exists.
FEBool CFECore::Sound::bGetProperty(const CFEString& _sProperty, FEPointer _pParam)
{
    return true; // Dummy return value
}
// ----------------------------------------------------------------------------
/// Sets a sound system property if it exists.
FEBool CFECore::Sound::bSetProperty(const CFEString& _sProperty, FEPointer _pParam)
{
    return true; // Dummy return value
}

/// Retrieves a sound property if exists.
FEBool CFECore::Sound::bGetSoundProperty(FEHandler _hSound, const CFEString& _sProperty, FEPointer _pParam)
{
    return true; // Dummy return value
}

/// Sets a sound property if exists and it's possible.
FEBool CFECore::Sound::bSetSoundProperty(FEHandler _hSound, const CFEString& _sProperty, FEPointer _pParam)
{
    return true; // Dummy return value
}

/// Plays a given sound in the given sound line.
FEHandler hPlaySound(FEHandler _hSoundResource, EFESoundMixerLine _eSoundLine, FEReal _rVol, FEReal _rPan)
{
    return 0; // Return a dummy handle
}

/// Stops a given sound.
void CFECore::Sound::StopSound(FEHandler _hSound)
{
    // Dummy implementation
}

/// Stops all the currently playing sounds.
void CFECore::Sound::StopSounds()
{
    // Dummy implementation
}

/// Pauses all the currently playing sounds.
void CFECore::Sound::PauseSounds()
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
/// Resumes the playing of the previously paused sounds.
void CFECore::Sound::ResumeSounds()
{
    // Dummy implementation
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
