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
#if (TARGETPLATFORM == USE_WINDOWS)

#include "CFESGlobals.h"

// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
#include "CFESGlobals.h"
CFESystemGlobals FESglobals;

// ----------------------------------------------------------------------------
CFESystemGlobals::CFESystemGlobals() :
            m_poCurrentRenderer(NULL),
			m_rCurDepth(0.0f),
            m_uiScrVWidth(640),
            m_uiScrVHeight(480),
            m_uiLastTick(0),
            m_bAutoLocEnabled(false)
{
    m_uiLastTick  = GetTickCount();    // system time at the start of the engine.
    m_eLocalID    = LID_ES;
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
