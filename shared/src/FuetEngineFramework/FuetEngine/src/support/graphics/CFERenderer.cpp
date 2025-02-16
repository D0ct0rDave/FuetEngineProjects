// ----------------------------------------------------------------------------
/*! \class CFRenderer
 *  \brief Class for Rendering Functionalities
 *  \author David M&aacute;rquez de la Cruz
 *  \version _1r
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
// NOTE: Many things can be optimized but has been left "as is" for code clarity
// reasons
// ----------------------------------------------------------------------------
#include "CFERenderer.h"
#include "System/CFESystem.h"
#include "Support/Misc/CFELogger.h"
#include "Graphics/Font/CFEFont.h"

// ----------------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------------
#include <stdio.h>

// #define DEBUG_SPRITES

// ----------------------------------------------------------------------------
// Class to store data for internal use
// ----------------------------------------------------------------------------
class CFERendererData
{
    public:
        CFERendererData() :
            m_hMaterial(NULL),
            m_poFont(NULL),
            m_eBlendMode(BM_NONE),
            m_hHandler(NULL),
            m_uiScreenVWidth(640),
            m_uiScreenVHeight(480),
            m_rDeltaT(_0r),
            m_bUpdateTransform(false)
        {
            m_rLastEngineTime = CFESystem::rGetEngineTime();

			m_oCamPos.x = _0r;
			m_oCamPos.y = _0r;
			m_rCamZoom = _1r;
			m_rDepth = _0r;
            m_oCamMat.LoadIdentity();
            m_oStack.push_back(m_oCamMat);
        }

        FEHandler           m_hMaterial;
        CFEFont*            m_poFont;
        EFEBlendMode        m_eBlendMode;
        FEHandler           m_hHandler;
        uint                m_uiScreenVWidth;
        uint                m_uiScreenVHeight;
        FEReal              m_rDeltaT;
        FEReal              m_rLastEngineTime;
        CFEArray<CFEMatrix> m_oStack;
        CFEVect2			m_oCamPos;
        FEReal				m_rCamZoom;
        CFEMatrix			m_oCamMat;
        bool				m_bUpdateTransform;
        FEReal              m_rDepth;
        
        // Per frame stats
        uint                m_uiQuads;
};
// ----------------------------------------------------------------------------
CFERenderer::CFERenderer()
{
    // Create the structure that holds all the internal data of the renderer.
    m_poData = new CFERendererData; // WARNING: Should use CFESystem allocators !?!
}
// ----------------------------------------------------------------------------
// Initialize the rendering system
// ----------------------------------------------------------------------------
void CFERenderer::Init(FEHandler _hParam)
{
    m_poData->m_hHandler = CFESystem::Graphics::hInit(_hParam);

    if (m_poData->m_hHandler == NULL)
    {
        CFESystem::Log::Print("ERROR: Unable to initialize renderer");
    }
}
// ----------------------------------------------------------------------------
// Finalize the rendering system
// ----------------------------------------------------------------------------
void CFERenderer::Finish()
{
    CFESystem::Graphics::Finish(m_poData->m_hHandler);
    m_poData->m_hHandler = NULL;
}
// ----------------------------------------------------------------------------
// Sets the screen virtual width.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVWidth(uint _uiScrVWidth)
{
    m_poData->m_uiScreenVWidth = _uiScrVWidth;
    CFESystem::Graphics::SetScreenVWidth(m_poData->m_uiScreenVWidth);   
}
// ----------------------------------------------------------------------------
// Sets the screen virtual height.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVHeight(uint _uiScrVHeight)
{
    m_poData->m_uiScreenVHeight = _uiScrVHeight;
    CFESystem::Graphics::SetScreenVHeight(m_poData->m_uiScreenVHeight);
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
// Starts a new frame
// ----------------------------------------------------------------------------
void CFERenderer::BeginScene()
{
    FEReal rEngineTime = CFESystem::rGetEngineTime();
    m_poData->m_rDeltaT = rEngineTime - m_poData->m_rLastEngineTime;
    m_poData->m_rLastEngineTime = rEngineTime;  
    m_poData->m_uiQuads = 0;

    CFESystem::Graphics::SetScreenVWidth(m_poData->m_uiScreenVWidth);
    CFESystem::Graphics::SetScreenVHeight(m_poData->m_uiScreenVHeight);
    CFESystem::Graphics::BeginScene(m_poData->m_hHandler);
}
// ----------------------------------------------------------------------------
// Ends the current drawing frame
// ----------------------------------------------------------------------------
void CFERenderer::EndScene()
{
    CFESystem::Graphics::EndScene();
}
// ----------------------------------------------------------------------------
// Sets up the blending mode for subsequent rendering operations. If blendinf is
// BM_NONE or BM_COPY, blending is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetBlendMode(EFEBlendMode _eBlendMode)
{
    m_poData->m_eBlendMode = _eBlendMode;
}
// ----------------------------------------------------------------------------
// Sets the current material for further operations. If material is NULL, material
// mapping is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetMaterial(FEHandler _hMaterial)
{
    m_poData->m_hMaterial = _hMaterial;
}
// ----------------------------------------------------------------------------
void CFERenderer::SetFont(CFEFont* _poFont)
{
    m_poData->m_poFont = _poFont;
}
// ----------------------------------------------------------------------------
/// Sets the current depth for the subsequent rendering calls.
void CFERenderer::SetDepth(FEReal _rDepth)
{
    m_poData->m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
inline void CFERenderer::RenderQuadINT(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC)
{
	if (m_poData->m_bUpdateTransform)
		UpdateTransform();

	CFESystem::Graphics::RenderQuads(_poVX,_poUV,_poVC,1);

    // new rendered quad
    m_poData->m_uiQuads++;
}
// ----------------------------------------------------------------------------
void CFERenderer::UpdateTransform()
{
    CFEMatrix* poMat = &m_poData->m_oStack[ m_poData->m_oStack.size()-1 ];
    CFEMatrix oM;
    oM.Multiply(*poMat,m_poData->m_oCamMat);
    CFESystem::Graphics::SetTransform(oM);

    m_poData->m_bUpdateTransform = false;
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poColor)
{
    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    
	if (_poUVs == NULL)
		CFESystem::Graphics::SetMaterial( NULL );
	else
		CFESystem::Graphics::SetMaterial( m_poData->m_hMaterial );
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    RenderQuadINT(_poVXs,_poUVs,_poColor);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor)
{
    CFEColor oVC[4];

    // Set color
    oVC[0] = _oColor;
    oVC[1] = _oColor;
    oVC[2] = _oColor;
    oVC[3] = _oColor;
    
    RenderQuad(_poVXs,_poUVs,oVC);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                             FEReal _rIU,FEReal _rIV,FEReal _rFU,FEReal _rFV,
                             const CFEColor& _oColor)
{
    CFEVect2 oVX[4];
    CFEVect2 oUV[4];
    CFEColor oVC[4];

    // Set coords
    oVX[0].x = _rIX;
    oVX[0].y = _rIY;

    oVX[1].x = _rFX;
    oVX[1].y = _rIY;

    oVX[2].x = _rFX;
    oVX[2].y = _rFY;

    oVX[3].x = _rIX;
    oVX[3].y = _rFY;

    // Set UVs
    oUV[0].x = _rIU;
    oUV[0].y = _rIV;

    oUV[1].x = _rFU;
    oUV[1].y = _rIV;

    oUV[2].x = _rFU;
    oUV[2].y = _rFV;

    oUV[3].x = _rIU;
    oUV[3].y = _rFV;

    // Set color
    oVC[0] = _oColor;
    oVC[1] = _oColor;
    oVC[2] = _oColor;
    oVC[3] = _oColor;

    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
	CFESystem::Graphics::SetMaterial( m_poData->m_hMaterial );	
	CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    RenderQuadINT(oVX,oUV,oVC);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderSprite(const CFEVect2& _oPos,const CFEVect2& _oPivot,const CFEVect2& _oScale,FEReal _rAngle,
							   const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                               const CFEColor& _oColor,FEReal _rAlpha)
{
    CFEVect2 oVX[4];
    CFEVect2 oUV[4];
    CFEColor oVC[4];

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
	    oVX[0]   = -_oPivot;
		oVX[2]   = CFEVect2(_1r,_1r) - _oPivot;
		oVX[1].x = oVX[2].x;
		oVX[1].y = oVX[0].y;
		oVX[3].x = oVX[0].x;
		oVX[3].y = oVX[2].y;

		for (uint i=0;i<4;i++)
		{
			oVX[i] *= _oScale;
			oVX[i] += _oPos;
		}
	}
	else
	{
		oVX[0]   = -_oPivot;
		oVX[2]   = CFEVect2(_1r,_1r) - _oPivot;
		oVX[1].x = oVX[2].x;
		oVX[1].y = oVX[0].y;
		oVX[3].x = oVX[0].x;
		oVX[3].y = oVX[2].y;

		FEReal rCosA = CFEMath::rCos(_rAngle);
		FEReal rSinA = CFEMath::rSin(_rAngle);

		for (uint i=0;i<4;i++)
		{
			// 2
			oVX[i] *= _oScale;

			// 3
			CFEVect2 oPos = oVX[i];
			oVX[i].x = oPos.x*rCosA - oPos.y*rSinA;
			oVX[i].y = oPos.x*rSinA + oPos.y*rCosA;

			// 5
			oVX[i] += _oPos;   
		}
	}

    // Set UVs
    oUV[0]   = _oIUV;
    oUV[2]   = _oFUV;
    oUV[1].x = oUV[2].x;
    oUV[1].y = oUV[0].y;
    oUV[3].x = oUV[0].x;
    oUV[3].y = oUV[2].y;

    // Set color
    oVC[0] = _oColor;
    oVC[0].a *= _rAlpha;
    oVC[1] = oVC[0];
    oVC[2] = oVC[1];
    oVC[3] = oVC[2];

    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetMaterial( m_poData->m_hMaterial );
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    RenderQuadINT(oVX,oUV,oVC);

    #ifdef DEBUG_SPRITES

		CFEVect2 oPivotPos = _oPivot + _oPos;
		
		const float LINE_SIZE = 5.0;
		SetBlendMode(BM_COPY);
		RenderLine(oPivotPos.x			,oPivotPos.y-LINE_SIZE	,oPivotPos.x			,oPivotPos.y+LINE_SIZE	,CFEColor(1,1,0,1));
		RenderLine(oPivotPos.x-LINE_SIZE,oPivotPos.y			,oPivotPos.x+LINE_SIZE	,oPivotPos.y			,CFEColor(1,1,0,1));
		
		for (uint j=0;j<4;j++)
			RenderLine(oVX[j].x,oVX[j].y,oVX[(j+1) & 0x03].x,oVX[(j+1) & 0x03].y,CFEColor(0,1,0,1));
    #endif
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderText(const CFEString& _sStr,FEReal _rX,FEReal _rY,const CFEColor& _oColor,EFETextHAlignmentMode _eHAlignment,EFETextVAlignmentMode _eVAlignment)
{
    if (m_poData->m_poFont == NULL) return;     // Nothing to render with this font
    if ((_sStr.szString() == NULL) || (_sStr == "")) return;       // Nothing to render
    
    FEReal rLen = m_poData->m_poFont->rStringLen(_sStr);

    // add horizontal alignment offset to the initial position
    switch (_eHAlignment)
    {
        case THAM_CENTER:
        _rX -= (_05r*rLen);
        break;

        case THAM_RIGHT:
        _rX -= rLen;
        break;

        case THAM_LEFT:
        default:
        break;
    }

    // add vertical alignment offset to the initial position
    FEReal rDefCharHeight = m_poData->m_poFont->rDefCharHeight();

    switch (_eVAlignment)
    {
        case TVAM_CENTER:
        _rY -= (_05r*rDefCharHeight);
        break;

        case TVAM_BOTTOM:
        _rY -= rDefCharHeight;
        break;

        case TVAM_TOP:
        default:
        break;
    }

    // Set current material font and blend mode
    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetMaterial( m_poData->m_poFont->hMaterial() );
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    CFEVect2 oVX[4];
    CFEVect2 oUV[4];
    CFEColor oVC[4];

    // Setup colors
    oVC[0] = oVC[1] = oVC[2] = oVC[3] = _oColor;

    FEReal rXOfs = _0r;    
    char* szStr = (char*)_sStr.szString();
    while (*szStr)
    {
        switch (*szStr)
        {
            // carriage return (skip it)
            case '\r':
            break;

            // new line
            case '\n':
            {
                //
                rXOfs = _0r;
                _rY += rDefCharHeight;
            }
            break;

            // space char
            case ' ':
            {
                //
                rXOfs += (m_poData->m_poFont->rCharWidth(' ') + m_poData->m_poFont->rCharKerning());
            }
            break;

            // any other char: just draw it.
            default:
            {
		        const CFERect& oCharUVs = m_poData->m_poFont->oCharUVs(*szStr);
		        FEReal rCharWidth  = m_poData->m_poFont->rCharWidth(*szStr);
		        FEReal rCharHeight = m_poData->m_poFont->rCharHeight(*szStr);
                FEReal rCharYOfs  = m_poData->m_poFont->rCharOfs(*szStr);

                // Setup Vertexs
                oVX[0].x = _rX + rXOfs;
                oVX[0].y = _rY + rCharYOfs;

                oVX[1].x = _rX + rXOfs + rCharWidth;
                oVX[1].y = _rY + rCharYOfs;

                oVX[2].x = _rX + rXOfs + rCharWidth;
                oVX[2].y = _rY + rCharYOfs + rCharHeight;

                oVX[3].x = _rX + rXOfs;
                oVX[3].y = _rY + rCharYOfs + rCharHeight;

                // Setup UVs
		        oUV[0].x = oCharUVs.m_oIni.x;
                oUV[0].y = oCharUVs.m_oIni.y;

                oUV[1].x = oCharUVs.m_oEnd.x;
                oUV[1].y = oCharUVs.m_oIni.y;

                oUV[2].x = oCharUVs.m_oEnd.x;
                oUV[2].y = oCharUVs.m_oEnd.y;

                oUV[3].x = oCharUVs.m_oIni.x;
                oUV[3].y = oCharUVs.m_oEnd.y;

                rXOfs += (rCharWidth + m_poData->m_poFont->rCharKerning());

                // Send new quad
                RenderQuadINT(oVX,oUV,oVC);
            }
        }

        // next char in the string.
        szStr ++;
    }
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiNumPoints)
{
	if (m_poData->m_bUpdateTransform)
		UpdateTransform();

	if (_poUVs == NULL)
		CFESystem::Graphics::SetMaterial( NULL );
	else
		CFESystem::Graphics::SetMaterial( m_poData->m_hMaterial );

    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    CFESystem::Graphics::RenderMesh(_pusIdx,_poVXs,_poUVs,_poVCs,_uiNumPoints);
}
// -----------------------------------------------------------------------------
void CFERenderer::RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
	CFEVect2 oIni(_rIX,_rIY);
	CFEVect2 oEnd(_rFX,_rFY);
	
	if (m_poData->m_bUpdateTransform)
		UpdateTransform();

    // Set current material font and blend mode
    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetMaterial( NULL );
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

	CFESystem::Graphics::RenderLine(oIni,oEnd,_oColor);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                             const CFEColor& _oColor)
{
	CFEVect2 oTopLeft(_rIX,_rIY);
	CFEVect2 oTopRight(_rFX,_rIY);
	CFEVect2 oBottomLeft(_rIX,_rFY);
	CFEVect2 oBottomRight(_rFX,_rFY);
	
	if (m_poData->m_bUpdateTransform)
		UpdateTransform();

    // Set current material font and blend mode    
    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetMaterial( NULL );
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

	CFESystem::Graphics::RenderLine(oTopLeft,oTopRight,_oColor);
	CFESystem::Graphics::RenderLine(oTopRight,oBottomRight,_oColor);
	CFESystem::Graphics::RenderLine(oBottomRight,oBottomLeft,_oColor);
	CFESystem::Graphics::RenderLine(oBottomLeft,oTopLeft,_oColor);
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
    m_poData->m_bUpdateTransform = true;
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
    m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
const CFEMatrix& CFERenderer::oGetTransform()
{
    return( m_poData->m_oStack[m_poData->m_oStack.size()-1] );
}
// -----------------------------------------------------------------------------
void CFERenderer::ScaleTransform(FEReal _rScaleX,FEReal _rScaleY)
{
    m_poData->m_oStack[m_poData->m_oStack.size()-1].Scale(_rScaleX,_rScaleY);
    m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateTransform(FEReal _rX,FEReal _rY)
{
    m_poData->m_oStack[m_poData->m_oStack.size()-1].Translate(_rX,_rY);
    m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::RotateTransform(FEReal _rAngle)
{
    m_poData->m_oStack[m_poData->m_oStack.size()-1].Rotate(_rAngle);
	m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::ResetCamera()
{
	m_poData->m_oCamPos.x = _0r;
	m_poData->m_oCamPos.y = _0r;
	m_poData->m_rCamZoom = _1r;
	m_poData->m_oCamMat.LoadIdentity();	
	
	m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateCamera(FEReal _rX,FEReal _rY)
{
	m_poData->m_oCamPos.x = _rX;
	m_poData->m_oCamPos.y = _rY;
	
	m_poData->m_oCamMat.LoadIdentity();
	m_poData->m_oCamMat.Scale(m_poData->m_rCamZoom,m_poData->m_rCamZoom);
	m_poData->m_oCamMat.Translate(-m_poData->m_oCamPos.x,-m_poData->m_oCamPos.y);

	m_poData->m_bUpdateTransform = true;
}
// -----------------------------------------------------------------------------
void CFERenderer::ZoomCamera(FEReal _rZoom)
{
	m_poData->m_rCamZoom = _rZoom;

	m_poData->m_oCamMat.LoadIdentity();				
	m_poData->m_oCamMat.Scale(m_poData->m_rCamZoom,m_poData->m_rCamZoom);	
	m_poData->m_oCamMat.Translate(-m_poData->m_oCamPos.x,-m_poData->m_oCamPos.y);		

	m_poData->m_bUpdateTransform = true;
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
bool CFERenderer::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty |= "RENDERED_QUADS")
    {
        *(uint*)_pParam = m_poData->m_uiQuads;
        return(true);
    }

    return(false);
}
// -----------------------------------------------------------------------------
