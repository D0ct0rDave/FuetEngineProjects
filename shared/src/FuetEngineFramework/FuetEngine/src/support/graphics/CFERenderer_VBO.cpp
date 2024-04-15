// ----------------------------------------------------------------------------
/*! \class CFRenderer
 *  \brief Class for Rendering Functionalities
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
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
#include "core/CFECore.h"
#include "support/misc/CFELogger.h"
#include "graphics/font/CFEFont.h"
#include "support/misc/CFEStringUtils.h"

#include <stdlib.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

/*

#include <nn/ulcd.h>
#include <nn/gx.h>
#include <nn/gx/CTR/gx_CommandAccess.h>*/


// ----------------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <algorithm>	// for std::sort
// ----------------------------------------------------------------------------
typedef enum 
{
	RPT_NONE,

	RPT_QUAD,
	RPT_LINE,
	RPT_RECT,
	RPT_MESH,

	RPT_NUM_PRIMS

}ERenderPrimType;

// ----------------------------------------------------------------------------
// Put here things that affect rendering state
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

			return ( 0 );
		}

		bool bPrimsCompFunc(const CRenderPrim& a,const CRenderPrim& b) const
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
			return ( true );
		}

		/// Comparaison operator for sorting purposes.
		bool operator < (const CRenderPrim& b) const
		{
			return( bPrimsCompFunc(*this,b) );
		}

	public:

		CFEVect2*		m_poVX;
		CFEVect2*		m_poUV;
		CFEColor*		m_poVC;
		unsigned short*	m_pusIdx;
		uint			m_uiVXNum;			// for meshes number of VX for the rest VX num to get the vertex idx

		uint			m_uiRndIdx;			// Render Idx
		ERenderPrimType m_ePT;				// Primitive Type

		CRenderState	m_oRS;				// Render state used by this primitive
		uint			m_uiHash;			// Hash	for faster sorting
};
// ----------------------------------------------------------------------------
// Class to store data for internal use
// ----------------------------------------------------------------------------
const unsigned short INITIAL_RESERVED_PRIMS = 1024;

#define STATE_FLAG_TRANSFORM	0x01
#define STATE_FLAG_BLENDMODE	0x02
#define STATE_FLAG_MATERIAL		0x04
#define STATE_FLAG_SHADER		0x08
#define STATE_FLAG_DEPTH		0x10

