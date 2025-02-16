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

#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CRendererHandler;
// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
class CFESystemGlobals
{
    public:

        CFESystemGlobals();

        uint   m_uiStartTime;                               // system time at the start of the engine.
        FEReal m_rSystemTime;                               // current system time

        CRendererHandler*	m_poCurrentRenderer;
        uint				m_uiScrVWidth;					// Screen Virtual Width
        uint				m_uiScrVHeight;					// Screen Virtual Height
        FEReal				m_rCurDepth;					// Current depth to draw primitives.
        bool                m_bNPO2Support;                 // Non Power of 2 texture support availability.
};
// ----------------------------------------------------------------------------
extern CFESystemGlobals FESglobals;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
