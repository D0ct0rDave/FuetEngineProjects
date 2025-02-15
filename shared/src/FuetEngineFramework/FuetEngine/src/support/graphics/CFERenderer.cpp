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
#include "types/CFEArray.h"
#include "System/CFESystem.h"
#include "Support/Misc/CFELogger.h"
#include "Graphics/Font/CFEFont.h"
// ----------------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <algorithm>	// for std::sort
// ----------------------------------------------------------------------------
typedef enum ERenderPrimType
{
	RPT_NONE,

	RPT_QUAD,
	RPT_LINE,
	RPT_RECT,
	RPT_MESH,

	RPT_NUM_PRIMS
};
// ----------------------------------------------------------------------------
class CRenderPrim
{
	public:
		
		static int iPrimsCompFunc(const void* a,const void* b)
		{
			#define PA ((CRenderPrim*)a)
			#define PB ((CRenderPrim*)b)

			if ( PA->m_rDepth != PB->m_rDepth)
			{
				return ((PA->m_rDepth > PB->m_rDepth)?-1:1);
			}
	   else if (PA->m_uiRndIdx != PB->m_uiRndIdx)
			{
				return ( PA->m_uiRndIdx - PB->m_uiRndIdx );
			}
		else
			{
				return( (uint)PB->m_hMat - (uint)PA->m_hMat);
			}

			#undef PA
			#undef PB
		}
		
		static bool bPrimsCompFunc(CRenderPrim a,CRenderPrim b)
		{
			if (a.m_rDepth != b.m_rDepth)
			{
				return ( a.m_rDepth > b.m_rDepth );
			}
	   else if (a.m_uiRndIdx != b.m_uiRndIdx)
			{
				return ( a.m_uiRndIdx < b.m_uiRndIdx );
			}
		else
			return ( a.m_hMat != b.m_hMat );
		}

		/// Comparaison operator for sorting purposes.
		bool operator < (const CRenderPrim& b)
		{
			return( bPrimsCompFunc(*this,b) );
		}

	public:

		CFEVect2		m_oVX[4];
		CFEVect2		m_oUV[4];
		CFEColor		m_oVC[4];

		CFEVect2*		m_poVX;
		CFEVect2*		m_poUV;
		CFEColor*		m_poVC;
		unsigned short* m_pusIdx;
		uint			m_uiNumVXs;


		ERenderPrimType m_ePT;				// Primitive Type

		FEReal			m_rDepth;			// Depth
		uint			m_uiRndIdx;			// Render Idx

