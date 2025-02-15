// ----------------------------------------------------------------------------
/*! \class
 *  \brief OpenGL Linux specific implementation.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if ((GRAPHICS == USE_OPENGL) && (TARGETPLATFORM == USE_LINUX))
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glx.h>
#include <GL/gl.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"
#include "CFESGlobals.h"

static int snglBuf[] = {GLX_RGBA, GLX_DEPTH_SIZE, 16, None};
static int dblBuf[]  = {GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None};
GLboolean  doubleBuffer = GL_TRUE;

// ----------------------------------------------------------------------------

void fatalError(char *message)
{
  fprintf(stderr, "main: %s\n", message);
  exit(1);
}
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();
#else
#define CHECKERROR()
#endif
// ----------------------------------------------------------------------------
// Rendering Context Creation Flags
#define OP_DOUBLE_BUFFERED 0x00000001
#define OP_GDI               0x00000002
#define OP_STEREO          0x00000004

// ----------------------------------------------------------------------------

typedef struct TRCResult
{
    Display*    m_poDisplay;
    Window      m_oWin;
    GLXContext  m_oGLRC;
};

// ----------------------------------------------------------------------------
TRCResult CreateRenderingContext(unsigned int Options,unsigned int ColorDepth,unsigned int StencilBits)
{
    TRCResult            oRes;
    XVisualInfo*         vi;
    Colormap             cmap;
    XSetWindowAttributes swa;
    XEvent               event;
    GLboolean            needRedraw = GL_FALSE, recalcModelView = GL_TRUE;
    int                  dummy;

    oRes.m_poDisplay = XOpenDisplay(NULL);
    if (oRes.m_poDisplay == NULL)
    fatalError("could not open display");

    /*** (2) make sure OpenGL's GLX extension supported ***/
    if(!glXQueryExtension(oRes.m_poDisplay, &dummy, &dummy))
    fatalError("X server has no OpenGL GLX extension");

    /*** (3) find an appropriate visual ***/
    /* find an OpenGL-capable RGB visual with depth buffer */
    vi = glXChooseVisual(oRes.m_poDisplay, DefaultScreen(oRes.m_poDisplay), dblBuf);
    if (vi == NULL)
    {
        vi = glXChooseVisual(oRes.m_poDisplay, DefaultScreen(oRes.m_poDisplay), snglBuf);
        if (vi == NULL) fatalError("no RGB visual with depth buffer");
        doubleBuffer = GL_FALSE;
    }

    /*
    if(vi->class != TrueColor)
        fatalError("TrueColor visual required for this program");
    */

    /*** (4) create an OpenGL rendering context  ***/

    /* create an OpenGL rendering context */
    oRes.m_oGLRC = glXCreateContext(oRes.m_poDisplay, vi, /* no shared dlists */ None,
                        /* direct rendering if possible */ GL_TRUE);
    if (oRes.m_oGLRC== NULL)
    fatalError("could not create rendering context");

    /*** (5) create an X window with the selected visual ***/
    /* create an X colormap since probably not using default visual */
    cmap = XCreateColormap(oRes.m_poDisplay, RootWindow(oRes.m_poDisplay, vi->screen), vi->visual, AllocNone);

    swa.colormap     = cmap;
    swa.border_pixel = 0;
    swa.event_mask   = KeyPressMask  | ExposureMask | PointerMotionMask
                                     | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

    oRes.m_oWin = XCreateWindow(oRes.m_poDisplay, RootWindow(oRes.m_poDisplay, vi->screen), 0, 0,
                                640, 480, 0, vi->depth, InputOutput, vi->visual,
                                CWBorderPixel | CWColormap | CWEventMask, &swa);

    XSetStandardProperties(oRes.m_poDisplay, oRes.m_oWin, "main", "main", None,NULL, 0, NULL);


    /*** (7) request the X window to be displayed on the screen ***/
    XMapWindow(oRes.m_poDisplay, oRes.m_oWin);

    return(oRes);
}
// -----------------------------------------------------------------------------
void DestroyRenderingContext()
{
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
FEHandler CFESystem::Graphics::hInit(FEHandler _hParam)
{
    CRendererHandler* poRnd = new CRendererHandler;

    poRnd->m_poVI = (XVisualInfo*)_hParam;

//    poRnd->m_hWnd = ((HWND)_hParam);
//    poRnd->m_hDC  = GetDC(poRnd->m_hWnd);

    TRCResult oRes      = CreateRenderingContext(OP_DOUBLE_BUFFERED,32,0);
    poRnd->m_oWin       = oRes.m_oWin;
    poRnd->m_hGLRC      = oRes.m_oGLRC;
    poRnd->m_poDisplay  = oRes.m_poDisplay;

    CHECKERROR();

    // ClientToScreen( poRnd->m_hWnd, &poRnd->m_oScrOfs);
    // GetWindowRect(poRnd->m_hWnd,&oRect);
    poRnd->m_uiScrWidth  = 640;
    poRnd->m_uiScrHeight = 480;

    // Initialize Matrix Transforms
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Setup screen virtual dimensions
    glEnable(GL_TEXTURE_2D);
    CHECKERROR();
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
    CHECKERROR();

    glEnable(GL_BLEND);
    CHECKERROR();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    CHECKERROR();

    glEnable(GL_ALPHA_TEST);
    CHECKERROR();

    glAlphaFunc(GL_GREATER,0.0f);
    CHECKERROR();

    glClearColor(0,0,0,1);
    CHECKERROR();

    glClearDepth(1.0f);
    CHECKERROR();

    glClearStencil(0);
    CHECKERROR();

    glDisable(GL_DEPTH_TEST);
    CHECKERROR();

    glDepthFunc(GL_ALWAYS);
    CHECKERROR();

    glEnable(GL_NORMALIZE);
    CHECKERROR();

    glShadeModel(GL_SMOOTH);
    CHECKERROR();

    glViewport(0, 0, poRnd->m_uiScrWidth,poRnd->m_uiScrHeight);
    CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECKERROR();

    glCullFace(GL_BACK);
    CHECKERROR();

    glDisable(GL_LIGHTING);
    CHECKERROR();

    return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::Finish(FEHandler _hRenderer)
{
    Check(_hRenderer != NULL,"NULL Renderer Handler");
    DestroyRenderingContext() ;
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::BeginScene(FEHandler _hRenderer)
{
    if (FESglobals.m_poCurrentRenderer != _hRenderer)
    {
        // In case the renderer is different ...
        FESglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;
        glXMakeCurrent( FESglobals.m_poCurrentRenderer->m_poDisplay,
                        FESglobals.m_poCurrentRenderer->m_oWin,
                        FESglobals.m_poCurrentRenderer->m_hGLRC);
    }

    // Setup screen virtual dimensions transform matrix.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-1.0f,+1.0f,0.0f);
    glScalef    ( 2.0f/(float)FESglobals.m_uiScrVWidth,-2.0f/(float)FESglobals.m_uiScrVHeight,1.0f);
    CHECKERROR();

    // Clear buffers
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::EndScene()
{
    glFinish();
    glXSwapBuffers( FESglobals.m_poCurrentRenderer->m_poDisplay,
                    FESglobals.m_poCurrentRenderer->m_oWin);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
