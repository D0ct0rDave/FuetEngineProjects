// ----------------------------------------------------------------------------
/*! \class Fuet Engine D3D Renderer Driver
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 *  \par this driver is based on the tutorials found at:
 *  \par http://www.drunkenhyena.com/
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if ((GRAPHICS == USE_DIRECT3D) && (TARGETPLATFORM == USE_WINDOWS))
// ----------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#include <D3D9.h>
#include <D3Dx9.h>

#include <stdlib.h>
#include <math.h>

#include "../CFESystem.h"

#include "../Common/CFESImageLib.h"
#include "FEBasicTypes.h"
#include "CFESGlobals.h"

// ----------------------------------------------------------------------------
typedef struct
{
    // Needed to retrieve original texture properties.
    uint                m_uiTX;
    uint                m_uiTY;
    LPDIRECT3DTEXTURE9	m_poTex;

}TMaterialHandler;

// ----------------------------------------------------------------------------
typedef struct
{
    FLOAT 	x,y,z;
    DWORD	color;

}TVertexDescriptor0;

typedef struct
{
    FLOAT 	x,y,z;
    DWORD	color; 
    FLOAT 	u,v;

}TVertexDescriptor1;
// ----------------------------------------------------------------------------
typedef struct{
	
	DWORD m_dwSize;
	DWORD m_dwFmt;
	
}TVertexFormatInfo;

const uint MAX_VFIs = 2;

static TVertexFormatInfo gsoVertexFormatInfo[MAX_VFIs] = 
{
	{ sizeof(TVertexDescriptor0), (D3DFVF_XYZ | D3DFVF_DIFFUSE)},
	{ sizeof(TVertexDescriptor1), (D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE)}
};
// ----------------------------------------------------------------------------
class CRendererHandler
{
    public:
        CRendererHandler() :	m_hDC(NULL),
								m_hWnd(NULL),
								m_hInstance(NULL),
								m_uiScrWidth(0),
								m_uiScrHeight(0)
		{
		};

        HDC						m_hDC;                      // Display Contex
        HWND					m_hWnd;                     // Window handler

		HINSTANCE				m_hInstance;
		LPDIRECT3D9				m_poD3D;
		IDirect3DDevice9*		m_poD3DDevice;
		IDirect3DVertexBuffer9*	m_poVB[MAX_VFIs];

        POINT   m_oScrOfs;                  // Screen offset
        uint    m_uiScrWidth;               // Actual Screen Width
        uint    m_uiScrHeight;              // Actual Screen Height
};

// just for global usage
static HRESULT hr;
// ----------------------------------------------------------------------------
// Function helper that handles localization of data.
// ----------------------------------------------------------------------------
HRESULT hLoadTexture(const CFEString& _sFilename,TMaterialHandler* _poMH)
{
	HRESULT hr;
	CFEString sFilename = _sFilename;
	if (CFESystem::Local::bIsAutoLocEnabled())
	{
		// try first localized version of the file.
		CFEString sLocalFilename = CFESystem::Local::sGetLocalID() + "/" + _sFilename;
		sFilename = sLocalFilename;

		// try to load a sample in memory.
		hr = D3DXCreateTextureFromFile(FESglobals.m_poCurrentRenderer->m_poD3DDevice,sLocalFilename.szString(), &_poMH->m_poTex);
	
		if (FAILED(hr))
		{		
			// try first localized version of the file.
			sLocalFilename = CFESystem::Local::sGetLocalID(LID_COMMON) + "/" + _sFilename;
			sFilename = sLocalFilename;

			// try to load a sample in memory.
			hr = D3DXCreateTextureFromFile(FESglobals.m_poCurrentRenderer->m_poD3DDevice,sLocalFilename.szString(), &_poMH->m_poTex);
		}
	}
	else
	{
		hr = D3DXCreateTextureFromFile(FESglobals.m_poCurrentRenderer->m_poD3DDevice,_sFilename.szString(), &_poMH->m_poTex);
	}
	
	// TMaterialHandler
    if (! FAILED(hr))
    {
		CFESImageInfo* poII = ImageLib::poLoadImage((char*)sFilename.szString());
		if (poII != NULL)
		{
	        _poMH->m_uiTX = poII->m_uiTX;
		    _poMH->m_uiTY = poII->m_uiTY;

			// free the resources we just want the texture info.
			ImageLib::FreeImage(poII);
		}
		else
		{
			// Retrieve texture properties from descriptor.
			// WARNING: conversion to system convenient format can mess these properties.
			D3DSURFACE_DESC oDesc;
			_poMH->m_poTex->GetLevelDesc(0,&oDesc);
			_poMH->m_uiTX = oDesc.Width;
			_poMH->m_uiTY = oDesc.Height;
		}
	}

	return(hr);
}
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Graphic functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

// Dirty trick to share textures among different rendering contexts.

FEHandler CFESystem::Graphics::hInit(FEHandler _hParam)
{
	CRendererHandler* poRnd = new CRendererHandler;
	poRnd->m_hWnd = ((HWND)_hParam);
	// poRnd->m_hDC  = GetDC(poRnd->m_hWnd);

	poRnd->m_oScrOfs.x = poRnd->m_oScrOfs.y = 0;
	ClientToScreen( poRnd->m_hWnd, &poRnd->m_oScrOfs);

	RECT oRect;
	GetClientRect(poRnd->m_hWnd,&oRect);

	poRnd->m_uiScrWidth  = oRect.right - oRect.left;
	poRnd->m_uiScrHeight = oRect.bottom - oRect.top;

	//Create Direct3D8, this is the first thing you have to do in any D3D8 program
	//Always pass D3D_SDK_VERSION to the function.
	poRnd->m_poD3D = Direct3DCreate9( D3D_SDK_VERSION );
	Check(poRnd->m_poD3D != NULL,"Error getting Direct3D");

	//Get the current(desktop) display mode.  This is really only needed if
	//we're running in a window.
	D3DDISPLAYMODE display_mode;
	hr=poRnd->m_poD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&display_mode);
	Check(! FAILED(hr),"Error getting display mode\n");

	//Clear out our D3DPRESENT_PARAMETERS structure.  Even though we're going
	//to set virtually all of it members, it's good practice to zero it out first.
	D3DPRESENT_PARAMETERS pp;  
	ZeroMemory(&pp,sizeof(D3DPRESENT_PARAMETERS));

	pp.BackBufferCount= 1;									//We only need a single back buffer
	pp.MultiSampleType=D3DMULTISAMPLE_NONE; 				//No multi-sampling
	pp.MultiSampleQuality=0;                				//No multi-sampling
	pp.SwapEffect = D3DSWAPEFFECT_COPY;   				    // Throw away previous frames, we don't need them
	pp.hDeviceWindow=poRnd->m_hWnd;							//This is our main (and only) window
	pp.Flags=0;												//No flags to set
	pp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;	//Default Refresh Rate
	pp.PresentationInterval=D3DPRESENT_INTERVAL_DEFAULT;	//Default Presentation rate
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;					//Display format
	
	// El portátil no soporta correctamente Z buffer o el formato 
	pp.EnableAutoDepthStencil = FALSE;						//We need a Z-Buffer so everything will be drawn properly
	pp.AutoDepthStencilFormat = D3DFMT_D16;


	/*
	if(is_app_fullscreen)
	{
		pp.Windowed          = FALSE;
		pp.BackBufferWidth   = 640;
		pp.BackBufferHeight  = 480;
	}
	else
	*/
	{
		pp.Windowed          = TRUE;
	}

	//After filling in our D3DPRESENT_PARAMETERS structure, we're ready to create our device.
	//Most of the options in how the device is created are set in the D3DPRESENT_PARAMETERS
	//structure.
	hr=poRnd->m_poD3D->CreateDevice(D3DADAPTER_DEFAULT, //The default adapter, on a multimonitor system
														//there can be more than one.
										
									//Use hardware acceleration rather than the software renderer
									D3DDEVTYPE_HAL,		
									//Our Window
									poRnd->m_hWnd,
									//Process vertices in software. This is slower than in hardware,
									//But will work on all graphics cards.
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									//Our D3DPRESENT_PARAMETERS structure, so it knows what we want to build
									&pp,
									//This will be set to point to the new device
									&poRnd->m_poD3DDevice);

	Check( ! FAILED(hr),"Error creating device\n");

	for (uint i=0;i<MAX_VFIs;i++)
	{
		hr=poRnd->m_poD3DDevice->CreateVertexBuffer(4*gsoVertexFormatInfo[i].m_dwSize,	// Size of memory to be allocated
																					// Number of vertices * size of a vertex
													D3DUSAGE_WRITEONLY,				// We never need to read from it so
																					// we specify write only, it's faster
													gsoVertexFormatInfo[i].m_dwFmt,	// Our custom vertex specifier (coordinates & a colour)
													D3DPOOL_MANAGED,				// Tell DirectX to manage the memory of this resource
													&poRnd->m_poVB[i],				// Pointer to our triangle, after this call
																					// It will point to a valid vertex buffer
													NULL);				
																					

		Check( ! FAILED(hr),"Error creating vertex buffer\n");
	};

	/*
	if(g_wireframe)
	{
		g_d3d_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	}
	else
	{
		g_d3d_device->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);
	}
	*/

	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,TRUE);
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ALPHAREF,0);
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATER);

	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ZENABLE,TRUE);
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ZFUNC,D3DCMP_LESSEQUAL);

	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_LIGHTING,FALSE);

	hr = poRnd->m_poD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = poRnd->m_poD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);

	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	hr = poRnd->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_MODULATE);

	// default alpha blending enabled
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	hr = poRnd->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
		
	// Set as the current renderer.	
	FESglobals.m_poCurrentRenderer = poRnd;

	return( (FEHandler)poRnd );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::Finish(FEHandler _hRenderer)
{
    if (_hRenderer == NULL) return;
	CRendererHandler* poRnd = (CRendererHandler*)_hRenderer;

	if (poRnd->m_poD3DDevice)
	{
		poRnd->m_poD3DDevice->Release();
		poRnd->m_poD3DDevice=NULL;
	}

	if (poRnd->m_poD3D)
	{
		poRnd->m_poD3D->Release();
		poRnd->m_poD3D=NULL;
	}

	delete poRnd;
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::BeginScene(FEHandler _hRenderer)
{	
    if (FESglobals.m_poCurrentRenderer != _hRenderer)
    {
        // In case the renderer is different ...
        FESglobals.m_poCurrentRenderer = (CRendererHandler*)_hRenderer;
    }

    if (FESglobals.m_poCurrentRenderer == NULL) return;
	if ( ((CRendererHandler*)_hRenderer)->m_poD3DDevice == NULL) return;

    // Clear the buffer to our new colour.
    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->Clear(0,//Number of rectangles to clear, we're clearing everything so set it to 0
                       NULL,									//Pointer to the rectangles to clear, NULL to clear whole display
                       D3DCLEAR_TARGET,							//What to clear.  We don't have a Z Buffer or Stencil Buffer
                       D3DCOLOR_XRGB(0,0,0),					//Colour to clear to
                       1.0f,									//Value to clear ZBuffer to, doesn't matter since we don't have one
                       0 );										//Stencil clear value, again, we don't have one, this value doesn't matter

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->BeginScene();
	
	
	D3DXMATRIX oViewMatrix;
	D3DXMatrixIdentity(&oViewMatrix);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_VIEW,&oViewMatrix);

	D3DXMATRIX oProjMatrix;
	D3DXMatrixIdentity(&oProjMatrix);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_PROJECTION, &oProjMatrix );

	/*
	D3DXMATRIX oWorldMatrix;
	D3DXMatrixIdentity(&oWorldMatrix);

	D3DXMATRIX oS;
	D3DXMatrixScaling(&oS,2.0f/(FEReal)FESglobals.m_uiScrVWidth,-2.0f/(FEReal)FESglobals.m_uiScrVHeight,1.0);
	D3DXMATRIX oT;
	D3DXMatrixTranslation(&oT,-1.0,1.0,0.0);
	oWorldMatrix = oS * oT;

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_WORLD,&oWorldMatrix);
    */

	D3DXMATRIX oWorldMatrix;
	
	D3DXMATRIX tmp;
	D3DXMatrixScaling(&oWorldMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&tmp, -0.5f, FESglobals.m_uiScrVHeight+0.5f, 0.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);
	D3DXMatrixOrthoOffCenterLH(&tmp, 0, (float)FESglobals.m_uiScrVWidth, 0, (float)FESglobals.m_uiScrVHeight, 0.0f, 1.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_PROJECTION,&oWorldMatrix);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::EndScene()
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

	//Notify the device that we're finished rendering for this frame
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->EndScene();

	//Show the results
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->Present(NULL,  //Source rectangle to display, NULL for all of it
                         NULL,  //Destination rectangle, NULL to fill whole display
                         NULL,  //Target window, if NULL uses device window set in CreateDevice
                         NULL );//Unused parameter, set it to NULL
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetScreenVWidth(uint _uiScrVWidth)
{
    FESglobals.m_uiScrVWidth = _uiScrVWidth;

    D3DXMATRIX oWorldMatrix;
	D3DXMATRIX tmp;
	D3DXMatrixScaling(&oWorldMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&tmp, -0.5f, FESglobals.m_uiScrVHeight+0.5f, 0.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);
	D3DXMatrixOrthoOffCenterLH(&tmp, 0, (float)FESglobals.m_uiScrVWidth, 0, (float)FESglobals.m_uiScrVHeight, 0.0f, 1.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_PROJECTION,&oWorldMatrix);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetScreenVHeight(uint _uiScrVHeight)
{
    FESglobals.m_uiScrVHeight = _uiScrVHeight;
    
    D3DXMATRIX oWorldMatrix;
	D3DXMATRIX tmp;
	D3DXMatrixScaling(&oWorldMatrix, 1.0f, -1.0f, 1.0f);
	D3DXMatrixTranslation(&tmp, -0.5f, FESglobals.m_uiScrVHeight+0.5f, 0.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);
	D3DXMatrixOrthoOffCenterLH(&tmp, 0, (float)FESglobals.m_uiScrVWidth, 0, (float)FESglobals.m_uiScrVHeight, 0.0f, 1.0f);
	D3DXMatrixMultiply(&oWorldMatrix, &oWorldMatrix, &tmp);

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_PROJECTION,&oWorldMatrix);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetTransform(const CFEMatrix& _rMat)
{
    D3DXMATRIX oViewMatrix;

	oViewMatrix._11 = _rMat.m[0][0];
	oViewMatrix._12 = _rMat.m[0][1];
	oViewMatrix._13 = 0.0f;
	oViewMatrix._14 = _rMat.m[0][2];

	oViewMatrix._21 = _rMat.m[1][0];
	oViewMatrix._22 = _rMat.m[1][1];
	oViewMatrix._23 = 0.0f;
	oViewMatrix._24 = _rMat.m[1][2];

	oViewMatrix._31 = 0.0f;
	oViewMatrix._32 = 0.0f;
	oViewMatrix._33 = 1.0f;
	oViewMatrix._34 = 0.0f;

	oViewMatrix._41 = _rMat.m[2][0];
	oViewMatrix._42 = _rMat.m[2][1];
	oViewMatrix._43 = 0.0f;
	oViewMatrix._44 = _rMat.m[2][2];

    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTransform(D3DTS_VIEW,&oViewMatrix);
}
// ----------------------------------------------------------------------------
FEHandler CFESystem::Graphics::hGetCurrentRenderer()
{
    return( (FEHandler)(FESglobals.m_poCurrentRenderer) );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::DeleteMaterial(FEHandler _hMaterial)
{
    TMaterialHandler* poTexHnd = (TMaterialHandler*)_hMaterial;
    if (poTexHnd->m_poTex != NULL)
		poTexHnd->m_poTex->Release();

    delete poTexHnd;
}
// ----------------------------------------------------------------------------
FEHandler CFESystem::Graphics::hLoadMaterial(const CFEString& _sFilename)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return(NULL);

	TMaterialHandler oTH;
    oTH.m_poTex = NULL;

	CFEString sFilename;

	sFilename = _sFilename + ".png";
	hr = hLoadTexture(sFilename,&oTH);

	if (FAILED(hr))
	{
		sFilename = _sFilename + ".tga";
		hr = hLoadTexture(sFilename,&oTH);

		if (FAILED(hr))
		{
			sFilename = _sFilename + ".bmp";
			hr = hLoadTexture(sFilename,&oTH);
			if (FAILED(hr))
				return(NULL);
		}
	}

    return( new TMaterialHandler(oTH) );
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::ReloadMaterial(FEHandler _hMaterial,const CFEString& _sFilename)
{
    TMaterialHandler* poNewTH = (TMaterialHandler*)hLoadMaterial(_sFilename);
    if (poNewTH == NULL) return;
    
    // Release previous resources.
    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;
    if (poTH->m_poTex != NULL)
		poTH->m_poTex->Release();

    poTH->m_poTex = poNewTH->m_poTex;
    poTH->m_uiTX  = poNewTH->m_uiTX;
    poTH->m_uiTY  = poNewTH->m_uiTY;

    delete poNewTH;
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetBlendMode(EFEBlendMode _eBlendMode)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

    switch (_eBlendMode)
    {
        case BM_NONE:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
        break;

        case BM_COPY:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
		break;

        case BM_ALPHA:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);
        break;

        case BM_ADD:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
        break;

        case BM_ALPHAADD:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ONE);
        break;

        case BM_MULT:
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCCOLOR);
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_DESTCOLOR);
        break;
    }
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetMaterial(FEHandler _hMaterial)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

    if (_hMaterial == NULL)
    {
		hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTexture(0,NULL);
        return;
    }

    // Material has been uploaded?
    TMaterialHandler* poTexHnd = (TMaterialHandler*)_hMaterial;

    // Bind the material
    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTexture(0,poTexHnd->m_poTex);
    
    
    
    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU,  D3DTADDRESS_MIRRORONCE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV,  D3DTADDRESS_MIRRORONCE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);



	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_MODULATE);

	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAARG2,D3DTA_DIFFUSE);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetTextureStageState(0,D3DTSS_ALPHAOP,  D3DTOP_MODULATE);   
}
// ----------------------------------------------------------------------------
void SetDepth(FEReal _rDepth)
{
	FESglobals.m_rCurDepth = _rDepth;
}
// ----------------------------------------------------------------------------
inline DWORD dwColor(const CFEColor& _oColor)
{
	// RGBA float -> BGRA DWORD !
	return(  ((uint)(_oColor.b*255.0)    )
			|((uint)(_oColor.g*255.0)<< 8)
			|((uint)(_oColor.r*255.0)<<16)
			| ((uint)(_oColor.a*255.0)<<24)
			);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderQuads(CFEVect2* _poVX,CFEVect2* _poUV,CFEColor* _poVC,uint _uiNumQuads)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

	uint uiVFI = (_poUV == NULL)?0:1;
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetFVF(gsoVertexFormatInfo[uiVFI].m_dwFmt);

	void* pVD;
	hr = FESglobals.m_poCurrentRenderer->m_poVB[uiVFI]->Lock(0,0,&pVD,0);
	Check(! FAILED(hr),"Error locking the vertex buffer");

		switch (uiVFI)
		{
			case 0:
			{
				TVertexDescriptor0* poVD = (TVertexDescriptor0*)pVD;
				for (uint i=0;i<4;i++)
				{
					// position
					poVD[i].x = _poVX[i].x;
					poVD[i].y = _poVX[i].y;
					poVD[i].z = FESglobals.m_rCurDepth;
					// poVD[i].w = 1.0;
					
					// color
					poVD[i].color = dwColor(_poVC[i]);
					
					// tex coords
				}
			}
			break;

			case 1:
			{
				TVertexDescriptor1* poVD = (TVertexDescriptor1*)pVD;
				for (uint i=0;i<4;i++)
				{
					// position
					poVD[i].x = _poVX[i].x;
					poVD[i].y = _poVX[i].y;
					poVD[i].z = 0.5f;// .FESglobals.m_rCurDepth;
					// poVD[i].w = 1.0;
					
					// color
					poVD[i].color = dwColor(_poVC[i]);
					
					// tex coords
					poVD[i].u = _poUV[i].x;
					poVD[i].v = _poUV[i].y;
				}
			}
			break;
		};
	hr = FESglobals.m_poCurrentRenderer->m_poVB[uiVFI]->Unlock();
    
//    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,pVD,gsoVertexFormatInfo[uiVFI].m_dwSize);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetStreamSource(0,FESglobals.m_poCurrentRenderer->m_poVB[uiVFI],0,gsoVertexFormatInfo[uiVFI].m_dwSize);
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,0,2);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderMesh(unsigned short* _pusIdx,CFEVect2* _poVXs,CFEVect2* _poUVs,CFEColor* _poVCs,uint _uiPoints)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;

	/*
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
			glDisableClientState(GL_COLOR_ARRAY);

		if (_poVXs)
		{
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(2, GL_FLOAT, 2*sizeof(GLfloat), _poVXs);
		}
		else
			glDisableClientState(GL_VERTEX_ARRAY);

		if (_pusIdx)
			glDrawElements(GL_TRIANGLES, _uiPoints, GL_UNSIGNED_SHORT, _pusIdx);
		else
			glDrawArrays(GL_TRIANGLES,0,_uiPoints);
    
    glPopMatrix();        
    */
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::RenderLine(const CFEVect2& _oIni,const CFEVect2& _oEnd,const CFEColor& _oColor)
{
	if (FESglobals.m_poCurrentRenderer == NULL) return;
 
    TVertexDescriptor0 oVX[2];
    oVX[0].x = _oIni.x;
    oVX[0].y = _oIni.y;
    oVX[0].z = FESglobals.m_rCurDepth;
    oVX[0].color = dwColor(_oColor);
    
    oVX[1].x = _oEnd.x;
    oVX[1].y = _oEnd.y;
    oVX[1].z = FESglobals.m_rCurDepth;
    oVX[1].color = oVX[0].color;
    
	hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->SetFVF(gsoVertexFormatInfo[0].m_dwFmt);
    hr = FESglobals.m_poCurrentRenderer->m_poD3DDevice->DrawPrimitiveUP(D3DPT_LINELIST,1,oVX,gsoVertexFormatInfo[0].m_dwSize);
}
// ----------------------------------------------------------------------------
bool CFESystem::Graphics::bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    if (_hMaterial == NULL) return(false);
    if (_pParam == NULL) return(false);

    TMaterialHandler* poTH = (TMaterialHandler*)_hMaterial;

    if (_sProperty == "DiffuseMap.Width")
    {
        *(uint*)_pParam = poTH->m_uiTX;
        return (true);
    }
else if (_sProperty == "DiffuseMap.Height")
    {
        *(uint*)_pParam = poTH->m_uiTY;
        return (true);
    }

    return(false);
}
// ----------------------------------------------------------------------------
bool CFESystem::Graphics::bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam)
{
    return(false);
}
// ----------------------------------------------------------------------------
void CFESystem::Graphics::SetDepth(FEReal _rDepth)
{

}
// ----------------------------------------------------------------------------
bool CFESystem::Graphics::bGetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
bool CFESystem::Graphics::bSetProperty(const CFEString& _sProperty,FEPointer _pParam)
{
	return(false);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
