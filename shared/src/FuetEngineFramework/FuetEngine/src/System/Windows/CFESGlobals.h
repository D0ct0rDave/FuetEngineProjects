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
#ifndef CFESGlobalsH
#define CFESGlobalsH
// ----------------------------------------------------------------------------
#include <windows.h>
#include <GL/gl.h>

#include "types/CFEString.h"
#include "FEBasicTypes.h"
#include "FEEnums.h"
// ----------------------------------------------------------------------------
class CRendererHandler;
// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
class CFESystemGlobals
{
    public:

        CFESystemGlobals();

        uint				m_uiLastTick;                   // last time tick the engine timer was queried.

        CRendererHandler*	m_poCurrentRenderer;
        uint				m_uiScrVWidth;					// Screen Virtual Width
        uint				m_uiScrVHeight;					// Screen Virtual Height
        FEReal				m_rCurDepth;					// Current depth to draw primitives.
        bool                m_bNPO2Support;                 // Non Power of 2 texture support availability.

        bool				m_bAutoLocEnabled;				// Is autolocalization enabled or not.
        EFELocalID			m_eLocalID;						// The local ID used by the system.
        
        CFEString			m_sApplicationName;				// The name of the application.
};
// ----------------------------------------------------------------------------
extern CFESystemGlobals FESglobals;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
