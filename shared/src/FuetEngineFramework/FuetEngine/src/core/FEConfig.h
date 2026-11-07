// ----------------------------------------------------------------------------
/*! \class Fuet Engine Configuration File
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEConfigH
#define FEConfigH
// ----------------------------------------------------------------------------
// CONFIG
#include "FEConfigOptions.h"

#if defined(DEVELOPMENT)

    #define TARGETPLATFORM  USE_DEVELOPMENT
    #include "Windows/FECConfig_Dev.h"

#elif defined(LINUX)
    #define TARGETPLATFORM  USE_LINUX
    #include "Linux/FECConfig.h"

#elif defined(MACOS)
    #define TARGETPLATFORM  USE_MACOS
    #include "MacOS/FECConfig.h"

#elif defined(DS)

	#define TARGETPLATFORM  USE_DS
	#include "DS/FECConfig.h"

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

#else

#pragma error("No platform defined")

#endif

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
