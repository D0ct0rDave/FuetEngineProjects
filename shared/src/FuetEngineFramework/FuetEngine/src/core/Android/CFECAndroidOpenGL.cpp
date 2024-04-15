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
#if ((GRAPHICS == USE_OPENGL) && (TARGETPLATFORM == USE_ANDROID))
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include <jni.h>
#include "jnihelper.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "esUtil.h"

#include "../CFECore.h"

 #include "FEBasicTypes.h"
#include "Types/CFEKFBFunc.h"
#include "CFECGlobals.h"
#include "FEDefs.h"
// #include <GL/gl.h>
// #include <GL/glext.h>

#include <stdlib.h>
#include <math.h>

#include "support/misc/CFEStringUtils.h"

#include "../CFECore.h"
#include "../CFECoreGlobals.h"

#include "../Common/CFECImageLib.h"
#include "../Common/CFERenderList.h"

#include "FEBasicTypes.h"

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// ----------------------------------------------------------------------------
#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}
// ----------------------------------------------------------------------------
#ifndef GETPROPVALUE
#define GETPROPVALUE(type,a) reinterpret_cast<type>(a)
#endif
// ----------------------------------------------------------------------------
#define GLAPI
#define APIENTRY

const uint MEM_ALIGNMENT = 32;
uint uiRoundUp(uint _uiSize)
{
	return( ((_uiSize) + ((MEM_ALIGNMENT)-1)) & ~((MEM_ALIGNMENT)-1) );
}
// ----------------------------------------------------------------------------
const unsigned int INITIAL_RESERVED_PRIMS = 1024;
// ----------------------------------------------------------------------------
const uint MAX_BUFFERS = 2;
// ----------------------------------------------------------------------------
#define STATE_FLAG_TRANSFORM	0x01
#define STATE_FLAG_BLENDMODE	0x02
#define STATE_FLAG_MATERIAL		0x04
#define STATE_FLAG_SHADER		0x08
#define STATE_FLAG_DEPTH		0x10

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

		/// Comparison operator for sorting purposes.
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
const uint NUM_SHADERS	= 4;
// ----------------------------------------------------------------------------
class CProgramInfo
{
	public:

		GLuint 	m_uiID;

		GLint   m_iVXLoc;					// Position Attribute Location
		GLint  	m_iUVLoc;					// TexCoord Attribute Location
		GLint  	m_iVCLoc;					// VertexColor Attribute/Uniform Location

		GLint  	m_iDepthLoc;			    // Uniform locations
		GLint  	m_iPVMMatrixLoc;			// Uniform locations
};
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:

        CRendererHandler(unsigned short _usReservedPrims) :
                                // m_hDC(NULL),
								// m_hWnd(NULL),
								// m_hGLRC(NULL),

								m_uiVScrWidth(640),
								m_uiVScrHeight(480),
								m_uiScrWidth(0),
								m_uiScrHeight(0),
								m_rCurDepth(_0r),
								m_hCurTex(NULL),

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

        // HDC     m_hDC;                      // Display Contex
        // HWND    m_hWnd;                     // Window handler
        // HGLRC   m_hGLRC;                    // GL Rendering Context

        uint    m_uiVScrWidth;              // Virtual Screen Width
        uint    m_uiVScrHeight;             // Virtual Screen Height
        uint    m_uiScrWidth;               // Actual Screen Width
        uint    m_uiScrHeight;              // Actual Screen Height
        FEReal  m_rCurDepth;
        FEHandler m_hCurTex;

		ESMatrix m_oPMVMatrix;				// Projection * ModelViewMatrix
		ESMatrix m_oModelMatrix;			// ModelViewMatrix
		ESMatrix m_oProjMatrix;				// ProjectionMatrix

		CFEArray<CRenderElem>				m_oRL;

        static CProgramInfo                 m_oPrograms[NUM_SHADERS];
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
CProgramInfo CRendererHandler::m_oPrograms[NUM_SHADERS];
// ----------------------------------------------------------------------------
void RecalcScreenOrientation()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;
    CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

    // ------------------------------------------------------------
    // BEGIN: Compute proper viewport
    // ------------------------------------------------------------
	FECglobals.m_bViewportRotated =
        ((poRnd->m_uiScrWidth < poRnd->m_uiScrHeight) && (poRnd->m_uiVScrWidth > poRnd->m_uiVScrHeight))
        ||
        ((poRnd->m_uiScrWidth > poRnd->m_uiScrHeight) && (poRnd->m_uiVScrWidth < poRnd->m_uiVScrHeight));

    FECglobals.m_uiViewportWidth    = poRnd->m_uiScrWidth;
    FECglobals.m_uiViewportHeight   = poRnd->m_uiScrHeight;
    FECglobals.m_uiViewportWMargin  = 0;
    FECglobals.m_uiViewportHMargin  = 0;

    // setup properly the viewport...
    FEReal rVRatio = ((FEReal)poRnd->m_uiVScrWidth/(FEReal)poRnd->m_uiVScrHeight);
    FEReal rRRatio = ((FEReal)poRnd->m_uiScrWidth /(FEReal)poRnd->m_uiScrHeight );

    if (FECglobals.m_bViewportRotated == false)
    {
        // horizontal layout
        if (rRRatio<rVRatio)
        {
            // horizontal bands

            // real ratio > virtual ratio
            FECglobals.m_uiViewportWidth   = poRnd->m_uiScrWidth;
            FECglobals.m_uiViewportHeight  = (FEReal)poRnd->m_uiScrWidth/rVRatio;
            FECglobals.m_uiViewportHMargin = (poRnd->m_uiScrHeight - FECglobals.m_uiViewportHeight) / 2;
            FECglobals.m_uiViewportWMargin = 0;
        }
        else
        {
            // vertical bands

            // real ratio > virtual ratio
            FECglobals.m_uiViewportWidth   = (FEReal)poRnd->m_uiScrHeight*rVRatio;
            FECglobals.m_uiViewportHeight  = poRnd->m_uiScrHeight;
            FECglobals.m_uiViewportWMargin = (poRnd->m_uiScrWidth - FECglobals.m_uiViewportWidth) / 2;
            FECglobals.m_uiViewportHMargin = 0;
        }
    }
    else
    {
        rRRatio = 1.0f / rRRatio;

        // vertical layout
        if (rRRatio<rVRatio)
        {
            // horizontal bands

            // real ratio > virtual ratio
            FECglobals.m_uiViewportWidth   = (FEReal)poRnd->m_uiScrHeight/rVRatio;
            FECglobals.m_uiViewportHeight  = poRnd->m_uiScrHeight;
            FECglobals.m_uiViewportWMargin = (poRnd->m_uiScrWidth - FECglobals.m_uiViewportWidth) / 2;
            FECglobals.m_uiViewportHMargin = 0;
        }
        else
        {
            // vertical bands

            // real ratio > virtual ratio
            FECglobals.m_uiViewportWidth   = poRnd->m_uiScrWidth;
            FECglobals.m_uiViewportHeight  = (FEReal)poRnd->m_uiScrWidth*rVRatio;
            FECglobals.m_uiViewportWMargin = 0;
            FECglobals.m_uiViewportHMargin = (poRnd->m_uiScrHeight - FECglobals.m_uiViewportHeight) / 2;
        }
    }
}
// ----------------------------------------------------------------------------
namespace CFECore { namespace Graphics {

