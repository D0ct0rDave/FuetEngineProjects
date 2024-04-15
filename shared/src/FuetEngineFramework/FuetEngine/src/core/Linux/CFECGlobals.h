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
#ifndef CFECGlobalsH
#define CFECGlobalsH
// ----------------------------------------------------------------------------
#include <GL/glx.h>
#include <GL/gl.h>

#include <X11/X.h>
#include <X11/keysym.h>
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:
        CRendererHandler() :
            m_poVI(NULL),
            m_uiScrWidth(0),
            m_uiScrHeight(0)
        {
        }

        XVisualInfo*        m_poVI;
        Window              m_oWin;
        GLXContext          m_hGLRC;
        Display*            m_poDisplay;

		uint                m_uiScrWidth;               // Actual Screen Width
        uint                m_uiScrHeight;              // Actual Screen Height
};
// ----------------------------------------------------------------------------
// Global variables for the system module
// ----------------------------------------------------------------------------
class CFECystemGlobals
{
    public:
        CFECystemGlobals();

        unsigned long long  m_ullStartTime;             // system time at the start of the engine.

        CRendererHandler*   m_poCurrentRenderer;
        uint                m_uiScrVWidth;              // Current Screen Virtual Width
        uint                m_uiScrVHeight;             // Current Screen Virtual Height
		FEReal				m_rCurDepth;				// Current depth to draw primitives.
};
// ----------------------------------------------------------------------------
extern CFECystemGlobals FECglobals;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
