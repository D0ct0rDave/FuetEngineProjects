// ----------------------------------------------------------------------------
/*! \class CFE
 *  \brief Sound Manager Class Definition
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_ANDROID)
// ----------------------------------------------------------------------------
#include "../CFECore.h"
#include <stdarg.h>
#include <math.h>

#include "CFECGlobals.h"
#include "FEDefs.h"
#include "types/CFEArray.h"

// for native asset manager
#include <sys/types.h>
#include <jni.h>
#include <android/log.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include "jnihelper.h"

// for native audio
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

// engine interfaces
static SLObjectItf engineObject = NULL;
static SLEngineItf engineEngine;

// output mix interfaces
static SLObjectItf outputMixObject = NULL;
static SLEnvironmentalReverbItf outputMixEnvironmentalReverb = NULL;
// ----------------------------------------------------------------------------
class CSoundDataHandler
{
    public:

        SLDataLocator_Address   m_oLocAddr;
        FEBool                  m_bLoop;
};

class CSoundPlayerHandler
{

    public:

        SLObjectItf         m_poPlayerObject;
        SLPlayItf           m_poPlayerPlay;
        SLSeekItf           m_poPlayerSeek;
        SLVolumeItf         m_poPlayerVolume;

        CSoundDataHandler*  m_poSndDataHnd;
        EFESoundMixerLine   m_eLine;
        FEReal              m_rIniVol;
};

class CSoundSysInternalData
{
	public:

		// Holds all the currently playing sounds.
		CFEArray<CSoundPlayerHandler*>  m_poPlayingSounds;

		// Holds the volume level of each line.
		FEReal                          m_rLines[SML_NUM_LINES];
};

static CSoundSysInternalData gsoSSID;
// ----------------------------------------------------------------------------
float gain_to_attenuation( float gain )
{
    return gain < 0.01F ? -96.0F : 20 * log10( gain );
}
// ----------------------------------------------------------------------------
namespace CFECore{
namespace Sound{

	void DeletePlayingSound(uint _iSound);
};
};
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Sound related stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Initializes the sound system.
FEHandler  CFECore::Sound::hInit(FEHandler _hParam)
{
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
    if (!(SL_RESULT_SUCCESS == result)) return(NULL);

    // realize the engine
    result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
    if (!(SL_RESULT_SUCCESS == result)) return(NULL);

    // get the engine interface, which is needed in order to create other objects
    result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &engineEngine);
    if (!(SL_RESULT_SUCCESS == result)) return(NULL);


    // create output mix, with environmental reverb specified as a non-required interface
    const SLInterfaceID ids[1] = {SL_IID_ENVIRONMENTALREVERB};
    const SLboolean req[1] = {SL_BOOLEAN_FALSE};
    result = (*engineEngine)->CreateOutputMix(engineEngine, &outputMixObject, 1, ids, req);

    if (!(SL_RESULT_SUCCESS == result)) return(NULL);

    // realize the output mix
    result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
    if (!(SL_RESULT_SUCCESS == result)) return(NULL);
    // ignore unsuccessful result codes for environmental reverb, as it is optional for this example

   	// Initialize sound lines
	uint i;
	for (i=0;i<SML_NUM_LINES;i++)
		gsoSSID.m_rLines[i] = _1r;
}
// ----------------------------------------------------------------------------
/// Finalizes the sound system.
void  CFECore::Sound::Finish()
{
    // destroy output mix object, and invalidate all associated interfaces
    if (outputMixObject != NULL) {
        (*outputMixObject)->Destroy(outputMixObject);
        outputMixObject = NULL;
        outputMixEnvironmentalReverb = NULL;
    }

    // destroy engine object, and invalidate all associated interfaces
    if (engineObject != NULL) {
        (*engineObject)->Destroy(engineObject);
        engineObject = NULL;
        engineEngine = NULL;
    }
}
// ----------------------------------------------------------------------------
/// Sets the volume of the given Line to a given level.
void  CFECore::Sound::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
    gsoSSID.m_rLines[_eSoundLine] = _rVol;

    for (uint i=0;i<gsoSSID.m_poPlayingSounds.size();i++)
    {
        CSoundPlayerHandler* poSnd = gsoSSID.m_poPlayingSounds[i];

        if (poSnd->m_eLine == _eSoundLine)
        {
            // Set the volume of the sound....
            FEReal rDBs = gain_to_attenuation(_rVol * gsoSSID.m_rLines[_eSoundLine]);
            (*poSnd->m_poPlayerVolume)->SetVolumeLevel (poSnd->m_poPlayerVolume,(SLmillibel)(rDBs*_100r));
        }
	}
}
// ----------------------------------------------------------------------------
/// Loads a sound indicating if we want to treat it as a BGM or not.
FEHandler  CFECore::Sound::hLoadSound(const CFEString& _sFilename,FEBool _bBGM)
{
    uint uiSize        = 0;
    FEPointer pBuffer  = CFECore::File::pReadFile(_sFilename,&uiSize);
    if (pBuffer == NULL) return(NULL);

    /*
    FEHandler hHnd = CFECore::File::hOpen(_sFilename,FOM_READ);
    if (hHnd == NULL) return(NULL);

    // Android file Handlers are exactly Asset objects
    AAsset* asset = CASTVALUE(AAsset*,hHnd);

    // open asset as file descriptor
    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    if (!(0 <= fd)) return(NULL);
    CFECore::File::Close(hHnd);

    // configure audio source
    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime  = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};
    */

    CSoundDataHandler* poSnd = new CSoundDataHandler;
    poSnd->m_oLocAddr.locatorType = SL_DATALOCATOR_ADDRESS;
    poSnd->m_oLocAddr.pAddress    = pBuffer;
    poSnd->m_oLocAddr.length      = uiSize;
    poSnd->m_bLoop  = false;

    return(poSnd);
}
// ----------------------------------------------------------------------------
/// Deletes a given sound.
void  CFECore::Sound::DeleteSound(FEHandler _hSound)
{
    if (_hSound == NULL) return;
    CSoundDataHandler* poSnd = CASTVALUE(CSoundDataHandler*,_hSound);
    CFECore::Mem::Free( (FEPointer)poSnd->m_oLocAddr.pAddress );
    delete poSnd;
}
// ----------------------------------------------------------------------------
/// Plays a given sound in the given sound line.
void CFECore::Sound::PlaySound(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
    if (_hSound == NULL) return;

    SLresult result;
    CSoundDataHandler* poSH = CASTVALUE(CSoundDataHandler*,_hSound);

   // configure audio source
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&poSH->m_oLocAddr, &format_mime};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject};
    SLDataSink audioSnk = {&loc_outmix, NULL};

    // create audio player
    const SLInterfaceID ids[2] = {SL_IID_VOLUME, SL_IID_SEEK};
    const SLboolean     req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};

    CSoundPlayerHandler* poSnd = new CSoundPlayerHandler;
    poSnd->m_poSndDataHnd = poSH;
    poSnd->m_eLine   = _eSoundLine;
    poSnd->m_rIniVol = _rVol;

    result = (*engineEngine)->CreateAudioPlayer(engineEngine, &poSnd->m_poPlayerObject, &audioSrc, &audioSnk,2, ids, req);
    if (result != SL_RESULT_SUCCESS)
    {
        delete poSnd;
        return;
    }

    // realize the player
    result = (*poSnd->m_poPlayerObject)->Realize(poSnd->m_poPlayerObject, SL_BOOLEAN_FALSE);
    if (result != SL_RESULT_SUCCESS)
    {
        (*poSnd->m_poPlayerObject)->Destroy(poSnd->m_poPlayerObject);
        delete poSnd;
        return;
    }

    // get the play interface
    result = (*poSnd->m_poPlayerObject)->GetInterface(poSnd->m_poPlayerObject, SL_IID_PLAY, &poSnd->m_poPlayerPlay);
    if (result != SL_RESULT_SUCCESS)
    {
        (*poSnd->m_poPlayerObject)->Destroy(poSnd->m_poPlayerObject);
        delete poSnd;
        return;
    }

    // get the volume interface
    result = (*poSnd->m_poPlayerObject)->GetInterface(poSnd->m_poPlayerObject, SL_IID_VOLUME, &poSnd->m_poPlayerVolume);

    // get the seek interface
    result = (*poSnd->m_poPlayerObject)->GetInterface(poSnd->m_poPlayerObject, SL_IID_SEEK, &poSnd->m_poPlayerSeek);

    // loop property
    result = (*poSnd->m_poPlayerSeek)->SetLoop(poSnd->m_poPlayerSeek,poSH->m_bLoop?SL_BOOLEAN_TRUE:SL_BOOLEAN_FALSE, 0, SL_TIME_UNKNOWN);

    // Set the volume of the sound....
    FEReal rDBs = gain_to_attenuation(_rVol * gsoSSID.m_rLines[_eSoundLine]);
    (*poSnd->m_poPlayerVolume)->SetVolumeLevel (poSnd->m_poPlayerVolume,(SLmillibel)(rDBs*_100r));

    // Set the panning of the sound....
    if (_rPan != _05r)
    {
        result = (*poSnd->m_poPlayerVolume)->EnableStereoPosition(poSnd->m_poPlayerVolume,SL_BOOLEAN_TRUE);
        if (result == SL_RESULT_SUCCESS)
        {
            result = (*poSnd->m_poPlayerVolume)->SetStereoPosition(poSnd->m_poPlayerVolume, -32768 + ((FEReal)65535*_rPan) );
        }
    }

    // Set the sound to playing state
    result = (*poSnd->m_poPlayerPlay)->SetPlayState(poSnd->m_poPlayerPlay,SL_PLAYSTATE_PLAYING);
    if (result == SL_RESULT_SUCCESS)
    {
        // add to the playing sound list
        gsoSSID.m_poPlayingSounds.push_back(poSnd);
    }

	return;
}
// ----------------------------------------------------------------------------
/// Stops a given sound.
void  CFECore::Sound::StopSound(FEHandler _hSound)
{
    for (uint i=0;i<gsoSSID.m_poPlayingSounds.size();)
    {
        if (gsoSSID.m_poPlayingSounds[i]->m_poSndDataHnd == _hSound)
        {
            // set the player's state
            SLresult result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->SetPlayState(
                                    gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                                    SL_PLAYSTATE_STOPPED); // SL_PLAYSTATE_PAUSED

            // remove from the list
            DeletePlayingSound(i);
        }
        else
            i++;
    }
}
// ----------------------------------------------------------------------------
/// Stops all the currently playing sounds.
void  CFECore::Sound::StopSounds()
{
    while (gsoSSID.m_poPlayingSounds.size()>0)
    {
        // set the player's state
        SLresult result = (*gsoSSID.m_poPlayingSounds[0]->m_poPlayerPlay)->SetPlayState(
                                    gsoSSID.m_poPlayingSounds[0]->m_poPlayerPlay,
                                    SL_PLAYSTATE_STOPPED);
        DeletePlayingSound(0);
	}
}
// ----------------------------------------------------------------------------
/// Pauses all the currently playing sounds.
void  CFECore::Sound::PauseSounds()
{
    for (uint i=0;i<gsoSSID.m_poPlayingSounds.size();i++)
    {
        SLresult result;
        SLuint32 iState;

        result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->GetPlayState(
                   gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                   &iState);

        if ((iState == SL_PLAYSTATE_PLAYING) && (result == SL_RESULT_SUCCESS))
        {
            // set the player's state
            result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->SetPlayState(
                       gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                       SL_PLAYSTATE_PAUSED); // SL_PLAYSTATE_PAUSED
        }
	}
}
// ----------------------------------------------------------------------------
/// Resumes the playing of the previously paused sounds.
void CFECore::Sound::ResumeSounds()
{
    for (uint i=0;i<gsoSSID.m_poPlayingSounds.size();i++)
    {
        SLuint32 iState;
        SLresult result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->GetPlayState(
                            gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                            &iState);

        if ((iState == SL_PLAYSTATE_PAUSED) && (result == SL_RESULT_SUCCESS))
        {
            // set the player's state
            result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->SetPlayState(
                       gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                       SL_PLAYSTATE_PLAYING); // SL_PLAYSTATE_PAUSED
        }
	}
}
// ----------------------------------------------------------------------------
/// Performs an update step over the sound system, once per application frame.
// ----------------------------------------------------------------------------
void CFECore::Sound::Update()
{
	for (uint i=0;i<gsoSSID.m_poPlayingSounds.size();)
    {
        SLuint32 iState;
        SLresult result = (*gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay)->GetPlayState(
                            gsoSSID.m_poPlayingSounds[i]->m_poPlayerPlay,
                            &iState);

        if ((iState == SL_PLAYSTATE_STOPPED) && (result == SL_RESULT_SUCCESS))
        {
            // remove from the list
            DeletePlayingSound(i);
        }
        else
            i++;
	}
}
// ----------------------------------------------------------------------------
void CFECore::Sound::DeletePlayingSound(uint _iSound)
{
    CSoundPlayerHandler* poHnd = gsoSSID.m_poPlayingSounds[_iSound];
    (*poHnd->m_poPlayerObject)->Destroy(poHnd->m_poPlayerObject);
    gsoSSID.m_poPlayingSounds.Delete(_iSound);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
FEBool  CFECore::Sound::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);

    CSoundDataHandler* poSH = CASTVALUE(CSoundDataHandler*,_hSound);
    if (_sProperty |= "Size")
    {
        *(uint*)_pParam = poSH->m_oLocAddr.length;
        return (true);
    }

	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);

    CSoundDataHandler* poSH = CASTVALUE(CSoundDataHandler*,_hSound);
    if (_sProperty   |= "Loop")
    {
        poSH->m_bLoop = (FEBool)_pParam;
        return(true);
    }

    return(false);
}
// ----------------------------------------------------------------------------
#endif // (TARGETPLATFORM == USE_ANDROID)
// ---------------------------------------------------------------------------
