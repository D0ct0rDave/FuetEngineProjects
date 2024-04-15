// ----------------------------------------------------------------------------
/*! \class CFESoundPlayer
 *  \brief A basic class to play sound.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESoundPlayerH
#define CFESoundPlayerH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types\CFEString.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CFESoundPlayer
{
    public:

        // Constructor of the audio player system.
        // CFESoundPlayer();

        // Constructor of the audio player system.
        // CFESoundPlayer(FEHandler _hParam) { Init(_hParam); };

        /// Initializes the audio player system.
        static void Init(FEHandler _hParam);

        /// Plays a sound in the given mix line with the supplied parameters.
        static FEHandler hPlay(FEHandler _hSoundResource,EFESoundMixerLine _eSoundLine,FEReal _rVol = _1r,FEReal _rPan = _05r);

        /// Stops a sound currently being played.
        static void Stop(FEHandler _hSound);

        /// Pauses all the currently playing sounds.
        static void PauseSounds();

        /// Resumes the playing of the previously paused sounds.
        static void ResumeSounds();

        /// Stop all the currently playing sounds.
        static void StopSounds();

        /// Updates the player.
        static void Update(FEReal _rDeltaT);

        /// Retrieves a sound system property if it exists.
        static FEBool bGetProperty(const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound system property if it exists.
        static FEBool bSetProperty(const CFEString& _sProperty,FEPointer _pParam);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
