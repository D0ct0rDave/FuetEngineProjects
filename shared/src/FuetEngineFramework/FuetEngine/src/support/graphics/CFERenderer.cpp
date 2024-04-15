// ----------------------------------------------------------------------------
/*! \class CFRenderer
 *  \brief Class for Rendering Functionalities
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
// NOTE: Many things can be optimized but has been left "as is" for code clarity
// reasons
// ----------------------------------------------------------------------------
#include "CFERenderer.h"
#include "types/CFEArray.h"
#include "core/CFECore.h"
#include "support/misc/CFELogger.h"
#include "graphics/font/CFEFont.h"
#include "support/misc/CFEStringUtils.h"

#include <stdlib.h>

/*
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <nn/ulcd.h>
#include <nn/gx.h>
#include <nn/gx/CTR/gx_CommandAccess.h>*/


// ----------------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <algorithm>	// for std::sort
// ----------------------------------------------------------------------------
// Class to store data for internal use
// ----------------------------------------------------------------------------
     
class CFERendererData
{
    public:

        CFERendererData() :
   
            m_poFont(NULL),
			m_rTextPointSize(_1r),
			m_rTextTracking(_1r),
			m_rTextInterlining(_0r),
   
            m_hHandler(NULL),
            m_uiScreenVWidth(640),
            m_uiScreenVHeight(480),
            m_rDeltaT(_0r),
            m_oStartTick(0),

            m_hCurMaterial(NULL),

            m_oCamPos(CFEVect2::ZERO()),
            m_rCamZoom(_1r),
            m_rCamAngle(_0r),

            m_oViewPos(CFEVect2::ZERO()),
            m_rViewAngle(_0r),

            m_bDebugSprites(false)
        {
			m_oStartTick = CFECore::oGetEngineTicks();
            m_oCamMat.LoadIdentity();
            m_oStack.push_back(m_oCamMat);
        }

        CFEFont*            	m_poFont;
        FEReal					m_rTextPointSize;
        FEReal					m_rTextTracking;
        FEReal					m_rTextInterlining;
        FEHandler           	m_hHandler;
        uint                	m_uiScreenVWidth;
        uint                	m_uiScreenVHeight;
        FEReal              	m_rDeltaT;
        FETick					m_oStartTick;
		
		FEHandler				m_hCurMaterial;

        CFEVect2				m_oCamPos;
        FEReal					m_rCamZoom;
        FEReal					m_rCamAngle;

		CFEVect2				m_oViewPos;
        FEReal					m_rViewAngle;

        CFEMatrix				m_oCamMat;
		CFEArray<CFEMatrix> 	m_oStack;
		bool					m_bUploadTransform;

		// -------------------------------------------------------
		// Other
		// -------------------------------------------------------      
		uint					m_uiScreenWidth;
		uint					m_uiScreenHeight;
		int						m_iXDIV;				// ScreenVWidth  / ScreenWidth
		int						m_iYDIV;				// ScreenVHeight / ScreenHeight

