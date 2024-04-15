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
#include "../FEConfig.h"
#if ((GRAPHICS == USE_OPENGL) && ((TARGETPLATFORM == USE_WINDOWS) || (TARGETPLATFORM == USE_DEVELOPMENT) || (TARGETPLATFORM == USE_PLAYTEST)) )
// ----------------------------------------------------------------------------
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>

#include "../CFECore.h"

#include "FEBasicTypes.h"
#include "Types/CFEKFBFunc.h"
#include "CFECGlobals.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <math.h>

#include "support/misc/CFEStringUtils.h"

#include "../CFECore.h"
#include "../CFECoreGlobals.h"

#include "../Common/CFECImageLib.h"
#include "../Common/CFERenderList.h"

#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
#ifndef GETPROPVALUE
#define GETPROPVALUE(type,a) reinterpret_cast<type>(a)
#endif
// ----------------------------------------------------------------------------
const uint MEM_ALIGNMENT = 32;
uint uiRoundUp(uint _uiSize)
{
	return( ((_uiSize) + ((MEM_ALIGNMENT)-1)) & ~((MEM_ALIGNMENT)-1) );
}
// --------------------------------------------------º--------------------------
const unsigned int INITIAL_RESERVED_PRIMS = 1024;
// ----------------------------------------------------------------------------
const uint MAX_BUFFERS = 2;
// ----------------------------------------------------------------------------
#define STATE_FLAG_TRANSFORM	0x01
#define STATE_FLAG_BLENDMODE	0x02
#define STATE_FLAG_MATERIAL		0x04
#define STATE_FLAG_SHADER		0x08
#define STATE_FLAG_DEPTH		0x10
#define STATE_FLAG_FOG			0x20
// ----------------------------------------------------------------------------
// Internal blend modes
// ----------------------------------------------------------------------------
const EFEBlendMode BM_LIGHTSABER_STEP_1_INT			= (EFEBlendMode)BM_NUM_MODES;
const EFEBlendMode BM_LIGHTSABER_STEP_2_INT			= (EFEBlendMode)((uint)BM_NUM_MODES+1);
const EFEBlendMode BM_LUMISHADE_REVEAL_STEP_1_INT	= (EFEBlendMode)((uint)BM_NUM_MODES+2);
const EFEBlendMode BM_LUMISHADE_REVEAL_STEP_2_INT	= (EFEBlendMode)((uint)BM_NUM_MODES+3);

// ----------------------------------------------------------------------------
typedef enum 
{
	RPT_NONE,

	RPT_QUAD,
	RPT_SPRITE,
	RPT_LINE,
	RPT_MESH,

	RPT_NUM_PRIMS,
	RPT_MAX = 0xffffffff // force 32bit enum

}ERenderPrimType;

// ----------------------------------------------------------------------------
// Put here things that affect rendering state
// ----------------------------------------------------------------------------
class CRenderState
{
	public:

		CFEMatrix				m_oTransf;
		FEReal              	m_rDepth;
		uint					m_uiShaderID;
        FEHandler           	m_hMaterial;
	    EFEBlendMode        	m_eBlendMode;
	    FEBool					m_bFogEnabled;
};
// ----------------------------------------------------------------------------
class CRenderPrim
{
	public:

		static int iPrimsCompFunc(const void* a,const void* b)
		{
			#define PA ((CRenderPrim*)a)
			#define PB ((CRenderPrim*)b)

			if ( PA->m_uiHash == PB->m_uiHash)
			{
				// rendering order is important and never equal in 2 prims.
				return ( PA->m_uiRndIdx - PB->m_uiRndIdx );
			}
			else
			{
	   			if (PA->m_oRS.m_rDepth != PB->m_oRS.m_rDepth)
				{
					return ( (int)(_1000r*(PA->m_oRS.m_rDepth - PB->m_oRS.m_rDepth)) );
				}
				else
				{
					// rendering order is important and never equal in 2 prims.
					return ( (int)PA->m_uiRndIdx - (int)PB->m_uiRndIdx );
				}
			}

			#undef PA
			#undef PB

			// return ( 0 ); // avoid statement unreachable warning
		}

		FEBool bPrimsCompFunc(const CRenderPrim& a,const CRenderPrim& b) const
		{
			if ( a.m_uiHash == b.m_uiHash)
			{
				// rendering order is important and never equal in 2 prims.
				return ( a.m_uiRndIdx < b.m_uiRndIdx );
			}
			else
			{
	   			if (a.m_oRS.m_rDepth != b.m_oRS.m_rDepth)
				{
					return ( a.m_oRS.m_rDepth > b.m_oRS.m_rDepth );
				}
				else
				{
					// rendering order is important and never equal in 2 prims.
					return ( a.m_uiRndIdx < b.m_uiRndIdx );
				}
			}

			// return ( true ); // avoid statement unreachable warning
		}

		/// Comparaison operator for sorting purposes.
		FEBool operator < (const CRenderPrim& b) const
		{
			return( bPrimsCompFunc(*this,b) );
		}

	public:

		CFEVect2*		m_poVX;
		CFEVect2*		m_poUV;
		CFEColor*		m_poVC;
		unsigned short*	m_pusIdx;
		uint			m_uNumVX;			// for meshes number of VX for the rest VX num to get the vertex idx

		CFEColor		m_oColor;			// color for this primitive.

		uint			m_uiRndIdx;			// Render Idx
		ERenderPrimType m_ePT;				// Primitive Type

		CRenderState	m_oRS;				// Render state used by this primitive
		uint			m_uiHash;			// Hash	for faster sorting
};

class CRenderElem
{
	public:

		CRenderPrim*	m_poIniPrim;
		uint			m_uiNumPrims;
};
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:

        CRendererHandler(unsigned short _usReservedPrims) :	m_hDC(NULL),
								m_hWnd(NULL),
								m_hGLRC(NULL),
								m_uiVScrWidth(640),
								m_uiVScrHeight(480),
								m_uiScrWidth(0),
								m_uiScrHeight(0),
								m_usReservedPrims(_usReservedPrims),
								m_oRPList(NULL),
								m_uiPrims(0),

								m_oVX(NULL),
								m_oUV(NULL),
								m_oVC(NULL),
								m_usIdx(NULL)
		{
			//
			m_oRPList =  new CRenderPrim[m_usReservedPrims];
			uint uiMaxVXs = m_usReservedPrims*4;

			// Initialize double buffers
			for (uint i=0;i<MAX_BUFFERS;i++)
			{
				m_oVXBufs[i]	= (CFEVect2*)CFECore::Mem::pAlloc( uiRoundUp(uiMaxVXs*sizeof(CFEVect2)),MEM_ALIGNMENT);
				m_oUVBufs[i]	= (CFEVect2*)CFECore::Mem::pAlloc( uiRoundUp(uiMaxVXs*sizeof(CFEVect2)),MEM_ALIGNMENT);
				m_oVCBufs[i]	= (CFEColor*)CFECore::Mem::pAlloc( uiRoundUp(uiMaxVXs*sizeof(CFEColor)),MEM_ALIGNMENT);
				m_usIdxBufs[i]	= (unsigned short*)CFECore::Mem::pAlloc( uiRoundUp(m_usReservedPrims*6*sizeof(unsigned short)),MEM_ALIGNMENT);
			}

			m_oVX	= m_oVXBufs[0];
			m_oUV	= m_oUVBufs[0];
			m_oVC	= m_oVCBufs[0];
			m_usIdx = m_usIdxBufs[0];
			
			m_oRenderState.m_eBlendMode = BM_COPY;
			m_oRenderState.m_hMaterial  = NULL;
			m_oRenderState.m_oTransf.LoadIdentity();
			m_oRenderState.m_rDepth = _0r;
			m_oRenderState.m_uiShaderID = 0;
			m_oRenderState.m_bFogEnabled	= false;

			m_uiDirtyFlags = 1;
			m_uiStateHash = uiGetStateHash();
		};

		~CRendererHandler()
		{
			// Delete double buffers
			for (uint i=0;i<MAX_BUFFERS;i++)
			{
				if (m_usIdxBufs[i] != NULL)
				{
					CFECore::Mem::Free((FEPointer)m_usIdxBufs[i]);
					m_usIdxBufs[i] = NULL;
				}

				if (m_oVCBufs[i] != NULL)
				{
					CFECore::Mem::Free((FEPointer)m_oVCBufs[i]);
					m_oVCBufs[i] = NULL;
				}
				if (m_oUVBufs[i] != NULL)
				{
					CFECore::Mem::Free((FEPointer)m_oUVBufs[i]);
					m_oUVBufs[i] = NULL;
				}
				if (m_oVXBufs[i] != NULL)
				{
					CFECore::Mem::Free((FEPointer)m_oVXBufs[i]);
					m_oVXBufs[i] = NULL;
				}
			}

			if (m_oRPList != NULL)
			{
				delete []m_oRPList;
				m_oRPList = NULL;
			}
		}

        HDC     m_hDC;                      // Display Contex
        HWND    m_hWnd;                     // Window handler
        HGLRC   m_hGLRC;                    // GL Rendering Context

        POINT   m_oScrOfs;                  // Screen offset
        uint    m_uiVScrWidth;              // Virtual Screen Width
        uint    m_uiVScrHeight;             // Virtual Screen Height
        uint    m_uiScrWidth;               // Actual Screen Width
        uint    m_uiScrHeight;              // Actual Screen Height

		CFEArray<CRenderElem>				m_oRL;

		// -------------------------------------------------------
		//	State Change Control
		// -------------------------------------------------------
		// Put here things that modify rendering state
		CRenderState				m_oRenderState;

		// Hash for faster sorting
		uint						m_uiStateHash;

		/// Flags to control state change
		uint						m_uiDirtyFlags;

		inline void SetFlag(uint _uiFlag)
		{
			m_uiDirtyFlags |= _uiFlag;
		}

		inline void ClearFlags()
		{
			m_uiDirtyFlags = 0;
		}

		// Convenient function to retrieve the primitive hashes
		inline uint uiGetStateHash()
		{
			if (m_uiDirtyFlags != 0)
			{
				m_uiStateHash = CFEStringUtils::uiGetCRC32((const char*)&m_oRenderState,sizeof(m_oRenderState));
				ClearFlags();
			}

			return(m_uiStateHash);
		}

		// -------------------------------------------------------
		// Primitive batch processing structures
		// -------------------------------------------------------
        unsigned short			m_usReservedPrims;
        unsigned char			m_usPAD[2];

        CRenderPrim*			m_oRPList;
        uint					m_uiPrims;

		// double buffers
        CFEVect2*				m_oVXBufs[MAX_BUFFERS];
        CFEVect2*				m_oUVBufs[MAX_BUFFERS];
		CFEColor*				m_oVCBufs[MAX_BUFFERS];
		unsigned short*			m_usIdxBufs[MAX_BUFFERS];

		// pointer to current buffers.
        CFEVect2*				m_oVX;
        CFEVect2*				m_oUV;
		CFEColor*				m_oVC;
		unsigned short*			m_usIdx;

		// -------------------------------------------------------
		// Render To Texture
		// -------------------------------------------------------
		uint					m_uiLightPrims;
		FEBool					m_bFBOCleared;	// Delayed clear: only clear if it's really needed
		FEBool					m_bFBOEnabled;
		GLuint					m_uiR2TTex;
        uint					m_uiR2TWidth;
        uint					m_uiR2THeight;
		GLuint					m_uiFBO;
};
// ----------------------------------------------------------------------------
namespace CFECore { namespace Graphics { 

