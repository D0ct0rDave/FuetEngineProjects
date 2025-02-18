// ----------------------------------------------------------------------------
/*! \class
 *  \brief OpenAL Sound Implementation.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (SOUND == USE_OPENAL)
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEDefs.h"
#include "../CFECore.h"
// ----------------------------------------------------------------------------
#include <stdlib.h> // free
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <AL/efx.h>
#include <AL/alext.h>
#include <AL/efx-presets.h>

// ----------------------------------------------------------------------------
// Effect object functions
// ----------------------------------------------------------------------------
void alGenEffects_FAKED(ALsizei, ALuint*) {};
void alDeleteEffects_FAKED(ALsizei, const ALuint*) {};
ALboolean alIsEffect_FAKED(ALuint) { return(AL_FALSE); };
void alEffecti_FAKED(ALuint, ALenum, ALint) {};
void alEffectiv_FAKED(ALuint, ALenum, const ALint*) {};
void alEffectf_FAKED(ALuint, ALenum, ALfloat) {};
void alEffectfv_FAKED(ALuint, ALenum, const ALfloat*) {};
void alGetEffecti_FAKED(ALuint, ALenum, ALint*) {};
void alGetEffectiv_FAKED(ALuint, ALenum, ALint*) {};
void alGetEffectf_FAKED(ALuint, ALenum, ALfloat*) {};
void alGetEffectfv_FAKED(ALuint, ALenum, ALfloat*) {};

// Auxiliary Effect Slot object function types.
void alGenAuxiliaryEffectSlots_FAKED(ALsizei, ALuint*) {};
void alDeleteAuxiliaryEffectSlots_FAKED(ALsizei, const ALuint*) {};
ALboolean alIsAuxiliaryEffectSlot_FAKED(ALuint) { return (AL_FALSE); };
void alAuxiliaryEffectSloti_FAKED(ALuint, ALenum, ALint) {};
void alAuxiliaryEffectSlotiv_FAKED(ALuint, ALenum, const ALint*) {};
void alAuxiliaryEffectSlotf_FAKED(ALuint, ALenum, ALfloat) {};
void alAuxiliaryEffectSlotfv_FAKED(ALuint, ALenum, const ALfloat*) {};
void alGetAuxiliaryEffectSloti_FAKED(ALuint, ALenum, ALint*) {};
void alGetAuxiliaryEffectSlotiv_FAKED(ALuint, ALenum, ALint*) {};
void alGetAuxiliaryEffectSlotf_FAKED(ALuint, ALenum, ALfloat*) {};
void alGetAuxiliaryEffectSlotfv_FAKED(ALuint, ALenum, ALfloat*) {};

// Auxiliary Effect Slot object functions
static LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = alGenAuxiliaryEffectSlots_FAKED;
static LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = alDeleteAuxiliaryEffectSlots_FAKED;
static LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = alIsAuxiliaryEffectSlot_FAKED;
static LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = alAuxiliaryEffectSloti_FAKED;
static LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = alAuxiliaryEffectSlotiv_FAKED;
static LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = alAuxiliaryEffectSlotf_FAKED;
static LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = alAuxiliaryEffectSlotfv_FAKED;
static LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti = alGetAuxiliaryEffectSloti_FAKED;
static LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv = alGetAuxiliaryEffectSlotiv_FAKED;
static LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf = alGetAuxiliaryEffectSlotf_FAKED;
static LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv = alGetAuxiliaryEffectSlotfv_FAKED;

// Effect object functions
static LPALGENEFFECTS alGenEffects = alGenEffects_FAKED;
static LPALDELETEEFFECTS alDeleteEffects = alDeleteEffects_FAKED;
static LPALISEFFECT alIsEffect = alIsEffect_FAKED;
static LPALEFFECTI alEffecti = alEffecti_FAKED;
static LPALEFFECTIV alEffectiv = alEffectiv_FAKED;
static LPALEFFECTF alEffectf = alEffectf_FAKED;
static LPALEFFECTFV alEffectfv = alEffectfv_FAKED;
static LPALGETEFFECTI alGetEffecti = alGetEffecti_FAKED;
static LPALGETEFFECTIV alGetEffectiv = alGetEffectiv_FAKED;
static LPALGETEFFECTF alGetEffectf = alGetEffectf_FAKED;
static LPALGETEFFECTFV alGetEffectfv = alGetEffectfv_FAKED;

static EFXEAXREVERBPROPERTIES REVERB_TYPES[] = 
{
	EFX_REVERB_PRESET_GENERIC,
	EFX_REVERB_PRESET_PADDEDCELL,
	EFX_REVERB_PRESET_ROOM,
	EFX_REVERB_PRESET_BATHROOM,
	EFX_REVERB_PRESET_LIVINGROOM,
    EFX_REVERB_PRESET_STONEROOM,
	EFX_REVERB_PRESET_AUDITORIUM,
    EFX_REVERB_PRESET_CONCERTHALL,
    EFX_REVERB_PRESET_CAVE,
	EFX_REVERB_PRESET_ARENA,
	EFX_REVERB_PRESET_HANGAR,
    EFX_REVERB_PRESET_CARPETEDHALLWAY,
    EFX_REVERB_PRESET_HALLWAY,
    EFX_REVERB_PRESET_STONECORRIDOR,
    EFX_REVERB_PRESET_ALLEY,
    EFX_REVERB_PRESET_FOREST,
    EFX_REVERB_PRESET_CITY,
    EFX_REVERB_PRESET_MOUNTAINS,
	EFX_REVERB_PRESET_QUARRY,
    EFX_REVERB_PRESET_PLAIN,
    EFX_REVERB_PRESET_PARKINGLOT,
    EFX_REVERB_PRESET_SEWERPIPE,
    EFX_REVERB_PRESET_UNDERWATER,
    EFX_REVERB_PRESET_DRUGGED,
    EFX_REVERB_PRESET_DIZZY,
    EFX_REVERB_PRESET_PSYCHOTIC,

	// Castle Presets
	EFX_REVERB_PRESET_CASTLE_SMALLROOM,
    EFX_REVERB_PRESET_CASTLE_SHORTPASSAGE,
	EFX_REVERB_PRESET_CASTLE_MEDIUMROOM,
	EFX_REVERB_PRESET_CASTLE_LARGEROOM,
	EFX_REVERB_PRESET_CASTLE_LONGPASSAGE,
	EFX_REVERB_PRESET_CASTLE_HALL,
	EFX_REVERB_PRESET_CASTLE_CUPBOARD,
	EFX_REVERB_PRESET_CASTLE_COURTYARD,
	EFX_REVERB_PRESET_CASTLE_ALCOVE,
	
	// Factory Presets
	EFX_REVERB_PRESET_FACTORY_SMALLROOM,
	EFX_REVERB_PRESET_FACTORY_SHORTPASSAGE,
	EFX_REVERB_PRESET_FACTORY_MEDIUMROOM,
	EFX_REVERB_PRESET_FACTORY_LARGEROOM,
	EFX_REVERB_PRESET_FACTORY_LONGPASSAGE,
	EFX_REVERB_PRESET_FACTORY_HALL,
	EFX_REVERB_PRESET_FACTORY_CUPBOARD,
	EFX_REVERB_PRESET_FACTORY_COURTYARD,
	EFX_REVERB_PRESET_FACTORY_ALCOVE,
	
	// Ice Palace Presets
	EFX_REVERB_PRESET_ICEPALACE_SMALLROOM,
	EFX_REVERB_PRESET_ICEPALACE_SHORTPASSAGE,
	EFX_REVERB_PRESET_ICEPALACE_MEDIUMROOM,
	EFX_REVERB_PRESET_ICEPALACE_LARGEROOM,
	EFX_REVERB_PRESET_ICEPALACE_LONGPASSAGE,
	EFX_REVERB_PRESET_ICEPALACE_HALL,
	EFX_REVERB_PRESET_ICEPALACE_CUPBOARD,
	EFX_REVERB_PRESET_ICEPALACE_COURTYARD,
	EFX_REVERB_PRESET_ICEPALACE_ALCOVE,

	// Space Station Presets
	EFX_REVERB_PRESET_SPACESTATION_SMALLROOM,
	EFX_REVERB_PRESET_SPACESTATION_SHORTPASSAGE,
	EFX_REVERB_PRESET_SPACESTATION_MEDIUMROOM,
	EFX_REVERB_PRESET_SPACESTATION_LARGEROOM,
	EFX_REVERB_PRESET_SPACESTATION_LONGPASSAGE,
	EFX_REVERB_PRESET_SPACESTATION_HALL,
	EFX_REVERB_PRESET_SPACESTATION_CUPBOARD,
	EFX_REVERB_PRESET_SPACESTATION_ALCOVE,

	// Wooden Galleon Presets
	EFX_REVERB_PRESET_WOODEN_SMALLROOM,
	EFX_REVERB_PRESET_WOODEN_SHORTPASSAGE,
	EFX_REVERB_PRESET_WOODEN_MEDIUMROOM,
	EFX_REVERB_PRESET_WOODEN_LARGEROOM,
	EFX_REVERB_PRESET_WOODEN_LONGPASSAGE,
	EFX_REVERB_PRESET_WOODEN_HALL,
	EFX_REVERB_PRESET_WOODEN_CUPBOARD,
	EFX_REVERB_PRESET_WOODEN_COURTYARD,
	EFX_REVERB_PRESET_WOODEN_ALCOVE,

	// Sports Presets
	EFX_REVERB_PRESET_SPORT_EMPTYSTADIUM,
	EFX_REVERB_PRESET_SPORT_SQUASHCOURT,
	EFX_REVERB_PRESET_SPORT_SMALLSWIMMINGPOOL,
	EFX_REVERB_PRESET_SPORT_LARGESWIMMINGPOOL,
	EFX_REVERB_PRESET_SPORT_GYMNASIUM,
	EFX_REVERB_PRESET_SPORT_FULLSTADIUM,
	EFX_REVERB_PRESET_SPORT_STADIUMTANNOY,

	// Prefab Presets
	EFX_REVERB_PRESET_PREFAB_WORKSHOP,
	EFX_REVERB_PRESET_PREFAB_SCHOOLROOM,
	EFX_REVERB_PRESET_PREFAB_PRACTISEROOM,
	EFX_REVERB_PRESET_PREFAB_OUTHOUSE,
	EFX_REVERB_PRESET_PREFAB_CARAVAN,

	// Dome and Pipe Presets
	EFX_REVERB_PRESET_DOME_TOMB,
	EFX_REVERB_PRESET_PIPE_SMALL,
	EFX_REVERB_PRESET_DOME_SAINTPAULS,
	EFX_REVERB_PRESET_PIPE_LONGTHIN,
	EFX_REVERB_PRESET_PIPE_LARGE,
	EFX_REVERB_PRESET_PIPE_RESONANT,

	// Outdoors Presets
	EFX_REVERB_PRESET_OUTDOORS_BACKYARD,
	EFX_REVERB_PRESET_OUTDOORS_ROLLINGPLAINS,
	EFX_REVERB_PRESET_OUTDOORS_DEEPCANYON,
	EFX_REVERB_PRESET_OUTDOORS_CREEK,
	EFX_REVERB_PRESET_OUTDOORS_VALLEY,

	// Mood Presets
	EFX_REVERB_PRESET_MOOD_HEAVEN,
	EFX_REVERB_PRESET_MOOD_HELL,
	EFX_REVERB_PRESET_MOOD_MEMORY,

	// Driving Presets
	EFX_REVERB_PRESET_DRIVING_COMMENTATOR,
	EFX_REVERB_PRESET_DRIVING_PITGARAGE,
	EFX_REVERB_PRESET_DRIVING_INCAR_RACER,
	EFX_REVERB_PRESET_DRIVING_INCAR_SPORTS,
	EFX_REVERB_PRESET_DRIVING_INCAR_LUXURY,
	EFX_REVERB_PRESET_DRIVING_FULLGRANDSTAND,
	EFX_REVERB_PRESET_DRIVING_EMPTYGRANDSTAND,
	EFX_REVERB_PRESET_DRIVING_TUNNEL,

	// City Presets

	EFX_REVERB_PRESET_CITY_STREETS,
	EFX_REVERB_PRESET_CITY_SUBWAY,
	EFX_REVERB_PRESET_CITY_MUSEUM,
	EFX_REVERB_PRESET_CITY_LIBRARY,
	EFX_REVERB_PRESET_CITY_UNDERPASS,
	EFX_REVERB_PRESET_CITY_ABANDONED,

	// Misc. Presets
	EFX_REVERB_PRESET_DUSTYROOM,
	EFX_REVERB_PRESET_CHAPEL,
	EFX_REVERB_PRESET_SMALLWATERROOM
};
// ----------------------------------------------------------------------------
/* LoadEffect loads the given reverb properties into a new OpenAL effect
 * object, and returns the new effect ID. */
