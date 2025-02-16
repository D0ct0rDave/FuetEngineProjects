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
#if ((GRAPHICS == USE_OPENGL) && (TARGETPLATFORM == USE_WINDOWS))
// ----------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>

#include "../CFESystem.h"

#include "FEBasicTypes.h"
#include "CFESGlobals.h"
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:
        CRendererHandler() :	m_hDC(NULL),
								m_hWnd(NULL),
								m_hGLRC(NULL),
								m_uiScrWidth(0),
								m_uiScrHeight(0)
		{
		};


        HDC     m_hDC;                      // Display Contex
        HWND    m_hWnd;                     // Window handler
        HGLRC   m_hGLRC;                    // GL Rendering Context
        
        POINT   m_oScrOfs;                  // Screen offset
        uint    m_uiScrWidth;               // Actual Screen Width
        uint    m_uiScrHeight;              // Actual Screen Height
};

// ----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();
#else
#define CHECKERROR()
#endif
// ----------------------------------------------------------------------------
// defined in OpenGL common source file.
int isExtensionSupported(const char *extension);
// ----------------------------------------------------------------------------
// Rendering Context Creation Flags
#define OP_DOUBLE_BUFFERED 0x00000001
#define OP_GDI               0x00000002
#define OP_STEREO          0x00000004
// -----------------------------------------------------------------------------
int SetupPixelFormat(HDC hDC,HWND hWnd)
{
    if (! hWnd)
    {
        PIXELFORMATDESCRIPTOR *ppfd;
        PIXELFORMATDESCRIPTOR  pfd;
        int  iPixelFormat;

        // get the current pixel format index
        iPixelFormat = GetPixelFormat(hDC);

        // obtain a detailed description of that pixel format
        DescribePixelFormat(hDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        //
        pfd.cStencilBits = 8;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW        // support window
                      | PFD_DOUBLEBUFFER        // double buffered
                      | PFD_SUPPORT_OPENGL;        // support OpenGL

        ppfd = &pfd;

        if ( (iPixelFormat = ChoosePixelFormat(hDC, ppfd)) == 0 )
        {
            MessageBox(NULL, "ChoosePixelFormat railed", "Error", MB_OK);
            return 0;
        }

            return 1;
    }
    else
    {
        PIXELFORMATDESCRIPTOR *ppfd;
        int pixelformat;

        PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd
        1,                                // version number

        PFD_DRAW_TO_WINDOW                // support window
        | PFD_DOUBLEBUFFER                // double buffered
        | PFD_SUPPORT_OPENGL,            // support OpenGL


        PFD_TYPE_RGBA,         // RGBA type
        32,                    // 16-bit color depth
        0, 0, 0, 0, 0, 0,      // color bits ignored
        8,                     // no alpha buffer
        0,                     // shift bit ignored
        0,                     // no accumulation buffer
        0, 0, 0, 0,            // accum bits ignored
        32,                    // 0-bit z-buffer      (2D MODE ON)
        8,                     // no stencil buffer
        0,                     // no auxiliary buffer
        PFD_MAIN_PLANE,        // main layer
        0,                     // reserved
        0, 0, 0                // layer masks ignored
        };

        pfd.cColorBits = GetDeviceCaps(hDC,BITSPIXEL);
        pfd.cStencilBits = 8;
        ppfd = &pfd;

        if ( (pixelformat = ChoosePixelFormat(hDC, ppfd)) == 0 )
        {
            MessageBox(NULL, "ChoosePixelFormat railed", "Error", MB_OK);
            return 0;
        }

        if (SetPixelFormat(hDC, pixelformat, ppfd) == false)
        {
            MessageBox(NULL, "SetPixelFormat railed", "Error", MB_OK);
            return 0;
        }
    }

    return 1;
}
// ----------------------------------------------------------------------------
HGLRC CreateRenderingContext(HWND wnd,HDC dc,unsigned int Options,unsigned int ColorDepth,unsigned int StencilBits)
{
    if (! SetupPixelFormat(dc,wnd))
    {
        // Error, can't find the correct pixel format
        // PostQuitMessage(0);
        return(0);
    }

    HGLRC glrc = wglCreateContext(dc);
    CHECKERROR();

    wglMakeCurrent(dc, glrc);
    CHECKERROR();

    return(glrc);
}
// -----------------------------------------------------------------------------
void DestroyRenderingContext(HGLRC RC)
{
    // Deactivate Rendering Context
    wglMakeCurrent(0,0);
    CHECKERROR();

    if (RC)
    {
        wglDeleteContext(RC);
        CHECKERROR();
    }
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Dirty trick to share textures among different rendering contexts.
static HGLRC gshFirstHGLRC = 0;

FEHandler CFESystem::Graphics::hInit(FEHandler _hParam)
{
    CRendererHandler* poRnd = new CRendererHandler;

    poRnd->m_hWnd = ((HWND)_hParam);
    poRnd->m_hDC  = GetDC(poRnd->m_hWnd);

    poRnd->m_hGLRC = CreateRenderingContext(poRnd->m_hWnd,poRnd->m_hDC,OP_DOUBLE_BUFFERED,32,0);    
    CHECKERROR();
    
    if (gshFirstHGLRC == 0)
		gshFirstHGLRC = poRnd->m_hGLRC;

    poRnd->m_oScrOfs.x = poRnd->m_oScrOfs.y = 0;
    ClientToScreen( poRnd->m_hWnd, &poRnd->m_oScrOfs);

    RECT oRect;
    GetClientRect(poRnd->m_hWnd,&oRect);

    poRnd->m_uiScrWidth  = oRect.right - oRect.left;
    poRnd->m_uiScrHeight = oRect.bottom - oRect.top;

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

    glEnable(GL_DEPTH_TEST);
    CHECKERROR();

    glDepthFunc(GL_LEQUAL);
    CHECKERROR();

    glDepthMask(GL_TRUE);
    CHECKERROR();

    glEnable(GL_NORMALIZE);
    CHECKERROR();

    glShadeModel(GL_SMOOTH);
    CHECKERROR();

    glViewport(oRect.left, oRect.top, poRnd->m_uiScrWidth,poRnd->m_uiScrHeight);
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
    
    if (gshFirstHGLRC != poRnd->m_hGLRC)
		wglShareLists(gshFirstHGLRC,poRnd->m_hGLRC);


    // retrieve whether NON Power of 2 textures are supported or not
    FESglobals.m_bNPO2Support = isExtensionSupported("GL_ARB_texture_non_power_of_two");

	FESglobals.m_poCurrentRenderer = poRnd;

    return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::Finish(FEHandler _hRenderer)
{
    Check(_hRenderer != NULL,"NULL Renderer Handler");
    DestroyRenderingContext( ((CRendererHandler*)_hRenderer )->m_hGLRC);
    ReleaseDC(((CRendererHandler*)_hRenderer )->m_hWnd, ((CRendererHandler*)_hRenderer)->m_hDC );
	delete ((CRendererHandler*)_hRenderer );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::BeginScene(FEHandler _hRenderer)
{
    if (FESglobals.m_poCurrentRenderer != _hRenderer)
    {
        // In case the renderer is different ...
        FESglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;

        if (FESglobals.m_poCurrentRenderer != NULL) 
			wglMakeCurrent(FESglobals.m_poCurrentRenderer->m_hDC, FESglobals.m_poCurrentRenderer->m_hGLRC);
		else
			return; 
    }

    // Setup screen virtual dimensions transform matrix.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(FEReal)FESglobals.m_uiScrVWidth,-2.0f/(FEReal)FESglobals.m_uiScrVHeight,1.0f);
    CHECKERROR();

	glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::EndScene()
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

	glFinish();
	::SwapBuffers( FESglobals.m_poCurrentRenderer->m_hDC );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