const uint MAX_BUFFERS = 1;
        
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

            m_oCamPos(CFEVect2::ZERO()),
            m_rCamZoom(_0r),
            m_rCamAngle(_0r),

            m_oViewPos(CFEVect2::ZERO()),
            m_rViewAngle(_0r),

			m_usReservedPrims(INITIAL_RESERVED_PRIMS),
			m_oRPList(NULL),
			m_oVX(NULL),
			m_oUV(NULL),
			m_oVC(NULL),
			m_usIdx(NULL),

            m_uiPrims(0),
            m_bDebugSprites(false),
            m_bStereoRender(false)
        {
			m_oStartTick = CFECore::oGetEngineTicks();
            m_oCamMat.LoadIdentity();
            m_oStack.push_back(m_oCamMat);

            m_oRenderState.m_oTransf.LoadIdentity();
            m_oRenderState.m_eBlendMode = BM_NONE;
            m_oRenderState.m_hMaterial  = NULL;
		    m_oRenderState.m_rDepth		= _0r;
			m_oRenderState.m_uiShaderID = 0;
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
        CFEArray<CFEMatrix> 	m_oStack;

        CFEVect2				m_oCamPos;
        FEReal					m_rCamZoom;
        FEReal					m_rCamAngle;

		CFEVect2				m_oViewPos;
        FEReal					m_rViewAngle;

        CFEMatrix				m_oCamMat;
        
		// -------------------------------------------------------
		//	State Change Control
		// -------------------------------------------------------
		uint					m_uiDirtyFlags;

		inline void SetFlag(uint _uiFlag)
		{
			m_uiDirtyFlags |= _uiFlag;
		}

		inline void ClearFlags()
		{
			m_uiDirtyFlags = 0;
		}

		// Put here things that modify rendering state
		CRenderState	m_oRenderState;
		void UpdateTransform()
		{
			CFEMatrix* poMat = &m_oStack[ m_oStack.size()-1 ];
			m_oRenderState.m_oTransf.Multiply(*poMat,m_oCamMat);
		}

		// Hash for faster sorting
		uint						m_uiStateHash;

		// Convenient function to retrieve the primitive hashes
		inline uint uiGetStateHash()
		{
			if (m_uiDirtyFlags != 0)
			{
				if (m_uiDirtyFlags & STATE_FLAG_TRANSFORM)
				{
					UpdateTransform();
				}

				m_uiStateHash = CFEStringUtils::uiGetCRC32((const char*)&m_oRenderState,sizeof(m_oRenderState));
				ClearFlags();
			}

			return(m_uiStateHash);
		}

		// -------------------------------------------------------
		// Primitive batch processing structures
		// -------------------------------------------------------      
        unsigned short			m_usReservedPrims;
        CRenderPrim*			m_oRPList;
        uint					m_uiPrims;

		// double buffers
        FEHandler				m_hVBBufs[MAX_BUFFERS];	// vertex buffers

        CFEVect2*				m_oVXBufs[MAX_BUFFERS];
        CFEVect2*				m_oUVBufs[MAX_BUFFERS];
		CFEColor*				m_oVCBufs[MAX_BUFFERS];
		unsigned short*			m_usIdxBufs[MAX_BUFFERS];

		// pointer to current buffers.
		FEHandler				m_hVB;
        CFEVect2*				m_oVX;
        CFEVect2*				m_oUV;
		CFEColor*				m_oVC;
		unsigned short*			m_usIdx;

		class CRenderPacket
		{
			public:

				uint			m_uiStateIdx;	// index to a primitive in the primitive list that represents the render state to setup
				unsigned short*	m_pusIdx;		// pointer to the index array
				unsigned int	m_uiPrims;		// number of primitives packed
		};

		CFEArray<CRenderPacket>	m_oRPKList;

		// -------------------------------------------------------
		// Other
		// -------------------------------------------------------      
        // Debug sprites
        bool					m_bDebugSprites;
        bool					m_bStereoRender;
};
// ----------------------------------------------------------------------------
// Generic method to render primitives.
// ----------------------------------------------------------------------------
void CFERenderer::GenericRenderPrimitive(CRenderPrim* _poRP)
{
    uint uiIdx = _poRP->m_uiVXNum;
    CFEVect2* poVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poVC = &m_poData->m_oVC[uiIdx];

	switch (_poRP->m_ePT)
	{
		case RPT_LINE:
		{
			CFECore::Graphics::RenderLine(poVX[0],poVX[1],poVC[0]);
		}
		break;

		case RPT_RECT:
		{
			CFECore::Graphics::RenderLine(poVX[0],poVX[1],poVC[0]);
			CFECore::Graphics::RenderLine(poVX[1],poVX[2],poVC[0]);
			CFECore::Graphics::RenderLine(poVX[2],poVX[3],poVC[0]);
			CFECore::Graphics::RenderLine(poVX[3],poVX[0],poVC[0]);
		}
		break;

		case RPT_QUAD:
		{
			CFECore::Graphics::RenderQuads(poVX,poUV,poVC,1);
		}
		break;

		case RPT_MESH:
		{
			CFECore::Graphics::RenderMesh(_poRP->m_pusIdx,_poRP->m_poVX,_poRP->m_poUV,_poRP->m_poVC,_poRP->m_uiVXNum);
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
    m_poData->m_hHandler = CFECore::Graphics::hInit(_hParam);

    if (m_poData->m_hHandler == NULL)
    {
        CFECore::Log::Print("ERROR: Unable to initialize renderer");
    }

	//
	m_poData->m_oRPList =  new CRenderPrim[m_poData->m_usReservedPrims];
	uint uiMaxVXs = m_poData->m_usReservedPrims*4;
	
	// Initialize double buffers
	for (uint i=0;i<MAX_BUFFERS;i++)
    {
		FEHandler hVB = CFECore::Graphics::hCreateVertexBuffer(CFECore::Graphics::VB_ALL_COMP,uiMaxVXs,m_poData->m_usReservedPrims*6);

		m_poData->m_oVXBufs[i]	= (CFEVect2*)CFECore::Graphics::pGetVertexBufferPointer(hVB,CFECore::Graphics::VB_COMP_VX);
		m_poData->m_oUVBufs[i]	= (CFEVect2*)CFECore::Graphics::pGetVertexBufferPointer(hVB,CFECore::Graphics::VB_COMP_UV);
		m_poData->m_oVCBufs[i]	= (CFEColor*)CFECore::Graphics::pGetVertexBufferPointer(hVB,CFECore::Graphics::VB_COMP_VC);
		m_poData->m_usIdxBufs[i]= (unsigned short*)CFECore::Graphics::pGetVertexBufferPointer(hVB,CFECore::Graphics::VB_COMP_IDX);
		
		m_poData->m_hVBBufs[i] = hVB;
	}

	m_poData->m_hVB = m_poData->m_hVBBufs[0];
	m_poData->m_oVX = m_poData->m_oVXBufs[0];
	m_poData->m_oUV = m_poData->m_oUVBufs[0];
	m_poData->m_oVC = m_poData->m_oVCBufs[0];
	m_poData->m_usIdx = m_poData->m_usIdxBufs[0];
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

	// Delete double buffers
	for (uint i=0;i<MAX_BUFFERS;i++)
    {
		CFECore::Graphics::DestroyVertexBuffer(m_poData->m_hVBBufs[i]);
		m_poData->m_hVBBufs[i] = NULL;
	}

	if (m_poData->m_oRPList != NULL)
	{
		delete []m_poData->m_oRPList;
		m_poData->m_oRPList = NULL;
	}

    m_poData->m_oStack.clear();
}
// ----------------------------------------------------------------------------
// Sets the screen virtual width.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVWidth(uint _uiScrVWidth)
{
    m_poData->m_uiScreenVWidth = _uiScrVWidth;
    CFECore::Graphics::SetScreenVWidth(m_poData->m_uiScreenVWidth);
}
// ----------------------------------------------------------------------------
// Sets the screen virtual height.
// ----------------------------------------------------------------------------
void CFERenderer::SetScreenVHeight(uint _uiScrVHeight)
{
    m_poData->m_uiScreenVHeight = _uiScrVHeight;
    CFECore::Graphics::SetScreenVHeight(m_poData->m_uiScreenVHeight);
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
    m_poData->m_uiPrims = 0;
	
	/*
	// Perform double buffering between frames
	if (m_poData->m_oVX == m_poData->m_oVXBufs[0])
	{
		m_poData->m_hVB = m_hVBBufs[1];
		m_poData->m_oVX = m_poData->m_oVXBufs[1];
		m_poData->m_oUV = m_poData->m_oUVBufs[1];
		m_poData->m_oVC = m_poData->m_oVCBufs[1];
		m_poData->m_usIdx = m_poData->m_usIdxBufs[1];
	}
	else
	*/
	{
		m_poData->m_hVB = m_poData->m_hVBBufs[0];
		m_poData->m_oVX = m_poData->m_oVXBufs[0];
		m_poData->m_oUV = m_poData->m_oUVBufs[0];
		m_poData->m_oVC = m_poData->m_oVCBufs[0];
		m_poData->m_usIdx = m_poData->m_usIdxBufs[0];
	}

	CFECore::Graphics::BeginScene(m_poData->m_hHandler);
}
// ----------------------------------------------------------------------------
// Ends the current drawing frame
// ----------------------------------------------------------------------------
void CFERenderer::EndScene()
{
	/*
	if (m_poData->m_bStereoRender == false)
	{
		FlushRenderPrims();
		CFECore::Graphics::EndScene();
	}
	else
	{
		CFECore::Graphics::bSetProperty("Switch2LeftStereoRendering",NULL);
		FlushRenderPrims();
		CFECore::Graphics::EndScene();

		CFECore::Graphics::bSetProperty("Switch2RightStereoRendering",NULL);
		FlushRenderPrims();
		CFECore::Graphics::EndScene();
	}
	*/
	
	BuildRenderPackets();

	if (m_poData->m_bStereoRender == false)
	{
		FlushRenderPackets();
		CFECore::Graphics::EndScene();
	}
	else
	{
		CFECore::Graphics::bSetProperty("Switch2LeftStereoRendering",NULL);
		FlushRenderPackets();
		CFECore::Graphics::EndScene();

		CFECore::Graphics::bSetProperty("Switch2RightStereoRendering",NULL);
		FlushRenderPackets();
		CFECore::Graphics::EndScene();
	}
}
// ----------------------------------------------------------------------------
// Flushes the contents of the render primitive list.
void CFERenderer::BuildRenderPackets()
{
	if (m_poData->m_uiPrims > 0)
	{
		// Sort primitives:
		#ifndef DS
		std::sort(&m_poData->m_oRPList[0],&m_poData->m_oRPList[0] + m_poData->m_uiPrims);
		#else
		std::sort(m_poData->m_oRPList.begin(),m_poData->m_oRPList.end(),CRenderPrim::bPrimsCompFunc);
		// qsort(&m_poData->m_oRPList[0],m_poData->m_oRPList.size(),sizeof(CRenderPrim),CRenderPrim::iPrimsCompFunc);
		#endif
		
		// build render packets
		CFERendererData::CRenderPacket oPK;

		unsigned short* pusIdx	= m_poData->m_usIdx;
		CRenderPrim* poPrim		= m_poData->m_oRPList;

		m_poData->m_oRPKList.clear();

		for (uint i=0;i<m_poData->m_uiPrims;)
		{
/*
			pusIdx = reinterpret_cast<unsigned short*>( uiRoundUp32(reinterpret_cast<uint>(pusIdx)) );
*/
			oPK.m_pusIdx		= pusIdx;
			oPK.m_uiStateIdx	= i;
			oPK.m_uiPrims		= 0;

			uint uiHash			= poPrim->m_uiHash;
			ERenderPrimType ePT = poPrim->m_ePT;
			
			if (ePT == RPT_QUAD)
			{
				while ((poPrim->m_uiHash == uiHash) && (poPrim->m_ePT == ePT) && (i<m_poData->m_uiPrims))
				{
					uint uiIdx = poPrim->m_uiVXNum; 

					pusIdx[0] = uiIdx;
					pusIdx[1] = uiIdx+1;
					pusIdx[2] = uiIdx+2;
					pusIdx[3] = uiIdx+2;
					pusIdx[4] = uiIdx+3;
					pusIdx[5] = uiIdx+0;

					pusIdx+=6;			// next idx
					oPK.m_uiPrims++;	// one more packed primitive.
					poPrim++;			// next primitive in the list
					i++;
				}
			}
			else
			{
				while ((poPrim->m_uiHash == uiHash) && (poPrim->m_ePT == ePT) && (i<m_poData->m_uiPrims))
				{
					oPK.m_uiPrims++;	// one more packed primitive.
					poPrim++;			// next primitive in the list
					i++;
				}			
			}

			m_poData->m_oRPKList.push_back(oPK);
		}
/*

		/// Flush vertex data
		nn::gx::CTR::UpdateBuffer(m_poData->m_oVX,uiRoundUp32(m_poData->m_usReservedPrims*4*sizeof(CFEVect2)) );
		nn::gx::CTR::UpdateBuffer(m_poData->m_oUV,uiRoundUp32(m_poData->m_usReservedPrims*4*sizeof(CFEVect2)) );
		nn::gx::CTR::UpdateBuffer(m_poData->m_oVC,uiRoundUp32(m_poData->m_usReservedPrims*4*sizeof(CFEColor)) );
		nn::gx::CTR::UpdateBuffer(m_poData->m_usIdx,uiRoundUp32(m_poData->m_usReservedPrims*6 * sizeof(unsigned short)) );
*/
	}

	CFECore::Graphics::InvalidateVertexBufferData(m_poData->m_hVB,CFECore::Graphics::VB_ALL_COMP);
}
// ----------------------------------------------------------------------------
// Flushes the contents of the render primitive list.
void CFERenderer::FlushRenderPackets(bool _bClearPrimList)
{
	if (m_poData->m_oRPKList.size()>0)
	{
		// Render primitives:
		FEHandler		hMat = (FEHandler)0xffffffff;
		EFEBlendMode	eBM  = BM_MAX;
		FEReal			rZ   = -_1r;
		// wait for shader supportuint			uiShaderID = 0xffffffff;

		for (uint i=0;i<m_poData->m_oRPKList.size();i++)
		{
			CFERendererData::CRenderPacket* poRP = &m_poData->m_oRPKList[i];
			CRenderPrim* poPrim = &m_poData->m_oRPList[poRP->m_uiStateIdx];

			// ------------------------------------
			// BEGIN: Avoid unnecessary changes
			// ------------------------------------
			if (poPrim->m_oRS.m_hMaterial != hMat)
			{
				hMat = poPrim->m_oRS.m_hMaterial;
				CFECore::Graphics::SetMaterial(poPrim->m_oRS.m_hMaterial);
			}

			if (poPrim->m_oRS.m_eBlendMode != eBM)
			{
				eBM = poPrim->m_oRS.m_eBlendMode;
				CFECore::Graphics::SetBlendMode(poPrim->m_oRS.m_eBlendMode);
			}

			if (poPrim->m_oRS.m_rDepth != rZ)
			{
				rZ = poPrim->m_oRS.m_rDepth;
				CFECore::Graphics::SetDepth(poPrim->m_oRS.m_rDepth);
			}

			/*
			// wait for shader support
			if (poPrim->m_oRS.m_uiShaderID != uiShaderID)
			{
				uiShaderID = poPrim->m_oRS.m_uiShaderID;
				CFECore::Graphics::SetShader(poPrim->m_oRS.m_uiShaderID);
			}
			*/

			// Cannot compare this 
			CFECore::Graphics::SetTransform(poPrim->m_oRS.m_oTransf);

			// ------------------------------------
			// END: Avoid unnecessary changes
			// ------------------------------------
			switch (poPrim->m_ePT)
			{
				case RPT_LINE:
				{
					for (uint p=0;p<poRP->m_uiPrims;p++)
					{
						uint uiIdx = poPrim->m_uiVXNum;

						CFEVect2* poVX = &m_poData->m_oVX[uiIdx];
						CFEColor* poVC = &m_poData->m_oVC[uiIdx];
						CFECore::Graphics::RenderLine(poVX[0],poVX[1],poVC[0]);

						poPrim++;
					}
				}
				break;

				case RPT_RECT:
				{
					for (uint p=0;p<poRP->m_uiPrims;p++)
					{
						uint uiIdx = poPrim->m_uiVXNum;

						CFEVect2* poVX = &m_poData->m_oVX[uiIdx];
						CFEColor* poVC = &m_poData->m_oVC[uiIdx];

						CFECore::Graphics::RenderLine(poVX[0],poVX[1],poVC[0]);
						CFECore::Graphics::RenderLine(poVX[1],poVX[2],poVC[0]);
						CFECore::Graphics::RenderLine(poVX[2],poVX[3],poVC[0]);
						CFECore::Graphics::RenderLine(poVX[3],poVX[0],poVC[0]);

						poPrim++;
					}
				}
				break;

				case RPT_MESH:
				{
					for (uint p=0;p<poRP->m_uiPrims;p++)
					{
						CFECore::Graphics::RenderMesh(poPrim->m_pusIdx,poPrim->m_poVX,poPrim->m_poUV,poPrim->m_poVC,poPrim->m_uiVXNum);
						poPrim++;
					}
				}
				break;

				case RPT_QUAD:
				{
					/*
					uint uiIdx = poPrim->m_uiVXNum;
					for (uint p=0;p<poRP->m_uiPrims;p++)
					{
						uint uiIdx = poPrim->m_uiVXNum;
						CFEVect2* poVX = &m_poData->m_oVX[uiIdx];
						CFEColor* poVC = &m_poData->m_oVC[uiIdx];
						CFEVect2* poUV = &m_poData->m_oUV[uiIdx];

						// CFECore::Graphics::RenderQuads(poVX,poUV,poVC,1);
						poPrim++;
					
						unsigned short* pusIdx = poRP->m_pusIdx + p*6;
						CFECore::Graphics::RenderMesh(pusIdx,m_poData->m_oVX,m_poData->m_oUV,m_poData->m_oVC,6);
					}
					*/

					CFECore::Graphics::DrawVertexBuffer(m_poData->m_hVB,poRP->m_uiPrims*6,poRP->m_pusIdx-m_poData->m_usIdx);
					// CFECore::Graphics::RenderMesh(poRP->m_pusIdx,m_poData->m_oVX,m_poData->m_oUV,m_poData->m_oVC,poRP->m_uiPrims*6);
				}
				break;
			}	// end switch (poPrim->m_ePT)
		}	// end for (i<m_poData->m_oRPKList.size())
	} // end if (m_poData->m_oRPKList.size()>0)
}
// ----------------------------------------------------------------------------
void CFERenderer::FlushRenderPrims(bool _bClearPrimList)
{
	if (m_poData->m_uiPrims>0)
	{
		// This algorithm gives priority to
		//		First	Depth
		//		Then	queue arrival order
		// Sort order: descending (first greater then lower depth values)

		// NOTE: iMaxLoops management is a kind of antibug in case some prim has weird depth value.

		// Render primitives:
		uint uiPrimsToPaint = (uint)m_poData->m_uiPrims;
		uint uiMaxLoops		= uiPrimsToPaint+1;
		FEReal rDepthToPaint = _INFr;

		while ((uiPrimsToPaint>0) && (uiMaxLoops>0))
		{
			FEReal rMaxPrimDepth = _nINFr;
			CFECore::Graphics::SetDepth(rDepthToPaint);

			for (uint i=0;(i<m_poData->m_uiPrims && (uiPrimsToPaint>0));i++)
			{
				CRenderPrim* poPrim = &m_poData->m_oRPList[i];

				if (poPrim->m_oRS.m_rDepth == rDepthToPaint)
				{
					CFECore::Graphics::SetTransform(poPrim->m_oRS.m_oTransf);
					CFECore::Graphics::SetBlendMode(poPrim->m_oRS.m_eBlendMode);
					CFECore::Graphics::SetMaterial(poPrim->m_oRS.m_hMaterial);

					GenericRenderPrimitive(poPrim);
					uiPrimsToPaint--;
				}
				else if (poPrim->m_oRS.m_rDepth < rDepthToPaint)
				{
					if (poPrim->m_oRS.m_rDepth>rMaxPrimDepth)
						rMaxPrimDepth = poPrim->m_oRS.m_rDepth;
				}
			}

			rDepthToPaint = rMaxPrimDepth;
			uiMaxLoops--;
		}

		if (_bClearPrimList)
		{
			m_poData->m_uiPrims = 0;
		}
	}
}
// ----------------------------------------------------------------------------
static CRenderPrim oRP;
void CFERenderer::PushRenderPrim(uint _uiPrimType)
{
	if (m_poData->m_uiPrims>=m_poData->m_usReservedPrims) return;

	CRenderPrim* poRP	= &m_poData->m_oRPList[m_poData->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= m_poData->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash = m_poData->uiGetStateHash();
	poRP->m_oRS = m_poData->m_oRenderState;

	poRP->m_ePT			= (ERenderPrimType)_uiPrimType;
	poRP->m_uiVXNum		= m_poData->m_uiPrims*4;

	m_poData->m_uiPrims++;
}	
// ----------------------------------------------------------------------------
void CFERenderer::PushRenderPrimQuad()
{
	if (m_poData->m_uiPrims>=m_poData->m_usReservedPrims) return;

	CRenderPrim* poRP	= &m_poData->m_oRPList[m_poData->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= m_poData->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash = m_poData->uiGetStateHash();
	poRP->m_oRS = m_poData->m_oRenderState;

	poRP->m_ePT			= RPT_QUAD;
	poRP->m_uiVXNum		= m_poData->m_uiPrims*4;

	m_poData->m_uiPrims++;
}
// ----------------------------------------------------------------------------
void CFERenderer::PushRenderPrimMesh(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumVXs,unsigned short* _pusIdx)
{
	if (m_poData->m_uiPrims>=m_poData->m_usReservedPrims) return;

	CRenderPrim* poRP	= &m_poData->m_oRPList[m_poData->m_uiPrims];

	// These determine the order of rendering
	poRP->m_uiRndIdx	= m_poData->m_uiPrims;

	/// Retrieve primitive hash
	poRP->m_uiHash = m_poData->uiGetStateHash();
	poRP->m_oRS = m_poData->m_oRenderState;

	poRP->m_ePT		 	= RPT_MESH;
	poRP->m_poVX		= _poVX;
	poRP->m_poUV		= _poUV;
	poRP->m_poVC		= _poVC;
	poRP->m_pusIdx		= _pusIdx;
	poRP->m_uiVXNum		= _uiNumVXs;

	m_poData->m_uiPrims++;
}
// ----------------------------------------------------------------------------
// Sets up the blending mode for subsequent rendering operations. If blendinf is
// BM_NONE or BM_COPY, blending is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetBlendMode(EFEBlendMode _eBlendMode)
{
    m_poData->m_oRenderState.m_eBlendMode = _eBlendMode;
	m_poData->SetFlag(STATE_FLAG_BLENDMODE);
}
// ----------------------------------------------------------------------------
// Sets the current material for further operations. If material is NULL, material
// mapping is disabled.
// ----------------------------------------------------------------------------
void CFERenderer::SetMaterial(FEHandler _hMaterial)
{
    m_poData->m_oRenderState.m_hMaterial = _hMaterial;
	m_poData->SetFlag(STATE_FLAG_MATERIAL);
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
    m_poData->m_oRenderState.m_rDepth = _rDepth;
	m_poData->SetFlag(STATE_FLAG_DEPTH);
}
// -----------------------------------------------------------------------------
void CFERenderer::RenderLine(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,const CFEColor& _oColor)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

    // Set coords
    poDstVX[0].x = _rIX;
    poDstVX[0].y = _rIY;

    poDstVX[1].x = _rFX;
    poDstVX[1].y = _rFY;

	poDstVC[0] = _oColor;

	PushRenderPrim(RPT_LINE);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderRect(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                             const CFEColor& _oColor)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

    // Set coords
    poDstVX[0].x = _rIX;
    poDstVX[0].y = _rIY;

    poDstVX[1].x = _rFX;
    poDstVX[1].y = _rIY;

    poDstVX[2].x = _rFX;
    poDstVX[2].y = _rFY;

    poDstVX[3].x = _rIX;
    poDstVX[3].y = _rFY;

	poDstVC[0] = _oColor;

	PushRenderPrim(RPT_RECT);
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poColor)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

    memcpy(poDstVX,_poVXs,4*sizeof(CFEVect2));

	if (_poColor != NULL)
		memcpy(poDstVC,_poColor,4*sizeof(CFEColor));

	if (_poUVs != NULL)
		memcpy(poDstUV,_poUVs,4*sizeof(CFEVect2));

	PushRenderPrimQuad();
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(CFEVect2* _poVXs,CFEVect2* _poUVs,const CFEColor& _oColor)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

    memcpy(poDstVX,_poVXs,4*sizeof(CFEVect2));
	poDstVC[0] = poDstVC[1] = poDstVC[2] = poDstVC[3] = _oColor;

	if (_poUVs != NULL)
		memcpy(poDstUV,_poUVs,4*sizeof(CFEVect2));

	PushRenderPrimQuad();
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderQuad(FEReal _rIX,FEReal _rIY,FEReal _rFX,FEReal _rFY,
                             FEReal _rIU,FEReal _rIV,FEReal _rFU,FEReal _rFV,
                             const CFEColor& _oColor)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

    // Set coords
    poDstVX[0].x = _rIX;
    poDstVX[0].y = _rIY;

    poDstVX[1].x = _rFX;
    poDstVX[1].y = _rIY;

    poDstVX[2].x = _rFX;
    poDstVX[2].y = _rFY;

    poDstVX[3].x = _rIX;
    poDstVX[3].y = _rFY;

    // Set UVs
    poDstUV[0].x = _rIU;
    poDstUV[0].y = _rIV;

    poDstUV[1].x = _rFU;
    poDstUV[1].y = _rIV;

    poDstUV[2].x = _rFU;
    poDstUV[2].y = _rFV;

    poDstUV[3].x = _rIU;
    poDstUV[3].y = _rFV;

    // Set color
    poDstVC[0] = poDstVC[1] = poDstVC[2] = poDstVC[3] = _oColor;

	PushRenderPrimQuad();
}
// ----------------------------------------------------------------------------
void CFERenderer::RenderSprite(const CFEVect2& _oPos,const CFEVect2& _oPivot,const CFEVect2& _oScale,FEReal _rAngle,
							   const CFEVect2& _oIUV,const CFEVect2& _oFUV,
                               const CFEColor& _oColor,FEReal _rAlpha)
{
    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

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

	PushRenderPrimQuad();

	// debug
    if (m_poData->m_bDebugSprites)
    {
		CFEVect2 oPivotPos = _oPivot + _oPos;

		const FEReal LINE_SIZE = _5r;
		SetBlendMode(BM_COPY);
		RenderLine(oPivotPos.x			,oPivotPos.y-LINE_SIZE	,oPivotPos.x			,oPivotPos.y+LINE_SIZE	,CFEColor(_1r,_1r,_0r,_1r));
		RenderLine(oPivotPos.x-LINE_SIZE,oPivotPos.y			,oPivotPos.x+LINE_SIZE	,oPivotPos.y			,CFEColor(_1r,_1r,_0r,_1r));

		for (uint j=0;j<4;j++)
			RenderLine(poDstVX[j].x,poDstVX[j].y,poDstVX[(j+1) & 0x03].x,poDstVX[(j+1) & 0x03].y,CFEColor(_0r,_1r,_0r,_1r));
    }
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

	// Cache font object.
	CFEFont* poFont = m_poData->m_poFont;

	// save material
	FEHandler hCurMaterial = m_poData->m_oRenderState.m_hMaterial;
	SetMaterial(poFont->hMaterial());

    uint uiIdx = m_poData->m_uiPrims*4;
    CFEVect2* poDstVX = &m_poData->m_oVX[uiIdx];
    CFEVect2* poDstUV = &m_poData->m_oUV[uiIdx];
    CFEColor* poDstVC = &m_poData->m_oVC[uiIdx];

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
			iLen  = szEnd-szStart;
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

		unsigned char cPrevChar = 0;
		unsigned char c = 0;

		for (uint i=0;((i<(uint)iLen) && (*szStart));i++)	// even if iLen == -1 it should work
		{
			c = *szStart;

			switch (c)
			{
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
					FEReal rCharWidth  = poFont->rCharWidth(c)*m_poData->m_rTextPointSize;
					FEReal rCharHeight = poFont->rCharHeight(c)*m_poData->m_rTextPointSize;
					FEReal rCharYOfs   = poFont->rCharLeading(c)*m_poData->m_rTextPointSize;

					// Setup Vertexs
					FEReal rX1 = _rX + rXOfs;
					FEReal rX2 = rX1 + rCharWidth;
					FEReal rY1 = _rY + rCharYOfs;
					FEReal rY2 = rY1 + rCharHeight;

					poDstVX[0].x = rX1;
					poDstVX[0].y = rY1;

					poDstVX[1].x = rX2;
					poDstVX[1].y = rY1;

					poDstVX[2].x = rX2;
					poDstVX[2].y = rY2;

					poDstVX[3].x = rX1;
					poDstVX[3].y = rY2;

					// Setup UVs
					poDstUV[0].x = oCharUVs.m_oIni.x;
					poDstUV[0].y = oCharUVs.m_oIni.y;

					poDstUV[1].x = oCharUVs.m_oEnd.x;
					poDstUV[1].y = oCharUVs.m_oIni.y;

					poDstUV[2].x = oCharUVs.m_oEnd.x;
					poDstUV[2].y = oCharUVs.m_oEnd.y;

					poDstUV[3].x = oCharUVs.m_oIni.x;
					poDstUV[3].y = oCharUVs.m_oEnd.y;

					rXOfs += rCharWidth + rTrackingToAdd;

					// color
					poDstVC[0] = poDstVC[1] = poDstVC[2] = poDstVC[3] = _oColor;

					// Send new quad
					PushRenderPrimQuad();

					poDstVX+=4;
					poDstUV+=4;
					poDstVC+=4;

					// Store character
					cPrevChar = c;
				}
			}

			// next char in the string.
			szStart++;
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
void CFERenderer::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiNumPoints)
{
	/*
	if (m_poData->m_bUpdateTransform)
	{
		UpdateTransform();
		CFECore::Graphics::SetTransform(m_poData->m_oCurTransf);
	}

	if (_poUVs == NULL)
		CFECore::Graphics::SetMaterial( NULL );
	else
		CFECore::Graphics::SetMaterial( m_poData->m_hMaterial );

    CFECore::Graphics::SetBlendMode(m_poData->m_eBlendMode);
    CFECore::Graphics::SetDepth(m_poData->m_rDepth);

    CFECore::Graphics::RenderMesh(_pusIdx,_poVXs,_poUVs,_poVCs,_uiNumPoints);
	*/
	
	PushRenderPrimMesh(_poVXs,_poUVs,_poVCs,_uiNumPoints,_pusIdx);
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
    m_poData->SetFlag(STATE_FLAG_TRANSFORM);
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
    m_poData->SetFlag(STATE_FLAG_TRANSFORM);
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
		m_poData->SetFlag(STATE_FLAG_TRANSFORM);
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::TranslateTransform(FEReal _rX,FEReal _rY)
{
	if ((_rX != _0r) || (_rY != _0r))
	{
		m_poData->m_oStack[m_poData->m_oStack.size()-1].Translate(_rX,_rY);
		m_poData->SetFlag(STATE_FLAG_TRANSFORM);
	}
}
// -----------------------------------------------------------------------------
void CFERenderer::RotateTransform(FEReal _rAngle)
{
	if (_rAngle != _0r)
	{
		m_poData->m_oStack[m_poData->m_oStack.size()-1].Rotate(_rAngle);
		m_poData->SetFlag(STATE_FLAG_TRANSFORM);
	}
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

	// optimizable: can be added to the previous transform
	m_poData->m_oCamMat.Translate(-m_poData->m_oViewPos.x,-m_poData->m_oViewPos.y);

	// Camera transforms
	m_poData->m_oCamMat.Scale(m_poData->m_rCamZoom,m_poData->m_rCamZoom);
	m_poData->m_oCamMat.Translate(-m_poData->m_oCamPos.x,-m_poData->m_oCamPos.y);
	m_poData->m_oCamMat.Rotate(m_poData->m_rCamAngle);

	m_poData->SetFlag(STATE_FLAG_TRANSFORM);
}
// -----------------------------------------------------------------------------
void CFERenderer::ResetCamera()
{
	m_poData->m_oCamPos.x = _0r;
	m_poData->m_oCamPos.y = _0r;
	m_poData->m_rCamZoom  = _1r;
	m_poData->m_rCamAngle = _0r;
	UpdateViewTransform();

	m_poData->SetFlag(STATE_FLAG_TRANSFORM);
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
	m_poData->m_oRenderState.m_uiShaderID = _uiShaderID;
	m_poData->SetFlag(STATE_FLAG_SHADER);
}
// -----------------------------------------------------------------------------
uint CFERenderer::uiGetShader()
{
	return(m_poData->m_oRenderState.m_uiShaderID);
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
	else
    {
        return(CFECore::Graphics::bGetProperty(_sProperty,_pParam) );
    }
}
// -----------------------------------------------------------------------------
bool CFERenderer::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
    if (_sProperty |= "DEBUG_SPRITES")
    {
        m_poData->m_bDebugSprites = (bool)(_pParam != NULL);
        return(true);
    }
else if (_sProperty |= "RESERVE_PRIMITIVES")
    {
    	m_poData->m_usReservedPrims = (uint)_pParam;
        return(true);
    }
else if (_sProperty |= "STEREO_RENDER")
    {
		bool bSupportStereoRender = false;
		CFECore::Graphics::bGetProperty("StereoRenderingSupport",(FEPointer)&bSupportStereoRender);

		m_poData->m_bStereoRender = (bool)(_pParam != NULL) && bSupportStereoRender;

        CFECore::Graphics::bSetProperty(_sProperty,_pParam);
		return(true);
	}
	else
    {
        return(CFECore::Graphics::bSetProperty(_sProperty,_pParam) );
    }
}
// -----------------------------------------------------------------------------
