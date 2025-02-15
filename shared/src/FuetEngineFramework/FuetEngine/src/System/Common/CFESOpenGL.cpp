// ----------------------------------------------------------------------------
/*! \class
 *  \brief Graphic Rendering functionality through OpenGL.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (GRAPHICS == USE_OPENGL)
// ----------------------------------------------------------------------------
#if (TARGETPLATFORM == USE_WINDOWS)    // this is something that i don't like
#include <windows.h>
#endif
// ----------------------------------------------------------------------------
#include <GL/gl.h>
#include <GL/glext.h>
#include <stdlib.h>
#include <math.h>

#include "../CFESystem.h"
#include "../CFESystemGlobals.h"

#include "CFESImageLib.h"
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
typedef struct
{
    GLint            m_iID;
    CFESImageInfo*   m_poII;

}TMaterialHandler;
// ----------------------------------------------------------------------------
#ifdef _DEBUG
static GLenum eErr = GL_NO_ERROR;
#define CHECKERROR() eErr=glGetError();
#else
#define CHECKERROR()
#endif
//-----------------------------------------------------------------------------------------
// from www.OpenGL.org
//-----------------------------------------------------------------------------------------
int isExtensionSupported(const char *extension)
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	// Extension names should not have spaces.
	where = (GLubyte *) strchr(extension, ' ');
	
	if (where || *extension == '\0')
		return 0;
	
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
				return 1;
		
		start = terminator;
	}
	
	return 0;
}
// ----------------------------------------------------------------------------
// Function helper that handles localization of data.
// ----------------------------------------------------------------------------
CFESImageInfo* poLoadImage(const CFEString& _sFilename)
{
	CFESImageInfo* poII = NULL;

	if (CFESystem::Local::bIsAutoLocEnabled())
	{
		// try first localized version of the file.
		CFEString sLocalFilename = CFESystem::Local::sGetLocalID(LID_COMMON) + "/" + _sFilename;
		poII = ImageLib::poLoadImage((char*)sLocalFilename.szString());
	
		if (poII != NULL) return(poII);
		
		// try first localized version of the file.
		sLocalFilename = CFESystem::Local::sGetLocalID() + "/" + _sFilename;
		poII = ImageLib::poLoadImage((char*)sLocalFilename.szString());
	}
	else
	{
		poII = ImageLib::poLoadImage((char*)_sFilename.szString());
	}

	return(poII);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetScreenVWidth(uint _uiScrVWidth)
{
    FESglobals.m_uiScrVWidth = _uiScrVWidth;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(float)FESglobals.m_uiScrVWidth,-2.0f/(float)FESglobals.m_uiScrVHeight,1.0f);
    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetScreenVHeight(uint _uiScrVHeight)
{
    FESglobals.m_uiScrVHeight = _uiScrVHeight;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glTranslatef(-1.0f,+ 1.0f,0.0f);
    glScalef    ( 2.0f/(float)FESglobals.m_uiScrVWidth,-2.0f/(float)FESglobals.m_uiScrVHeight,1.0f);
    CHECKERROR();
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetTransform(const CFEMatrix& _rMat)
{
	glMatrixMode(GL_MODELVIEW);

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
	m[3][2] = 0.0f;
	m[3][3] = _rMat.m[2][2];

	glLoadMatrixf((GLfloat*)m);
}
/*
// ----------------------------------------------------------------------------
uint CFESystem::Graphics::uiGetScreenVWidth(FEHandler _hRenderer)
{
    return(FESglobals.m_uiScrVWidth);
}
// ----------------------------------------------------------------------------
uint CFESystem::Graphics::uiGetScreenVHeight(FEHandler _hRenderer)
{
    return(FESglobals.m_uiScrVHeight);
}
*/
// ----------------------------------------------------------------------------
FEHandler CFESystem::Graphics::hGetCurrentRenderer()
{
    return( (FEHandler)(FESglobals.m_poCurrentRenderer) );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::DeleteMaterial(FEHandler _hMaterial)
{
    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    ImageLib::FreeImage(poTH->m_poII);
    glDeleteTextures(1,(const GLuint*)&poTH->m_iID);
    
    delete poTH;
}
// ----------------------------------------------------------------------------
FEHandler CFESystem::Graphics::hLoadMaterial(const CFEString& _sFilename)
{
    // TMaterialHandler    
    CFESImageInfo* poII = poLoadImage(_sFilename);
    if (poII == NULL) return(NULL);

    if (! FESglobals.m_bNPO2Support)
        ImageLib::MakePowerOf2(poII);

    TMaterialHandler* poTH = new TMaterialHandler;
    poTH->m_poII = poII;
    poTH->m_iID  = -1;

    return(poTH);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::ReloadMaterial(FEHandler _hMaterial,const CFEString& _sFilename)
{
    CFESImageInfo* poII = poLoadImage(_sFilename);
    if (poII == NULL) return;
    
    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;

    // Delete previos resources
    ImageLib::FreeImage(poTH->m_poII);
    glDeleteTextures(1,(const GLuint*)&poTH->m_iID);
    
    // Create new ones
    poTH->m_poII = poII;
    if (! FESglobals.m_bNPO2Support)
        ImageLib::MakePowerOf2(poTH->m_poII);

    poTH->m_iID = -1;
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetBlendMode(EFEBlendMode _eBlendMode)
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
void CFESystem::Graphics::SetMaterial(FEHandler _hMaterial)
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
void CFESystem::Graphics::SetDepth(FEReal _rDepth)
{
	FESglobals.m_rCurDepth = _rDepth;
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderQuads(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0,0,FESglobals.m_rCurDepth);
	
		glBegin(GL_QUADS);

			if (_poUV != NULL)
			{ 
				for (uint i=0;i<_uiNumQuads;i++)
				{
					for (uint j=0;j<4;j++)
					{
						// 
						glColor4f((float)_poVC->r,(float)_poVC->g,(float)_poVC->b,(float)_poVC->a);
						glTexCoord2f((float)_poUV->x,(float)_poUV->y);

						/*
						#ifdef DS_PC
						float fX = (float)_poVX->x;
						float fY = (float)_poVX->y;

						float fU = (float)_poUV->x;
						float fV = (float)_poUV->y;

						float fR = (float)_poVC->r;
						float fG = (float)_poVC->g;
						float fB = (float)_poVC->b;
						float fA = (float)_poVC->a;
						#endif
						*/

						glVertex2f((float)_poVX->x,(float)_poVX->y);

						// next vertex
						_poVX ++;
						_poUV ++;
						_poVC ++;
					}
				}
			}
			else
			{
				for (uint i=0;i<_uiNumQuads;i++)
				{
					for (uint j=0;j<4;j++)
					{
						glColor4f((float)_poVC->r,(float)_poVC->g,(float)_poVC->b,(float)_poVC->a);
						glVertex2f((float)_poVX->x,(float)_poVX->y);

						// next vertex
						_poVX ++;
						_poVC ++;
					}
				}
			}

		glEnd();

    glPopMatrix();
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints)
{
	#ifdef REAL_CLASS
	return;
	#endif

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0,0,FESglobals.m_rCurDepth);
	
	    if (_poUVs)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 2*sizeof(GLfloat), (float*)_poUVs);
		}
		else
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		if (_poVCs)
		{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_FLOAT, 4*sizeof(GLfloat), (float*)_poVCs);
		}
		else
		{
		    glColor4f(1,1,1,1);
			glDisableClientState(GL_COLOR_ARRAY);
		}

		if (_poVXs)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), _poVXs);
		}
		else
			glDisableClientState(GL_VERTEX_ARRAY);

		if (_pusIdx)
			glDrawElements(GL_TRIANGLE_STRIP, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
		else
			glDrawArrays(GL_TRIANGLE_STRIP,0,_uiPoints);

    glPopMatrix();
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor)
{
	// glMatrixMode(GL_MODELVIEW);
	// glPushMatrix();
	// glTranslatef(0,0,FESglobals.m_rCurDepth);

		glBegin(GL_LINES);

			glColor4f((float)_oColor.r,(float)_oColor.g,(float)_oColor.b,(float)_oColor.a);
			glVertex2f((float)_oIni.x,(float)_oIni.y);
			glVertex2f((float)_oEnd.x,(float)_oEnd.y);

		glEnd();

    // glPopMatrix();    
}
// ----------------------------------------------------------------------------
bool CFESystem::Graphics::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
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
bool CFESystem::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
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
