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
#ifndef CFECGlobalsH
#define CFECGlobalsH
// ----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CRendererHandler;
// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
class CFECoreGlobals
{
    public:

        CFECoreGlobals();

        uint				m_uiLastTick;                   // last time tick the engine timer was queried.

        CRendererHandler*	m_poCurrentRenderer;
        FEReal				m_rCurDepth;					// Current depth to draw primitives.
        FEBool              m_bNPO2Support;					// Non Power of 2 texture support availability.

        FEBool				m_bAutoLocEnabled;				// Is autolocalization enabled or not.
        EFELocalID			m_eLocalID;						// The local ID used by the system.
        EFELocalRegionID	m_eLocalRegionID;				// The local ID used by the system.
        
        CFEString			m_sApplicationName;				// The name of the application.
		uint				m_uiApplicationVersion;			// The version of the application 
		
		FEBool				m_bKeepDesignAspect;			// Keep design aspect ratio o stretch to fit.
        uint                m_uiViewportWidth;
        uint                m_uiViewportHeight;
        uint                m_uiViewportWMargin;
        uint                m_uiViewportHMargin;
};
// ----------------------------------------------------------------------------
extern CFECoreGlobals FECglobals;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