		EFEBlendMode	m_eBlendMode;		// Blend Mode
		FEHandler		m_hMat;				// Material		
		CFEMatrix		m_oMat;				// Matrix
};

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
            m_bUpdateTransform(false),
            m_bPrimitiveSorting(false),
            m_rDepth(_0r),
            m_oCamPos(CFEVect2::ZERO()),
            m_rCamZoom(_0r),
			m_uiPrims(0),
            m_bDebugSprites(false)
        {
			m_uiStartTick = CFESystem::uiGetEngineTicks();
            m_oCamMat.LoadIdentity();
            m_oCurTransf.LoadIdentity();
            m_oStack.push_back(m_oCamMat);
        }

        FEHandler           	m_hMaterial;
        CFEFont*            	m_poFont;
        EFEBlendMode        	m_eBlendMode;
        FEHandler           	m_hHandler;
        uint                	m_uiScreenVWidth;
        uint                	m_uiScreenVHeight;
        FEReal              	m_rDeltaT;
        uint					m_uiStartTick;
        CFEArray<CFEMatrix> 	m_oStack;

        CFEVect2				m_oCamPos;
        FEReal					m_rCamZoom;
        CFEMatrix				m_oCamMat;
        CFEMatrix				m_oCurTransf;
        bool					m_bUpdateTransform;

        FEReal              	m_rDepth;
		bool					m_bPrimitiveSorting;
		
        CFEArray<CRenderPrim>	m_oRPList;

        // Per frame stats
        uint					m_uiPrims;
        
        // Debug sprites
        bool					m_bDebugSprites;
        
};
// ----------------------------------------------------------------------------
// Generic method to render primitives.
// ----------------------------------------------------------------------------
inline void GenericRenderPrimitive(CRenderPrim* _poRP)
{
	switch (_poRP->m_ePT)
	{
		case RPT_LINE:
		{
			CFESystem::Graphics::RenderLine(_poRP->m_oVX[0],_poRP->m_oVX[1],_poRP->m_oVC[0]);
		}
		break;

		case RPT_RECT:
		{
			CFESystem::Graphics::RenderLine(_poRP->m_oVX[0],_poRP->m_oVX[1],_poRP->m_oVC[0]);
			CFESystem::Graphics::RenderLine(_poRP->m_oVX[1],_poRP->m_oVX[2],_poRP->m_oVC[0]);
			CFESystem::Graphics::RenderLine(_poRP->m_oVX[2],_poRP->m_oVX[3],_poRP->m_oVC[0]);
			CFESystem::Graphics::RenderLine(_poRP->m_oVX[3],_poRP->m_oVX[0],_poRP->m_oVC[0]);
		}
		break;
		
		case RPT_QUAD:
		{
			CFESystem::Graphics::RenderQuads(_poRP->m_oVX,_poRP->m_oUV,_poRP->m_oVC,1);
		}
		break;

		case RPT_MESH:
		{
			CFESystem::Graphics::RenderMesh(_poRP->m_pusIdx,_poRP->m_poVX,_poRP->m_poUV,_poRP->m_poVC,_poRP->m_uiNumVXs);
		}
		break;
	}
}
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
    m_poData->m_oRPList.clear();
    m_poData->m_oStack.clear();
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
// Resets the timer of the renderer.
// ----------------------------------------------------------------------------
void CFERenderer::ResetTimer()
{
    m_poData->m_uiStartTick = CFESystem::uiGetEngineTicks();
    m_poData->m_rDeltaT = _0r;
}
// ----------------------------------------------------------------------------
// Starts a new frame
// ----------------------------------------------------------------------------
void CFERenderer::BeginScene()
{
	uint uiTick				= CFESystem::uiGetEngineTicks();
    
	// ----------------------
	// avoid conflicts:
	// ----------------------
	
	#ifdef MASTER_RELEASE_VERSION 
	if (m_poData->m_uiStartTick > uiTick)
	{
		// possible overflow??
		m_poData->m_rDeltaT = _0r;
	}
	else
	{
		m_poData->m_rDeltaT		= CFESystem::rGetTickTime(uiTick - m_poData->m_uiStartTick);

		// too much time spent somewhere??
		if (m_poData->m_rDeltaT > _025r)
			m_poData->m_rDeltaT		= _025r;
	}
	#else
	m_poData->m_rDeltaT		= CFESystem::rGetTickTime(uiTick - m_poData->m_uiStartTick);
	#endif

	m_poData->m_uiStartTick = uiTick;
    m_poData->m_uiPrims = 0;

    CFESystem::Graphics::SetScreenVWidth(m_poData->m_uiScreenVWidth);
    CFESystem::Graphics::SetScreenVHeight(m_poData->m_uiScreenVHeight);
    CFESystem::Graphics::BeginScene(m_poData->m_hHandler);   
}
// ----------------------------------------------------------------------------
// Ends the current drawing frame
// ----------------------------------------------------------------------------
void CFERenderer::EndScene()
{
	if (m_poData->m_bPrimitiveSorting)
		FlushRenderPrims();

    CFESystem::Graphics::EndScene();
}
// ----------------------------------------------------------------------------
void CFERenderer::FlushRenderPrims()
{
	if (m_poData->m_oRPList.size()>0)
	{	
		/*
		// Sort primitives:
		std::sort(&m_poData->m_oRPList[0],&m_poData->m_oRPList[0] + m_poData->m_oRPList.size());
		
		// Render primitives:
		for (uint i=0;i<m_poData->m_oRPList.size();i++)
		{
			CRenderPrim* poPrim = &m_poData->m_oRPList[i];

			CFESystem::Graphics::SetTransform(poPrim->m_oMat);
			CFESystem::Graphics::SetBlendMode(poPrim->m_eBlendMode);
			CFESystem::Graphics::SetMaterial(poPrim->m_hMat);
			CFESystem::Graphics::SetDepth(poPrim->m_rDepth);

			GenericRenderPrimitive(poPrim);
		}
		m_poData->m_oRPList.clear();
		*/

		// This algorithm gives priority to 
		//		First	Depth
		//		Then	queue arrival order
		// Sort order: descending (first greater then lower depth values)

		// NOTE: iMaxLoops management is a kind of antibug in case some prim has weird depth value.

		// Render primitives:
		int iPrimsToPaint 	= m_poData->m_oRPList.size();
		int iMaxLoops		= iPrimsToPaint+1;
		FEReal rDepthToPaint = _INFr;
		

		while ((iPrimsToPaint>0) && (iMaxLoops>0))
		{
			FEReal rMaxPrimDepth = _nINFr;
			CFESystem::Graphics::SetDepth(rDepthToPaint);

			for (uint i=0;(i<m_poData->m_oRPList.size() && (iPrimsToPaint>0));i++)
			{
				CRenderPrim* poPrim = &m_poData->m_oRPList[i];
			
				if (poPrim->m_rDepth == rDepthToPaint)
				{
					CFESystem::Graphics::SetTransform(poPrim->m_oMat);
					CFESystem::Graphics::SetBlendMode(poPrim->m_eBlendMode);
					CFESystem::Graphics::SetMaterial(poPrim->m_hMat);
					
					GenericRenderPrimitive(poPrim);
					iPrimsToPaint--;
				}
				else if (poPrim->m_rDepth < rDepthToPaint)
				{
					if (poPrim->m_rDepth>rMaxPrimDepth)
						rMaxPrimDepth = poPrim->m_rDepth;
				}
			}

			rDepthToPaint = rMaxPrimDepth;
			iMaxLoops--;
		}

		m_poData->m_oRPList.clear();
	}
}
// ----------------------------------------------------------------------------
void CFERenderer::FillRenderPrim(CRenderPrim* _poRP,CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs,unsigned short* _pusIdx)
{
	_poRP->m_oMat		 = m_poData->m_oCurTransf;
	_poRP->m_hMat		 = _hMat;
	_poRP->m_rDepth		 = m_poData->m_rDepth;
	_poRP->m_eBlendMode  = m_poData->m_eBlendMode;
	_poRP->m_uiRndIdx	 = m_poData->m_uiPrims;
	_poRP->m_ePT		 = (ERenderPrimType)_uiPrimType;

	if (_uiPrimType != RPT_MESH)
	{
		memcpy(_poRP->m_oVX,_poVX,sizeof(CFEVect2)*4);
		if (_poUV!=NULL) memcpy(_poRP->m_oUV,_poUV,sizeof(CFEVect2)*4);
		memcpy(_poRP->m_oVC,_poVC,sizeof(CFEColor)*4);

		_poRP->m_poVX		= _poRP->m_oVX;
		_poRP->m_poUV		= _poRP->m_oUV;
		_poRP->m_poVC		= _poRP->m_oVC;
		_poRP->m_pusIdx	= _pusIdx;
		_poRP->m_uiNumVXs	= _uiNumVXs;
	}
	else
	{
		_poRP->m_poVX		= _poVX;
		_poRP->m_poUV		= _poUV;
		_poRP->m_poVC		= _poVC;
		_poRP->m_pusIdx	= _pusIdx;
		_poRP->m_uiNumVXs	= _uiNumVXs;
	}	
}
// ----------------------------------------------------------------------------
void CFERenderer::PushRenderPrim(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs,unsigned short* _pusIdx)
{
    CRenderPrim oRP;
	FillRenderPrim(&oRP,_poVX,_poUV,_poVC,_hMat,_uiPrimType,_uiNumVXs,_pusIdx);
	m_poData->m_oRPList.push_back(oRP);
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
void CFERenderer::UpdateTransform()
{
    CFEMatrix* poMat = &m_poData->m_oStack[ m_poData->m_oStack.size()-1 ];
    m_poData->m_oCurTransf.Multiply(*poMat,m_poData->m_oCamMat);
    m_poData->m_bUpdateTransform = false;
}
// ----------------------------------------------------------------------------
inline void CFERenderer::RenderPrimitive(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,FEHandler _hMat,uint _uiPrimType,uint _uiNumVXs,unsigned short* _pusIdx)
{
	if (m_poData->m_bUpdateTransform)
		UpdateTransform();	

	// Path when rendering primitives in sorted mode.
	if (m_poData->m_bPrimitiveSorting)
	{
		PushRenderPrim(_poVX,_poUV,_poVC,_hMat,_uiPrimType,_uiNumVXs,_pusIdx);
	}

	// Path when rendering primitives in unsorted mode.
	else
	{
		CFESystem::Graphics::SetTransform(m_poData->m_oCurTransf);
		CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
		CFESystem::Graphics::SetMaterial(_hMat);
		CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

		CRenderPrim oRP;
		FillRenderPrim(&oRP,_poVX,_poUV,_poVC,_hMat,_uiPrimType,_uiNumVXs,_pusIdx);
		GenericRenderPrimitive(&oRP);
	}

	// new rendered primitive.
	m_poData->m_uiPrims++;
}
// -----------------------------------------------------------------------------
void CFERenderer::RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
	CFEVect2 oVX[4];
	oVX[0].x = _rIX;
	oVX[0].y = _rIY;
	oVX[1].x = _rFX;
	oVX[1].y = _rFY;

	RenderPrimitive(oVX,NULL,(CFEColor*)&_oColor,NULL,RPT_LINE,2,NULL);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                             const CFEColor& _oColor)
{
	CFEVect2 oVX[4] = {
		CFEVect2(_rIX,_rIY),
		CFEVect2(_rFX,_rIY),
		CFEVect2(_rFX,_rFY),
		CFEVect2(_rIX,_rFY)
	};

	RenderPrimitive(oVX,NULL,(CFEColor*)&_oColor,NULL,RPT_RECT,4,NULL);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poColor)
{
	RenderPrimitive(_poVXs,_poUVs,_poColor,(_poUVs != NULL)?m_poData->m_hMaterial:NULL,RPT_QUAD,4,NULL);
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

	RenderPrimitive(oVX,oUV,oVC,m_poData->m_hMaterial,RPT_QUAD,4,NULL);
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

    RenderPrimitive(oVX,oUV,oVC,m_poData->m_hMaterial,RPT_QUAD,4,NULL);

	// debug
    if (m_poData->m_bDebugSprites)
    {
		CFEVect2 oPivotPos = _oPivot + _oPos;

		const FEReal LINE_SIZE = _5r;
		SetBlendMode(BM_COPY);
		RenderLine(oPivotPos.x			,oPivotPos.y-LINE_SIZE	,oPivotPos.x			,oPivotPos.y+LINE_SIZE	,CFEColor(_1r,_1r,_0r,_1r));
		RenderLine(oPivotPos.x-LINE_SIZE,oPivotPos.y			,oPivotPos.x+LINE_SIZE	,oPivotPos.y			,CFEColor(_1r,_1r,_0r,_1r));

		for (uint j=0;j<4;j++)
			RenderLine(oVX[j].x,oVX[j].y,oVX[(j+1) & 0x03].x,oVX[(j+1) & 0x03].y,CFEColor(_0r,_1r,_0r,_1r));
    }
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderText(const CFEString& _sStr,FEReal _rX,FEReal _rY,const CFEColor& _oColor,EFETextHAlignmentMode _eHAlignment,EFETextVAlignmentMode _eVAlignment,FEReal _rFontPointSize)
{
    if (m_poData->m_poFont == NULL) return;							// Nothing to render with this font
    if ((_sStr.szString() == NULL) || (_sStr == "")) return;		// Nothing to render

	// Cache font object.
	CFEFont* poFont = m_poData->m_poFont;
    FEReal rLen = poFont->rStringLen(_sStr)*_rFontPointSize;

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
    FEReal rDefCharHeight = poFont->rDefCharHeight();

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

    CFEVect2 oVX[4];
    CFEVect2 oUV[4];
    CFEColor oVC[4];

    // Setup colors
    oVC[0] = oVC[1] = oVC[2] = oVC[3] = _oColor;

    FEReal rXOfs = _0r;    
    char* szStr = (char*)_sStr.szString();
    char cPrevChar = 0;
    char c = 0;

    while (*szStr)
    {
		c = *szStr;

        switch (c)
        {
            // carriage return (skip it)
            case '\r':
            break;

            // new line
            case '\n':
            {
                //
                rXOfs = _0r;
                _rY += rDefCharHeight*_rFontPointSize;
                cPrevChar = 0;
            }
            break;

            // space char
            case ' ':
            {
                //
                rXOfs += poFont->rCharWidth(' ');
                cPrevChar = 0;
            }
            break;

			// any other char: just draw it.
            default:
            {
				if (cPrevChar > 0)
				{
					// add the kerning of this character and previous one.
					rXOfs += poFont->rCharKerning(cPrevChar,c);
				}

		        const CFERect& oCharUVs = poFont->oCharUVs(c);
		        FEReal rCharWidth  = poFont->rCharWidth(c)*_rFontPointSize;
		        FEReal rCharHeight = poFont->rCharHeight(c)*_rFontPointSize;
                FEReal rCharYOfs   = poFont->rCharLeading(c)*_rFontPointSize;

                // Setup Vertexs
                FEReal rX1 = _rX + rXOfs;
                FEReal rX2 = rX1 + rCharWidth;
                FEReal rY1 = _rY + rCharYOfs;
                FEReal rY2 = rY1 + rCharHeight;

                oVX[0].x = rX1;
                oVX[0].y = rY1;
                
                oVX[1].x = rX2;
                oVX[1].y = rY1;
                
                oVX[2].x = rX2;
                oVX[2].y = rY2;

                oVX[3].x = rX1;
                oVX[3].y = rY2;

                // Setup UVs
		        oUV[0].x = oCharUVs.m_oIni.x;
                oUV[0].y = oCharUVs.m_oIni.y;

                oUV[1].x = oCharUVs.m_oEnd.x;
                oUV[1].y = oCharUVs.m_oIni.y;

                oUV[2].x = oCharUVs.m_oEnd.x;
                oUV[2].y = oCharUVs.m_oEnd.y;

                oUV[3].x = oCharUVs.m_oIni.x;
                oUV[3].y = oCharUVs.m_oEnd.y;

                rXOfs += rCharWidth;
                
				// Send new quad
                RenderPrimitive(oVX,oUV,oVC,poFont->hMaterial(),RPT_QUAD,4,NULL);

                // Store character
                cPrevChar = c;
            }
        }

        // next char in the string.		
        szStr ++;
    }
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiNumPoints)
{
	/*
	if (m_poData->m_bUpdateTransform)
	{
		UpdateTransform();
		CFESystem::Graphics::SetTransform(m_poData->m_oCurTransf);
	}

	if (_poUVs == NULL)
		CFESystem::Graphics::SetMaterial( NULL );
	else
		CFESystem::Graphics::SetMaterial( m_poData->m_hMaterial );

    CFESystem::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFESystem::Graphics::SetDepth(m_poData->m_rDepth);

    CFESystem::Graphics::RenderMesh(_pusIdx,_poVXs,_poUVs,_poVCs,_uiNumPoints);
	*/

    RenderPrimitive(_poVXs,_poUVs,_poVCs,(_poUVs != NULL)?m_poData->m_hMaterial:NULL,RPT_MESH,_uiNumPoints,_pusIdx);
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
    if (_sProperty |= "RENDERED_PRIMS")
    {
        *(uint*)_pParam = m_poData->m_uiPrims;
        return(true);
    }

else if (_sProperty |= "DEBUG_SPRITES")
    {
        *(bool*)_pParam = m_poData->m_bDebugSprites ;
        return(true);
    }
else if (_sProperty |= "PRIMITIVE_SORTING")
    {
        *(bool*)_pParam = m_poData->m_bPrimitiveSorting;
        return(true);
    }
    
	else 
    {
        return(CFESystem::Graphics::bGetProperty(_sProperty,_pParam) );
    }
}
// -----------------------------------------------------------------------------
bool CFERenderer::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty |= "DEBUG_SPRITES")
    {
        m_poData->m_bDebugSprites = (bool)_pParam;
        return(true);
    }
else if (_sProperty |= "PRIMITIVE_SORTING")
    {
    	m_poData->m_bPrimitiveSorting = (bool)_pParam;

		if (m_poData->m_bPrimitiveSorting == false)
			FlushRenderPrims();

        return(true);
    }
else if (_sProperty |= "RESERVE_PRIMITIVES")
    {
    	uint uiNumPrims = (uint)_pParam;
		m_poData->m_oRPList.reserve(uiNumPrims);

        return(true);
    }
	else 
    {
        return(CFESystem::Graphics::bSetProperty(_sProperty,_pParam) );
    }
}
// -----------------------------------------------------------------------------