	void BuildRenderList();
	void FlushRenderList();
	void EnableRender2Texture(bool _bEnable);

	void INT_SetTransform(const CFEMatrix& _oTransf);
	void INT_SetMaterial(FEHandler _hHnd);
	void INT_SetDepth(FEReal _rDepth);
	void INT_SetBlendMode(EFEBlendMode _eBM);

	void INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints);
	void INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints);

	static uint m_uiInitialReservedPrims = INITIAL_RESERVED_PRIMS;
};
};
// ----------------------------------------------------------------------------
void PrintMatrices()
{
    CFECore::Log::Print("-----------------------------");
    CFECore::Log::Print("ModelView %.2f %.2f %.2f %.2f\n",  FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[0][0],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[0][1],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[0][2],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[0][3]);

    CFECore::Log::Print("ModelView %.2f %.2f %.2f %.2f\n",  FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[1][0],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[1][1],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[1][2],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[1][3]);

    CFECore::Log::Print("ModelView %.2f %.2f %.2f %.2f\n",  FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[2][0],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[2][1],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[2][2],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[2][3]);

    CFECore::Log::Print("ModelView %.2f %.2f %.2f %.2f\n",  FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[3][0],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[3][1],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[3][2],
                                                            FECglobals.m_poCurrentRenderer->m_oModelMatrix.m[3][3]);

    CFECore::Log::Print("-----------------------------");
    CFECore::Log::Print("Projection %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[0][0],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[0][1],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[0][2],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[0][3]);

    CFECore::Log::Print("Projection %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[1][0],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[1][1],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[1][2],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[1][3]);

    CFECore::Log::Print("Projection %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[2][0],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[2][1],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[2][2],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[2][3]);

    CFECore::Log::Print("Projection %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[3][0],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[3][1],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[3][2],
                                                            FECglobals.m_poCurrentRenderer->m_oProjMatrix.m[3][3]);

    CFECore::Log::Print("-----------------------------");
    CFECore::Log::Print("m_oPMVMatrix %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[0][0],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[0][1],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[0][2],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[0][3]);

    CFECore::Log::Print("m_oPMVMatrix %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[1][0],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[1][1],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[1][2],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[1][3]);

    CFECore::Log::Print("m_oPMVMatrix %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[2][0],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[2][1],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[2][2],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[2][3]);

    CFECore::Log::Print("m_oPMVMatrix %.4f %.4f %.4f %.4f\n",  FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[3][0],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[3][1],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[3][2],
                                                            FECglobals.m_poCurrentRenderer->m_oPMVMatrix.m[3][3]);

}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
static int gs_iGLChecks = 0;
#define CHECKERROR() eErr=glGetError();gs_iGLChecks++;if (eErr!=0) CFECore::Log::Print("%d - GLError %d",gs_iGLChecks, eErr);
#else
#define CHECKERROR()
#define eErr
#define gs_iGLChecks 0
#endif
// ----------------------------------------------------------------------------
GLAPI void APIENTRY glBlendFuncSeparateEXT_FACKED(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
	glBlendFunc(sfactorRGB, dfactorRGB);
}

GLAPI void APIENTRY glBlendEquationEXT_FACKED(GLenum mode)
{
	// glBlendEquation(sfactorRGB, dfactorRGB);
}

GLAPI void APIENTRY glBlendEquationSeparateEXT_FACKED(GLenum modeRGB, GLenum modeAlpha)
{
	glBlendEquationEXT_FACKED(modeRGB);
}

GLAPI void APIENTRY glActiveTexture_FACKED(GLenum texture)
{
	// glBlendEquationEXT_FACKED(modeRGB);
}

GLAPI void APIENTRY glGenFramebuffers_FACKED(GLsizei n, GLuint *framebuffers)
{

};

GLAPI void APIENTRY glGenRenderbuffers_FACKED(GLsizei n, GLuint *renderbuffers)
{

};

GLAPI void APIENTRY glBindFramebuffer_FACKED(GLenum target, GLuint framebuffer)
{

};

GLAPI void APIENTRY glBlitFramebuffer_FACKED(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{

};

GLAPI void APIENTRY glFramebufferTexture2D_FACKED(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)
{

};

GLAPI GLenum APIENTRY glCheckFramebufferStatus_FACKED(GLenum target)
{
	return(0);
};

GLAPI void APIENTRY glBindRenderbuffer_FACKED(GLenum target, GLuint renderbuffer)
{

};

GLAPI void APIENTRY glRenderbufferStorage_FACKED(GLenum target, GLenum internalformat, GLsizei width, GLsizei height)
{

};

GLAPI void APIENTRY glFramebufferRenderbuffer_FACKED(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)
{

};
// ----------------------------------------------------------------------------
/*
PFNGLBLENDEQUATIONPROC				glBlendEquationEXT			= glBlendEquationEXT_FACKED;
PFNGLBLENDEQUATIONSEPARATEEXTPROC	glBlendEquationSeparateEXT	= glBlendEquationSeparateEXT_FACKED;
PFNGLBLENDFUNCSEPARATEEXTPROC		glBlendFuncSeparateEXT		= glBlendFuncSeparateEXT_FACKED;

PFNGLACTIVETEXTUREPROC 				glActiveTexture				= glActiveTexture_FACKED;

PFNGLGENFRAMEBUFFERSEXTPROC			glGenFramebuffers			= glGenFramebuffers_FACKED;
PFNGLGENRENDERBUFFERSEXTPROC		glGenRenderbuffers			= glGenRenderbuffers_FACKED;
PFNGLBINDFRAMEBUFFEREXTPROC			glBindFramebuffer			= glBindFramebuffer_FACKED;
PFNGLBLITFRAMEBUFFEREXTPROC			glBlitFramebuffer			= glBlitFramebuffer_FACKED;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC	glFramebufferTexture2D		= glFramebufferTexture2D_FACKED;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatus	= glCheckFramebufferStatus_FACKED;
PFNGLBINDRENDERBUFFEREXTPROC		glBindRenderbuffer			= glBindRenderbuffer_FACKED;
PFNGLRENDERBUFFERSTORAGEEXTPROC		glRenderbufferStorage		= glRenderbufferStorage_FACKED;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer	= glFramebufferRenderbuffer_FACKED;
*/
// ----------------------------------------------------------------------------
// defined in OpenGL common source file.
FEBool bIsExtensionSupported(const char *extension);
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// UserData *userData = esContext->userData;
static const char* vShaderStr[NUM_SHADERS] =
{
        // ---------------------------------------
        // VX UV VC
        // ---------------------------------------
		// in arrays
		"attribute highp vec4		VX;"
		"attribute mediump vec4		UV;"
		"attribute mediump vec4		VC;"
		// in vars
		"uniform mediump float      Depth;"
		"uniform mediump mat4		PMVMatrix;"
		// out
		"varying mediump vec2		TexCoord;"
		"varying mediump vec4		VertexColor;"
		"void main(void)"
		"{"
		"	gl_Position = vec4((PMVMatrix * VX).xy,Depth,1.0);"
		"	TexCoord = UV.st;"
		"   VertexColor = VC;"
		"}",

        // ---------------------------------------
        // VX UV                    (VC constant)
        // ---------------------------------------
        // in arrays
		"attribute highp vec4		VX;"
		"attribute mediump vec4		UV;"
		// in vars
		"uniform mediump vec4		VC;"
		"uniform mediump float      Depth;"
		"uniform mediump mat4		PMVMatrix;"
		// out
		"varying mediump vec2		TexCoord;"
		"varying mediump vec4		VertexColor;"
		"void main(void)"
		"{"
		"	gl_Position = vec4((PMVMatrix * VX).xy,Depth,1.0);"
		"	TexCoord = UV.st;"
		"   VertexColor = VC;"
		"}",

        // ---------------------------------------
        // VX VC
        // ---------------------------------------
        // in arrays
		"attribute highp vec4		VX;"
		"attribute mediump vec4		VC;"
		// in vars
		"uniform mediump float      Depth;"
		"uniform mediump mat4		PMVMatrix;"
        // out
		"varying mediump vec4		VertexColor;"

		"void main(void)"
		"{"
		"	gl_Position = vec4((PMVMatrix * VX).xy,Depth,1.0);"
		"   VertexColor = VC;"
		"}",

        // ---------------------------------------
        // VX                       (VC constant)
        // ---------------------------------------
        // in arrays
		"attribute highp vec4		VX;"

		// in vars
		"uniform mediump vec4		VC;"
		"uniform mediump float      Depth;"
		"uniform mediump mat4		PMVMatrix;"
        // out
		"varying mediump vec4		VertexColor;"

		"void main(void)"
		"{"
		"	gl_Position = vec4((PMVMatrix * VX).xy,Depth,1.0);"
		"   VertexColor = VC;"
		"}",
};

static const char* fShaderStr[NUM_SHADERS] =
{
        // ---------------------------------------
        // VX UV VC
        // ---------------------------------------
		"uniform sampler2D		texture;"
		"varying mediump vec2	TexCoord;"
		"varying mediump vec4	VertexColor;"
		"void main (void)"
		"{"
		"	gl_FragColor = texture2D(texture,TexCoord) * VertexColor;"
		// "	gl_FragColor = vec4(0.0,1.0,1.0,1.0);"
		"}",

        // ---------------------------------------
        // UV UV VC
        // ---------------------------------------
		"uniform sampler2D		texture;"
		"varying mediump vec2	TexCoord;"
		"varying mediump vec4	VertexColor;"
		"void main (void)"
		"{"
		"	gl_FragColor = texture2D(texture,TexCoord) * VertexColor;"
		// "	gl_FragColor = vec4(1.0,0.0,1.0,1.0);"
		"}",

        // ---------------------------------------
        // VX VC
        // ---------------------------------------
		"varying mediump vec4	VertexColor;"
		"void main (void)"
		"{"
		"	gl_FragColor = VertexColor;"
		// "	gl_FragColor = vec4(1.0,0.0,0.0,1.0);"
		"}",

        // ---------------------------------------
        // VX VC
        // ---------------------------------------
		"varying mediump vec4	VertexColor;"
		"void main (void)"
		"{"
		"	gl_FragColor = VertexColor;"
		// "	gl_FragColor = vec4(0.0,1.0,0.0,1.0);"
		"}",
};
// ----------------------------------------------------------------------------
GLuint loadShader(GLenum shaderType, const char* pSource)
{
    GLuint shader = glCreateShader(shaderType);
    CHECKERROR();

    if (shader)
    {
        glShaderSource(shader, 1, &pSource, NULL);
        CHECKERROR();

        glCompileShader(shader);
        CHECKERROR();

        GLint compiled = 0;

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        CHECKERROR();

        if (!compiled)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            CHECKERROR();

            if (infoLen)
            {
                char* buf = (char*) CFECore::Mem::pAlloc(infoLen);
                if (buf)
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    CHECKERROR();

                    CFECore::Log::Print("loadShader Error: %s",buf);

                    CFECore::Mem::Free((FEPointer)buf);
                }

                glDeleteShader(shader);
                CHECKERROR();
                shader = 0;
            }
        }
    }

    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource)
{
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader)
    {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader)
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    CHECKERROR();
    if (program)
    {
        glAttachShader(program, vertexShader);
        CHECKERROR();

        glAttachShader(program, pixelShader);
        CHECKERROR();

        glLinkProgram(program);
        CHECKERROR();

        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        CHECKERROR();

        if (linkStatus != GL_TRUE)
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            CHECKERROR();

            if (bufLength)
            {
                char* buf = (char*) CFECore::Mem::pAlloc(bufLength);
                if (buf)
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    CHECKERROR();

                    CFECore::Log::Print("createProgram Error: %s",buf);
                    CFECore::Mem::Free((FEPointer)buf);
                }
            }

            glDeleteProgram(program);
            CHECKERROR();
            program = 0;
        }
    }
    return program;
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hInit(FEHandler _hParam)
{
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    CFECore::Log::Print("BEGIN CFECore::Graphics::hInit\n");
    CRendererHandler* poRnd = new CRendererHandler(CFECore::Graphics::m_uiInitialReservedPrims);

    poRnd->m_uiScrWidth  = GetScreenWidth();
    poRnd->m_uiScrHeight = GetScreenHeight();

	CFECore::Log::Print("%d - CFECore::Graphics::hInit WidthxHeight=%dx%d\n",gs_iGLChecks,poRnd->m_uiScrWidth,poRnd->m_uiScrHeight);

	// Create the program objects
	for (int i=0;i<NUM_SHADERS;i++)
    {
        GLuint progID = createProgram( (const char*)vShaderStr[i], (const char*)fShaderStr[i] );
        CFECore::Log::Print("%d - ProgramCreated\n",gs_iGLChecks);
        CHECKERROR();

        poRnd->m_oPrograms[i].m_uiID = progID;

        // Get the attribute locations
        poRnd->m_oPrograms[i].m_iVXLoc = glGetAttribLocation ( progID, "VX" );
        CHECKERROR();

        // special cases
        switch (i)
        {
            case 0:
            {
                poRnd->m_oPrograms[i].m_iUVLoc = glGetAttribLocation ( progID, "UV" );
                CHECKERROR();
                poRnd->m_oPrograms[i].m_iVCLoc = glGetAttribLocation ( progID, "VC" );
                CHECKERROR();

                // Sets the sampler2D variable to the first texture unit
                GLint iLoc = glGetUniformLocation(progID, "texture");
                CHECKERROR();
                CFECore::Log::Print("Texture Loc %d\n",iLoc);

                if (iLoc!=-1) glUniform1i(iLoc, 0);
                CHECKERROR();
            }
            break;

            case 1:
            {
                poRnd->m_oPrograms[i].m_iUVLoc = glGetAttribLocation ( progID, "UV" );
                CHECKERROR();
                poRnd->m_oPrograms[i].m_iVCLoc = glGetUniformLocation( progID, "VC" );
                CHECKERROR();

                // Sets the sampler2D variable to the first texture unit
                GLint iLoc = glGetUniformLocation(progID, "texture");
                CHECKERROR();
                CFECore::Log::Print("Texture Loc %d\n",iLoc);

                if (iLoc!=-1) glUniform1i(iLoc, 0);
                CHECKERROR();
            }
            break;

            case 2:
            {
                poRnd->m_oPrograms[i].m_iUVLoc = -1;
                poRnd->m_oPrograms[i].m_iVCLoc = glGetAttribLocation ( progID, "VC" );
                CHECKERROR();
            }
            break;

            case 3:
            {
                poRnd->m_oPrograms[i].m_iUVLoc = -1;
                poRnd->m_oPrograms[i].m_iVCLoc = glGetUniformLocation( progID, "VC" );
                CHECKERROR();
            }
            break;
        }

        // Get the uniform locations
        poRnd->m_oPrograms[i].m_iDepthLoc    = glGetUniformLocation( progID, "Depth" );
        CHECKERROR();

        poRnd->m_oPrograms[i].m_iPVMMatrixLoc= glGetUniformLocation( progID, "PMVMatrix" );
        CHECKERROR();

        CFECore::Log::Print("%d CFECore::Graphics::hInit prog(%d), %d %d %d %d %d %d\n",
                                gs_iGLChecks,
                                i,
                                poRnd->m_oPrograms[i].m_uiID,
                                poRnd->m_oPrograms[i].m_iVXLoc,
                                poRnd->m_oPrograms[i].m_iUVLoc,
                                poRnd->m_oPrograms[i].m_iVCLoc,
                                poRnd->m_oPrograms[i].m_iDepthLoc,
                                poRnd->m_oPrograms[i].m_iPVMMatrixLoc);
    }

    // Initialize Matrix Transforms
	esMatrixLoadIdentity(&poRnd->m_oModelMatrix);
	esMatrixLoadIdentity(&poRnd->m_oProjMatrix);
	esMatrixLoadIdentity(&poRnd->m_oPMVMatrix);

	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	CHECKERROR();

    // Setup screen virtual dimensions
    glEnable(GL_TEXTURE_2D);
    CHECKERROR();

    glEnable(GL_BLEND);
    CHECKERROR();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    CHECKERROR();

    // glEnable(GL_ALPHA_TEST);
    // CHECKERROR();

    // glAlphaFunc(GL_GREATER,0.0f);
    // CHECKERROR();

    // glClearDepth(1.0f);
    // CHECKERROR();

    glClearStencil(0);
    CHECKERROR();

    glEnable(GL_DEPTH_TEST);
    CHECKERROR();

    glDepthFunc(GL_LEQUAL);
    CHECKERROR();

    glDepthMask(GL_TRUE);
    CHECKERROR();

    // glEnable(GL_NORMALIZE);
    // CHECKERROR();

    // glShadeModel(GL_SMOOTH);
    // CHECKERROR();

	glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
	CHECKERROR();

    glViewport(0,0, poRnd->m_uiScrWidth,poRnd->m_uiScrHeight);
    CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    CHECKERROR();

    glCullFace(GL_BACK);
    CHECKERROR();

    // glDisable(GL_LIGHTING);
    // CHECKERROR();

/*
    if (gshFirstHGLRC != poRnd->m_hGLRC)
		wglShareLists(gshFirstHGLRC,poRnd->m_hGLRC);
*/

    // retrieve whether NON Power of 2 textures are supported or not
    FECglobals.m_bNPO2Support		= bIsExtensionSupported("GL_ARB_texture_non_power_of_two");
	FECglobals.m_poCurrentRenderer	= poRnd;

	// GLint major,minor;
	// glGetIntegerv(GL_MAJOR_VERSION, &major);
	// glGetIntegerv(GL_MINOR_VERSION, &minor);

    /*
	#define RETRIEVE_EXTENSION(FUNCNAME,FUNCPROTOTYPE)\
	FUNCNAME = (FUNCPROTOTYPE)wglGetProcAddress(#FUNCNAME);\
	if (FUNCNAME == NULL) FUNCNAME = FUNCNAME##_FACKED;

	#define RETRIEVE_EXTENSION2(FUNCNAME,FUNCNAME2,FUNCPROTOTYPE)\
	FUNCNAME = (FUNCPROTOTYPE)wglGetProcAddress(#FUNCNAME2);\
	if (FUNCNAME == NULL) FUNCNAME = FUNCNAME##_FACKED;

	RETRIEVE_EXTENSION(glBlendFuncSeparateEXT,		PFNGLBLENDFUNCSEPARATEEXTPROC);
	RETRIEVE_EXTENSION(glBlendEquationEXT,			PFNGLBLENDEQUATIONPROC);
 	RETRIEVE_EXTENSION(glBlendEquationSeparateEXT,	PFNGLBLENDEQUATIONSEPARATEEXTPROC);

  	RETRIEVE_EXTENSION(glActiveTexture,			PFNGLACTIVETEXTUREPROC)

	RETRIEVE_EXTENSION(glGenFramebuffers,		PFNGLGENFRAMEBUFFERSEXTPROC);
	RETRIEVE_EXTENSION(glBindFramebuffer,		PFNGLBINDFRAMEBUFFEREXTPROC);

	RETRIEVE_EXTENSION(glGenRenderbuffers,		PFNGLGENRENDERBUFFERSEXTPROC);
	RETRIEVE_EXTENSION(glBindRenderbuffer,		PFNGLBINDRENDERBUFFEREXTPROC);
	RETRIEVE_EXTENSION(glRenderbufferStorage,	PFNGLRENDERBUFFERSTORAGEEXTPROC);

	RETRIEVE_EXTENSION(glFramebufferTexture2D,	PFNGLFRAMEBUFFERTEXTURE2DEXTPROC);
	RETRIEVE_EXTENSION(glFramebufferRenderbuffer,PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC);

	// RETRIEVE_EXTENSION(glBlitFramebuffer,		PFNGLBLITFRAMEBUFFEREXTPROC);
	RETRIEVE_EXTENSION(glCheckFramebufferStatus,	PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC);
*/
// #define GL_READ_FRAMEBUFFER_EXT		0x8CA8
// #define GL_DRAW_FRAMEBUFFER_EXT		0x8CA9

    // Generate the Render2Texture texture
    /*
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

	// bind system frame buffer as the current target
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	CHECKERROR();
    */

    RecalcScreenOrientation();

	CFECore::Log::Print("END CFECore::Graphics::hInit\n");
    return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::Finish(FEHandler _hRenderer)
{
    if (_hRenderer == NULL) return;

    Check(_hRenderer != NULL,"NULL Renderer Handler");
    // DestroyRenderingContext( ((CRendererHandler*)_hRenderer )->m_hGLRC);
    // ReleaseDC(((CRendererHandler*)_hRenderer )->m_hWnd, ((CRendererHandler*)_hRenderer)->m_hDC );
	delete ((CRendererHandler*)_hRenderer );
}
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hGetCurrentRenderer()
{
    return( (FEHandler)(FECglobals.m_poCurrentRenderer) );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetCurrentRenderer(FEHandler _hRenderer)
{
    // In case the renderer is different ...
    FECglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;
    return;

    /*
    // Android: always only 1 rendering context
    if (FECglobals.m_poCurrentRenderer != _hRenderer)
    {
        FECglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;

        if (FECglobals.m_poCurrentRenderer != NULL)
			wglMakeCurrent(FECglobals.m_poCurrentRenderer->m_hDC, FECglobals.m_poCurrentRenderer->m_hGLRC);
		else
			return;
    }
    */
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::BeginScene()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;
    CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

	/// Clear and initialize the render list
	poRnd->m_oRL.clear();
    poRnd->m_uiPrims = 0;
	poRnd->m_bFBOCleared = false;
	poRnd->m_bFBOEnabled = false;
	poRnd->m_uiLightPrims = 0;

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
		poRnd->m_oVX	= poRnd->m_oVXBufs[0];
		poRnd->m_oUV	= poRnd->m_oUVBufs[0];
		poRnd->m_oVC	= poRnd->m_oVCBufs[0];
		poRnd->m_usIdx  = poRnd->m_usIdxBufs[0];
	}

    /*
    static int iDo = 0;
    if (iDo == 0)
    {
        CFECore::Log::Print("rRRatio %.3f rVRatio %.3f w:%d h:%d wm:%d hm: %d",rRRatio, rVRatio, w, h, wm, hm);
        iDo = 1;
    }
    */

    // Setup screen virtual dimensions transform matrix.
    glViewport(FECglobals.m_uiViewportWMargin,
               FECglobals.m_uiViewportHMargin,
               FECglobals.m_uiViewportWidth,
               FECglobals.m_uiViewportHeight);

    // ------------------------------------------------------------
    // END: Compute proper viewport
    // ------------------------------------------------------------
    glClearColor(_0r,_0r,_0r,_0r);
    CHECKERROR();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECKERROR();

	// setup projection matrix
	esMatrixLoadIdentity(&poRnd->m_oProjMatrix);

    if (FECglobals.m_bViewportRotated)
    {
        esRotate(&poRnd->m_oProjMatrix,90,0.0f,0.0f,1.0f);
    }

	esTranslate(&poRnd->m_oProjMatrix,-1.0f,+ 1.0f,0.0f);
	esScale(&poRnd->m_oProjMatrix, 2.0f/(float)poRnd->m_uiVScrWidth,-2.0f/(float)poRnd->m_uiVScrHeight,1.0f);

    // iniialize modelview matrix
	esMatrixLoadIdentity(&poRnd->m_oModelMatrix);
    CFEMatrix m;
    m.LoadIdentity();

    // update rendering context
    INT_SetTransform(m);

    /*
    CFECore::Log::Print("Virtual Dims %d %d\n",poRnd->m_uiVScrWidth,
                                               poRnd->m_uiVScrHeight);

    PrintMatrices();
    */

	glBlendFunc(GL_ONE,GL_ZERO);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::EndScene()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

	if (poRnd->m_uiPrims > 0)
	{
		// Sort primitives:
		std::sort(&poRnd->m_oRPList[0],&poRnd->m_oRPList[0] + poRnd->m_uiPrims);
		BuildRenderList();
		FlushRenderList();

		if (poRnd->m_uiLightPrims > 0)
		{
			INT_SetBlendMode(BM_MULT);
			CHECKERROR();

			// INT_SetMaterial(NULL);
			// CHECKERROR();

			glEnable(GL_TEXTURE_2D);
			CHECKERROR();

			glBindTexture(GL_TEXTURE_2D,poRnd->m_uiR2TTex);
			CHECKERROR();

            // setup projection matrix
            esMatrixLoadIdentity(&poRnd->m_oModelMatrix);
            esTranslate(&poRnd->m_oModelMatrix,_0r,_0r,_1r);

            esMatrixLoadIdentity(&poRnd->m_oProjMatrix);

			static unsigned short susIdx[6] = {0,1,2,2,3,0};
			static CFEVect2 soUV[4] =
			{
				CFEVect2(_0r,_0r),
				CFEVect2(_1r,_0r),
				CFEVect2(_1r,_1r),
				CFEVect2(_0r,_1r),
			};
			static CFEColor soVC[4] =
			{
				CFEColor(_1r,_1r,_1r,_1r),
				CFEColor(_1r,_1r,_1r,_1r),
				CFEColor(_1r,_1r,_1r,_1r),
				CFEColor(_05r,_1r,_1r,_1r),
			};
			static CFEVect2 soVX[4] =
			{
				CFEVect2(-_1r,-_1r),
				CFEVect2( _1r,-_1r),
				CFEVect2( _1r, _1r),
				CFEVect2(-_1r, _1r),
			};

			glDisable(GL_DEPTH_TEST);
			CHECKERROR();

 			INT_RenderMesh(susIdx,soVX,soUV,soVC,6);
			CHECKERROR();

			glEnable(GL_DEPTH_TEST);
			CHECKERROR();
		}
	}

	glFinish();
//  ANDROID: ::SwapBuffers( poRnd->m_hDC );
}
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
void CFECore::Graphics::SetScreenVWidth(uint _uiScrVWidth)
{
    FECglobals.m_poCurrentRenderer->m_uiVScrWidth = _uiScrVWidth;
    RecalcScreenOrientation();
    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetScreenVHeight(uint _uiScrVHeight)
{
	FECglobals.m_poCurrentRenderer->m_uiVScrHeight = _uiScrVHeight;
	RecalcScreenOrientation();
    CHECKERROR();
}
// ----------------------------------------------------------------------------
typedef struct
{
    GLint            m_iID;
    CFECImageInfo*   m_poII;

}TMaterialHandler;
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
    CFECore::Log::Print("hLoadMaterial");

    // TMaterialHandler
    CFECImageInfo* poII = ImageLib::poLoadImage((char*)_sFilename.szString());
    if (poII == NULL) return(NULL);

    if (! FECglobals.m_bNPO2Support)
        ImageLib::MakePowerOf2(poII);

    TMaterialHandler* poTH = new TMaterialHandler;
    poTH->m_poII = poII;
    poTH->m_iID  = -1;

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
// ----------------------------------------------------------------------------
int iCreateTexture(void* _pData,unsigned int _uiWidth,unsigned int _uiHeight,EFEPixelFormat _eTexFormat)
{
    GLuint uiTexID;

    // Generate a new material handler
    glGenTextures(1,&uiTexID);
    CHECKERROR();

    // Establecer el contexto de la nueva textura
    glBindTexture(GL_TEXTURE_2D,uiTexID);
    CHECKERROR();

    glTexImage2D (GL_TEXTURE_2D,
                  0,
                  eGetTextureFormat(_eTexFormat),
                  _uiWidth,
                  _uiHeight,
                  0,
                  iGetTextureComponents(_eTexFormat),
                  GL_UNSIGNED_BYTE,
                  _pData);
    CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);CHECKERROR();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);CHECKERROR();
    CHECKERROR();

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

	CRenderPrim* poRP	= &poRnd->m_oRPList[poRnd->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= poRnd->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash		= poRnd->uiGetStateHash();
	poRP->m_oRS			= poRnd->m_oRenderState;

	poRP->m_ePT		 	= RPT_MESH;
	poRP->m_poVX		= _poVXs;
	poRP->m_poUV		= _poUVs;
	poRP->m_poVC		= NULL;
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
    poDstUV[0].x = _oIUV.x;
    poDstUV[0].y = _oIUV.y;

    poDstUV[1].x = _oFUV.x;
    poDstUV[1].y = _oIUV.y;

    poDstUV[2].x = _oFUV.x;
    poDstUV[2].y = _oFUV.y;

    poDstUV[3].x = _oIUV.x;
    poDstUV[3].y = _oFUV.y;

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

		if (_poUVs != NULL)
		{
			CFEVect2* poDstUV = &poRnd->m_oUV[uiIdx];
			poDstVX[0] = _poUVs[0];
			poDstVX[1] = _poUVs[1];
			poDstVX[2] = _poUVs[2];
			poDstVX[3] = _poUVs[3];
		}

		if (_poVCs != NULL)
		{
			CFEColor* poDstVC = &poRnd->m_oVC[uiIdx];
			poDstVC[0] = _poVCs[0];
			poDstVC[1] = _poVCs[1];
			poDstVC[2] = _poVCs[2];
			poDstVC[3] = _poVCs[3];
		}

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
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderSprite(	const CFEVect2& _oPos,const CFEVect2& _oPivot,
					const CFEVect2& _oScale,FEReal _rAngle,
					const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                    const CFEColor& _oColor,FEReal _rAlpha)
{
	CRendererHandler* poRnd = FECglobals.m_poCurrentRenderer;

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

    // Set UVs
    poDstUV[0]   = _oIUV;
    poDstUV[2]   = _oFUV;
    poDstUV[1].x = poDstUV[2].x;
    poDstUV[1].y = poDstUV[0].y;
    poDstUV[3].x = poDstUV[0].x;
    poDstUV[3].y = poDstUV[2].y;

    // Set color
    poDstVC[0] = _oColor;
    poDstVC[0].a *= _rAlpha;
    poDstVC[1] = poDstVC[0];
    poDstVC[2] = poDstVC[1];
    poDstVC[3] = poDstVC[2];

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
		INT_SetMaterial(_hMaterial);

		GLint iRes;
        glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,&iRes);CHECKERROR();
        *(uint*)_pParam = uiGetWrapModeFE_ID(iRes);

        return (true);
    }
else if (_sProperty == "DiffuseMap.TWrapMode")
    {
		INT_SetMaterial(_hMaterial);

		GLint iRes;
		glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,&iRes);CHECKERROR();
        *(uint*)_pParam = uiGetWrapModeFE_ID(iRes);

        return (true);
    }
else if (_sProperty == "DiffuseMap.Filter")
		{
			INT_SetMaterial(_hMaterial);

			GLint iRes;
			glGetTexParameteriv(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,&iRes);CHECKERROR();
			*(uint*)_pParam = uiGetFilterFE_ID(iRes);

			return (true);
		}

    return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
	const uint CLAMP_MODES[] = {GL_CLAMP_TO_EDGE,GL_REPEAT,GL_MIRRORED_REPEAT,GL_CLAMP_TO_EDGE};

	TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    if (_sProperty == "DiffuseMap.SWrapMode")
    {
		INT_SetMaterial(_hMaterial);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,CLAMP_MODES[(uint)_pParam & 0x03]);CHECKERROR();
        return (true);
    }
else if (_sProperty == "DiffuseMap.TWrapMode")
    {
		INT_SetMaterial(_hMaterial);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,CLAMP_MODES[(uint)_pParam & 0x03]);CHECKERROR();
        return (true);
    }
else if (_sProperty == "DiffuseMap.Filter")
    {
		const uint FILTER_MODES[] = {GL_NEAREST,GL_LINEAR};
		INT_SetMaterial(_hMaterial);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FILTER_MODES[(uint)_pParam & 0x01]);CHECKERROR();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FILTER_MODES[(uint)_pParam & 0x01]);CHECKERROR();
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

	FEHandler		hMat = (FEHandler)0xffffffff;
	EFEBlendMode	eBM  = BM_MAX;
	FEReal			rZ   = -_1r;

	for (uint i=0;i<poRnd->m_oRL.size();i++)
	{
		CRenderElem* poElem = &poRnd->m_oRL[i];

		if (poElem->m_poIniPrim->m_oRS.m_eBlendMode != eBM)
		{
			eBM = poElem->m_poIniPrim->m_oRS.m_eBlendMode;

			EnableRender2Texture( (eBM == BM_LIGHTMARKER) );

			INT_SetBlendMode(poElem->m_poIniPrim->m_oRS.m_eBlendMode);
			CHECKERROR();
		}

		if (poElem->m_poIniPrim->m_oRS.m_hMaterial != hMat)
		{
			hMat = poElem->m_poIniPrim->m_oRS.m_hMaterial;
			INT_SetMaterial(poElem->m_poIniPrim->m_oRS.m_hMaterial);
			CHECKERROR();
		}

		if (poElem->m_poIniPrim->m_oRS.m_rDepth != rZ)
		{
			rZ = poElem->m_poIniPrim->m_oRS.m_rDepth;
			INT_SetDepth(poElem->m_poIniPrim->m_oRS.m_rDepth);
		}

        // setup projection matrix
        INT_SetTransform(poElem->m_poIniPrim->m_oRS.m_oTransf);

			switch (poElem->m_poIniPrim->m_ePT)
			{
				case RPT_QUAD:
				INT_RenderMesh( poElem->m_poIniPrim->m_pusIdx,
								poRnd->m_oVX,
								poRnd->m_oUV,
								poRnd->m_oVC,
								poElem->m_uiNumPrims*6);
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
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetTransform(const CFEMatrix& _rMat)
{
	ESMatrix* m = &FECglobals.m_poCurrentRenderer->m_oModelMatrix;

	m->m[0][0] = _rMat.m[0][0];
	m->m[0][1] = _rMat.m[0][1];
	m->m[0][2] = 0.0f;
	m->m[0][3] = _rMat.m[0][2];

	m->m[1][0] = _rMat.m[1][0];
	m->m[1][1] = _rMat.m[1][1];
	m->m[1][2] = 0.0f;
	m->m[1][3] = _rMat.m[1][2];

	m->m[2][0] = 0.0f;
	m->m[2][1] = 0.0f;
	m->m[2][2] = 1.0f;
	m->m[2][3] = 0.0f;

	m->m[3][0] = _rMat.m[2][0];
	m->m[3][1] = _rMat.m[2][1];
	m->m[3][2] = 0.0f;
	m->m[3][3] = _rMat.m[2][2];

	// Compute the final MVP by multiplying the
	// modelview and perspective matrices together
	esMatrixMultiply(&FECglobals.m_poCurrentRenderer->m_oPMVMatrix,
					 &FECglobals.m_poCurrentRenderer->m_oModelMatrix,
					 &FECglobals.m_poCurrentRenderer->m_oProjMatrix);

	/*
	if (eErr != 0)
        CFECore::Log::Print("Matrix errror Loc %d",eErr);
    */

    // PrintMatrices();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetMaterial(FEHandler _hMaterial)
{
    if (_hMaterial == NULL)
    {
        glDisable(GL_TEXTURE_2D);
        FECglobals.m_poCurrentRenderer->m_hCurTex = NULL;
        return;
    }

    // Material has been uploaded?
    TMaterialHandler* poTexHnd = (TMaterialHandler*)_hMaterial;
    if (poTexHnd->m_iID == -1)
    {
        // Material needs to be uploaded!
        poTexHnd->m_iID = iCreateTexture(poTexHnd->m_poII->m_pData,poTexHnd->m_poII->m_uiFixedTX,poTexHnd->m_poII->m_uiFixedTY,poTexHnd->m_poII->m_eFormat);
    }

    //
    glEnable(GL_TEXTURE_2D);

    // Bind the material
    glBindTexture(GL_TEXTURE_2D,poTexHnd->m_iID);
    CHECKERROR();

	FECglobals.m_poCurrentRenderer->m_hCurTex = _hMaterial;
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetDepth(FEReal _rDepth)
{
    FECglobals.m_poCurrentRenderer->m_rCurDepth = _rDepth;
	CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_SetBlendMode(EFEBlendMode _eBlendMode)
{
    switch (_eBlendMode)
    {
        case BM_NONE:
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ZERO);
        break;

        case BM_COPY:
        glDisable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ZERO);
        break;

        case BM_ALPHA:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        break;

        case BM_ADD:
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE,GL_ONE);
        break;

        case BM_ALPHAADD:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        break;

        case BM_MULT:
        glEnable(GL_BLEND);
        glBlendFunc(GL_DST_COLOR,GL_ZERO);
        break;
    }

    /*
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
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gsuiLastTexture);

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

			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);
			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,GL_ZERO,GL_ZERO);
		}
		break;

		case BM_FOGADD:
		{
			glActiveTexture(GL_TEXTURE0);
			glEnable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);

			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);			//ALPHA*ALPHA
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);

			glActiveTexture(GL_TEXTURE1);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D,gsuiLastTexture);

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
			glAlphaFunc(GL_GREATER,0.0f);
			glEnable(GL_BLEND);

			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
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

			glAlphaFunc(GL_ALWAYS,0.0f);
			CHECKERROR();

			glEnable(GL_BLEND);
			CHECKERROR();

			glBlendFuncSeparateEXT(GL_SRC_ALPHA,GL_ONE,GL_SRC_ALPHA,GL_ONE);
			CHECKERROR();

			CHECKERROR();
		}
        break;
    }
    */
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiPoints)
{
    // to simplify things ...
	CRendererHandler* poCurRnd = FECglobals.m_poCurrentRenderer;
	if (poCurRnd->m_hCurTex==NULL) _poUVs = NULL;

	// -------------------
	uint uiFlags = 0;
	#define UV_FLAG	0x01
	if (_poUVs!=NULL) uiFlags |= UV_FLAG;

	// idx 3 = VX, No UV, no VC
    // idx 2 = VX, no UV, VC
    // idx 1 = VX, UV, no VC
    // idx 0 = VX, UV, VC

	uint uiProgIdxs[NUM_SHADERS] = {3,1,2,0};
	uint uiPIdx = uiProgIdxs[uiFlags];

	glUseProgram(poCurRnd->m_oPrograms[uiPIdx].m_uiID);

	// -------------------
    if (_poUVs != NULL)
	{
		glVertexAttribPointer( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc, 2, GL_FLOAT, GL_FALSE,0, _poUVs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc);
		CHECKERROR();
	}

    if (_poVXs)
	{
		glVertexAttribPointer( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc, 2, GL_FLOAT, GL_FALSE,0, _poVXs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc );
		CHECKERROR();
	}

    GLfloat color[4] = {_oColor.r,_oColor.g,_oColor.b,_oColor.a};
    glUniform4fv(poCurRnd->m_oPrograms[uiPIdx].m_iVCLoc,1,color);
    glUniform1f(poCurRnd->m_oPrograms[uiPIdx].m_iDepthLoc,poCurRnd->m_rCurDepth);
	glUniformMatrix4fv( poCurRnd->m_oPrograms[uiPIdx].m_iPVMMatrixLoc, 1, GL_FALSE, (GLfloat*) &poCurRnd->m_oPMVMatrix.m[0][0] );
	CHECKERROR();

	if (_pusIdx)
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::INT_RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints)
{
    if (_poVCs == NULL)
    {
        INT_RenderMesh(_pusIdx,_poVXs,_poUVs,CFEColor::WHITE(),_uiPoints);
        return;
    }

    // to simplify things ...
	CRendererHandler* poCurRnd = FECglobals.m_poCurrentRenderer;
	if (poCurRnd->m_hCurTex==NULL) _poUVs = NULL;

	// -------------------
	uint uiFlags = 0x02;
	#define UV_FLAG	0x01

	if (_poUVs!=NULL) uiFlags |= UV_FLAG;

    // idx 3 = VX, No UV, no VC
    // idx 2 = VX, no UV, VC
    // idx 1 = VX, UV, no VC
    // idx 0 = VX, UV, VC

	uint uiProgIdxs[NUM_SHADERS] = {3,1,2,0};
	uint uiPIdx = uiProgIdxs[uiFlags];

	glUseProgram(poCurRnd->m_oPrograms[uiPIdx].m_uiID);

	// -------------------
    if (_poUVs != NULL)
	{
		glVertexAttribPointer( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc, 2, GL_FLOAT, GL_FALSE,0, _poUVs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iUVLoc);
		CHECKERROR();
	}

	glVertexAttribPointer( poCurRnd->m_oPrograms[uiPIdx].m_iVCLoc, 4, GL_FLOAT, GL_FALSE,0, _poVCs );
	CHECKERROR();
	glEnableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iVCLoc );
	CHECKERROR();

    if (_poVXs)
	{
		glVertexAttribPointer( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc, 2, GL_FLOAT, GL_FALSE,0, _poVXs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_oPrograms[uiPIdx].m_iVXLoc );
		CHECKERROR();
	}

    glUniform1f(poCurRnd->m_oPrograms[uiPIdx].m_iDepthLoc,poCurRnd->m_rCurDepth);
	glUniformMatrix4fv( poCurRnd->m_oPrograms[uiPIdx].m_iPVMMatrixLoc, 1, GL_FALSE, (GLfloat*) &poCurRnd->m_oPMVMatrix.m[0][0] );
	CHECKERROR();

	if (_pusIdx)
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);
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

	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty == "ScreenWidth")
    {
		FECglobals.m_poCurrentRenderer->m_uiScrWidth = GETPROPVALUE(uint,_pParam);
		glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight);
        return (true);
    }
else if (_sProperty == "ScreenHeight")
    {
		FECglobals.m_poCurrentRenderer->m_uiScrHeight =  GETPROPVALUE(uint,_pParam);
		glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight);
        return (true);
    }
else if (_sProperty |= "RESERVE_PRIMITIVES")
    {
    	CFECore::Graphics::m_uiInitialReservedPrims = GETPROPVALUE(uint,_pParam);
        return(true);
    }

	return(false);
}
//-----------------------------------------------------------------------------------------
void CFECore::Graphics::EnableRender2Texture(bool _bEnable)
{
    /*
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
	    glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight);

		FECglobals.m_poCurrentRenderer->m_bFBOEnabled = false;

		GLenum status;
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		CHECKERROR();

	}
	*/
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