	void BuildRenderList();
	void FlushRenderList();
	void EnableRender2Texture(bool _bEnable);

	void INT_SetTransform(const CFEMatrix& _oTransf,FEReal _rDepth);
	void INT_SetMaterial(FEHandler _hHnd);
	void INT_SetBlendMode(EFEBlendMode _eBM);

	void INT_RenderQuads(unsigned short* _pusIdx,CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads);
	void INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints);
	void INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints);
	void INT_RenderLine(CFEVect2* _poVXs,CFEColor* _poVCs,uint _uiPrims);

	void SaveTGA(const CFEString& _sFilename,FEPointer _pData,uint _uiWidth,uint _uiHeight,uint _uiPixSize);
	void RecalcViewport(uint _uiNewWidth,uint _uiNewHeight,CRendererHandler* _poRenderer);

	/// Static vars
	static uint m_uiInitialReservedPrims = INITIAL_RESERVED_PRIMS;
	static uint gsuiLastTexture			 = 0;
	static uint gsuiNumTexUnits			 = 1;

	// Dirty trick to share textures among different rendering contexts.
	static HGLRC gshFirstHGLRC = 0;
};
};
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();

/*
#define CHECKERROR()\
eErr=glGetError();\
if (eErr != GL_NO_ERROR)\
{\
	_asm\
	{\
		int 3\
	}\
}
*/
#else
#define CHECKERROR()
#endif
// ----------------------------------------------------------------------------
GLAPI void APIENTRY glBlendFuncSeparateEXT_FAKED(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	glBlendFunc(sfactorRGB, dfactorRGB);
}

GLAPI void APIENTRY glBlendEquationEXT_FAKED(GLenum mode)
{
	// glBlendEquation(sfactorRGB, dfactorRGB);
}

GLAPI void APIENTRY glBlendEquationSeparateEXT_FAKED(GLenum modeRGB, GLenum modeAlpha)
{
	glBlendEquationEXT_FAKED(modeRGB);
}

GLAPI void APIENTRY glActiveTexture_FAKED(GLenum texture)
{
	// glBlendEquationEXT_FAKED(modeRGB);
}

GLAPI void APIENTRY glClientActiveTexture_FAKED(GLenum texture)
{

}

GLAPI void APIENTRY glGenFramebuffers_FAKED(GLsizei n, GLuint *framebuffers)
{

};

GLAPI void APIENTRY glGenRenderbuffers_FAKED(GLsizei n, GLuint *renderbuffers)
{

};

GLAPI void APIENTRY glBindFramebuffer_FAKED(GLenum target, GLuint framebuffer)
{

};

