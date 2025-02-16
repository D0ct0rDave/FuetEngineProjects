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
#ifndef CFESystemGlobalsH
#define CFESystemGlobalsH
// ----------------------------------------------------------------------------
#include "FEConfig.h"

#if (TARGETPLATFORM == USE_WINDOWS)
    #include "Windows/CFESGlobals.h"

#elif (TARGETPLATFORM == USE_LINUX)
    #include "Linux/CFESGlobals.h"

#elif (TARGETPLATFORM == USE_MACOS)
    #include "MacOS/CFESGlobals.h"

#endif
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