        // Debug sprites
        FEBool					m_bDebugSprites;
};
// ----------------------------------------------------------------------------
CFERenderer::CFERenderer()
{
    // Create the structure that holds all the internal data of the renderer.
    m_poData = new CFERendererData; // WARNING: Should use CFESystem allocators !?!
}
// ----------------------------------------------------------------------------
/// Constructor of the rendering system
CFERenderer::~CFERenderer()
{
	Finish();
	delete m_poData;
}
// ----------------------------------------------------------------------------
// Initialize the rendering system
// ----------------------------------------------------------------------------
void CFERenderer::Init(FEHandler _hParam)
{
    m_poData->m_hHandler = CFECore::Graphics::hInit(_hParam);

    if (m_poData->m_hHandler == NULL)
    {
        CFECore::Log::Print("ERROR: Unable to initialize renderer");
    }
}
// ----------------------------------------------------------------------------
// Finalize the rendering system
// ----------------------------------------------------------------------------
void CFERenderer::Finish()
{
	if (m_poData->m_hHandler != NULL)
	{
		CFECore::Graphics::Finish(m_poData->m_hHandler);
		m_poData->m_hHandler = NULL;
	}

    m_poData->m_oStack.clear();
}
// ----------------------------------------------------------------------------
// Sets the screen virtual width.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVWidth(uint _uiScrVWidth)
{
    m_poData->m_uiScreenVWidth = _uiScrVWidth;
	
	FEHandler hOldHnd = CFECore::Graphics::hGetCurrentRenderer();
	CFECore::Graphics::SetCurrentRenderer(m_poData->m_hHandler);
    CFECore::Graphics::SetScreenVWidth(m_poData->m_uiScreenVWidth);

	uint uiData = 0;
	if (CFECore::Graphics::bGetProperty("ScreenWidth",(FEPointer)&uiData))
		m_poData->m_uiScreenWidth = uiData;
	else
		m_poData->m_uiScreenWidth = _uiScrVWidth;

	m_poData->m_iXDIV = m_poData->m_uiScreenVWidth /m_poData->m_uiScreenWidth;
	if (m_poData->m_iXDIV<=0) m_poData->m_iXDIV = 1;

    CFECore::Graphics::SetCurrentRenderer(hOldHnd);
}
// ----------------------------------------------------------------------------
// Sets the screen virtual height.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVHeight(uint _uiScrVHeight)
{
    m_poData->m_uiScreenVHeight = _uiScrVHeight;

	FEHandler hOldHnd = CFECore::Graphics::hGetCurrentRenderer();
	CFECore::Graphics::SetCurrentRenderer(m_poData->m_hHandler);
    CFECore::Graphics::SetScreenVHeight(m_poData->m_uiScreenVHeight);

	uint uiData = 0;
	if (CFECore::Graphics::bGetProperty("ScreenHeight",(FEPointer)&uiData))
		m_poData->m_uiScreenHeight = uiData;
	else
		m_poData->m_uiScreenHeight = _uiScrVHeight;

	m_poData->m_iYDIV = m_poData->m_uiScreenVHeight/m_poData->m_uiScreenHeight;
	if (m_poData->m_iYDIV<=0) m_poData->m_iYDIV = 1;

    CFECore::Graphics::SetCurrentRenderer(hOldHnd);
}
// ----------------------------------------------------------------------------
// Retrieves the virtual screen width.
// ----------------------------------------------------------------------------
uint CFERenderer::uiGetScreenVWidth()
{
    return( m_poData->m_uiScreenVWidth );
}
// ----------------------------------------------------------------------------
// Retrieves the virtual screen height.
// ----------------------------------------------------------------------------
uint CFERenderer::uiGetScreenVHeight()
{
    return( m_poData->m_uiScreenVHeight );
}
// ----------------------------------------------------------------------------
// Resets the timer of the renderer.
// ----------------------------------------------------------------------------
void CFERenderer::ResetTimer()
{
    m_poData->m_oStartTick = CFECore::oGetEngineTicks();
    m_poData->m_rDeltaT = _0r;
}
// ----------------------------------------------------------------------------
// Starts a new frame
// ----------------------------------------------------------------------------
void CFERenderer::BeginScene()
{
	FETick oTick = CFECore::oGetEngineTicks();

	// ----------------------
	// avoid conflicts:
	// ----------------------
	#ifdef MASTER_RELEASE_VERSION
	if (m_poData->m_oStartTick > oTick)
	{
		// possible overflow??
		m_poData->m_rDeltaT = _0r;
	}
	else
	{
		m_poData->m_rDeltaT		= CFECore::rGetTickTime(oTick - m_poData->m_oStartTick);

		// too much time spent somewhere??
		if (m_poData->m_rDeltaT > _025r)
			m_poData->m_rDeltaT		= _025r;
	}
	#else
	m_poData->m_rDeltaT		= CFECore::rGetTickTime(oTick - m_poData->m_oStartTick);
	#endif

	m_poData->m_oStartTick = oTick;
    
    CFECore::Graphics::SetCurrentRenderer(m_poData->m_hHandler);
	CFECore::Graphics::BeginScene();
}
// ----------------------------------------------------------------------------
// Ends the current drawing frame
// ----------------------------------------------------------------------------
void CFERenderer::EndScene()
{
	CFECore::Graphics::EndScene();
}
// ----------------------------------------------------------------------------
// Sets up the blending mode for subsequent rendering operations. If blendinf is
// BM_NONE or BM_COPY, blending is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetBlendMode(EFEBlendMode _eBlendMode)
{
	CFECore::Graphics::SetBlendMode(_eBlendMode);    
}
// ----------------------------------------------------------------------------
// Sets the current material for further operations. If material is NULL, material
// mapping is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetMaterial(FEHandler _hMaterial)
{
	m_poData->m_hCurMaterial = _hMaterial;
	CFECore::Graphics::SetMaterial(_hMaterial);
}
// ----------------------------------------------------------------------------
void CFERenderer::SetFont(CFEFont* _poFont)
{
    m_poData->m_poFont = _poFont;
}
// ----------------------------------------------------------------------------
/// Sets the point size for the rendered text.
void CFERenderer::SetTextPointSize(FEReal _rTextPointSize)
{
	m_poData->m_rTextPointSize = _rTextPointSize;
}
// ----------------------------------------------------------------------------
/// Sets the tracking for the rendered text.
void CFERenderer::SetTextTracking(FEReal _rTextTracking)
{
	m_poData->m_rTextTracking = _rTextTracking;
}
// ----------------------------------------------------------------------------
/// Sets the interlining space for the rendered text.
void CFERenderer::SetTextInterlining(FEReal _rTextInterlining)
{
	m_poData->m_rTextInterlining = _rTextInterlining;
}
// ----------------------------------------------------------------------------
/// Sets the current depth for the subsequent rendering calls.
void CFERenderer::SetDepth(FEReal _rDepth)
{
	CFECore::Graphics::SetDepth(_rDepth);	
}
// -----------------------------------------------------------------------------
void CFERenderer::RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
	if (m_poData->m_bUploadTransform)
		UploadTransform();

	CFEVect2 oIni(_rIX,_rIY);
	CFEVect2 oEnd(_rFX,_rFY);

	// save material
	FEHandler hCurMaterial = m_poData->m_hCurMaterial;
	SetMaterial(NULL);

	CFECore::Graphics::RenderLine(oIni,oEnd,_oColor);
	
	SetMaterial(hCurMaterial);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
	if (m_poData->m_bUploadTransform)
		UploadTransform();

    // Set coords
    CFEVect2 oVX[4];
    oVX[0].x = _rIX;
    oVX[0].y = _rIY;

    oVX[1].x = _rFX;
    oVX[1].y = _rIY;

    oVX[2].x = _rFX;
    oVX[2].y = _rFY;

    oVX[3].x = _rIX;
    oVX[3].y = _rFY;

	// save material
	FEHandler hCurMaterial = m_poData->m_hCurMaterial;
	SetMaterial(NULL);

	CFECore::Graphics::RenderLine(oVX[0],oVX[1],_oColor);
	CFECore::Graphics::RenderLine(oVX[1],oVX[2],_oColor);
	CFECore::Graphics::RenderLine(oVX[2],oVX[3],_oColor);
	CFECore::Graphics::RenderLine(oVX[3],oVX[0],_oColor);

	SetMaterial(hCurMaterial);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
	if (m_poData->m_bUploadTransform)
		UploadTransform();

	CFECore::Graphics::RenderQuad(CFEVect2(_rIX,_rIY),CFEVect2(_rFX,_rFY),CFEVect2::ZERO(),CFEVect2::ONE(),_oColor);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuadList(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads)
{
	if (m_poData->m_bUploadTransform)
		UploadTransform();

	CFECore::Graphics::RenderQuadList(_poVX,_poUV,_poVC,_uiNumQuads);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderSprite(const CFEVect2& _oPos,const CFEVect2& _oPivot,const CFEVect2& _oScale,FEReal _rAngle,
							   const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                               const CFEColor& _oColor,FEReal _rAlpha)
{
	if (m_poData->m_bUploadTransform)
		UploadTransform();

	CFECore::Graphics::RenderSprite(_oPos,_oPivot,_oScale,_rAngle,_oIUV,_oFUV,_oColor,_rAlpha);
}
// ----------------------------------------------------------------------------
// set point size debería ir fuera en otra función
// set interline:
// set kerning:
// ----------------------------------------------------------------------------
// NOTE: We have to use /n instead of \n because libConfig internally converts
// the 2 characters ('\' and 'n') in only one ('\n'), and then it doesn't work.
// ----------------------------------------------------------------------------
void CFERenderer::RenderText(const CFEString& _sStr,FEReal _rX,FEReal _rY,const CFEColor& _oColor,EFETextHAlignmentMode _eHAlignment,EFETextVAlignmentMode _eVAlignment)
{
    if (m_poData->m_poFont == NULL) return;							// Nothing to render with this font
    if ((_sStr.szString() == NULL) || (_sStr == "")) return;		// Nothing to render

	if (m_poData->m_bUploadTransform)
		UploadTransform();

	// Cache font object.
	CFEFont* poFont = m_poData->m_poFont;

	// save material
	FEHandler hCurMaterial = m_poData->m_hCurMaterial;
	SetMaterial(poFont->hMaterial());

	// First of all count the number of \n
	char* szStr = (char*)_sStr.szString();
	char* szStart = szStr;

	uint uiLines = 1;
	while (*szStart)
	{
		if ((szStart[0] == '/') && (szStart[1] == 'n'))
			uiLines++;

		szStart++;
	}

	// add vertical alignment offset to the initial position
	FEReal rDefCharHeight = (poFont->rDefCharHeight()+m_poData->m_rTextInterlining) * m_poData->m_rTextPointSize;

	switch (_eVAlignment)
	{
		case TVAM_CENTER:
		_rY -= (_05r*(rDefCharHeight*uiLines));
		break;

		case TVAM_BOTTOM:
		_rY -= (rDefCharHeight*uiLines);
		break;

		case TVAM_TOP:
		default:
		break;
	}

	FEReal rTrackingToAdd = (m_poData->m_rTextTracking * m_poData->m_rTextPointSize);

	// fix X offset to properly align fonts to the device.
	_rX   = (((int)_rX)/m_poData->m_iXDIV)*m_poData->m_iXDIV;

	// start processing the input string
	szStart = szStr;
	while (*szStart)
	{
		FEReal rXOfs = _0r;
		char* szEnd   = szStart;

		// count the number of characters to render
		uint uiPos = 0;
		int   iLen = 0;

		if (uiLines > 1)
		{
			while ((*szEnd) && (! ((szEnd[0] == '/') && (szEnd[1] == 'n'))) )
				szEnd++;

			uiPos = szStart-szStr;
			iLen  = poFont->uiStringLen(szStart,szEnd);
		}
		else
		{
			uiPos = 0;
			iLen  = -1;
		}

		// add horizontal alignment offset to the initial position
		switch (_eHAlignment)
		{
			case THAM_CENTER:
			{
				// retrieve the length of the string
				FEReal rLen = (poFont->rStringLen(_sStr,uiPos,iLen) + (((iLen == -1)?_sStr.uiLen():iLen)*m_poData->m_rTextTracking)) * m_poData->m_rTextPointSize;
				rXOfs -= (_05r*rLen);
			}
			break;

			case THAM_RIGHT:
			{
				// retrieve the length of the string
				FEReal rLen = (poFont->rStringLen(_sStr,uiPos,iLen) + (((iLen == -1)?_sStr.uiLen():iLen)*m_poData->m_rTextTracking)) * m_poData->m_rTextPointSize;
				rXOfs -= rLen;
			}
			break;

			case THAM_LEFT:
			default:
			break;
		}

		// fix Y and X offset to properly align fonts to the device.
		rXOfs = (((int)rXOfs)/m_poData->m_iXDIV)*m_poData->m_iXDIV;
		_rY   = (((int)  _rY)/m_poData->m_iYDIV)*m_poData->m_iYDIV;

		const CCharInfo* poPrevChar = NULL;
		for (uint i=0;((i<(uint)iLen) && (*szStart));i++)	// even if iLen == -1 it should work
		{
			uint c = poFont->uiGetCurrentChar(szStart);

			switch (c)
			{
				// space char
				case ' ':
				{
					//
					rXOfs += poFont->rCharWidth(' ')*m_poData->m_rTextPointSize;
					// cPrevChar = 0;
				}
				break;

				// any other char: just draw it.
				default:
				{
					const CCharInfo* poCurChar = poFont->poGetCharInfo(c);
					if (poCurChar != NULL)
					{
						if (poPrevChar != NULL)
						{
							// add the kerning of this character and previous one.
							rXOfs += poFont->rCharKerning(poPrevChar,poCurChar);
						}

						const CFERect& oCharUVs = poCurChar->m_oUV;
						FEReal rCharWidth  = poCurChar->m_usCW * m_poData->m_rTextPointSize;
						FEReal rCharHeight = poCurChar->m_usCH * m_poData->m_rTextPointSize;
						FEReal rCharYOfs   = poCurChar->m_sYOfs * m_poData->m_rTextPointSize;

						// Setup Vertexs
						CFEVect2 oIVX,oFVX;

						oIVX.x = _rX + rXOfs;
						oFVX.x = oIVX.x + rCharWidth;

						oIVX.y = _rY + rCharYOfs;
						oFVX.y = oIVX.y + rCharHeight;

						rXOfs += rCharWidth + rTrackingToAdd;

						// Send new quad
						CFECore::Graphics::RenderQuad(oIVX,oFVX,oCharUVs.m_oIni,oCharUVs.m_oEnd,_oColor);
					}

					// Store character
					poPrevChar = poCurChar;
				}
			}

			// next char in the string.
			szStart = poFont->szGotoNextChar(szStart);
		}

		// new line?
		if ((szEnd[0] == '/') && (szEnd[1] == 'n'))
		{
			//
			_rY += rDefCharHeight;
			szStart+=2;
		}
	}

	// restore material
	SetMaterial(hCurMaterial);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor,uint _uiNumPoints)
{
	/// Draws a mesh.
	if (m_poData->m_bUploadTransform)
		UploadTransform();

    CFECore::Graphics::RenderMesh(_pusIdx,_poVXs,_poUVs,_oColor,_uiNumPoints);
}
// -----------------------------------------------------------------------------
FEReal CFERenderer::rGetDeltaT()
{
    return(m_poData->m_rDeltaT);
}
// -----------------------------------------------------------------------------
void CFERenderer::ResetTransform()
{
    m_poData->m_oStack[m_poData->m_oStack.size()-1].LoadIdentity();
    m_poData->m_bUploadTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::PushTransform()
{
    if (m_poData->m_oStack.size() == 0)
    {
        // assert or error
        return;
    }

    m_poData->m_oStack.push_back( m_poData->m_oStack[m_poData->m_oStack.size()-1] );
}
// -----------------------------------------------------------------------------
void CFERenderer::PopTransform()
{
    if (m_poData->m_oStack.size() == 1)
    {
        // assert or error
        return;
    }

    m_poData->m_oStack.pop_back();
    m_poData->m_bUploadTransform = true;
}
// -----------------------------------------------------------------------------
const CFEMatrix& CFERenderer::oGetTransform()
{
    return( m_poData->m_oStack[m_poData->m_oStack.size()-1] );
}
// -----------------------------------------------------------------------------
void CFERenderer::ScaleTransform(FEReal _rScaleX,FEReal _rScaleY)
{
    if ((_rScaleX != _1r) || (_rScaleY != _1r))
	{
		m_poData->m_oStack[m_poData->m_oStack.size()-1].Scale(_rScaleX,_rScaleY);
		m_poData->m_bUploadTransform = true;
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateTransform(FEReal _rX,FEReal _rY)
{
	if ((_rX != _0r) || (_rY != _0r))
	{
		m_poData->m_oStack[m_poData->m_oStack.size()-1].Translate(_rX,_rY);
		m_poData->m_bUploadTransform = true;
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::RotateTransform(FEReal _rAngle)
{
	if (_rAngle != _0r)
	{
		m_poData->m_oStack[m_poData->m_oStack.size()-1].Rotate(_rAngle);
		m_poData->m_bUploadTransform = true;
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::UploadTransform()
{
    CFEMatrix oMat;
    oMat.Multiply(m_poData->m_oStack[ m_poData->m_oStack.size()-1 ],m_poData->m_oCamMat);
	
	CFECore::Graphics::SetTransform(oMat);

	m_poData->m_bUploadTransform = false;
}
// -----------------------------------------------------------------------------
void CFERenderer::UpdateViewTransform()
{
	m_poData->m_oCamMat.LoadIdentity();

	// View transforms
 	m_poData->m_oCamMat.Translate(_05r*m_poData->m_uiScreenVWidth,_05r*m_poData->m_uiScreenVHeight);
	m_poData->m_oCamMat.Rotate(m_poData->m_rViewAngle);
	m_poData->m_oCamMat.Translate(-_05r*(FEReal)(m_poData->m_uiScreenVWidth),
								  -_05r*(FEReal)(m_poData->m_uiScreenVHeight));

	// Optimizable: can be added to the previous transform
	m_poData->m_oCamMat.Translate(-m_poData->m_oViewPos.x,-m_poData->m_oViewPos.y);

	// Camera transforms
	m_poData->m_oCamMat.Scale(m_poData->m_rCamZoom,m_poData->m_rCamZoom);
	m_poData->m_oCamMat.Translate(-m_poData->m_oCamPos.x,-m_poData->m_oCamPos.y);
	m_poData->m_oCamMat.Rotate(m_poData->m_rCamAngle);
	
	m_poData->m_bUploadTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::ResetCamera()
{
	m_poData->m_oCamPos.x = _0r;
	m_poData->m_oCamPos.y = _0r;
	m_poData->m_rCamZoom  = _1r;
	m_poData->m_rCamAngle = _0r;

	UpdateViewTransform();
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateCamera(FEReal _rX,FEReal _rY)
{
	m_poData->m_oCamPos.x = _rX;
	m_poData->m_oCamPos.y = _rY;

	UpdateViewTransform();
}
// -----------------------------------------------------------------------------
void CFERenderer::ZoomCamera(FEReal _rZoom)
{
	if (_rZoom != m_poData->m_rCamZoom)
	{
		m_poData->m_rCamZoom = _rZoom;
		UpdateViewTransform();
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::RotateCamera(FEReal _rAngle)
{
	if (_rAngle != m_poData->m_rCamAngle)
	{
		m_poData->m_rCamAngle = _rAngle;
		UpdateViewTransform();
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateView(FEReal _rX,FEReal _rY)
{
	m_poData->m_oViewPos.x = _rX;
	m_poData->m_oViewPos.y = _rY;

	UpdateViewTransform();
}
// -----------------------------------------------------------------------------
void CFERenderer::RotateView(FEReal _rAngle)
{
	if (_rAngle != m_poData->m_rViewAngle)
	{
		m_poData->m_rViewAngle = _rAngle;
		UpdateViewTransform();
	}
}
// -----------------------------------------------------------------------------
CFEVect2 CFERenderer::oGetCameraTranslation()
{
	return(m_poData->m_oCamPos);
}
// -----------------------------------------------------------------------------
FEReal CFERenderer::rGetCameraZoom()
{
	return(m_poData->m_rCamZoom);
}
// -----------------------------------------------------------------------------
FEReal CFERenderer::rGetCameraRotation()
{
	return(m_poData->m_rCamAngle);
}
// -----------------------------------------------------------------------------
CFEVect2 CFERenderer::oGetViewTranslation()
{
	return(m_poData->m_oViewPos);
}
// -----------------------------------------------------------------------------
FEReal CFERenderer::rGetViewRotation()
{
	return(m_poData->m_rViewAngle);
}
// -----------------------------------------------------------------------------
uint CFERenderer::uiRegisterShader(const CFEString& _sShader)
{
	return(0);
}
// -----------------------------------------------------------------------------
void CFERenderer::SetShader(uint _uiShaderID)
{
	// m_poData->m_oRenderState.m_uiShaderID = _uiShaderID;
}
// -----------------------------------------------------------------------------
uint CFERenderer::uiGetShader()
{
	return(0); // m_poData->m_oRenderState.m_uiShaderID);
}
// -----------------------------------------------------------------------------
const CFEMatrix& CFERenderer::oGetCamViewTransform()
{
	return(m_poData->m_oCamMat);
}
// -----------------------------------------------------------------------------
FEBool CFERenderer::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	 if (_sProperty |= "DEBUG_SPRITES")
    {
        *(FEBool*)_pParam = m_poData->m_bDebugSprites ;
        return(true);
    }
	else
    {
        return(CFECore::Graphics::bGetProperty(_sProperty,_pParam) );
    }
}
// -----------------------------------------------------------------------------
FEBool CFERenderer::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty |= "DEBUG_SPRITES")
    {
        m_poData->m_bDebugSprites = (FEBool)(_pParam != NULL);
        return(true);
    }
	else
    {
		FEHandler hOldHnd = CFECore::Graphics::hGetCurrentRenderer();
		CFECore::Graphics::SetCurrentRenderer(m_poData->m_hHandler);
        FEBool bRes = CFECore::Graphics::bSetProperty(_sProperty,_pParam);
        CFECore::Graphics::SetCurrentRenderer(hOldHnd);

        return(bRes);
    }
}
// -----------------------------------------------------------------------------
