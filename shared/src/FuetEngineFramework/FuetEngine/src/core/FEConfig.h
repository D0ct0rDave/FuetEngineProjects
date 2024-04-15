// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEConfigH
#define FEConfigH
// ----------------------------------------------------------------------------
// CONFIG
#include "FEConfigOptions.h"

#if defined(PLAYTEST)

    #define TARGETPLATFORM  USE_PLAYTEST
    #include "Windows/FECConfig_PlayTest.h"

#elif defined(DEVELOPMENT)

    #define TARGETPLATFORM  USE_DEVELOPMENT
    #include "Windows/FECConfig_Dev.h"

#elif defined(DEVELOPMENT_WITH_SOUND)

    #define TARGETPLATFORM  USE_DEVELOPMENT
    #include "Windows/FECConfig_DevWithSound.h"

#elif defined(LINUX)
    #define TARGETPLATFORM  USE_LINUX
    #include "Linux/FECConfig.h"

#elif defined(MACOS)
    #define TARGETPLATFORM  USE_MACOS
    #include "MacOS/FECConfig.h"

#elif defined(DS)

	#if defined(HYBRID_DSi)

		#define TARGETPLATFORM  USE_DS
	    #include "DS/FECConfig.h"

	#elif !defined(SDK_TWL)

	    #define TARGETPLATFORM  USE_DS
	    #include "DS/FECConfig.h"

	#else

	    #define TARGETPLATFORM  USE_DS
	    #include "DSi/FECConfig.h"

	#endif

#elif defined(WII)
    #define TARGETPLATFORM  USE_WII
    #include "Wii/FECConfig.h"

#elif defined(N3DS)
    #define TARGETPLATFORM  USE_N3DS
    #include "N3DS/FECConfig.h"

#elif defined(COCOS2D_X)

    #define TARGETPLATFORM  USE_COCOS2D_X
    #include "Cocos2D_X/FECConfig.h"

// order important: COCOS2D_X needs to be placed before WIN32
#elif defined(WIN32)

    #define TARGETPLATFORM  USE_WINDOWS
    #include "Windows/FECConfig.h"

#elif defined(ANDROID)

    #define TARGETPLATFORM  USE_ANDROID
    #include "Android/FECConfig.h"

#elif defined(WIIU)

	#define TARGETPLATFORM	USE_CAFE
	#include "WiiU\FECConfig.h"

#else

#pragma error("No platform defined")

#endif

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
