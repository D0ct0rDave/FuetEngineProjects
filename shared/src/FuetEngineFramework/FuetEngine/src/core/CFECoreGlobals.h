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
#ifndef CFECoreGlobalsH
#define CFECoreGlobalsH
// ----------------------------------------------------------------------------
#include "FEConfig.h"

#if (TARGETPLATFORM == USE_DEVELOPMENT)
    #include "Windows/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_WINDOWS)
    #include "Windows/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_LINUX)
    #include "Linux/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_MACOS)
    #include "MacOS/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_DS)
    #include "DS/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_WII)
    #include "Wii/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_N3DS)
    #include "N3DS/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_COCOS2D_X)
    #include "Cocos2D_X/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_ANDROID)
    #include "Android/CFECGlobals.h"

#elif (TARGETPLATFORM == USE_CAFE)
	#include "WiiU/CFECGlobals.h"

#else
	#pragma error("No target platform defined to compile engine")
#endif
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