GLAPI void APIENTRY glBlitFramebuffer_FAKED(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{

};

GLAPI void APIENTRY glFramebufferTexture2D_FAKED(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{

};

GLAPI GLenum APIENTRY glCheckFramebufferStatus_FAKED(GLenum target)
{
	return(0);
};

GLAPI void APIENTRY glBindRenderbuffer_FAKED(GLenum target, GLuint renderbuffer)
{

};

GLAPI void APIENTRY glRenderbufferStorage_FAKED(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{

};

GLAPI void APIENTRY glFramebufferRenderbuffer_FAKED(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{

};

GLAPI void APIENTRY glGetFramebufferAttachmentParameteriv_FAKED(GLenum target, GLenum attachment, GLenum pname, GLint * params)
{

};
// ----------------------------------------------------------------------------
PFNGLBLENDEQUATIONPROC				glBlendEquationEXT			= glBlendEquationEXT_FAKED;
PFNGLBLENDEQUATIONSEPARATEEXTPROC	glBlendEquationSeparateEXT	= glBlendEquationSeparateEXT_FAKED;
PFNGLBLENDFUNCSEPARATEEXTPROC		glBlendFuncSeparateEXT		= glBlendFuncSeparateEXT_FAKED;

PFNGLACTIVETEXTUREPROC 				glActiveTexture				= glActiveTexture_FAKED;
PFNGLCLIENTACTIVETEXTUREPROC		glClientActiveTexture		= glClientActiveTexture_FAKED;

PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv = glGetFramebufferAttachmentParameteriv_FAKED;

PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffers			= glGenFramebuffers_FAKED;
PFNGLGENRENDERBUFFERSEXTPROC		glGenRenderbuffers			= glGenRenderbuffers_FAKED;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebuffer			= glBindFramebuffer_FAKED;
PFNGLBLITFRAMEBUFFEREXTPROC			glBlitFramebuffer			= glBlitFramebuffer_FAKED;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2D		= glFramebufferTexture2D_FAKED;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatus	= glCheckFramebufferStatus_FAKED;
PFNGLBINDRENDERBUFFEREXTPROC		glBindRenderbuffer			= glBindRenderbuffer_FAKED;
PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorage		= glRenderbufferStorage_FAKED;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer	= glFramebufferRenderbuffer_FAKED;


// ----------------------------------------------------------------------------
// defined in OpenGL common source file.
FEBool bIsExtensionSupported(const char *extension);
// ----------------------------------------------------------------------------
// Rendering Context Creation Flags
#define OP_DOUBLE_BUFFERED	0x00000001
#define OP_GDI              0x00000002
#define OP_STEREO          	0x00000004
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
HGLRC CreateRenderingContext(HWND wnd,HDC dc)
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
void CFECore::Graphics::RecalcViewport(uint _uiNewWidth,uint _uiNewHeight,CRendererHandler* _poRenderer)
{
    if (FECglobals.m_bKeepDesignAspect)
    {	
		// setup properly the viewport...
		FEReal rVRatio = ((FEReal)_poRenderer->m_uiVScrWidth/(FEReal)_poRenderer->m_uiVScrHeight);	// virtual screen aspect ratio
		FEReal rRRatio = ((FEReal)_poRenderer->m_uiScrWidth /(FEReal)_poRenderer->m_uiScrHeight );	// real screen aspect ratio

		uint uiFlags = 0;
		if (_poRenderer->m_uiVScrWidth <=_poRenderer->m_uiScrWidth ) uiFlags |= 0x01;
		if (_poRenderer->m_uiVScrHeight<=_poRenderer->m_uiScrHeight) uiFlags |= 0x02;

		switch (uiFlags)
		{
			// width physical dims are bigger but not height
			case 0x01:
			{
				// Physical screen bigger than the design screen. Height of the physical screen is the constraint.
				FECglobals.m_uiViewportHeight  = _poRenderer->m_uiScrHeight;
				FECglobals.m_uiViewportHMargin = 0;

				FECglobals.m_uiViewportWidth   = FECglobals.m_uiViewportHeight * rVRatio;
				FECglobals.m_uiViewportWMargin = (_poRenderer->m_uiScrWidth - FECglobals.m_uiViewportWidth)/2;
			}
			break;

			// height physical dims are bigger but not width. Width of the physical screen is the constraint.
			case 0x02:
			{
				// Physical screen bigger than the design screen
				FECglobals.m_uiViewportWidth   = _poRenderer->m_uiScrWidth;
				FECglobals.m_uiViewportWMargin = 0;

				FECglobals.m_uiViewportHeight  = FECglobals.m_uiViewportWidth / rVRatio;
				FECglobals.m_uiViewportHMargin = (_poRenderer->m_uiScrHeight - FECglobals.m_uiViewportHeight)/2;
			}
			break;
			
			// both physical dims are bigger
			case 0x03:
			{
				// Physical screen bigger than the design screen
				FECglobals.m_uiViewportWidth   = _poRenderer->m_uiVScrWidth;
				FECglobals.m_uiViewportHeight  = _poRenderer->m_uiVScrHeight;
				FECglobals.m_uiViewportWMargin = (_poRenderer->m_uiScrWidth  - FECglobals.m_uiViewportWidth )/2;
				FECglobals.m_uiViewportHMargin = (_poRenderer->m_uiScrHeight - FECglobals.m_uiViewportHeight)/2;
			}
			break;

			case 0x00:
			{
				// horizontal layout
				if (rRRatio<rVRatio)
				{
					// horizontal bands

					// real ratio > virtual ratio
					FECglobals.m_uiViewportWidth   = _poRenderer->m_uiScrWidth;
					FECglobals.m_uiViewportHeight  = (FEReal)_poRenderer->m_uiScrWidth/rVRatio;
					FECglobals.m_uiViewportHMargin = (_poRenderer->m_uiScrHeight - FECglobals.m_uiViewportHeight) / 2;
					FECglobals.m_uiViewportWMargin = 0;
				}
				else
				{
					// vertical bands

					// real ratio > virtual ratio
					FECglobals.m_uiViewportWidth   = (FEReal)_poRenderer->m_uiScrHeight*rVRatio;
					FECglobals.m_uiViewportHeight  = _poRenderer->m_uiScrHeight;
					FECglobals.m_uiViewportWMargin = (_poRenderer->m_uiScrWidth - FECglobals.m_uiViewportWidth) / 2;
					FECglobals.m_uiViewportHMargin = 0;
				}
			}
			break;
		}
	}
	else
	{
		FECglobals.m_uiViewportWidth    = _uiNewWidth;
		FECglobals.m_uiViewportHeight   = _uiNewHeight;
		FECglobals.m_uiViewportWMargin  = 0;
		FECglobals.m_uiViewportHMargin  = 0;
    }

	// Setup screen virtual dimensions transform matrix.
    glViewport(FECglobals.m_uiViewportWMargin,
               FECglobals.m_uiViewportHMargin,
               FECglobals.m_uiViewportWidth,
               FECglobals.m_uiViewportHeight);
	CHECKERROR();

    glScissor(FECglobals.m_uiViewportWMargin,
               FECglobals.m_uiViewportHMargin,
               FECglobals.m_uiViewportWidth,
               FECglobals.m_uiViewportHeight);
	CHECKERROR();
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hInit(FEHandler _hParam)
{
	if (_hParam == NULL) return(NULL);
    CRendererHandler* poRnd = new CRendererHandler(CFECore::Graphics::m_uiInitialReservedPrims);

    poRnd->m_hWnd = ((HWND)_hParam);
    poRnd->m_hDC  = GetDC(poRnd->m_hWnd);

    poRnd->m_hGLRC = CreateRenderingContext(poRnd->m_hWnd,poRnd->m_hDC);    
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
	
	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	CHECKERROR();

    RecalcViewport(poRnd->m_uiScrWidth,poRnd->m_uiScrHeight,poRnd);
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
    
    glFogf (GL_FOG_MODE,GL_LINEAR);
    
    if (gshFirstHGLRC != poRnd->m_hGLRC)
		wglShareLists(gshFirstHGLRC,poRnd->m_hGLRC);


    // retrieve whether NON Power of 2 textures are supported or not
    FECglobals.m_bNPO2Support		= bIsExtensionSupported("GL_ARB_texture_non_power_of_two");
	FECglobals.m_poCurrentRenderer	= poRnd;

	GLint major,minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major); 
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	/*
	glBlendFuncSeparateEXT = (PFNGLBLENDFUNCSEPARATEEXTPROC)wglGetProcAddress("glBlendFuncSeparateEXT");
	if (glBlendFuncSeparateEXT == NULL) glBlendFuncSeparateEXT = glBlendFuncSeparateEXT_FAKED;

	glBlendEquationEXT = (PFNGLBLENDEQUATIONPROC)wglGetProcAddress("glBlendEquationEXT");
	if (glBlendEquationEXT == NULL) glBlendEquationEXT = glBlendEquationEXT_FAKED;

	glBlendEquationSeparateEXT = (PFNGLBLENDEQUATIONSEPARATEEXTPROC)wglGetProcAddress("glBlendEquationSeparateEXT");
	if (glBlendEquationSeparateEXT == NULL) glBlendEquationSeparateEXT = glBlendEquationSeparateEXT_FAKED;

	glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
	if (glActiveTexture == NULL) glActiveTexture = glActiveTexture_FAKED;
	*/

	#define RETRIEVE_EXTENSION(FUNCNAME,FUNCPROTOTYPE)\
	FUNCNAME = (FUNCPROTOTYPE)wglGetProcAddress(#FUNCNAME);\
	if (FUNCNAME == NULL) FUNCNAME = FUNCNAME##_FAKED;

	#define RETRIEVE_EXTENSION2(FUNCNAME,FUNCNAME2,FUNCPROTOTYPE)\
	FUNCNAME = (FUNCPROTOTYPE)wglGetProcAddress(#FUNCNAME2);\
	if (FUNCNAME == NULL) FUNCNAME = FUNCNAME##_FAKED;
 
	RETRIEVE_EXTENSION(glBlendFuncSeparateEXT,		PFNGLBLENDFUNCSEPARATEEXTPROC);
	RETRIEVE_EXTENSION(glBlendEquationEXT,			PFNGLBLENDEQUATIONPROC);
 	RETRIEVE_EXTENSION(glBlendEquationSeparateEXT,	PFNGLBLENDEQUATIONSEPARATEEXTPROC);

  	RETRIEVE_EXTENSION(glActiveTexture,			PFNGLACTIVETEXTUREPROC)
	RETRIEVE_EXTENSION(glClientActiveTexture , PFNGLCLIENTACTIVETEXTUREPROC);

	RETRIEVE_EXTENSION(glGenFramebuffers,		PFNGLGENFRAMEBUFFERSEXTPROC);	
	RETRIEVE_EXTENSION(glBindFramebuffer,		PFNGLBINDFRAMEBUFFEREXTPROC);	

	RETRIEVE_EXTENSION(glGenRenderbuffers,		PFNGLGENRENDERBUFFERSEXTPROC);	
	RETRIEVE_EXTENSION(glBindRenderbuffer,		PFNGLBINDRENDERBUFFEREXTPROC);	
	RETRIEVE_EXTENSION(glRenderbufferStorage,	PFNGLRENDERBUFFERSTORAGEEXTPROC);	
	
	RETRIEVE_EXTENSION(glFramebufferTexture2D,	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC);	
	RETRIEVE_EXTENSION(glFramebufferRenderbuffer,PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC);	

	// RETRIEVE_EXTENSION(glBlitFramebuffer,		PFNGLBLITFRAMEBUFFEREXTPROC);	
	RETRIEVE_EXTENSION(glCheckFramebufferStatus,	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC);	
	RETRIEVE_EXTENSION(glGetFramebufferAttachmentParameteriv, PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC);
	

// #define GL_READ_FRAMEBUFFER_EXT		0x8CA8
// #define GL_DRAW_FRAMEBUFFER_EXT		0x8CA9

    // Generate the Render2Texture texture
	glGenTextures(1,&poRnd->m_uiR2TTex);
	CHECKERROR();

	// Establecer el contexto de la nueva textura
	glBindTexture(GL_TEXTURE_2D,poRnd->m_uiR2TTex);
	CHECKERROR();

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECKERROR();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECKERROR();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    CHECKERROR();
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    CHECKERROR();
	poRnd->m_uiR2TWidth		= poRnd->m_uiScrWidth;
	poRnd->m_uiR2THeight	= poRnd->m_uiScrHeight;
	glTexImage2D (GL_TEXTURE_2D,0,GL_RGBA8,poRnd->m_uiR2TWidth,poRnd->m_uiR2THeight,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	CHECKERROR();

	glBindTexture(GL_TEXTURE_2D, 0);
	CHECKERROR();
	
	GLint xxx = 0;
	glGetFramebufferAttachmentParameteriv (GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,&xxx);
	CHECKERROR();

	glGenFramebuffers(1, &poRnd->m_uiFBO); 
    CHECKERROR();

	glBindFramebuffer(GL_FRAMEBUFFER, poRnd->m_uiFBO);
	CHECKERROR();

	// Creamos el render buffer object para el depth, y lo configuramos 
	GLuint rboId;
	glGenRenderbuffers(1, &rboId);
	CHECKERROR();

	glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	CHECKERROR();

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,poRnd->m_uiR2TWidth,poRnd->m_uiR2THeight);
	CHECKERROR();

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CHECKERROR();

	// Attach Render2Texture Texture to FrameBufferObject
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, poRnd->m_uiR2TTex, 0);
	CHECKERROR();
	
	// attach the renderbuffer to depth attachment point of the FrameBufferObject
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboId);
	CHECKERROR();

	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
	CHECKERROR();

	switch(status)
	{ 
		case GL_FRAMEBUFFER_COMPLETE: 
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_UNSUPPORTED:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		CHECKERROR();
		break; 

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		CHECKERROR();
		break; 

		default:
		CHECKERROR();
		break;
	}
	
/*
	// Set the texture as draw and read buffers for this buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glReadBuffer(GL_COLOR_ATTACHMENT0);*/


	// bind system frame buffer as the current target
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECKERROR();

    return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::Finish(FEHandler _hRenderer)
{
    if (_hRenderer == NULL) return;

    Check(_hRenderer != NULL,"NULL Renderer Handler");
    DestroyRenderingContext( ((CRendererHandler*)_hRenderer )->m_hGLRC);
    ReleaseDC(((CRendererHandler*)_hRenderer )->m_hWnd, ((CRendererHandler*)_hRenderer)->m_hDC );
	delete ((CRendererHandler*)_hRenderer );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetScreenVWidth(uint _uiScrVWidth)
{
    FECglobals.m_poCurrentRenderer->m_uiVScrWidth = _uiScrVWidth;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrWidth,-2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrHeight,1.0f);

    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetScreenVHeight(uint _uiScrVHeight)
{
	FECglobals.m_poCurrentRenderer->m_uiVScrHeight = _uiScrVHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrWidth,-2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrHeight,1.0f);

    CHECKERROR();
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hGetCurrentRenderer()
{
    return( (FEHandler)(FECglobals.m_poCurrentRenderer) );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetCurrentRenderer(FEHandler _hRenderer)
{
    if (FECglobals.m_poCurrentRenderer != _hRenderer)
    {
        // In case the renderer is different ...
        FECglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;

        if (FECglobals.m_poCurrentRenderer != NULL) 
			wglMakeCurrent(FECglobals.m_poCurrentRenderer->m_hDC, FECglobals.m_poCurrentRenderer->m_hGLRC);
		else
			return; 
    }
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::BeginScene()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;

	/// Clear and initialize the render list
	FECglobals.m_poCurrentRenderer->m_oRL.clear();
    FECglobals.m_poCurrentRenderer->m_uiPrims = 0;
	FECglobals.m_poCurrentRenderer->m_uiLightPrims = 0;

	/*
	// Perform double buffering between frames
	if (m_poData->m_oVX == m_poData->m_oVXBufs[0])
	{
		m_poData->m_oVX = m_poData->m_oVXBufs[1];
		m_poData->m_oUV = m_poData->m_oUVBufs[1];
		m_poData->m_oVC = m_poData->m_oVCBufs[1];
		m_poData->m_usIdx = m_poData->m_usIdxBufs[1];
	}
	else
	*/
	{
		FECglobals.m_poCurrentRenderer->m_oVX	= FECglobals.m_poCurrentRenderer->m_oVXBufs[0];
		FECglobals.m_poCurrentRenderer->m_oUV	= FECglobals.m_poCurrentRenderer->m_oUVBufs[0];
		FECglobals.m_poCurrentRenderer->m_oVC	= FECglobals.m_poCurrentRenderer->m_oVCBufs[0];
		FECglobals.m_poCurrentRenderer->m_usIdx = FECglobals.m_poCurrentRenderer->m_usIdxBufs[0];
	}

	
    // Setup screen virtual dimensions transform matrix.
    RecalcViewport(FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight,FECglobals.m_poCurrentRenderer);
   
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrWidth,-2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrHeight,1.0f);
    CHECKERROR();

	glBlendFunc(GL_ONE,GL_ZERO);

	glClearColor(_0r,_0r,_0r,_0r);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Clear FBO for the next frame.
	FECglobals.m_poCurrentRenderer->m_bFBOCleared = false;
	FECglobals.m_poCurrentRenderer->m_bFBOEnabled = false;
	EnableRender2Texture(true);
	EnableRender2Texture(false);

	// for debugging purposes
	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::EndScene()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;
	
	if (FECglobals.m_poCurrentRenderer->m_uiPrims > 0)
	{
		// Sort primitives:
		std::sort(&FECglobals.m_poCurrentRenderer->m_oRPList[0],&FECglobals.m_poCurrentRenderer->m_oRPList[0] + FECglobals.m_poCurrentRenderer->m_uiPrims);
		BuildRenderList();
		FlushRenderList();
		
		// Disable rendering to texture in case it was the last operation
		if (FECglobals.m_poCurrentRenderer->m_bFBOEnabled)
			EnableRender2Texture(false);
	}

	glFinish();
	::SwapBuffers( FECglobals.m_poCurrentRenderer->m_hDC );
}
// ----------------------------------------------------------------------------
typedef struct
{
    GLint           m_iID;
	GLint           m_iMinFilter;
	GLint			m_iMagFilter;
	GLint			m_iSWrapMode;
	GLint			m_iTWrapMode;
    CFECImageInfo*  m_poII;
    
    #ifdef _DEBUG
    CFEString		m_sFilename;
    #endif

}TMaterialHandler;
//-----------------------------------------------------------------------------------------
// from www.OpenGL.org
//-----------------------------------------------------------------------------------------
FEBool bIsExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	// Extension names should not have spaces.
	where = (GLubyte *) strchr(extension, ' ');
	
	if (where || *extension == '\0')
		return(false);
	
	extensions = glGetString(GL_EXTENSIONS);
	if (extensions == NULL) return(false);

	// It takes a bit of care to be fool-proof about parsing the
	// OpenGL extensions string. Don't be fooled by sub-strings,
	// etc.
	start = extensions;
	for (;;)
	{
		where = (GLubyte *) strstr((const char *) start, extension);
		
		if (!where)
			break;
		
		terminator = where + strlen(extension);
		
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return(true);

		start = terminator;
	}
	
	return(false);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::DeleteMaterial(FEHandler _hMaterial)
{
    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    ImageLib::FreeImage(poTH->m_poII);
    glDeleteTextures(1,(const GLuint*)&poTH->m_iID);
    
    delete poTH;
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hLoadMaterial(const CFEString& _sFilename)
{
    // TMaterialHandler
    CFECImageInfo* poII = ImageLib::poLoadImage((char*)_sFilename.szString());
    if (poII == NULL) return(NULL);

    if ((gshFirstHGLRC != 0) && (! FECglobals.m_bNPO2Support))
        ImageLib::MakePowerOf2(poII);

    TMaterialHandler* poTH = new TMaterialHandler;
    poTH->m_poII = poII;
    poTH->m_iID  = -1;
	poTH->m_iMagFilter = GL_LINEAR;
	poTH->m_iMinFilter = GL_LINEAR;
	poTH->m_iSWrapMode = GL_CLAMP_TO_EDGE;
	poTH->m_iTWrapMode = GL_CLAMP_TO_EDGE;
	#ifdef _DEBUG
	poTH->m_sFilename  = _sFilename;
	#endif

    return(poTH);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::ReloadMaterial(FEHandler _hMaterial,const CFEString& _sFilename)
{
    CFECImageInfo* poII = ImageLib::poLoadImage((char*)_sFilename.szString());
    if (poII == NULL) return;
    
    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;

    // Delete previos resources
    ImageLib::FreeImage(poTH->m_poII);
    glDeleteTextures(1,(const GLuint*)&poTH->m_iID);
    
    // Create new ones
    poTH->m_poII = poII;
    if (! FECglobals.m_bNPO2Support)
        ImageLib::MakePowerOf2(poTH->m_poII);

    poTH->m_iID = -1;
}
//---------------------------------------------------------------------------
GLint iGetTextureComponents(EFEPixelFormat _ePixelFormat)
{
    switch (_ePixelFormat)
    {
        case PF_RGBA16:
        return(GL_RGBA);
        break;

        case PF_RGB24:
        return(GL_RGB);
        break;

        case PF_RGBA32:
        return(GL_RGBA);
        break;

        case PF_GRAY:
        return(GL_LUMINANCE);
        break;
    }

    return(GL_RGBA);
}
//---------------------------------------------------------------------------
GLenum eGetTextureFormat(EFEPixelFormat _ePixelFormat)
{
    switch (_ePixelFormat)
    {
        case PF_RGBA16:
        return(GL_RGBA16);
        break;

        case PF_RGB24:
        return(GL_RGB8);
        break;

        case PF_RGBA32:
        return(GL_RGBA);
        break;

        case PF_GRAY:
        return(GL_LUMINANCE);
        break;
    }

    return(GL_RGBA);
}
// ----------------------------------------------------------------------------
int iCreateTexture(TMaterialHandler* _poMH)
{
    GLuint uiTexID = 0xffffffff;

    // Generate a new material handler
    glGenTextures(1,&uiTexID);
	CHECKERROR();
	if (uiTexID == 0xffffffff) return(-1);

    // Establecer el contexto de la nueva textura
    glBindTexture(GL_TEXTURE_2D,uiTexID);
    CHECKERROR();

    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  eGetTextureFormat(_poMH->m_poII->m_eFormat),
                  _poMH->m_poII->m_uiFixedTX,
                  _poMH->m_poII->m_uiFixedTY,
                  0,
                  iGetTextureComponents(_poMH->m_poII->m_eFormat),
                  GL_UNSIGNED_BYTE,
                  _poMH->m_poII->m_pData);
    CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,_poMH->m_iSWrapMode);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,_poMH->m_iTWrapMode);CHECKERROR();

    #ifdef DS_PC
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);CHECKERROR();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);CHECKERROR();
    #else
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,_poMH->m_iMagFilter);CHECKERROR();
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,_poMH->m_iMinFilter);CHECKERROR();
    #endif

    return(uiTexID);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetMaterial(FEHandler _hMaterial)
{
	if (FECglobals.m_poCurrentRenderer->m_oRenderState.m_hMaterial == _hMaterial) return;

	FECglobals.m_poCurrentRenderer->m_oRenderState.m_hMaterial = _hMaterial;
	FECglobals.m_poCurrentRenderer->SetFlag(STATE_FLAG_MATERIAL);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetTransform(const CFEMatrix& _oMat)
{
	FECglobals.m_poCurrentRenderer->m_oRenderState.m_oTransf = _oMat;
	FECglobals.m_poCurrentRenderer->SetFlag(STATE_FLAG_TRANSFORM);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetBlendMode(EFEBlendMode _eBlendMode)
{
	if (FECglobals.m_poCurrentRenderer->m_oRenderState.m_eBlendMode == _eBlendMode) return;

	FECglobals.m_poCurrentRenderer->m_oRenderState.m_eBlendMode = _eBlendMode;
	FECglobals.m_poCurrentRenderer->SetFlag(STATE_FLAG_BLENDMODE);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetDepth(FEReal _rDepth)
{
	if (FECglobals.m_poCurrentRenderer->m_oRenderState.m_rDepth == _rDepth) return;

	FECglobals.m_poCurrentRenderer->m_oRenderState.m_rDepth = _rDepth;
	FECglobals.m_poCurrentRenderer->SetFlag(STATE_FLAG_DEPTH);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;
	if (poRnd->m_uiPrims >= poRnd->m_usReservedPrims) return;

	CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];
	
	// These determine the order of rendering
	poRP->m_uiRndIdx	= poRnd->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_oRS			= poRnd->m_oRenderState;
	poRP->m_uiHash		= poRnd->uiGetStateHash();
	poRP->m_ePT			= RPT_LINE;

	uint uiIdx = poRnd->m_uiPrims*4;
    CFEVect2* poDstVX = &poRnd->m_oVX[uiIdx];
    CFEColor* poDstVC = &poRnd->m_oVC[uiIdx];

    // Set coords
    poDstVX[0] = _oIni;
    poDstVX[1] = _oEnd;

	poDstVC[0] = _oColor;
	poDstVC[1] = _oColor;

	/*
	// really don't needed
	poRP->m_uNumVX		= poRP->m_uiRndIdx*4;
	poRP->m_poVX		= poRnd->m_oVX[uiIdx];
	poRP->m_poVC		= poRnd->m_oVC[uiIdx];
	poRP->m_poUV		= NULL;
	poRP->m_pusIdx		= NULL;
	*/

	poRnd->m_uiPrims++;
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;
	if (poRnd->m_uiPrims >= poRnd->m_usReservedPrims) return;

	if (poRnd->m_oRenderState.m_eBlendMode == BM_LIGHTSABER)
	{
		SetBlendMode(BM_LIGHTSABER_STEP_1_INT);
		RenderMesh(_pusIdx,_poVXs,_poUVs,_oColor,_uiPoints);
		SetBlendMode(BM_LIGHTSABER_STEP_2_INT);
		RenderMesh(_pusIdx,_poVXs,_poUVs,_oColor,_uiPoints);
		return;
	}

	else if (poRnd->m_oRenderState.m_eBlendMode == BM_REVEAL)
	{
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_1_INT);
		RenderMesh(_pusIdx,_poVXs,_poUVs,_oColor,_uiPoints);
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_2_INT);
		RenderMesh(_pusIdx,_poVXs,_poUVs,_oColor,_uiPoints);
		return;
	}

	
	CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= poRnd->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash		= poRnd->uiGetStateHash();
	poRP->m_oRS			= poRnd->m_oRenderState;

	poRP->m_ePT		 	= RPT_MESH;
	poRP->m_poVX			= _poVXs;
	poRP->m_poUV			= _poUVs;
	poRP->m_poVC			= NULL;
	poRP->m_oColor		= _oColor;
	poRP->m_pusIdx		= _pusIdx;
	poRP->m_uNumVX		= _uiPoints;

	poRnd->m_uiPrims++;	
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderQuad(	const CFEVect2& _oIVX,const CFEVect2& _oFVX,
					const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                    const CFEColor& _oColor)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;
	if (poRnd->m_uiPrims >= poRnd->m_usReservedPrims) return;

	if (poRnd->m_oRenderState.m_eBlendMode == BM_LIGHTSABER)
	{
		SetBlendMode(BM_LIGHTSABER_STEP_1_INT);
		RenderQuad(_oIVX,_oFVX,_oIUV,_oFUV,_oColor);
		SetBlendMode(BM_LIGHTSABER_STEP_2_INT);
		RenderQuad(_oIVX,_oFVX,_oIUV,_oFUV,_oColor);
		return;
	}
	
	else if (poRnd->m_oRenderState.m_eBlendMode == BM_REVEAL)
	{
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_1_INT);
		RenderQuad(_oIVX,_oFVX,_oIUV,_oFUV,_oColor);
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_2_INT);
		RenderQuad(_oIVX,_oFVX,_oIUV,_oFUV,_oColor);
		return;
	}

	CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= poRnd->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash = poRnd->uiGetStateHash();
	poRP->m_oRS = poRnd->m_oRenderState;

	poRP->m_ePT			= RPT_QUAD;

    uint uiIdx = poRnd->m_uiPrims*4;
    CFEVect2* poDstVX = &poRnd->m_oVX[uiIdx];
    CFEVect2* poDstUV = &poRnd->m_oUV[uiIdx];
    CFEColor* poDstVC = &poRnd->m_oVC[uiIdx];

    // Set coords
    poDstVX[0].x = _oIVX.x;
    poDstVX[0].y = _oIVX.y;

    poDstVX[1].x = _oFVX.x;
    poDstVX[1].y = _oIVX.y;

    poDstVX[2].x = _oFVX.x;
    poDstVX[2].y = _oFVX.y;

    poDstVX[3].x = _oIVX.x;
    poDstVX[3].y = _oFVX.y;

    // Set UVs
	if (
	(poRP->m_oRS.m_eBlendMode != BM_REVEAL) 
	&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_1_INT) 
	&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_2_INT)
	)
	{
		poDstUV[0].x = _oIUV.x;
		poDstUV[0].y = _oIUV.y;

		poDstUV[1].x = _oFUV.x;
		poDstUV[1].y = _oIUV.y;

		poDstUV[2].x = _oFUV.x;
		poDstUV[2].y = _oFUV.y;

		poDstUV[3].x = _oIUV.x;
		poDstUV[3].y = _oFUV.y;
    }
	else
	{
		// Can be preprocessed 
		CFEMatrix oP;
		oP.LoadIdentity();
		oP.Translate(_0r,_1r);
		oP.Scale(1.0f/(float)poRnd->m_uiVScrWidth,-1.0f/(float)poRnd->m_uiVScrHeight);

		CFEMatrix oMVP;
		oMVP.Multiply(poRP->m_oRS.m_oTransf,oP);

		// process UV
		for (uint v=0;v<4;v++)
			poDstUV[v] = oMVP.Transform(  poDstVX[v] );
	}

    // Set color
    poDstVC[0] = poDstVC[1] = poDstVC[2] = poDstVC[3] = _oColor;
		
	/*
	// really don't needed
	poRP->m_uNumVX		= poRP->m_uiRndIdx*4;
	poRP->m_poVX		= poRnd->m_oVX[uiIdx];
	poRP->m_poVC		= poRnd->m_oVC[uiIdx];
	poRP->m_poUV		= NULL;
	poRP->m_pusIdx		= NULL;
	*/
	
	poRnd->m_uiPrims++;	
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderQuadList(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiQuads)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;
	if (poRnd->m_oRenderState.m_eBlendMode == BM_LIGHTSABER)
	{
		SetBlendMode(BM_LIGHTSABER_STEP_1_INT);
		RenderQuadList(_poVXs,_poUVs,_poVCs,_uiQuads);
		SetBlendMode(BM_LIGHTSABER_STEP_2_INT);
		RenderQuadList(_poVXs,_poUVs,_poVCs,_uiQuads);
		return;
	}
	
	else if (poRnd->m_oRenderState.m_eBlendMode == BM_REVEAL)
	{
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_1_INT);
		RenderQuadList(_poVXs,_poUVs,_poVCs,_uiQuads);
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_2_INT);
		RenderQuadList(_poVXs,_poUVs,_poVCs,_uiQuads);
		return;
	}

	

	for (uint i=0;i<_uiQuads;i++)
	{
		if (poRnd->m_uiPrims >= poRnd->m_usReservedPrims) return;
		CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];

		// These determine the order of rendering
		poRP->m_uiRndIdx	= poRnd->m_uiPrims;

		/// Retrieve primitive hash
		poRP->m_oRS			= poRnd->m_oRenderState;
		poRP->m_uiHash		= poRnd->uiGetStateHash();
		poRP->m_ePT			= RPT_QUAD;
		poRP->m_uNumVX		= poRnd->m_uiPrims*4;

		uint uiIdx = poRnd->m_uiPrims*4;
		CFEVect2* poDstVX = &poRnd->m_oVX[uiIdx];		

		poDstVX[0] = _poVXs[0];
		poDstVX[1] = _poVXs[1];
		poDstVX[2] = _poVXs[2];
		poDstVX[3] = _poVXs[3];
		
	    // Set UVs
		if (
		(poRP->m_oRS.m_eBlendMode != BM_REVEAL) 
		&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_1_INT) 
		&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_2_INT)
		)
		{
			if (_poUVs != NULL)
			{
				CFEVect2* poDstUV = &poRnd->m_oUV[uiIdx];
				poDstUV[0] = _poUVs[0];
				poDstUV[1] = _poUVs[1];
				poDstUV[2] = _poUVs[2];
				poDstUV[3] = _poUVs[3];
			}
		}
		else
		{
			// Can be preprocessed 
			CFEMatrix oP;
			oP.LoadIdentity();
			oP.Translate(_0r,_1r);
			oP.Scale(1.0f/(float)poRnd->m_uiVScrWidth,-1.0f/(float)poRnd->m_uiVScrHeight);

			CFEMatrix oMVP;
			oMVP.Multiply(poRP->m_oRS.m_oTransf,oP);

			// process UV
			CFEVect2* poDstUV = &poRnd->m_oUV[uiIdx];

			for (uint v=0;v<4;v++)
				poDstUV[v] = oMVP.Transform(  poDstVX[v] );
		}

		CFEColor* poDstVC = &poRnd->m_oVC[uiIdx];
		if (_poVCs != NULL)
		{
			poDstVC[0] = _poVCs[0];
			poDstVC[1] = _poVCs[1];
			poDstVC[2] = _poVCs[2];
			poDstVC[3] = _poVCs[3];
		}
		else
		{
			poDstVC[0] = CFEColor::WHITE();
			poDstVC[1] = CFEColor::WHITE();
			poDstVC[2] = CFEColor::WHITE();
			poDstVC[3] = CFEColor::WHITE();
		}

		// really don't needed
		/*
		poRP->m_uNumVX		= poRP->m_uiRndIdx*4;
		poRP->m_poVX		= poRnd->m_oVX[uiIdx];
		poRP->m_poVC		= poRnd->m_oVC[uiIdx];
		poRP->m_poUV		= NULL;
		poRP->m_pusIdx		= NULL;
		*/
	
		poRnd->m_uiPrims++;	
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderSprite(	const CFEVect2& _oPos,const CFEVect2& _oPivot,
					const CFEVect2& _oScale,FEReal _rAngle,
					const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                    const CFEColor& _oColor,FEReal _rAlpha)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

	if (poRnd->m_oRenderState.m_eBlendMode == BM_LIGHTSABER)
	{
		SetBlendMode(BM_LIGHTSABER_STEP_1_INT);
		RenderSprite(_oPos,_oPivot,_oScale,_rAngle,_oIUV,_oFUV,_oColor,_rAlpha);
		SetBlendMode(BM_LIGHTSABER_STEP_2_INT);
		RenderSprite(_oPos,_oPivot,_oScale,_rAngle,_oIUV,_oFUV,_oColor,_rAlpha);
		return;
	}
	
	else if (poRnd->m_oRenderState.m_eBlendMode == BM_REVEAL)
	{
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_1_INT);
		RenderSprite(_oPos,_oPivot,_oScale,_rAngle,_oIUV,_oFUV,_oColor,_rAlpha);
		SetBlendMode(BM_LUMISHADE_REVEAL_STEP_2_INT);
		RenderSprite(_oPos,_oPivot,_oScale,_rAngle,_oIUV,_oFUV,_oColor,_rAlpha);
		return;
	}

	if (poRnd->m_uiPrims >= poRnd->m_usReservedPrims) return;
	CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];
	
	CFEMatrix oMV = poRnd->m_oRenderState.m_oTransf;

	CFEMatrix oMat;
	oMat.LoadIdentity();
	SetTransform(oMat);

	// These determine the order of rendering
	poRP->m_uiRndIdx	= poRnd->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_oRS			= poRnd->m_oRenderState;
	poRP->m_uiHash		= poRnd->uiGetStateHash();
	poRP->m_ePT			= RPT_QUAD;
	poRP->m_uNumVX		= poRnd->m_uiPrims*4;

	uint uiIdx = poRnd->m_uiPrims*4;
    CFEVect2* poDstVX = &poRnd->m_oVX[uiIdx];
    CFEVect2* poDstUV = &poRnd->m_oUV[uiIdx];
    CFEColor* poDstVC = &poRnd->m_oVC[uiIdx];

    /*
    CFEVect2 oPivot = (CFEVect2(0.5,0.5) - _oPivot);
    oVX[0]   = CFEVect2(-0.5,-0.5);
    oVX[2]   = CFEVect2( 0.5, 0.5);
    oVX[1].x = oVX[2].x;
    oVX[1].y = oVX[0].y;
    oVX[3].x = oVX[0].x;
    oVX[3].y = oVX[2].y;

    for (uint i=0;i<4;i++)
    {
        // 1
        oVX[i] += oPivot;

	....

    // step 1
    oVX[0] + (oPivot) = (-0.5,-0.5) + (CFEVect2(0.5,0.5) - _oPivot) = (_0r,_0r) - _oPivot) =  -_oPivot.x, -_oPivot.y
    oVX[2] + (oPivot) = ( 0.5, 0.5) + (CFEVect2(0.5,0.5) - _oPivot) = (_1r,_1r) - _oPivot) = 1-_oPivot.x,1-_oPivot.y
	*/

	// Slight optimization
	if (_rAngle == _0r)
	{
	    poDstVX[0]   = -_oPivot;
		poDstVX[2]   = CFEVect2(_1r,_1r) - _oPivot;
		poDstVX[1].x = poDstVX[2].x;
		poDstVX[1].y = poDstVX[0].y;
		poDstVX[3].x = poDstVX[0].x;
		poDstVX[3].y = poDstVX[2].y;

		for (uint i=0;i<4;i++)
		{
			poDstVX[i] *= _oScale;
			poDstVX[i] += _oPos;
		}
	}
	else
	{
		poDstVX[0]   = -_oPivot;
		poDstVX[2]   = CFEVect2(_1r,_1r) - _oPivot;
		poDstVX[1].x = poDstVX[2].x;
		poDstVX[1].y = poDstVX[0].y;
		poDstVX[3].x = poDstVX[0].x;
		poDstVX[3].y = poDstVX[2].y;

		FEReal rCosA = CFEMath::rCos(_rAngle);
		FEReal rSinA = CFEMath::rSin(_rAngle);

		for (uint i=0;i<4;i++)
		{
			// 2
			poDstVX[i] *= _oScale;

			// 3
			CFEVect2 oPos = poDstVX[i];
			poDstVX[i].x = oPos.x*rCosA - oPos.y*rSinA;
			poDstVX[i].y = oPos.x*rSinA + oPos.y*rCosA;

			// 5
			poDstVX[i] += _oPos;
		}
	}
	
	for (uint i=0;i<4;i++)
		poDstVX[i] = oMV.Transform(poDstVX[i]);
	
	SetTransform(oMV);

    // Set UVs
	if (
	(poRP->m_oRS.m_eBlendMode != BM_REVEAL) 
	&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_1_INT) 
	&& (poRP->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_2_INT)
	)	
	{
		poDstUV[0]   = _oIUV;
		poDstUV[2]   = _oFUV;
		poDstUV[1].x = poDstUV[2].x;
		poDstUV[1].y = poDstUV[0].y;
		poDstUV[3].x = poDstUV[0].x;
		poDstUV[3].y = poDstUV[2].y;
	}
	else
	{
		// Can be preprocessed 
		CFEMatrix oP;
		oP.LoadIdentity();
		oP.Translate(_0r,_1r);
		oP.Scale(1.0f/(float)poRnd->m_uiVScrWidth,-1.0f/(float)poRnd->m_uiVScrHeight);

		CFEMatrix oMVP;
		oMVP.Multiply(poRP->m_oRS.m_oTransf,oP);

		// process UV
		for (uint v=0;v<4;v++)
			poDstUV[v] = oMVP.Transform(  poDstVX[v] );

		/*
		CFEVect2 oOfs(100.0f,100.0f);
		for (uint i=0;i<4;i++)
			poDstVX[i] += oOfs;
		*/
	}

    // Set color
    poDstVC[0] = _oColor;
    poDstVC[0].a *= _rAlpha;
    poDstVC[1] = poDstVC[0];
    poDstVC[2] = poDstVC[1];
    poDstVC[3] = poDstVC[2];
	
	/*
	// really don't needed
	poRP->m_uNumVX		= poRP->m_uiRndIdx*4;
	poRP->m_poVX		= poRnd->m_oVX[uiIdx];
	poRP->m_poVC		= poRnd->m_oVC[uiIdx];
	poRP->m_poUV		= NULL;
	poRP->m_pusIdx		= NULL;
	*/

	poRnd->m_uiPrims++;	
}
// ----------------------------------------------------------------------------
uint uiGetWrapModeFE_ID(GLint _iWrapMode)
{
    if (_iWrapMode == GL_REPEAT)
		return(1);

    else if (_iWrapMode == GL_MIRRORED_REPEAT)
		return(2);

	else
		return(0);
}
// ----------------------------------------------------------------------------
uint uiGetFilterFE_ID(GLint _iFilterMode)
{
    if (_iFilterMode == GL_LINEAR)
		return(1);
	else
		return(0);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{	
    if (_hMaterial == NULL) return(false);
    if (_pParam == NULL) return(false);

    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    if (_sProperty == "DiffuseMap.Width")
    {
        *(uint*)_pParam = poTH->m_poII->m_uiTX;
        return (true);
    }
else if (_sProperty == "DiffuseMap.Height")
    {
        *(uint*)_pParam = poTH->m_poII->m_uiTY;
        return (true);
    }
else if (_sProperty == "DiffuseMap.SWrapMode")
    {
		*(uint*)_pParam = uiGetWrapModeFE_ID(poTH->m_iSWrapMode);
		return (true);
    }
else if (_sProperty == "DiffuseMap.TWrapMode")
    {
		*(uint*)_pParam = uiGetWrapModeFE_ID(poTH->m_iTWrapMode);
		return (true);
    }
else if (_sProperty == "DiffuseMap.Filter")
	{
		*(uint*)_pParam = uiGetFilterFE_ID(poTH->m_iMagFilter);
		return (true);
	}

	if (gshFirstHGLRC == 0) return(false);
    return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    if (gshFirstHGLRC == 0) return(false);
    if (_hMaterial == NULL) return(false);

	const uint CLAMP_MODES[] = {GL_CLAMP_TO_EDGE,GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE};
	const uint FILTER_MODES[] = {GL_NEAREST,GL_LINEAR};

	TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    if (_sProperty == "DiffuseMap.SWrapMode")
    {
		INT_SetMaterial(_hMaterial);

		poTH->m_iSWrapMode = CLAMP_MODES[(uint)_pParam & 0x03];

		if ( poTH->m_iID != -1)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,poTH->m_iSWrapMode);CHECKERROR();
		}
        return (true);
    }
