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
#include "../../FEConfig.h"
#if ((GRAPHICS == USE_OPENGLES2) && (TARGETPLATFORM == USE_DEVELOPMENT))
// ----------------------------------------------------------------------------
// #include <windows.h>
#include <stdlib.h>
#include <math.h>
// #include <GL/gl.h>
#include <windows.h> // for POINT
#include <EGL/egl.h>

#include "../../CFECore.h"
#include "../../CFECoreGlobals.h"

#include "../../common/CFECImageLib.h"
#include "FEBasicTypes.h"
#include "../CFECGlobals.h"
#include "esUtil.h"
// ----------------------------------------------------------------------------
// Index to bind the attributes to vertex shaders
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:

        CRendererHandler() :	
			m_poCtx(NULL),
			m_uiVScrWidth(640),
			m_uiVScrHeight(480),
			m_uiScrWidth(0),
			m_uiScrHeight(0),
			m_uiDefProg(0)
		{
		};

        ESContext* m_poCtx;                 // GL Rendering Context
		
        POINT   m_oScrOfs;                  // Screen offset
        uint    m_uiScrWidth;               // Actual Screen Width
        uint    m_uiScrHeight;              // Actual Screen Height
        uint    m_uiVScrWidth;              // Virtual Screen Width
        uint    m_uiVScrHeight;             // Virtual Screen Height

		GLuint m_uiDefProg;
		GLint  m_iVXLoc;					// Position Attribute Location
		GLint  m_iUVLoc;					// TexCoord Attribute Location
		GLint  m_iVCLoc;					// VertexColor Attribute Location
		GLint  m_iMVMatrixLoc;				// Uniform locations
		
		ESMatrix m_oPMVMatrix;				// Projection * ModelViewMatrix
		ESMatrix m_oModelMatrix;			// ModelViewMatrix
		ESMatrix m_oProjMatrix;				// ProjectionMatrix
};
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static EGLint eErr = GL_NO_ERROR;
#define CHECKERROR() eErr = eglGetError();
#else
#define CHECKERROR()
#endif


