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

#if defined(WIN32)
    #define TARGETPLATFORM  USE_WINDOWS
    #include "Windows/FESConfig.h"

#elif defined(LINUX)
    #define TARGETPLATFORM  USE_LINUX
    #include "Linux/FESConfig.h"

#elif defined(MACOS)
    #define TARGETPLATFORM  USE_MACOS
    #include "MacOS/FESConfig.h"

#elif defined(DS)
    #define TARGETPLATFORM  USE_DS
    #include "DS/FESConfig.h"
#endif

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