else if (_sProperty == "DiffuseMap.TWrapMode")
    {
		INT_SetMaterial(_hMaterial);

		poTH->m_iTWrapMode = CLAMP_MODES[(uint)_pParam & 0x03];

		if ( poTH->m_iID != -1)
		{
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,poTH->m_iTWrapMode);CHECKERROR();
		}
        return (true);
    }
else if (_sProperty == "DiffuseMap.Filter")
    {
		INT_SetMaterial(_hMaterial);

		poTH->m_iMinFilter = FILTER_MODES[(uint)_pParam & 0x01];
		poTH->m_iMagFilter = FILTER_MODES[(uint)_pParam & 0x01];

		if ( poTH->m_iID != -1)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, poTH->m_iMagFilter);CHECKERROR();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, poTH->m_iMinFilter);CHECKERROR();
        }

		return (true);
    }

    return(false);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::BuildRenderList()
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;
	poRnd->m_oRL.clear();
	
	unsigned short* pusIdx = poRnd->m_usIdx;
	CRenderPrim* poPrim	   = poRnd->m_oRPList;

	for (uint i=0;i<poRnd->m_uiPrims;)
	{
		// pusIdx = reinterpret_cast<unsigned short*>( uiRoundUp(reinterpret_cast<uint>(pusIdx)) );
		uint m_uiStateIdx	= i;
		uint m_uiPrims		= 0;
		uint uiHash			= poPrim->m_uiHash;
		ERenderPrimType ePT = poPrim->m_ePT;
		CRenderPrim* poPrevPrim	= poPrim;

		if (ePT == RPT_QUAD)
		{
			poPrevPrim->m_pusIdx	= pusIdx;

			while ((poPrim->m_uiHash == uiHash) && (poPrim->m_ePT == ePT) && (i<poRnd->m_uiPrims))
			{
				uint uiIdx = poPrim->m_uiRndIdx*4;

				pusIdx[0] = uiIdx;
				pusIdx[1] = uiIdx+1;
				pusIdx[2] = uiIdx+2;
				pusIdx[3] = uiIdx+2;
				pusIdx[4] = uiIdx+3;
				pusIdx[5] = uiIdx+0;

				pusIdx+=6;			// next idx
				m_uiPrims++;		// one more primitive.
				poPrim++;			// next primitive in the list
				i++;
			}
		}
		else
		{
			// while ((poPrim->m_uiHash == uiHash) && (poPrim->m_ePT == ePT) && (i<poRnd->m_uiPrims))
			{
				m_uiPrims++;		// one more packed primitive.
				poPrim++;			// next primitive in the list
				i++;
			}
		}

		CRenderElem oElem;
		oElem.m_poIniPrim  = poPrevPrim;
		oElem.m_uiNumPrims = m_uiPrims;

		FECglobals.m_poCurrentRenderer->m_oRL.push_back(oElem);
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::FlushRenderList()
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

	// cache projection matrix
	FEHandler		hMat = (FEHandler)0xffffffff;
	EFEBlendMode	eBM  = BM_MAX;
	FEReal			rZ   = -_1r;
	FEBool			bFogEnabled = false;

	for (uint i=0;i<poRnd->m_oRL.size();i++)
	{
		CRenderElem* poElem = &poRnd->m_oRL[i];

		if (poElem->m_poIniPrim->m_oRS.m_eBlendMode != eBM)
		{
			eBM = poElem->m_poIniPrim->m_oRS.m_eBlendMode;

			EnableRender2Texture( (eBM == BM_LIGHTMARKER) || (eBM == BM_MAGICMARKER) || (eBM == BM_LIGHTSABER_STEP_1_INT));

			INT_SetBlendMode(poElem->m_poIniPrim->m_oRS.m_eBlendMode);
			CHECKERROR();
		}

		if (
		(poElem->m_poIniPrim->m_oRS.m_eBlendMode != BM_REVEAL) 
		&& (poElem->m_poIniPrim->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_1_INT) 
		&& (poElem->m_poIniPrim->m_oRS.m_eBlendMode != BM_LUMISHADE_REVEAL_STEP_2_INT)
		)
		{
			if (poElem->m_poIniPrim->m_oRS.m_hMaterial  != hMat)
			{
				hMat = poElem->m_poIniPrim->m_oRS.m_hMaterial;
				INT_SetMaterial(poElem->m_poIniPrim->m_oRS.m_hMaterial);
				CHECKERROR();
			}
			
			glDepthMask(GL_ONE);
		}
		else
		{
			hMat = (FEHandler)0xffffffff;
			glBindTexture(GL_TEXTURE_2D,FECglobals.m_poCurrentRenderer->m_uiR2TTex);
			glDepthMask(GL_ZERO);
		}

		/*
		if (poElem->m_poIniPrim->m_oRS.m_rDepth != rZ)
		{
			rZ = poElem->m_poIniPrim->m_oRS.m_rDepth;
			INT_SetDepth(poElem->m_poIniPrim->m_oRS.m_rDepth);
		}
		*/

		if (poElem->m_poIniPrim->m_oRS.m_bFogEnabled != bFogEnabled)
		{
			bFogEnabled = poElem->m_poIniPrim->m_oRS.m_bFogEnabled;

			if (bFogEnabled)
				glEnable(GL_FOG);
			else 
				glDisable(GL_FOG);
			
			CHECKERROR();
		}
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

			INT_SetTransform(poElem->m_poIniPrim->m_oRS.m_oTransf, poElem->m_poIniPrim->m_oRS.m_rDepth);

			switch (poElem->m_poIniPrim->m_ePT)
			{
				case  RPT_LINE:
				INT_RenderLine(	poRnd->m_oVX + poElem->m_poIniPrim->m_uiRndIdx*4,
								poRnd->m_oVC + poElem->m_poIniPrim->m_uiRndIdx*4,
								poElem->m_uiNumPrims);
				break;

				case RPT_QUAD:
				#ifndef DS_PC
				INT_RenderMesh( poElem->m_poIniPrim->m_pusIdx,
								poRnd->m_oVX,
								poRnd->m_oUV,
								poRnd->m_oVC,
								poElem->m_uiNumPrims*6);
				#else
				INT_RenderQuads(poElem->m_poIniPrim->m_pusIdx,
								poRnd->m_oVX,
								poRnd->m_oUV,
								poRnd->m_oVC,
								poElem->m_uiNumPrims);
				#endif
				break;

				case RPT_MESH:
				INT_RenderMesh(	poElem->m_poIniPrim->m_pusIdx,
								poElem->m_poIniPrim->m_poVX,
								poElem->m_poIniPrim->m_poUV,
								poElem->m_poIniPrim->m_oColor,
								poElem->m_poIniPrim->m_uNumVX);
				break;
			}

			if (poRnd->m_bFBOEnabled)
				poRnd->m_uiLightPrims += poElem->m_uiNumPrims;

		glMatrixMode(GL_MODELVIEW); 
		glPopMatrix();
		CHECKERROR();

		if (
		(poElem->m_poIniPrim->m_oRS.m_eBlendMode == BM_REVEAL) 
		|| (poElem->m_poIniPrim->m_oRS.m_eBlendMode == BM_LUMISHADE_REVEAL_STEP_1_INT) 
		|| (poElem->m_poIniPrim->m_oRS.m_eBlendMode == BM_LUMISHADE_REVEAL_STEP_2_INT)
		)
		{
			// once reveal is done, clean the texture frame buffer.
			FECglobals.m_poCurrentRenderer->m_bFBOCleared = false;
		}
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetTransform(const CFEMatrix& _rMat,FEReal _rDepth)
{
	glMatrixMode(GL_MODELVIEW);
	CHECKERROR();

	GLfloat m[4][4];
	m[0][0] = _rMat.m[0][0];
	m[0][1] = _rMat.m[0][1];
	m[0][2] = 0.0f;
	m[0][3] = _rMat.m[0][2];

	m[1][0] = _rMat.m[1][0];
	m[1][1] = _rMat.m[1][1];
	m[1][2] = 0.0f;
	m[1][3] = _rMat.m[1][2];

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = _rMat.m[2][0];
	m[3][1] = _rMat.m[2][1];
	m[3][2] = _rDepth/1000.0f;		// 1000.0f = "MAX_DEPTH_VALUE"
									// Should only be useful for ZBuffer sorting purposes
	m[3][3] = _rMat.m[2][2];

	if (_rDepth > _1r)
	{
		glLoadIdentity();
		
		float fXOfs = FECglobals.m_poCurrentRenderer->m_uiVScrWidth*0.5f;
		float fYOfs = FECglobals.m_poCurrentRenderer->m_uiVScrHeight*0.5f;

		glTranslatef(fXOfs, fYOfs, 0.0f);
		glScalef(1.0f / _rDepth, 1.0f / _rDepth,_1r);
		glTranslatef(-fXOfs, -fYOfs, 0.0f); 

		glMultMatrixf((GLfloat*)m);
		CHECKERROR();
	}
	else
	{
		glLoadMatrixf((GLfloat*)m);
		CHECKERROR();
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetMaterial(FEHandler _hMaterial)
{
    if (_hMaterial == NULL)
    {
        glDisable(GL_TEXTURE_2D);
		CHECKERROR();
        return;
    }

    // Material has been uploaded?
    TMaterialHandler* poTexHnd = (TMaterialHandler*)_hMaterial;
    if (poTexHnd->m_iID == -1)
    {
        // Material needs to be uploaded!
        poTexHnd->m_iID = iCreateTexture(poTexHnd);
        if (poTexHnd->m_iID==-1)
        {
			glDisable(GL_TEXTURE_2D);
			CHECKERROR();
			return;
        }
    }

	/*
	for (uint i=0;i<gsuiNumTexUnits;i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		CHECKERROR();

	    //
		glEnable(GL_TEXTURE_2D);
		CHECKERROR();
	
		// Bind the material
		glBindTexture(GL_TEXTURE_2D,poTexHnd->m_iID);
		CHECKERROR();
	}
	*/

	//
	glEnable(GL_TEXTURE_2D);
	CHECKERROR();

	// Bind the material
	glBindTexture(GL_TEXTURE_2D,poTexHnd->m_iID);
	CHECKERROR();

	gsuiLastTexture = poTexHnd->m_iID;
}
// ----------------------------------------------------------------------------
/*
void CFECore::Graphics::INT_SetDepth(FEReal _rDepth)
{
	FECglobals.m_rCurDepth = _rDepth;
}
*/
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetBlendMode(EFEBlendMode _eBlendMode)
{
	gsuiNumTexUnits = 1;

	switch (_eBlendMode)
    {
        case BM_NONE:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glAlphaFunc(GL_GREATER,0.0f);
			glDisable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_ONE,GL_ZERO,GL_ZERO,GL_ZERO);
		}
        break;

        case BM_COPY:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);

			glAlphaFunc(GL_GREATER,0.0f);
			glDisable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_ONE,GL_ZERO,GL_ZERO,GL_ZERO);
		}
        break;

        case BM_ALPHA:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ZERO,GL_ZERO);

		}
		break;

        case BM_ADD:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_ONE,GL_ONE,GL_ZERO,GL_ZERO);
		}
        break;

        case BM_ALPHAADD:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
		}
        break;

        case BM_MULT:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_DST_COLOR,GL_ZERO,GL_ZERO,GL_ZERO);
		}
        break;

		case BM_FOG:
		{
			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);		
			glBindTexture(GL_TEXTURE_2D,gsuiLastTexture);			
		
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);				//Add RGB with RGB
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB, GL_SRC_ALPHA);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
						
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,1.0f);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

			glAlphaFunc(GL_GREATER,0.0f);

			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ZERO,GL_ZERO);
		}
		break;

		case BM_FOGADD:
		{
			gsuiNumTexUnits = 2;

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gsuiLastTexture);

						
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);				//Add RGB with RGB
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE0);			
			glEnable(GL_TEXTURE_2D);			
			
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,1.0f);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

			glAlphaFunc(GL_GREATER,0.0f);
			
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
		}
		break;

		case BM_MAGICMARKER:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);

			glAlphaFunc(GL_ALWAYS,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_ZERO,GL_ZERO,GL_ONE,GL_ONE);
