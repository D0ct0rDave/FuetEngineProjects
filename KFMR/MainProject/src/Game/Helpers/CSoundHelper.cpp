//-----------------------------------------------------------------------------
#include "CSoundHelper.h"
//-----------------------------------------------------------------------------
CSoundHelper::TSoundData CSoundHelper::m_oSoundDataTable[SND_NUM_SOUNDS];
FEHandler   CSoundHelper::m_hBGM = NULL;
//-----------------------------------------------------------------------------
#define NO_SOUND
//-----------------------------------------------------------------------------
void CSoundHelper::Init(EGameSounds _eSounds[])
{
	#ifdef NO_SOUND
	return;
	#endif
	
    uint i = 0;
    while (_eSounds[i] != SND_NONE)
    {
        // Try loading sounds one by one.
        FEHandler hSound;
        uint j = 0;

        do{
            CFEString sSound;
            sSound.Format("data/sound/fx/%s_%d",GAME_SOUNDS[ _eSounds[i] ].m_sName, j+1);

            hSound = CFESoundMgr::hLoad( sSound );
            if (hSound != NULL)
            {
                m_oSoundDataTable[ _eSounds[i] ].m_hSounds[j] = hSound;
                if (GAME_SOUNDS[ _eSounds[i] ].m_bLoop)
                    CFESoundMgr::bSetSoundProperty(hSound,"Loop",(FEPointer)true);
                j++;
            }
        }while (hSound != NULL);

        m_oSoundDataTable[ _eSounds[i] ].m_uiNumSounds = j;
        i++;
    }
}
//-----------------------------------------------------------------------------
void CSoundHelper::Finish()
{
	#ifdef NO_SOUND
	return;
	#endif
    
    CFESoundPlayer::StopSounds();
}
//-----------------------------------------------------------------------------
FEHandler CSoundHelper::hPlay(EGameSounds _eSound, FEReal _rVol, FEReal _rPan,bool _bLoop)
{
	#ifdef NO_SOUND
		return(NULL);
	#endif

    if (m_oSoundDataTable[_eSound].m_uiNumSounds > 0)
    {
        uint uiIdx = CFEMath::iRand(0,m_oSoundDataTable[_eSound].m_uiNumSounds - 1 );
        CFESoundPlayer::Play(m_oSoundDataTable[_eSound].m_hSounds[uiIdx], SML_FX, _rVol, _rPan);

        return(m_oSoundDataTable[_eSound].m_hSounds[uiIdx]);
    }

    return(NULL);
}
//-----------------------------------------------------------------------------
void CSoundHelper::Stop(FEHandler _hSound)
{
   	#ifdef NO_SOUND
	return;
	#endif

	CFESoundPlayer::Stop(_hSound);
}
//-----------------------------------------------------------------------------
void CSoundHelper::LoadBGM(const CFEString& _sBGM)
{
	#ifdef NO_SOUND
		return;
	#endif

    #ifndef _DS
        
        CFEString sBGM = CFEString("data/sound/bgm/") + _sBGM;
	    m_hBGM = CFESoundMgr::hLoad(sBGM);
	    CFESoundMgr::bSetSoundProperty(m_hBGM,"Loop",(FEPointer)true);
	
	#else
	
	    m_hBGM == NULL;
	
	#endif
}
//-----------------------------------------------------------------------------
void CSoundHelper::PlayBGM()
{
	#ifdef NO_SOUND
		return;
	#endif

	if (m_hBGM != NULL)
	    CFESoundPlayer::Play(m_hBGM,SML_BGM,0.2f,0.5f);
}
//-----------------------------------------------------------------------------
void CSoundHelper::StopBGM()
{
	#ifdef NO_SOUND
	return;
	#endif

    if (m_hBGM != NULL)
	    CFESoundPlayer::Stop(m_hBGM);
}
//-----------------------------------------------------------------------------

