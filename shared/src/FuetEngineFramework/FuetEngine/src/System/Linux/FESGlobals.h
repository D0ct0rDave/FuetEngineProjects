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
#ifndef FESGlobalsH
#define FESGlobalsH
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
class CFESystemGlobals
{
    public:
        CSystemGlobals() :
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

        unsigned long long  m_ullStartTime;             // system time at the start of the engine.

        CRendererHandler*   m_poCurrentRenderer;
        uint                m_uiScrVWidth;              // Current Screen Virtual Width
        uint                m_uiScrVHeight;             // Current Screen Virtual Height
};
// ----------------------------------------------------------------------------
extern CFESystemGlobals globals;
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