static FEBool bLoadEffect(const EFXEAXREVERBPROPERTIES *reverb,ALuint effect)
{
    if(alGetEnumValue("AL_EFFECT_EAXREVERB") != 0)
    {
        CFECore::Log::Print("SOUND: Using EAX Reverb\n");

		// EAX Reverb is available. Set the EAX effect type then load the
        // reverb properties.
        alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_EAXREVERB);

        alEffectf(effect, AL_EAXREVERB_DENSITY, reverb->flDensity);
        alEffectf(effect, AL_EAXREVERB_DIFFUSION, reverb->flDiffusion);
        alEffectf(effect, AL_EAXREVERB_GAIN, reverb->flGain);
        alEffectf(effect, AL_EAXREVERB_GAINHF, reverb->flGainHF);
        alEffectf(effect, AL_EAXREVERB_GAINLF, reverb->flGainLF);
        alEffectf(effect, AL_EAXREVERB_DECAY_TIME, reverb->flDecayTime);
        alEffectf(effect, AL_EAXREVERB_DECAY_HFRATIO, reverb->flDecayHFRatio);
        alEffectf(effect, AL_EAXREVERB_DECAY_LFRATIO, reverb->flDecayLFRatio);
        alEffectf(effect, AL_EAXREVERB_REFLECTIONS_GAIN, reverb->flReflectionsGain);
        alEffectf(effect, AL_EAXREVERB_REFLECTIONS_DELAY, reverb->flReflectionsDelay);
        alEffectfv(effect, AL_EAXREVERB_REFLECTIONS_PAN, reverb->flReflectionsPan);
        alEffectf(effect, AL_EAXREVERB_LATE_REVERB_GAIN, reverb->flLateReverbGain);
        alEffectf(effect, AL_EAXREVERB_LATE_REVERB_DELAY, reverb->flLateReverbDelay);
        alEffectfv(effect, AL_EAXREVERB_LATE_REVERB_PAN, reverb->flLateReverbPan);
        alEffectf(effect, AL_EAXREVERB_ECHO_TIME, reverb->flEchoTime);
        alEffectf(effect, AL_EAXREVERB_ECHO_DEPTH, reverb->flEchoDepth);
        alEffectf(effect, AL_EAXREVERB_MODULATION_TIME, reverb->flModulationTime);
        alEffectf(effect, AL_EAXREVERB_MODULATION_DEPTH, reverb->flModulationDepth);
        alEffectf(effect, AL_EAXREVERB_AIR_ABSORPTION_GAINHF, reverb->flAirAbsorptionGainHF);
        alEffectf(effect, AL_EAXREVERB_HFREFERENCE, reverb->flHFReference);
        alEffectf(effect, AL_EAXREVERB_LFREFERENCE, reverb->flLFReference);
        alEffectf(effect, AL_EAXREVERB_ROOM_ROLLOFF_FACTOR, reverb->flRoomRolloffFactor);
        alEffecti(effect, AL_EAXREVERB_DECAY_HFLIMIT, reverb->iDecayHFLimit);
    }
    else
    {
        CFECore::Log::Print("SOUND: Using Standard Reverb\n");

        // No EAX Reverb. Set the standard reverb effect type then load the
        // available reverb properties.
        alEffecti(effect, AL_EFFECT_TYPE, AL_EFFECT_REVERB);

        alEffectf(effect, AL_REVERB_DENSITY, reverb->flDensity);
        alEffectf(effect, AL_REVERB_DIFFUSION, reverb->flDiffusion);
        alEffectf(effect, AL_REVERB_GAIN, reverb->flGain);
        alEffectf(effect, AL_REVERB_GAINHF, reverb->flGainHF);
        alEffectf(effect, AL_REVERB_DECAY_TIME, reverb->flDecayTime);
        alEffectf(effect, AL_REVERB_DECAY_HFRATIO, reverb->flDecayHFRatio);
        alEffectf(effect, AL_REVERB_REFLECTIONS_GAIN, reverb->flReflectionsGain);
        alEffectf(effect, AL_REVERB_REFLECTIONS_DELAY, reverb->flReflectionsDelay);
        alEffectf(effect, AL_REVERB_LATE_REVERB_GAIN, reverb->flLateReverbGain);
        alEffectf(effect, AL_REVERB_LATE_REVERB_DELAY, reverb->flLateReverbDelay);
        alEffectf(effect, AL_REVERB_AIR_ABSORPTION_GAINHF, reverb->flAirAbsorptionGainHF);
        alEffectf(effect, AL_REVERB_ROOM_ROLLOFF_FACTOR, reverb->flRoomRolloffFactor);
        alEffecti(effect, AL_REVERB_DECAY_HFLIMIT, reverb->iDecayHFLimit);
    }
	
	ALenum err = alGetError();
    if(err != AL_NO_ERROR)
    {
        CFECore::Log::Print("OpenAL error: %s\n", alGetString(err));
		return(false);
	}

	return(true);
}

