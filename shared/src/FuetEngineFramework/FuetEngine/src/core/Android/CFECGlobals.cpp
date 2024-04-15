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
#if (TARGETPLATFORM == USE_ANDROID)

// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
#include "CFECGlobals.h"
CFECoreGlobals FECglobals;

// ----------------------------------------------------------------------------
CFECoreGlobals::CFECoreGlobals() :

            m_bViewportRotated(false),
            m_uiViewportWidth(0),
            m_uiViewportHeight(0),
            m_uiViewportWMargin(0),
            m_uiViewportHMargin(0),

			m_poCurrentRenderer(NULL),
            m_bAutoLocEnabled(false)
{
    // m_uiLastTick  = GetTickCount();    // system time at the start of the engine.
    m_eLocalID    = LID_ES;
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
