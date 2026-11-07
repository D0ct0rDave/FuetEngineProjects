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
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_LINUX)
// ----------------------------------------------------------------------------

#include <GL/glx.h>
#include <GL/gl.h>

#include <X11/X.h>
#include <X11/keysym.h>

// timing functionality
#include <sys/time.h>

// ----------------------------------------------------------------------------
// Global variables for the system module
#include "CFECGlobals.h"
CFECystemGlobals FECglobals;

// ----------------------------------------------------------------------------
CFECystemGlobals::CFECystemGlobals() :
            m_poCurrentRenderer(NULL),
            m_uiScrVWidth(640),
            m_uiScrVHeight(480)
{
    timeval oTime;
    if (gettimeofday(&oTime,NULL) == 0)
    {
        //
        m_ullStartTime = oTime.tv_sec*1000000 + oTime.tv_usec;
    }
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
