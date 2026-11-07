// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (TARGETPLATFORM == USE_LINUX)

#include <time.h>
#include "CFECGlobals.h"
// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
CFECoreGlobals FECglobals;
// ----------------------------------------------------------------------------
CFECoreGlobals::CFECoreGlobals() :
			m_uiLastTick(0),
			m_poCurrentRenderer(NULL),
			m_rCurDepth(0.0f),
			m_bNPO2Support(false),
			m_bAutoLocEnabled(false),
			m_eLocalID(LID_ES),
			m_eLocalRegionID((EFELocalRegionID)0),
			m_uiApplicationVersion(0),

			m_bKeepDesignAspect(false),
			m_uiViewportWidth(0),
			m_uiViewportHeight(0),
			m_uiViewportWMargin(0),
			m_uiViewportHMargin(0)
{
    timespec oTime;
    clock_gettime(CLOCK_MONOTONIC,&oTime);
    m_uiLastTick = (uint)(oTime.tv_sec*1000ULL + oTime.tv_nsec/1000000ULL);   // system time at the start of the engine.
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