/*
			glActiveTexture(GL_TEXTURE1);
			CHECKERROR();
			
			glDisable(GL_TEXTURE_2D);
			CHECKERROR();
			
			glActiveTexture(GL_TEXTURE0);
			CHECKERROR();

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			CHECKERROR();

			glAlphaFunc(GL_ALWAYS,0.0f);
			CHECKERROR();
			
			glEnable(GL_BLEND);
			CHECKERROR();

			glBlendFuncSeparateEXT(GL_DST_COLOR,GL_ZERO,GL_ZERO,GL_ZERO);
*/
		}
		break;

		case BM_LIGHTMARKER:
		{
			glActiveTexture(GL_TEXTURE1);
			CHECKERROR();
			
			glDisable(GL_TEXTURE_2D);
			CHECKERROR();
			
			glActiveTexture(GL_TEXTURE0);
			CHECKERROR();

			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			CHECKERROR();

			glAlphaFunc(GL_ALWAYS,0.0f);
			CHECKERROR();
			
			glEnable(GL_BLEND);
			CHECKERROR();
			
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
			CHECKERROR();
		}
        break;

		case BM_REVEAL:
		{
			glActiveTexture(GL_TEXTURE1);
			CHECKERROR();

			glDisable(GL_TEXTURE_2D);
			CHECKERROR();

			glActiveTexture(GL_TEXTURE0);
			CHECKERROR();
			
			glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_MODULATE);
			CHECKERROR();
			
			glAlphaFunc(GL_GREATER,0.0f);
			CHECKERROR();
			
			glEnable(GL_BLEND);
			CHECKERROR();

			glBlendFuncSeparateEXT(GL_DST_COLOR,GL_ZERO,GL_ZERO,GL_ZERO);
			CHECKERROR();

			glEnable(GL_TEXTURE_2D);
			CHECKERROR();
		}
		break;

		case BM_LIGHTSABER_STEP_1_INT:
		{
			glActiveTexture(GL_TEXTURE1);
			CHECKERROR();

			glDisable(GL_TEXTURE_2D);
			CHECKERROR();

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);			// GL_COMBINE

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_ADD);				// Texture RGB + Texture A
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB,	GL_TEXTURE);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,1.0f);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA,	GL_REPLACE);			// Vertex Color A
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,	GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA,GL_SRC_ALPHA);

			glAlphaFunc(GL_GREATER,0.0f);

			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_ONE,GL_ZERO);
		}
		break;
		
		case BM_LIGHTSABER_STEP_2_INT:
		{
			gsuiNumTexUnits = 2;

			glActiveTexture(GL_TEXTURE1);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gsuiLastTexture);					// believe or not, this is important.			

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_ADD);				// Texture RGB + PreviousResult
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_TEXTURE0);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB,	GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB,	GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);			// Previous A (see below)
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);			

			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);			// GL_COMBINE

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_MODULATE);			// Texture A * Texture RGB
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB,	GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB,	GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,1.0f);
			
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA,	GL_REPLACE);		// Vertex Color A
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA,	GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA,GL_SRC_ALPHA);

			glAlphaFunc(GL_GREATER,0.0f);
			
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_ONE,GL_ZERO);
		}
		break;

		case BM_LUMISHADE_REVEAL_STEP_1_INT:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);

			glActiveTexture(GL_TEXTURE0);			
			glEnable(GL_TEXTURE_2D);
			
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);			// GL_COMBINE

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_REPLACE);			// 2*Texture color*Vertex Color RGB 
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,2.0f);

			glAlphaFunc(GL_ALWAYS,0.0f);

			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_DST_COLOR,GL_ZERO,GL_ZERO,GL_ZERO);
		}
		break;

		case BM_LUMISHADE_REVEAL_STEP_2_INT:
		{
			glActiveTexture(GL_TEXTURE1);
			glDisable(GL_TEXTURE_2D);
		
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);			// GL_COMBINE

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB,	GL_SUBTRACT);			// 2*(Previous - 0.5)
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB,	GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB,	GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB,	GL_CONSTANT);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB,	GL_SRC_COLOR);
			glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE,2.0f);
			static GLfloat constColor[4] = {0.5f,0.5f,0.5f,0.5f};
			glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,constColor);						

			glAlphaFunc(GL_ALWAYS,0.0f);

			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_ONE,GL_ONE,GL_ZERO,GL_ZERO);
		}
		break;
		
    }
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints)
{
// return;
/*
	if (_poUVs)
	{
		for (uint i=0;i<gsuiNumTexUnits;i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			CHECKERROR();
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKERROR();
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), (float*)_poUVs);
			CHECKERROR();
		}

		glClientActiveTexture(GL_TEXTURE0);
		CHECKERROR();
	}
	else
	{
		for (uint i=0;i<gsuiNumTexUnits;i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			CHECKERROR();
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKERROR();
		}
		
		glClientActiveTexture(GL_TEXTURE0);
		CHECKERROR();
	}
*/

	if (_poUVs)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKERROR();
		glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), (float*)_poUVs);
		CHECKERROR();
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKERROR();
	}
	
    glColor4f(_oColor.r,_oColor.g,_oColor.b,_oColor.a);
    CHECKERROR();
	glDisableClientState(GL_COLOR_ARRAY);
	CHECKERROR();

	if (_poVXs)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		CHECKERROR();
		glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), _poVXs);
		CHECKERROR();
	}
	else
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		CHECKERROR();
	}

	if (_pusIdx)
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);
	CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints)
{
	/*
	if (_poUVs)
	{
		for (uint i=0;i<gsuiNumTexUnits;i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			CHECKERROR();
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKERROR();
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), (float*)_poUVs);
			CHECKERROR();
		}

		glClientActiveTexture(GL_TEXTURE0);
		CHECKERROR();
	}
	else
	{
		for (uint i=0;i<gsuiNumTexUnits;i++)
		{
			glClientActiveTexture(GL_TEXTURE0 + i);
			CHECKERROR();
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			CHECKERROR();
		}
		
		glClientActiveTexture(GL_TEXTURE0);
		CHECKERROR();
	}
	*/
	
	if (_poUVs)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKERROR();
		glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), (float*)_poUVs);
		CHECKERROR();
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		CHECKERROR();
	}

	if (_poVCs)
	{
		glEnableClientState(GL_COLOR_ARRAY);
		CHECKERROR();
		glColorPointer(4, GL_FLOAT, 4*sizeof(GLfloat), (float*)_poVCs);
		CHECKERROR();
	}
	else
	{
	    glColor4f(1,1,1,1);
	    CHECKERROR();
		glDisableClientState(GL_COLOR_ARRAY);
		CHECKERROR();
	}

	if (_poVXs)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		CHECKERROR();
		glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), _poVXs);
		CHECKERROR();
	}
	else
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		CHECKERROR();
	}

	if (_pusIdx)
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);
	CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderQuads(unsigned short* _pusIdx,CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads)
{
	glBegin(GL_TRIANGLES);

		if (_poUV != NULL)
		{ 
			for (uint i=0;i<_uiNumQuads;i++)
			{
				for (uint j=0;j<6;j++)
				{
					//
					uint uiIdx = *_pusIdx;

					glColor4f((float)_poVC[uiIdx].r,(float)_poVC[uiIdx].g,(float)_poVC[uiIdx].b,(float)_poVC[uiIdx].a);
					glTexCoord2f((float)_poUV[uiIdx].x,(float)_poUV[uiIdx].y);

					/*
					#ifdef DS_PC
					float fX = (float)_poVX.x;
					float fY = (float)_poVX.y;

					float fU = (float)_poUV.x;
					float fV = (float)_poUV.y;

					float fR = (float)_poVC.r;
					float fG = (float)_poVC.g;
					float fB = (float)_poVC.b;
					float fA = (float)_poVC.a;
					#endif
					*/

					glVertex2f((float)_poVX[uiIdx].x,(float)_poVX[uiIdx].y);

					// next vertex
					_pusIdx++;
				}				
			}
		}
		else
		{
			for (uint i=0;i<_uiNumQuads;i++)
			{
				for (uint j=0;j<6;j++)
				{
					//
					uint uiIdx = *_pusIdx;
					glColor4f((float)_poVC[uiIdx].r,(float)_poVC[uiIdx].g,(float)_poVC[uiIdx].b,(float)_poVC[uiIdx].a);
					glVertex2f((float)_poVX[uiIdx].x,(float)_poVX[uiIdx].y);

					// next vertex
					_pusIdx++;
				}
			}
		}

	glEnd();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderLine(CFEVect2* _poVXs,CFEColor* _poVCs,uint _uiPrims)
{
	glBegin(GL_LINES);

		for (uint i=0;i<_uiPrims;i++)
		{
			glColor4fv((float*)_poVCs);
			glVertex2fv((float*)&_poVXs[0]);
			glVertex2fv((float*)&_poVXs[1]);
			
			_poVXs += 4;
			_poVCs += 4;
		}

	glEnd();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SaveTGA(const CFEString& _sFilename,FEPointer _pData,uint _uiWidth,uint _uiHeight,uint _uiPixSize)
{
	#pragma pack(push,1)

		typedef struct {
		
			unsigned char	id_len;
			unsigned char	colormap_type;
			unsigned char	image_type;

			unsigned short	colormap_origin;
			unsigned short	colormap_len;
			unsigned char	colormap_entry;

			unsigned short	x_origin;
			unsigned short	y_origin;
			unsigned short	width; 
			unsigned short	height;

			unsigned char	pixsize;
			unsigned char	descriptor;
		
		} TTGAHeader;

		TTGAHeader tga;

	#pragma pack(pop)

	CFEString sFilename = _sFilename + ".tga";

	#define STDFILESYS
	#ifndef STDFILESYS
	FEHandler hHnd = CFECore::File::hOpen(sFilename,FOM_WRITE);
	#else
	FILE* hHnd = NULL;
	fopen_s(&hHnd,sFilename.szString(),"wb");
	#endif

	if (hHnd != NULL)
	{
		#define TGA_HEADER_SIZE	18

		tga.id_len			= 0;
		tga.colormap_type	= 0x00;
		tga.image_type		= 0x02;
		tga.colormap_origin = 0;
		tga.colormap_len	= 0;
		tga.colormap_entry	= 0;
		tga.x_origin		= 0;
		tga.y_origin		= 0;
		tga.width			= _uiWidth;
		tga.height			= _uiHeight;
		tga.pixsize			= _uiPixSize*8;
		tga.descriptor		= 0x00;
		
		// swap r and b channels
		unsigned char* pucData	   = _pData;
		uint uiTotalPixels = _uiWidth*_uiHeight;
		for (uint i=0;i<uiTotalPixels;i++)
		{
			char ucVal = pucData[0];
			pucData[0] = pucData[2];
			pucData[2] = ucVal;

			pucData += _uiPixSize;
		}

		// write data
		#ifndef STDFILESYS
		CFECore::File::uiWrite(hHnd,&tga,TGA_HEADER_SIZE);
		CFECore::File::uiWrite(hHnd,_pData,_uiWidth*_uiHeight*_uiPixSize);
		CFECore::File::Close(hHnd);
		#else
		fwrite(&tga,TGA_HEADER_SIZE,1,hHnd);
		fwrite(_pData,_uiWidth*_uiHeight*_uiPixSize,1,hHnd);
		fclose(hHnd);
		#endif
	}
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty == "RENDERED_PRIMS")
    {
		if (_pParam != NULL)
		{
			*reinterpret_cast<uint*>(_pParam) = FECglobals.m_poCurrentRenderer->m_uiPrims;
			return (true);
		}
    }
else if (_sProperty == "ScreenWidth")
    {
		if (_pParam != NULL)
		{
			*reinterpret_cast<uint*>(_pParam) = FECglobals.m_poCurrentRenderer->m_uiScrWidth;
			return (true);
		}
    } 
else if (_sProperty == "ScreenHeight")
    {
		if (_pParam != NULL)
		{
			*reinterpret_cast<uint*>(_pParam) = FECglobals.m_poCurrentRenderer->m_uiScrHeight;
			return (true);
		}
    } 
	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty == "ScreenWidth")
    {
		if (FECglobals.m_poCurrentRenderer != NULL)
		{
			FECglobals.m_poCurrentRenderer->m_uiScrWidth = GETPROPVALUE(uint,_pParam);
			RecalcViewport(FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight,FECglobals.m_poCurrentRenderer);
			return (true);
		}
		else
			return (false);
    }
else if (_sProperty == "ScreenHeight")
    {
		if (FECglobals.m_poCurrentRenderer != NULL)
		{
			FECglobals.m_poCurrentRenderer->m_uiScrHeight =  GETPROPVALUE(uint,_pParam);		
			RecalcViewport(FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight,FECglobals.m_poCurrentRenderer);
			return (true);
		}
		else
			return (false);
    }
else if (_sProperty |= "KEEP_DESIGN_RATIO")
    {
		FECglobals.m_bKeepDesignAspect = (_pParam == NULL)?false:true; // GETPROPVALUE(FEBool,_pParam);

		if (FECglobals.m_poCurrentRenderer != NULL)
			RecalcViewport(FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight,FECglobals.m_poCurrentRenderer);

        return(true);
    }
else if (_sProperty |= "RESERVE_PRIMITIVES")
    {
    	CFECore::Graphics::m_uiInitialReservedPrims = GETPROPVALUE(uint,_pParam);
        return(true);
    }
else if (_sProperty == "FOG_RENDER")
    {
		if (FECglobals.m_poCurrentRenderer != NULL)
		{
			FECglobals.m_poCurrentRenderer->m_oRenderState.m_bFogEnabled = (_pParam == NULL)?false:true; // GETPROPVALUE(FEBool,_pParam);
			FECglobals.m_poCurrentRenderer->SetFlag(STATE_FLAG_FOG);
			return (true);
		}
		else
			return (false);
    }
else if (_sProperty == "FOG_COLOR")
    {
   		glFogfv (GL_FOG_COLOR, GETPROPVALUE(float*,_pParam));
        return (true);
    }
else if (_sProperty == "FOG_DENSITY")
    {
		glFogf  (GL_FOG_START,*GETPROPVALUE(float*,_pParam));
		glFogf  (GL_FOG_END  ,1.0f);
		glFogf  (GL_FOG_DENSITY,*GETPROPVALUE(float*,_pParam));
        return (true);
    }
else if (_sProperty == "SCREENSHOT")
    {
		if (FECglobals.m_poCurrentRenderer != NULL)
		{
			uint uiWidth  = uiRoundUp(FECglobals.m_poCurrentRenderer->m_uiScrWidth);
			uint uiHeight = uiRoundUp(FECglobals.m_poCurrentRenderer->m_uiScrHeight);
			FEPointer pData = CFECore::Mem::pAlloc(uiWidth*uiHeight*4);
			if (pData == NULL)  return(false);

			// Render always the centered image
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				FlushRenderList();
				glFinish();

				glReadPixels(0,0,uiWidth,uiHeight,GL_RGB,GL_UNSIGNED_BYTE,pData);
				SaveTGA(*(CFEString*)_pParam,pData,uiWidth,uiHeight,3);

			::SwapBuffers( FECglobals.m_poCurrentRenderer->m_hDC );
				
			CFECore::Mem::Free(pData);
			return (true);
		}
		else
			return (false);
	}
else if (_sProperty == "ZSCREENSHOT")
    {
		if (FECglobals.m_poCurrentRenderer != NULL)
		{
			uint uiWidth  = uiRoundUp(FECglobals.m_poCurrentRenderer->m_uiScrWidth);
			uint uiHeight = uiRoundUp(FECglobals.m_poCurrentRenderer->m_uiScrHeight);
			FEPointer pData = CFECore::Mem::pAlloc(uiWidth*uiHeight*4);
			if (pData == NULL)  return(false);

			// Render always the centered image
			glClearColor(0,0,0,1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				FlushRenderList();
				glFinish();

				glReadPixels(0,0,uiWidth,uiHeight,GL_DEPTH_COMPONENT,GL_FLOAT,pData);
				SaveTGA(*(CFEString*)_pParam,pData,uiWidth,uiHeight,4);

			::SwapBuffers( FECglobals.m_poCurrentRenderer->m_hDC );
				
			CFECore::Mem::Free(pData);
			return (true);
		}
		else
			return (false);
	}

	return(false);
}
//-----------------------------------------------------------------------------------------
void CFECore::Graphics::EnableRender2Texture(bool _bEnable)
{
	// if the operation is already done ...
	if (_bEnable == FECglobals.m_poCurrentRenderer->m_bFBOEnabled) return;

	if(_bEnable)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, FECglobals.m_poCurrentRenderer->m_uiFBO);
		CHECKERROR();
	
		GLenum status; 
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
		CHECKERROR();

		if (! FECglobals.m_poCurrentRenderer->m_bFBOCleared) 
		{
			glClearDepth(_1r);
			glClearColor(_0r,_0r,_0r,_1r);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			FECglobals.m_poCurrentRenderer->m_bFBOCleared = true;
		}

		glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiR2TWidth,FECglobals.m_poCurrentRenderer->m_uiR2THeight);
		FECglobals.m_poCurrentRenderer->m_bFBOEnabled = true;
	}
	else
	{
		glFinish();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		CHECKERROR();

		// Setup screen virtual dimensions transform matrix.
		RecalcViewport(FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight,FECglobals.m_poCurrentRenderer);
		FECglobals.m_poCurrentRenderer->m_bFBOEnabled = false;

		GLenum status; 
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER); 
		CHECKERROR();
	}
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