// ----------------------------------------------------------------------------
ESContext* CreateRenderingContext(HWND wnd)
{
	ESContext esContext;

	EGLint attribList[] =
   {
       EGL_RED_SIZE,       EGL_DONT_CARE,
       EGL_GREEN_SIZE,     EGL_DONT_CARE,
       EGL_BLUE_SIZE,      EGL_DONT_CARE,
       EGL_ALPHA_SIZE,     EGL_DONT_CARE,
       EGL_DEPTH_SIZE,     EGL_DONT_CARE,
       EGL_STENCIL_SIZE,   EGL_DONT_CARE,
       EGL_SAMPLE_BUFFERS, EGL_DONT_CARE,
       EGL_NONE
   };

   esContext.hWnd = wnd;
   if ( !CreateEGLContext ( esContext.hWnd,
                            &esContext.eglDisplay,
                            &esContext.eglContext,
                            &esContext.eglSurface,
                            attribList) )
   {
      return NULL;
   }
   
	/*  
	EGLConfig oCfg = NULL;
	EGLDisplay oDsp = eglGetDisplay(dc);
	EGLSurface oSrf = eglCreateWindowSurface(oDsp, oCfg,wnd,NULL);
	*/

	ESContext* pesContext = new ESContext(esContext);
    return(pesContext);
}
// -----------------------------------------------------------------------------
void DestroyRenderingContext(ESContext* surf)
{
	/*
    // Deactivate Rendering Context
    wglMakeCurrent(0,0);
    CHECKERROR();

    if (RC)
    {
        wglDeleteContext(RC);
        CHECKERROR();
    }
    */
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// Dirty trick to share textures among different rendering contexts.
static EGLSurface gshFirstSurface = 0;

FEHandler CFECore::Graphics::hInit(FEHandler _hParam)
{
	if (_hParam == NULL) return(NULL);
    CRendererHandler* poRnd = new CRendererHandler;

    poRnd->m_poCtx = CreateRenderingContext((HWND)_hParam);    
    CHECKERROR();

    if (gshFirstSurface == 0)
		gshFirstSurface = poRnd->m_poCtx;

	poRnd->m_oScrOfs.x = poRnd->m_oScrOfs.y = 0;
    ClientToScreen( (HWND)_hParam, &poRnd->m_oScrOfs);

    RECT oRect;
    GetClientRect((HWND)_hParam,&oRect);

    poRnd->m_poCtx->width  = oRect.right - oRect.left;
    poRnd->m_poCtx->height = oRect.bottom - oRect.top;

	// UserData *userData = esContext->userData;
	GLbyte vShaderStr[] =  

		"attribute highp vec4		VX;"
		"attribute mediump vec4		UV;"
		"attribute mediump vec4		VC;"
		"uniform mediump mat4		PMVMatrix;"
		"varying mediump vec2		TexCoord;"
		"varying mediump vec4		VertexColor;"
		"void main(void)"
		"{"
		"	gl_Position = PMVMatrix * VX;"
		"	TexCoord = UV.st;"
		"   VertexColor = VC;"
		"}";


	GLbyte fShaderStr[] =  

		"uniform sampler2D		sampler2d;"
		"varying mediump vec2	TexCoord;"
		"varying mediump vec4	VertexColor;"
		"void main (void)"
		"{"
		"	gl_FragColor = texture2D(sampler2d,TexCoord) * VertexColor;"
		"}";
	

	// Create the program object
	poRnd->m_uiDefProg = esLoadProgram ( (const char*)vShaderStr, (const char*)fShaderStr );
	CHECKERROR();
	if ( poRnd->m_uiDefProg == 0 )
		return 0;

	// Bind vPosition to attribute 0   

	// Get the attribute locations
	poRnd->m_iVXLoc = glGetAttribLocation ( poRnd->m_uiDefProg, "VX" );
	CHECKERROR();
	poRnd->m_iUVLoc = glGetAttribLocation ( poRnd->m_uiDefProg, "UV" );
	CHECKERROR();
	poRnd->m_iVCLoc = glGetAttribLocation ( poRnd->m_uiDefProg, "VC" );
	CHECKERROR();

	// Get the uniform locations
	poRnd->m_iMVMatrixLoc = glGetUniformLocation( poRnd->m_uiDefProg, "PMVMatrix" );
	CHECKERROR();

	// Sets the sampler2D variable to the first texture unit
	glUniform1i(glGetUniformLocation(poRnd->m_uiDefProg, "sampler2d"), 0);
		
	glClearColor ( 0.0f, 0.0f, 0.0f, 0.0f );
	CHECKERROR();
	
	// glBindAttribLocation(m_uiProgramObject, VERTEX_ARRAY, "myVertex");
	// glBindAttribLocation ( poRnd->m_uiDefProg, VERTEX_ARRAY, "myVertex" );
	// glBindAttribLocation ( poRnd->m_uiDefProg, TEXCOORD_ARRAY, "myTexCoord" );

    // Initialize Matrix Transforms
	esMatrixLoadIdentity(&poRnd->m_oModelMatrix);
	esMatrixLoadIdentity(&poRnd->m_oProjMatrix);
	esMatrixLoadIdentity(&poRnd->m_oPMVMatrix);

    glEnable(GL_TEXTURE_2D);
    CHECKERROR();

    glEnable(GL_BLEND);
    CHECKERROR();

    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    CHECKERROR();
/*
    glEnable(GL_ALPHA_TEST);
    CHECKERROR();

    glAlphaFunc(GL_GREATER,0.0f);
    CHECKERROR();
*/
    glClearColor(0,0,0,1);
    CHECKERROR();

    // glClearDepth(1.0f);
    CHECKERROR();

   // glClearStencil(0);
    CHECKERROR();

    glEnable(GL_DEPTH_TEST);
    CHECKERROR();

    glDepthFunc(GL_LEQUAL);
    CHECKERROR();

    glDepthMask(GL_TRUE);
    CHECKERROR();

    //glEnable(GL_NORMALIZE);
    CHECKERROR();

    //glShadeModel(GL_SMOOTH);
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

    //glDisable(GL_LIGHTING);
    CHECKERROR();
    
    // retrieve whether NON Power of 2 textures are supported or not
    FECglobals.m_bNPO2Support = true;
	FECglobals.m_poCurrentRenderer = poRnd;

    return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::Finish(FEHandler _hRenderer)
{
    if (_hRenderer == NULL) return;

    Check(_hRenderer != NULL,"NULL Renderer Handler");
    DestroyRenderingContext( ((CRendererHandler*)_hRenderer )->m_poCtx );
    // ReleaseDC(((CRendererHandler*)_hRenderer )->m_hWnd, ((CRendererHandler*)_hRenderer)->m_hDC );
	delete ((CRendererHandler*)_hRenderer );
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::BeginScene(FEHandler _hRenderer)
{
    if (FECglobals.m_poCurrentRenderer != _hRenderer)
    {
        // In case the renderer is different ...
        FECglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;

        /*
        if (FECglobals.m_poCurrentRenderer != NULL) 
			wglMakeCurrent(FECglobals.m_poCurrentRenderer->m_hDC, FECglobals.m_poCurrentRenderer->m_hGLRC);
		else
			return; 
		*/
    }

    ESContext* poCtx = FECglobals.m_poCurrentRenderer->m_poCtx;

	// Set the viewport
	glViewport ( 0, 0, poCtx->width, poCtx->height );
	CHECKERROR();
	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CHECKERROR();
	// Use the program object
	glUseProgram ( FECglobals.m_poCurrentRenderer->m_uiDefProg );
	CHECKERROR();
	// setup projection matrix
	esMatrixLoadIdentity(&FECglobals.m_poCurrentRenderer->m_oProjMatrix);
	esTranslate(&FECglobals.m_poCurrentRenderer->m_oProjMatrix,-1.0f,+ 1.0f,0.0f);
	esScale(&FECglobals.m_poCurrentRenderer->m_oProjMatrix, 2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrWidth,-2.0f/(float)FECglobals.m_poCurrentRenderer->m_uiVScrHeight,1.0f);

	// for debugging purposes
	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::EndScene()
{
	if (FECglobals.m_poCurrentRenderer == NULL) return;
	ESContext* poCtx = FECglobals.m_poCurrentRenderer->m_poCtx;
	
	eglSwapBuffers ( poCtx->eglDisplay, poCtx->eglSurface );
	CHECKERROR();
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty == "ScreenWidth")
    {
		FECglobals.m_poCurrentRenderer->m_uiScrWidth = (uint)_pParam;
		glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight);
        return (true);
    }
else if (_sProperty == "ScreenHeight")
    {
		FECglobals.m_poCurrentRenderer->m_uiScrHeight = (uint)_pParam;
		glViewport(0,0,FECglobals.m_poCurrentRenderer->m_uiScrWidth,FECglobals.m_poCurrentRenderer->m_uiScrHeight);
        return (true);
    }

	return(false);
}
// ----------------------------------------------------------------------------
typedef struct
{
    GLint            m_iID;
    CFECImageInfo*   m_poII;

}TMaterialHandler;
//-----------------------------------------------------------------------------------------
// Function helper that handles localization of data.
// ----------------------------------------------------------------------------
CFECImageInfo* poLoadImage(const CFEString& _sFilename)
{
	CFECImageInfo* poII = NULL;
	poII = ImageLib::poLoadImage((char*)_sFilename.szString());
	return(poII);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetScreenVWidth(uint _uiScrVWidth)
{
    FECglobals.m_poCurrentRenderer->m_uiVScrWidth = _uiScrVWidth;
    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetScreenVHeight(uint _uiScrVHeight)
{
    FECglobals.m_poCurrentRenderer->m_uiVScrHeight = _uiScrVHeight;
    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetTransform(const CFEMatrix& _rMat)
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
	// modevleiw and perspective matrices together
	esMatrixMultiply(&FECglobals.m_poCurrentRenderer->m_oPMVMatrix, 
					 &FECglobals.m_poCurrentRenderer->m_oModelMatrix,
					 &FECglobals.m_poCurrentRenderer->m_oProjMatrix);
}
/*
// ----------------------------------------------------------------------------
uint CFECore::Graphics::uiGetScreenVWidth(FEHandler _hRenderer)
{
    return(FECglobals.m_uiScrVWidth);
}
// ----------------------------------------------------------------------------
uint CFECore::Graphics::uiGetScreenVHeight(FEHandler _hRenderer)
{
    return(FECglobals.m_uiScrVHeight);
}
*/
// ----------------------------------------------------------------------------
FEHandler CFECore::Graphics::hGetCurrentRenderer()
{
    return( (FEHandler)(FECglobals.m_poCurrentRenderer) );
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
    CFECImageInfo* poII = poLoadImage(_sFilename);
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
    CFECImageInfo* poII = poLoadImage(_sFilename);
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
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetBlendMode(EFEBlendMode _eBlendMode)
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
    
    #ifndef DS_PC
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);CHECKERROR();
    #else
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);CHECKERROR();
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);CHECKERROR();
    #endif

    CHECKERROR();

    return(uiTexID);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetMaterial(FEHandler _hMaterial)
{
    if (_hMaterial == NULL)
    {
        glDisable(GL_TEXTURE_2D);
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
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::SetDepth(FEReal _rDepth)
{
	if (_rDepth != FECglobals.m_rCurDepth)
	{
		FECglobals.m_rCurDepth = _rDepth;
		CRendererHandler* poCurRnd = FECglobals.m_poCurrentRenderer;
		esTranslate(&poCurRnd->m_oModelMatrix,0,0,FECglobals.m_rCurDepth);

		// modevleiw and perspective matrices together
		esMatrixMultiply(&poCurRnd->m_oPMVMatrix, 
			&poCurRnd->m_oProjMatrix,
			&poCurRnd->m_oModelMatrix);
	}
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderQuads(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads)
{
#ifdef REAL_CLASS
	return;
#endif
	CRendererHandler* poCurRnd = FECglobals.m_poCurrentRenderer;

	if (_poUV)
	{
		glVertexAttribPointer( poCurRnd->m_iUVLoc, 2, GL_FLOAT, GL_FALSE,0, _poUV );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iUVLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iUVLoc );
		CHECKERROR();
	}

	if (_poVC)
	{
		glVertexAttribPointer( poCurRnd->m_iVCLoc, 4, GL_FLOAT, GL_FALSE,0, _poVC );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iVCLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iVCLoc );
		CHECKERROR();
	}

	if (_poVX)
	{
		glVertexAttribPointer( poCurRnd->m_iVXLoc, 2, GL_FLOAT, GL_FALSE,0, _poVX );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iVXLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iVXLoc );
		CHECKERROR();
	}

	glUniformMatrix4fv( poCurRnd->m_iMVMatrixLoc, 1, GL_FALSE, (GLfloat*) &poCurRnd->m_oPMVMatrix.m[0][0] );
	CHECKERROR();
	
	static GLuint uiIdxs[64] = {0,1,2,2,3,0};
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,uiIdxs); 
	/// glDrawArrays(GL_TRIANGLES, 0, 2);

	CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints)
{
	#ifdef REAL_CLASS
	return;
	#endif

	CRendererHandler* poCurRnd = FECglobals.m_poCurrentRenderer;

	if (_poUVs)
	{
		glVertexAttribPointer( poCurRnd->m_iUVLoc, 2, GL_FLOAT, GL_FALSE,0, _poUVs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iUVLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iUVLoc );
		CHECKERROR();
	}

	if (_poVCs)
	{
		glVertexAttribPointer( poCurRnd->m_iVCLoc, 4, GL_FLOAT, GL_FALSE,0, _poVCs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iVCLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iVCLoc );
		CHECKERROR();
	}

	if (_poVXs)
	{
		glVertexAttribPointer( poCurRnd->m_iVXLoc, 2, GL_FLOAT, GL_FALSE,0, _poVXs );
		CHECKERROR();
		glEnableVertexAttribArray ( poCurRnd->m_iVXLoc );
		CHECKERROR();
	}
	else
	{
		glDisableVertexAttribArray ( poCurRnd->m_iVXLoc );
		CHECKERROR();
	}

	glUniformMatrix4fv( poCurRnd->m_iMVMatrixLoc, 1, GL_FALSE, (GLfloat*) &poCurRnd->m_oPMVMatrix.m[0][0] );
	CHECKERROR();

	if (_pusIdx)
	{
		glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
	}
	else
		glDrawArrays(GL_TRIANGLES,0,_uiPoints);
}
// ----------------------------------------------------------------------------
void CFECore::Graphics::RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor)
{
	/*
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0,0,FECglobals.m_rCurDepth);

		glBegin(GL_LINES);

			glColor4f((float)_oColor.r,(float)_oColor.g,(float)_oColor.b,(float)_oColor.a);
			glVertex2f((float)_oIni.x,(float)_oIni.y);
			glVertex2f((float)_oEnd.x,(float)_oEnd.y);

		glEnd();

    glPopMatrix();   
    */ 
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

    return(false);
}
// ----------------------------------------------------------------------------
FEBool CFECore::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
	const uint CLAMP_MODES[] = {GL_CLAMP_TO_EDGE,GL_REPEAT};

	TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    if (_sProperty == "DiffuseMap.SWrapMode")
    {
		SetMaterial(_hMaterial);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,CLAMP_MODES[(uint)_pParam & 0x01]);CHECKERROR();
        return (true);
    }
else if (_sProperty == "DiffuseMap.TWrapMode")
    {
		SetMaterial(_hMaterial);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,CLAMP_MODES[(uint)_pParam & 0x01]);CHECKERROR();
        return (true);
    }

    return(false);
}

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
