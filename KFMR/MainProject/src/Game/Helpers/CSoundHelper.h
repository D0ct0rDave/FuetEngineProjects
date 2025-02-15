//-----------------------------------------------------------------------------
/*! \class CSoundHelper
 *  \brief Helper class to load and play game sounds
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CSoundHelperH
#define CSoundHelperH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
#include "GameSounds.h"
const uint MAX_SOUNDSET_SIZE = 10;
//-----------------------------------------------------------------------------
class CSoundHelper
{
	public:

		/// 
		static void Init(EGameSounds _eSounds[]);

		/// 
		static void Finish();

        /// Plays a game sound.
		static FEHandler hPlay(EGameSounds _eSound, FEReal _rVol = _1r, FEReal _rPan = _05r,bool _bLoop = false);
		
		/// Stops a possible playing sound associated with the given handler.
		static void Stop(FEHandler _hSound);

        /// Loads a specific BGM
		static void LoadBGM(const CFEString& _sBGM);

        /// Starts the playing of the BGM.
		static void PlayBGM();

        /// Stops the currently played bgm
		static void StopBGM();

    protected:

        typedef struct
        {
            uint m_uiNumSounds;
            FEHandler m_hSounds[MAX_SOUNDSET_SIZE];

        }TSoundData;

        static TSoundData   m_oSoundDataTable[SND_NUM_SOUNDS];
        static FEHandler    m_hBGM;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
