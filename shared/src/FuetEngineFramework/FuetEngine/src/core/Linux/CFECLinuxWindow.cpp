// ----------------------------------------------------------------------------
/*! \brief Linux (X11/GLX) window and OpenGL context support.
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if ((GRAPHICS == USE_OPENGL) && (TARGETPLATFORM == USE_LINUX))
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <GL/glx.h>

#include "FECLinuxWindow.h"
// ----------------------------------------------------------------------------
struct TFELinuxGLContext
{
    Display*    m_poDisplay;
    Window      m_oWin;
    GLXContext  m_oGLRC;
    bool        m_bOwnsWindow;      // The engine created the window (and the display connection).
    Atom        m_oWMDelete;
};
// ----------------------------------------------------------------------------
static TFELinuxWindow       gsoMainWindow = { NULL, 0 };
static TFELinuxGLContext*   gspoFirstCtx  = NULL;
// ----------------------------------------------------------------------------
static const unsigned int DEFAULT_WIN_WIDTH  = 1024;
static const unsigned int DEFAULT_WIN_HEIGHT = 768;
// ----------------------------------------------------------------------------
static XVisualInfo* ChooseOwnVisual(Display* _poDpy)
{
    // From the most to the least demanding.
    static int aiAttrs[][13] =
    {
        { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 24, GLX_STENCIL_SIZE, 8, None },
        { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, None },
        { GLX_RGBA, GLX_DEPTH_SIZE, 16, None },
    };

    for (unsigned int i=0;i<sizeof(aiAttrs)/sizeof(aiAttrs[0]);i++)
    {
        XVisualInfo* poVI = glXChooseVisual(_poDpy,DefaultScreen(_poDpy),aiAttrs[i]);
        if (poVI != NULL) return(poVI);
    }
    return(NULL);
}
// ----------------------------------------------------------------------------
TFELinuxGLContext* FELinux_CreateContext(const TFELinuxWindow* _poWindow,unsigned int* _puiWidth,unsigned int* _puiHeight)
{
    TFELinuxGLContext* poCtx = new TFELinuxGLContext;
    poCtx->m_poDisplay   = NULL;
    poCtx->m_oWin        = 0;
    poCtx->m_oGLRC       = NULL;
    poCtx->m_bOwnsWindow = (_poWindow == NULL);
    poCtx->m_oWMDelete   = 0;

    XVisualInfo* poVI = NULL;

    if (_poWindow != NULL)
    {
        // Host window: we can't change its visual, so we build the context for it.
        poCtx->m_poDisplay = (Display*)_poWindow->m_pDisplay;
        poCtx->m_oWin      = (Window)_poWindow->m_ulWindow;

        XWindowAttributes oAttrs;
        if (XGetWindowAttributes(poCtx->m_poDisplay,poCtx->m_oWin,&oAttrs) == 0)
        {
            fprintf(stderr,"FuetEngine: invalid X11 window handler\n");
            delete poCtx;
            return(NULL);
        }

        XVisualInfo oTemplate;
        oTemplate.visualid = XVisualIDFromVisual(oAttrs.visual);
        int iCount = 0;
        poVI = XGetVisualInfo(poCtx->m_poDisplay,VisualIDMask,&oTemplate,&iCount);
    }
    else
    {
        poCtx->m_poDisplay = XOpenDisplay(NULL);
        if (poCtx->m_poDisplay == NULL)
        {
            fprintf(stderr,"FuetEngine: could not open X display (is DISPLAY set?)\n");
            delete poCtx;
            return(NULL);
        }
        poVI = ChooseOwnVisual(poCtx->m_poDisplay);
    }

    if (poVI == NULL)
    {
        fprintf(stderr,"FuetEngine: no suitable OpenGL visual found\n");
        if (poCtx->m_bOwnsWindow) XCloseDisplay(poCtx->m_poDisplay);
        delete poCtx;
        return(NULL);
    }

    // Share objects (textures, display lists) with the first context created.
    poCtx->m_oGLRC = glXCreateContext(poCtx->m_poDisplay,poVI,(gspoFirstCtx != NULL)?gspoFirstCtx->m_oGLRC:NULL,True);
    if (poCtx->m_oGLRC == NULL)
    {
        fprintf(stderr,"FuetEngine: could not create GLX rendering context\n");
        XFree(poVI);
        if (poCtx->m_bOwnsWindow) XCloseDisplay(poCtx->m_poDisplay);
        delete poCtx;
        return(NULL);
    }

    if (poCtx->m_bOwnsWindow)
    {
        Window oRoot = RootWindow(poCtx->m_poDisplay,poVI->screen);

        XSetWindowAttributes oSWA;
        oSWA.colormap     = XCreateColormap(poCtx->m_poDisplay,oRoot,poVI->visual,AllocNone);
        oSWA.border_pixel = 0;
        oSWA.event_mask   = ExposureMask | StructureNotifyMask;

        poCtx->m_oWin = XCreateWindow(  poCtx->m_poDisplay,oRoot,0,0,
                                        DEFAULT_WIN_WIDTH,DEFAULT_WIN_HEIGHT,0,
                                        poVI->depth,InputOutput,poVI->visual,
                                        CWBorderPixel | CWColormap | CWEventMask,&oSWA);

        XStoreName(poCtx->m_poDisplay,poCtx->m_oWin,"FuetEngine");

        // Be notified (instead of killed) when the user closes the window.
        poCtx->m_oWMDelete = XInternAtom(poCtx->m_poDisplay,"WM_DELETE_WINDOW",False);
        XSetWMProtocols(poCtx->m_poDisplay,poCtx->m_oWin,&poCtx->m_oWMDelete,1);

        XMapWindow(poCtx->m_poDisplay,poCtx->m_oWin);
    }
    XFree(poVI);

    if (! glXMakeCurrent(poCtx->m_poDisplay,poCtx->m_oWin,poCtx->m_oGLRC))
    {
        fprintf(stderr,"FuetEngine: glXMakeCurrent failed\n");
        FELinux_DestroyContext(poCtx);
        return(NULL);
    }

    if (gspoFirstCtx == NULL)
    {
        gspoFirstCtx = poCtx;
        gsoMainWindow.m_pDisplay = poCtx->m_poDisplay;
        gsoMainWindow.m_ulWindow = (unsigned long)poCtx->m_oWin;
    }

    FELinux_GetWindowSize(poCtx,_puiWidth,_puiHeight);
    return(poCtx);
}
// ----------------------------------------------------------------------------
void FELinux_DestroyContext(TFELinuxGLContext* _poCtx)
{
    if (_poCtx == NULL) return;

    glXMakeCurrent(_poCtx->m_poDisplay,None,NULL);

    if (_poCtx->m_oGLRC != NULL)
        glXDestroyContext(_poCtx->m_poDisplay,_poCtx->m_oGLRC);

    if (_poCtx->m_bOwnsWindow)
    {
        if (_poCtx->m_oWin != 0) XDestroyWindow(_poCtx->m_poDisplay,_poCtx->m_oWin);
        XCloseDisplay(_poCtx->m_poDisplay);
    }

    if (gspoFirstCtx == _poCtx)
    {
        gspoFirstCtx = NULL;
        gsoMainWindow.m_pDisplay = NULL;
        gsoMainWindow.m_ulWindow = 0;
    }
    delete _poCtx;
}
// ----------------------------------------------------------------------------
void FELinux_MakeCurrent(TFELinuxGLContext* _poCtx)
{
    if (_poCtx != NULL)
        glXMakeCurrent(_poCtx->m_poDisplay,_poCtx->m_oWin,_poCtx->m_oGLRC);
}
// ----------------------------------------------------------------------------
void FELinux_SwapBuffers(TFELinuxGLContext* _poCtx)
{
    if (_poCtx != NULL)
        glXSwapBuffers(_poCtx->m_poDisplay,_poCtx->m_oWin);
}
// ----------------------------------------------------------------------------
void* FELinux_GetProcAddress(const char* _szName)
{
    return( (void*)glXGetProcAddressARB((const GLubyte*)_szName) );
}
// ----------------------------------------------------------------------------
void FELinux_GetWindowSize(TFELinuxGLContext* _poCtx,unsigned int* _puiWidth,unsigned int* _puiHeight)
{
    XWindowAttributes oAttrs;
    if ((_poCtx != NULL) && (XGetWindowAttributes(_poCtx->m_poDisplay,_poCtx->m_oWin,&oAttrs) != 0))
    {
        *_puiWidth  = (unsigned int)oAttrs.width;
        *_puiHeight = (unsigned int)oAttrs.height;
    }
    else
    {
        *_puiWidth  = DEFAULT_WIN_WIDTH;
        *_puiHeight = DEFAULT_WIN_HEIGHT;
    }
}
// ----------------------------------------------------------------------------
void FELinux_PumpEvents(TFELinuxGLContext* _poCtx)
{
    // Host windows have their events processed by the host.
    if ((_poCtx == NULL) || (! _poCtx->m_bOwnsWindow)) return;

    while (XPending(_poCtx->m_poDisplay) > 0)
    {
        XEvent oEvent;
        XNextEvent(_poCtx->m_poDisplay,&oEvent);

        if ((oEvent.type == ClientMessage) && ((Atom)oEvent.xclient.data.l[0] == _poCtx->m_oWMDelete))
        {
            // The user closed the window.
            FELinux_DestroyContext(_poCtx);
            exit(0);
        }
    }
}
// ----------------------------------------------------------------------------
const TFELinuxWindow* FELinux_GetMainWindow()
{
    return( (gsoMainWindow.m_pDisplay != NULL)?&gsoMainWindow:NULL );
}
// ----------------------------------------------------------------------------
bool FELinux_bIsKeyDown(const TFELinuxWindow* _poWindow,unsigned long _ulKeySym)
{
    if (_poWindow == NULL) return(false);

    Display* poDpy = (Display*)_poWindow->m_pDisplay;
    KeyCode  oCode = XKeysymToKeycode(poDpy,(KeySym)_ulKeySym);
    if (oCode == 0) return(false);

    char acKeys[32];
    XQueryKeymap(poDpy,acKeys);
    return( (acKeys[oCode >> 3] & (1 << (oCode & 7))) != 0 );
}
// ----------------------------------------------------------------------------
void FELinux_GetMouse(const TFELinuxWindow* _poWindow,int* _piX,int* _piY,bool* _pbLeft,bool* _pbRight,bool* _pbMiddle)
{
    *_piX = *_piY = 0;
    *_pbLeft = *_pbRight = *_pbMiddle = false;
    if (_poWindow == NULL) return;

    Window       oRoot,oChild;
    int          iRX,iRY;
    unsigned int uiMask = 0;

    if (XQueryPointer((Display*)_poWindow->m_pDisplay,(Window)_poWindow->m_ulWindow,&oRoot,&oChild,&iRX,&iRY,_piX,_piY,&uiMask))
    {
        *_pbLeft   = (uiMask & Button1Mask) != 0;
        *_pbMiddle = (uiMask & Button2Mask) != 0;
        *_pbRight  = (uiMask & Button3Mask) != 0;
    }
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