static ALuint LoadEffect(const EFXEAXREVERBPROPERTIES *reverb)
{
    ALuint effect = 0;

    // Create the effect object and check if we can do EAX reverb.
    alGenEffects(1, &effect);
	if (effect != 0)
	{
		// Check if an error occured, and clean up if so.
		if (! bLoadEffect(reverb,effect))
		{
			alDeleteEffects(1, &effect);
			effect =0;
		}
	}
	else
	{
		CFECore::Log::Print("OpenAL error: Unable to create effect\n");
	}
    
	return effect;
}
// ----------------------------------------------------------------------------
// EFX extensions
// -------------------------------------------------------
void LoadExtensions()
{
	#define RETRIEVE_EXTENSION(FUNCNAME,FUNCPROTOTYPE)\
	FUNCNAME = (FUNCPROTOTYPE)alGetProcAddress(#FUNCNAME);\
	if (FUNCNAME == NULL) FUNCNAME = FUNCNAME##_FAKED;

	RETRIEVE_EXTENSION(alGenEffects,LPALGENEFFECTS);
	RETRIEVE_EXTENSION(alDeleteEffects,LPALDELETEEFFECTS);
	RETRIEVE_EXTENSION(alIsEffect,LPALISEFFECT);
	RETRIEVE_EXTENSION(alEffecti,LPALEFFECTI);
	RETRIEVE_EXTENSION(alEffectiv,LPALEFFECTIV);
	RETRIEVE_EXTENSION(alEffectf,LPALEFFECTF);
	RETRIEVE_EXTENSION(alEffectfv,LPALEFFECTFV);
	RETRIEVE_EXTENSION(alGetEffecti,LPALGETEFFECTI);
	RETRIEVE_EXTENSION(alGetEffectiv,LPALGETEFFECTIV);
	RETRIEVE_EXTENSION(alGetEffectf,LPALGETEFFECTF);
	RETRIEVE_EXTENSION(alGetEffectfv,LPALGETEFFECTFV);
		
	RETRIEVE_EXTENSION(alGenAuxiliaryEffectSlots,LPALGENAUXILIARYEFFECTSLOTS);
	RETRIEVE_EXTENSION(alDeleteAuxiliaryEffectSlots,LPALDELETEAUXILIARYEFFECTSLOTS);
	RETRIEVE_EXTENSION(alIsAuxiliaryEffectSlot,LPALISAUXILIARYEFFECTSLOT);
	RETRIEVE_EXTENSION(alAuxiliaryEffectSloti,LPALAUXILIARYEFFECTSLOTI);
	RETRIEVE_EXTENSION(alAuxiliaryEffectSlotiv,LPALAUXILIARYEFFECTSLOTIV);
	RETRIEVE_EXTENSION(alAuxiliaryEffectSlotf,LPALAUXILIARYEFFECTSLOTF);
	RETRIEVE_EXTENSION(alAuxiliaryEffectSlotfv,LPALAUXILIARYEFFECTSLOTFV);
	RETRIEVE_EXTENSION(alGetAuxiliaryEffectSloti,LPALGETAUXILIARYEFFECTSLOTI);
	RETRIEVE_EXTENSION(alGetAuxiliaryEffectSlotiv,LPALGETAUXILIARYEFFECTSLOTIV);
	RETRIEVE_EXTENSION(alGetAuxiliaryEffectSlotf,LPALGETAUXILIARYEFFECTSLOTF);
	RETRIEVE_EXTENSION(alGetAuxiliaryEffectSlotfv,LPALGETAUXILIARYEFFECTSLOTFV);
}
// ----------------------------------------------------------------------------
typedef struct TSoundHandler
{

        ALvoid* m_pData;
        ALenum  m_eFormat;
        ALsizei m_iSize;
        ALfloat m_fFrequency;
        ALuint  m_uiBuffer;
		ALsizei m_iSamples;
        FEBool	m_bLoop;
		FEBool	m_bBGM;
		FEReal	m_rPBPos;	// Playback position

		#ifdef _DEBUG
		CFEString m_sFilename;
		#endif

}TSoundHandler;
// ----------------------------------------------------------------------------
typedef struct TSoundVoice
{
    ALuint				m_uiSource;
	FEReal				m_rIniVol;
	TSoundHandler*		m_poSI;
	EFESoundMixerLine	m_eLine;
	ALuint				m_uiEffect;

}TSoundVoice;
// ----------------------------------------------------------------------------
#define MAX_VOICES	64
class CSoundSysInternalData
{
	public:
		CSoundSysInternalData():
			m_bEvFXEnabled(false),
			m_uiEffect(0),
			m_uiAuxFX(0),
			m_rGlobalAttFact(_1r)
		{
		}

		// Holds all the currently loaded sounds.
		// CFEArray<TSoundInfo>	m_oSnds;

		// Array of polyphonic voices.
		TSoundVoice m_oHnds[MAX_VOICES];

		// Holds the volume level of each line.
		FEReal m_rLines[SML_NUM_LINES];

		//
		FEBool  m_bEvFXEnabled;
		ALuint	m_uiEffect;
		ALuint	m_uiAuxFX;
		FEReal  m_rGlobalAttFact;
};
// ----------------------------------------------------------------------------
namespace CFECore{
namespace Sound{

	int iGetFreeVoice();
	static CSoundSysInternalData gsoSSID;
};
};
// ----------------------------------------------------------------------------
// Function helper that handles localization of data.
// ----------------------------------------------------------------------------
void* pLoadSample(const CFEString& _sFilename,ALenum* _peFormat,ALsizei* _piSize,ALfloat* _pfFrequency)
{
	void*	pData = NULL;
	uint uiSize = 0;
	FEPointer pFileBuffer = CFECore::File::pReadFile(_sFilename+".wav",&uiSize);

	// Now try to load the file into AL
	if (pFileBuffer != NULL)
	{
		// this loop is a workaround for the alut library weird behaviour	
		for (uint i=0;((i<2) && (pData==NULL));i++)
		{	
			pData  = alutLoadMemoryFromFileImage(pFileBuffer,uiSize,_peFormat,_piSize,_pfFrequency);
		}

		CFECore::Mem::Free(pFileBuffer);
	}

	return(pData);
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Sound::hInit(FEHandler _hParam)
{
    // Init openAL
    ALboolean bRes = alutInit(0, NULL);
    if (bRes == AL_FALSE)
    {
        ALenum eError = alutGetError();
        const char* szError = alutGetErrorString(eError);
        return(NULL);
    }

    ALCcontext *ctx   = NULL;
	ALCdevice *device = NULL;
	ctx = alcGetCurrentContext();

    if(ctx != NULL)
	{
	    ALint iMajor = alutGetMajorVersion();
		ALint iMinor = alutGetMinorVersion();

		device = alcGetContextsDevice(ctx);
		CFECore::Log::Print("OpenAL %d.%d %s driver.\n",iMajor,iMinor,alcGetString(device, ALC_DEVICE_SPECIFIER));
	}

	// alDistanceModel(AL_NONE);
	
	/// Initialize sound lines
	uint i;	
	for (i=0;i<SML_NUM_LINES;i++)
		gsoSSID.m_rLines[i] = _1r;

	// Create voices (sources)
	ALenum eError;
	for (i=0;i<MAX_VOICES;i++)
	{
		gsoSSID.m_oHnds[i].m_poSI    = NULL;
		gsoSSID.m_oHnds[i].m_rIniVol = _1r;

		// Create a sound source
		alGenSources (1, &gsoSSID.m_oHnds[i].m_uiSource);

		// If there is an error...
		eError = alGetError();
		if (eError != AL_NO_ERROR)
		{
			const char* szError = alutGetErrorString(eError);
			CFECore::Log::Print("ERROR: %s\n",szError);
			return(NULL);
		}
	}

	ALfloat listenerPos[]={0.0,0.0,0.0};
	ALfloat listenerVel[]={0.0,0.0,0.0};
	ALfloat listenerOri[]={0.0,0.0,-1.0, 0.0,1.0,0.0};
	// Position ...
	
	alListenerfv(AL_POSITION,listenerPos);
	if ((eError = alGetError()) != AL_NO_ERROR)
	{
		const char* szError = alutGetErrorString(eError);
		CFECore::Log::Print("ERROR: %s\n",szError);
		return(NULL);
	}
	// Velocity ...
	alListenerfv(AL_VELOCITY,listenerVel);
	if ((eError = alGetError()) != AL_NO_ERROR)
	{
		const char* szError = alutGetErrorString(eError);
		CFECore::Log::Print("ERROR: %s\n",szError);
		return(NULL);
	}
	// Orientation ...
	alListenerfv(AL_ORIENTATION,listenerOri);
	if ((eError = alGetError()) != AL_NO_ERROR)
	{
		const char* szError = alutGetErrorString(eError);
		CFECore::Log::Print("ERROR: %s\n",szError);
		return(NULL);
	}

	alDistanceModel(AL_NONE); 
	// alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

	// -------------------------------------------------------
	if(!alcIsExtensionPresent(device, "ALC_EXT_EFX"))
		CFECore::Log::Print("EFX not supported\n");
	else
		LoadExtensions();
	
	EFXEAXREVERBPROPERTIES reverb = EFX_REVERB_PRESET_GENERIC;
	gsoSSID.m_uiEffect = LoadEffect(&reverb);
	
	// Create the effect slot object. This is what "plays" an effect on sources
    // that connect to it.
    alGenAuxiliaryEffectSlots(1, &gsoSSID.m_uiAuxFX);

	// Tell the effect slot to use the loaded effect object. Note that the this
    // effectively copies the effect properties. You can modify or delete the
    // effect object afterward without affecting the effect slot.
    alAuxiliaryEffectSloti(gsoSSID.m_uiAuxFX, AL_EFFECTSLOT_EFFECT, gsoSSID.m_uiEffect);
    if ((eError = alGetError()) != AL_NO_ERROR)
	{
		CFECore::Log::Print("Failed to set effect slot\n");
	}

    return((FEHandler)1);
}
// ----------------------------------------------------------------------------
void CFECore::Sound::Finish()
{
    alDeleteAuxiliaryEffectSlots(1, &gsoSSID.m_uiAuxFX);
    alDeleteEffects(1, &gsoSSID.m_uiEffect);

    alutExit();
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Sound::hLoadSound(const CFEString& _sFilename,FEBool _bBGM)
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
		eError = alGetError();
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

	ALint iBits,iChannels;
	alGetBufferi(uiBuffer, AL_BITS, &iBits);
	alGetBufferi(uiBuffer, AL_CHANNELS, &iChannels);

    // Create and fill a sound info struct.
    TSoundHandler* poSndHnd = new TSoundHandler;
    poSndHnd->m_eFormat   = eFormat;
    poSndHnd->m_iSize     = iSize;
    poSndHnd->m_fFrequency= fFrequency;
    poSndHnd->m_pData     = pData;
    poSndHnd->m_uiBuffer  = uiBuffer;
	poSndHnd->m_bLoop     = false;
	poSndHnd->m_bBGM	  = _bBGM;
	poSndHnd->m_rPBPos	  = _0r;
	poSndHnd->m_iSamples  = iSize / (iChannels*(iBits>>3));

	#ifdef _DEBUG
	poSndHnd->m_sFilename = _sFilename;
	#endif

    return(poSndHnd);
}
// ----------------------------------------------------------------------------
void CFECore::Sound::DeleteSound(FEHandler _hSound)
{
	if (_hSound==NULL) return;

    // first of all stop all the voices playing this sounds...
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI == _hSound)
		{
			// we can do this always safely (see OpenAL 1.1 specification page 43)
			alSourceStop(gsoSSID.m_oHnds[i].m_uiSource);
			gsoSSID.m_oHnds[i].m_poSI = NULL;
		}
	}

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
int CFECore::Sound::iGetFreeVoice()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI == NULL)
			return(i);
	}

	return(-1);
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Sound::hPlaySound(FEHandler _hSoundResource,EFESoundMixerLine _eSoundLine,FEReal _rVol,FEReal _rPan)
{
	if (_hSoundResource == NULL) return(NULL);
	TSoundHandler* poSndHnd = (TSoundHandler*)_hSoundResource;

	// Do not allow bgm files be played over non bgm line. But allow non bgm files to be played over bgm ...
	// for instances for little music wav sequences.
	if (poSndHnd->m_bBGM && (_eSoundLine!= SML_BGM)) return(NULL);

	// Retrieve a free voice to play the sound over.
    int iVoice = iGetFreeVoice();
    if (iVoice == -1) return(NULL);

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
		// Loop it?
		alSourcei( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_LOOPING, poSndHnd->m_bLoop);
		
		// Playback position
		if (poSndHnd->m_rPBPos != _0r)
		{
			ALint iPos = (ALint)(poSndHnd->m_rPBPos * (FEReal)poSndHnd->m_iSamples);
			alSourcei( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_SAMPLE_OFFSET, iPos);
		}
		
		// Play the sample
		CFEVect2 oDir = CFEVect2::X();
		oDir.Rotate((_1r-_rPan)*_PIr);
		
		if (gsoSSID.m_bEvFXEnabled && (_eSoundLine == SML_ENVIRONMENT))
		{
			alSource3i(gsoSSID.m_oHnds[iVoice].m_uiSource, AL_AUXILIARY_SEND_FILTER, gsoSSID.m_uiAuxFX, 0, AL_FILTER_NULL);
		}
		else
		{
			alSource3i(gsoSSID.m_oHnds[iVoice].m_uiSource, AL_AUXILIARY_SEND_FILTER, AL_EFFECTSLOT_NULL, 0, AL_FILTER_NULL);
		}

		// 04/02/2015: IMPORTANT NOTE: If panning is not properly achieved check control panel
		// 3D sound (or Advanced Sound or DTS audio) properties. Enhace sound etc can be a pain in the ass
		// trying to figure out what's happening.
		alSource3f( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_POSITION, oDir.x,_0r,oDir.y);

		alSourcef( gsoSSID.m_oHnds[iVoice].m_uiSource, AL_GAIN,_rVol * gsoSSID.m_rLines[_eSoundLine] * gsoSSID.m_rGlobalAttFact);
		alSourcePlay( gsoSSID.m_oHnds[iVoice].m_uiSource );

		eError = alGetError();
		if (eError != AL_NO_ERROR)
		{
			szError = alutGetErrorString(eError);
		}
	}

	return((FEHandler)&gsoSSID.m_oHnds[iVoice]);
}
// ----------------------------------------------------------------------------
void CFECore::Sound::StopSound(FEHandler _hSound)
{
	if (_hSound == NULL) return;
	TSoundVoice* poVoice = (TSoundVoice*)_hSound;

	// if the sound was already stopoped, return.
 	if (poVoice->m_poSI == NULL) return;

	// we can do this always safely (see OpenAL 1.1 specification page 43)
	alSourceStop(poVoice->m_uiSource);
	poVoice->m_poSI = NULL;
}
// ----------------------------------------------------------------------------
void CFECore::Sound::SetLineLevel(EFESoundMixerLine _eSoundLine,FEReal _rVol)
{
	gsoSSID.m_rLines[_eSoundLine] = _rVol;
	
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if ((gsoSSID.m_oHnds[i].m_poSI != NULL) && (gsoSSID.m_oHnds[i].m_eLine == _eSoundLine))
		{
			alSourcef( gsoSSID.m_oHnds[i].m_uiSource, AL_GAIN,gsoSSID.m_oHnds[i].m_rIniVol * gsoSSID.m_rLines[_eSoundLine]);
		}
	}
}
// ----------------------------------------------------------------------------
void CFECore::Sound::PauseSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI != NULL)
		{
			ALint iState;
			alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

			if (iState == AL_PLAYING)
				alSourcePause(gsoSSID.m_oHnds[i].m_uiSource);
		}
	}
}
// ----------------------------------------------------------------------------
void CFECore::Sound::ResumeSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI != NULL)
		{
			ALint iState;
			alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

			if (iState == AL_PAUSED)
				alSourcePlay(gsoSSID.m_oHnds[i].m_uiSource);
		}
	}
}
// ----------------------------------------------------------------------------
void CFECore::Sound::StopSounds()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI != NULL)
		{
			// we can do this always safely (see OpenAL 1.1 specification page 43)
			alSourceStop(gsoSSID.m_oHnds[i].m_uiSource);
			gsoSSID.m_oHnds[i].m_poSI = NULL;
		}
	}
}
// ----------------------------------------------------------------------------
void CFECore::Sound::Update()
{
	for (uint i=0;i<MAX_VOICES;i++)
	{
		if (gsoSSID.m_oHnds[i].m_poSI != NULL)
		{
			ALint iState;
			alGetSourcei(gsoSSID.m_oHnds[i].m_uiSource, AL_SOURCE_STATE, &iState);

			// See if the sound was still being played.
			if ((iState != AL_PLAYING) && (iState != AL_PAUSED))
				gsoSSID.m_oHnds[i].m_poSI = NULL;
		}
	}
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    // if (_pParam == NULL) return(false);

    if (_sProperty |= "ENABLE_ENV_LINE_REVERB")
    {
		gsoSSID.m_bEvFXEnabled = CASTVAR(FEBool,_pParam);
        return (true);
    }
else if (_sProperty |= "ENV_LINE_REVERB_LEVEL")
    {
		FEReal rVal = CASTVAR(FEReal,_pParam);
		// control dry / wet balance
		alAuxiliaryEffectSlotf(gsoSSID.m_uiAuxFX,AL_EFFECTSLOT_GAIN, rVal);
		return (true);
    }
else if (_sProperty |= "ENV_LINE_REVERB_TYPE")
    {
		uint uiType = CASTVAR(uint,_pParam);
		uint uiNumTypes = sizeof(REVERB_TYPES) / sizeof(EFXEAXREVERBPROPERTIES);

		if (uiType >= uiNumTypes) uiType = 0;
		bLoadEffect(&REVERB_TYPES[uiType],gsoSSID.m_uiEffect);
	}
else if (_sProperty |= "GLOBAL_ATTENUATION_FACTOR")
    {
		FEReal rVal = CASTVAR(FEReal,_pParam);
		gsoSSID.m_rGlobalAttFact = rVal;
	}
else if (_sProperty |= "")
    {
        return (true);
    }

	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);
	
	// --------------------------
	// Sound resource properties
	// --------------------------
    if (_sProperty |= "SampleRate")
    {
		TSoundHandler* poSH = (TSoundHandler*)_hSound;
		*(uint*)_pParam = (int)poSH->m_fFrequency;
        
		return (true);
    }

