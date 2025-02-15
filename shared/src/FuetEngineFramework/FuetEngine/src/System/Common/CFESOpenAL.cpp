// ----------------------------------------------------------------------------
/*! \class
 *  \brief OpenAL Sound Implementation.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (SOUND == USE_OPENAL)
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "../CFESystem.h"
// ----------------------------------------------------------------------------
#include <stdlib.h> // free
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
// ----------------------------------------------------------------------------
namespace CFESystem{
namespace Sound{
	int iGetFreeVoice();
};
};

// ----------------------------------------------------------------------------
typedef struct TSoundHandler
{

        ALvoid* m_pData;
        ALenum  m_eFormat;
        ALsizei m_iSize;
        ALfloat m_fFrequency;
        ALuint  m_uiBuffer;
        bool	m_bLoop;
		bool	m_bBGM;

}TSoundHandler;
// ----------------------------------------------------------------------------
typedef struct TSoundVoice
{
    ALuint				m_uiSource;
	FEReal				m_rIniVol;
	bool				m_bPaused;
	TSoundHandler*		m_poSI;
	EFESoundMixerLine	m_eLine;

}TSoundVoice;
// ----------------------------------------------------------------------------
#define MAX_VOICES	64

class CSoundSysInternalData
{
	public:

		// Holds all the currently loaded sounds.
		// CFEArray<TSoundInfo>	m_oSnds;

		// Array of polyphonic voices.
		TSoundVoice m_oHnds[MAX_VOICES];

		// Holds the volume level of each line.
		FEReal m_rLines[SML_NUM_LINES];
};

static CSoundSysInternalData gsoSSID;
// ----------------------------------------------------------------------------
// Function helper that handles localization of data.
// ----------------------------------------------------------------------------
void* pLoadSample(const CFEString& _sFilename,ALenum* _peFormat,ALsizei* _piSize,ALfloat* _pfFrequency)
{
	void*	pData = NULL;
	
	// this loop is a workaround for the alut library weird behaviour	
	for (uint i=0;((i<2) && (pData==NULL));i++)
	{
		if (CFESystem::Local::bIsAutoLocEnabled())
		{
			// try first the non localized or common version of the file.
			CFEString sLocalFilename = CFESystem::Local::sGetLocalID(LID_COMMON) + "/" + _sFilename + ".wav";

			// try to load a sample in memory.
			pData = alutLoadMemoryFromFile(sLocalFilename.szString(),_peFormat,_piSize,_pfFrequency);
		
			if (pData == NULL)
			{		
				// then try the localized version of the file.
				sLocalFilename = CFESystem::Local::sGetLocalID() + "/" + _sFilename + ".wav";	

				// try to load a sample in memory.
				pData = alutLoadMemoryFromFile(sLocalFilename.szString(),_peFormat,_piSize,_pfFrequency);
			}
		}
		else
		{
			CFEString sFilename = _sFilename + ".wav";

			// try to load a sample in memory.
			pData = alutLoadMemoryFromFile(sFilename.szString(),_peFormat,_piSize,_pfFrequency);
		}
	}

	return(pData);
}
// ----------------------------------------------------------------------------
FEHandler CFESystem::Sound::hInit(FEHandler _hParam)
{
    // Init openAL
    ALboolean bRes = alutInit(0, NULL);
    if (bRes == AL_FALSE)
    {
        ALenum eError = alutGetError();
        const char* szError = alutGetErrorString(eError);
        return(NULL);
    }

    ALint iMajor = alutGetMajorVersion();
    ALint iMinor = alutGetMinorVersion();

    /*
    ALCcontext* poContext = alcCreateContext(Device, NULL);
    ALCdevice* poDevice   = alcOpenDevice( (ALchar*)"DirectSound" );
    alcMakeContextCurrent(poContext);

    // Clear Error Code (so we can catch any new errors)
    alGetError();

    // Setup initial listener props
    ALfloat fZero[3] = {0,0,0};
    ALfloat fOr[6]   = {0,0,1,0,1,0};
    alListenerfv(AL_POSITION,fZero);
    alListenerfv(AL_VELOCITY,fZero);
    alListenerfv(AL_ORIENTATION,fOr);
    */

	// alDistanceModel(AL_NONE);
	
	/// Initialize sound lines
	uint i;	
	for (i=0;i<SML_NUM_LINES;i++)
		gsoSSID.m_rLines[i] = _1r;

	// Create voices (sources)
	for (i=0;i<MAX_VOICES;i++)
	{
		ALenum eError;

		gsoSSID.m_oHnds[i].m_poSI    = NULL;
		gsoSSID.m_oHnds[i].m_bPaused = false;
		gsoSSID.m_oHnds[i].m_rIniVol = _1r;

		// Create a sound source
		alGenSources (1, &gsoSSID.m_oHnds[i].m_uiSource);

		// If there is an error...
		eError = alGetError();
		if (eError != AL_NO_ERROR)
		{
			const char* szError = alutGetErrorString(eError);
			return(NULL);
		}
	}

    return((FEHandler)1);
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::Finish()
{
    alutExit();
}
// ----------------------------------------------------------------------------
FEHandler CFESystem::Sound::hLoadSound(const CFEString& _sFilename,bool _bBGM)
{
    ALvoid*     pData = NULL;
    ALenum      eFormat;
    ALsizei     iSize;
    ALfloat     fFrequency;
    ALenum      eError;
	
	// Try loading data.
	pData = pLoadSample(_sFilename,&eFormat,&iSize,&fFrequency);

	// Check if loading was successful.	
	if (pData == NULL)
	{
		// error
		ALenum eError = alGetError();
		const char* szError = alutGetErrorString(eError);
		return(NULL);
	}
    

    // Create a sound buffer
    ALuint uiBuffer;
    alGenBuffers(1, &uiBuffer);
    eError = alGetError();

    // If there is an error...
    eError = alGetError();
    if (eError != AL_NO_ERROR)
    {
        const char* szError = alutGetErrorString(eError);

        //
        free(pData);

        //
        return(NULL);
	}

    // Bind data to buffer
	alBufferData(uiBuffer,eFormat,pData,iSize,(ALsizei)fFrequency);

    // Create and fill a sound info struct.
    TSoundHandler* poSndHnd = new TSoundHandler;
    poSndHnd->m_eFormat   = eFormat;
    poSndHnd->m_iSize     = iSize;
    poSndHnd->m_fFrequency= fFrequency;
    poSndHnd->m_pData     = pData;
    poSndHnd->m_uiBuffer  = uiBuffer;
	poSndHnd->m_bLoop     = false;
	poSndHnd->m_bBGM	  = _bBGM;

    return(poSndHnd);
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::DeleteSound(FEHandler _hSound)
{
    TSoundHandler* poSndHnd = (TSoundHandler*)_hSound;
    alDeleteBuffers(1,&poSndHnd->m_uiBuffer);

    // If there is an error...
	ALenum eError = alGetError();

    if (eError != AL_NO_ERROR)
    {
		delete poSndHnd;    
		return;
    }

    // free data
    if (poSndHnd->m_pData != NULL)
    {
		#pragma message("#################")
		#pragma message("WARNING Memory leak!!!!. The application crashes if we deallocate the data !?!?!?")
		#pragma message("#################")

		// this crashes the app ...	
		// free(poSndHnd->m_pData);
	}

    // delete the handler
    delete poSndHnd;
}
// ----------------------------------------------------------------------------
// Search for an unused voice. Frees unused voices on demand.
// ----------------------------------------------------------------------------
int CFESystem::Sound::iGetFreeVoice()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI == NULL)
			return(i);

		// See if the sound was still being played.
		ALint iState;
		alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

		if (iState != AL_PLAYING)
		{
			gsoSSID.m_oHnds[i].m_poSI = NULL;
			return(i);
		}
	}

	return(-1);
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::PlaySound(FEHandler _hSound,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
	if (_hSound == NULL) return;
	TSoundHandler* poSndHnd = (TSoundHandler*)_hSound;

	// Do not allow bgm files be played over non bgm line. But allow non bgm files to be played over bgm ...
	// for instances for little music wav sequences.
	if (poSndHnd->m_bBGM && (_eSoundLine!= SML_BGM)) return;

	// Retrieve a free voice to play the sound over.
    int iVoice = iGetFreeVoice();
    if (iVoice == -1) return;
	
	gsoSSID.m_oHnds[iVoice].m_poSI = poSndHnd;
	gsoSSID.m_oHnds[iVoice].m_rIniVol = _rVol;
	gsoSSID.m_oHnds[iVoice].m_eLine = _eSoundLine;

    // Bind buffer to source.
    alSourcei(gsoSSID.m_oHnds[iVoice].m_uiSource,AL_BUFFER,poSndHnd->m_uiBuffer);

    // If there is an error...
    ALenum eError = alGetError();
    const char* szError;

    if (eError != AL_NO_ERROR)
    {
		szError = alutGetErrorString(eError);
    }
	else
	{
		// Play the sample
		alSourcei( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_LOOPING, poSndHnd->m_bLoop);
	
		// alSourcef( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_SOURCE_RELATIVE,AL_TRUE);
		
		float fScale = 5.0f;
		float fX = -_1r*((_2r*_rPan)-_1r);

		alSource3f( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_POSITION,fScale*fX,_0r,_0r);
		alSource3f( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_DIRECTION,fX,_0r,_0r);
		alSourcef( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_GAIN,_rVol * gsoSSID.m_rLines[_eSoundLine]);
		alSourcePlay( gsoSSID.m_oHnds[iVoice].m_uiSource );

		ALenum eError = alGetError();
		if (eError != AL_NO_ERROR)
		{
			szError = alutGetErrorString(eError);
		}
	}
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::StopSound(FEHandler _hSound)
{
	if (_hSound == NULL) return;

	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI == (TSoundHandler*)_hSound)
		{
			ALint iState;
			alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

			if (iState == AL_PLAYING)
				alSourceStop(gsoSSID.m_oHnds[i].m_uiSource);

            gsoSSID.m_oHnds[i].m_bPaused = false;
			gsoSSID.m_oHnds[i].m_poSI = NULL;
			return;
		}
	}
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
	gsoSSID.m_rLines[_eSoundLine] = _rVol;
	
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if ((gsoSSID.m_oHnds[i].m_poSI != NULL) && (gsoSSID.m_oHnds[i].m_eLine == _eSoundLine))
		{
			ALint iState;
			alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);
			if (iState == AL_PLAYING)
			{
				
				{
					alSourcef( gsoSSID.m_oHnds[i].m_uiSource, AL_GAIN,gsoSSID.m_oHnds[i].m_rIniVol * gsoSSID.m_rLines[_eSoundLine]);
				}
			}
			else
			{
				// sound stoped playing sometime ago, so we can safely free it...
				gsoSSID.m_oHnds[i].m_bPaused = false;
				gsoSSID.m_oHnds[i].m_poSI = NULL;
			}
		}
	}
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::PauseSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		ALint iState;
		alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

		if (iState == AL_PLAYING)
		{
			gsoSSID.m_oHnds[i].m_bPaused = true;
			alSourceStop(gsoSSID.m_oHnds[i].m_uiSource);
		}
		else
			gsoSSID.m_oHnds[i].m_bPaused = false;
	}
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::StopSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		ALint iState;
		alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

		if (iState == AL_PLAYING)
			alSourceStop(gsoSSID.m_oHnds[i].m_uiSource);

        gsoSSID.m_oHnds[i].m_bPaused = false;
	}
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::ResumeSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_bPaused)
		{
			alSourcePlay(gsoSSID.m_oHnds[i].m_uiSource);
			gsoSSID.m_oHnds[i].m_bPaused = false;
		}
	}
}
// ----------------------------------------------------------------------------
bool CFESystem::Sound::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);

    TSoundHandler* poSH = (TSoundHandler*)_hSound;

    if (_sProperty |= "SampleRate")
    {
        *(uint*)_pParam = (int)poSH->m_fFrequency;
        return (true);
    }

else if (_sProperty |= "Size")
    {
        *(uint*)_pParam = poSH->m_iSize;
        return (true);
    }

    return(false);
}
// ----------------------------------------------------------------------------
bool CFESystem::Sound::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);

    TSoundHandler* poSH = (TSoundHandler*)_hSound;
    if (_sProperty |= "Loop")
    {
        poSH->m_bLoop = (bool)_pParam;
        return(true);
    }

    return(false);
}
// ----------------------------------------------------------------------------
void CFESystem::Sound::Update()
{

}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