else if (_sProperty |= "Size")
    {
		TSoundHandler* poSH = (TSoundHandler*)_hSound;
		*(uint*)_pParam = (int)poSH->m_iSize;
        return (true);
    }

	// --------------------------
	// Sound properties
	// --------------------------
	/*
	...
	TSoundVoice* poVoice = (TSoundVoice*)_hSound;
	*/
    return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Sound::bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hSound == NULL) return(false);
    if (_pParam == NULL) return(false);
	
	// --------------------------
	// Sound resource properties
	// --------------------------
    if (_sProperty |= "Loop")
    {	
		TSoundHandler* poSH = (TSoundHandler*)_hSound;
        poSH->m_bLoop = (FEBool)_pParam;
        return(true);
    }
else if (_sProperty |= "PlaybackPos")
    {
		TSoundHandler* poSH = (TSoundHandler*)_hSound;
		// in factor (0..1) 0 beginning, 1:end
		poSH->m_rPBPos = *((FEReal*)&_pParam);
        return(true);
    }
	// --------------------------
	// Sound properties
	// --------------------------
else if (_sProperty |= "Panning")
	{
		TSoundVoice* poVoice = (TSoundVoice*)_hSound;
		
		FEReal rPan = *((FEReal*)&_pParam);
		
		CFEVect2 oDir = CFEVect2::X();
		oDir.Rotate((_1r-rPan)*_PIr);

		alSource3f( poVoice->m_uiSource, AL_POSITION, oDir.x,_0r,oDir.y);
	}
	// --------------------------
	// Sound properties
	// --------------------------
else if (_sProperty |= "Volume")
	{
		TSoundVoice* poVoice = (TSoundVoice*)_hSound;

		FEReal rVol = *((FEReal*)&_pParam);
		alSourcef( poVoice->m_uiSource, AL_GAIN,rVol * gsoSSID.m_rLines[poVoice->m_eLine]);
	}
    return(false);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------