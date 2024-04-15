//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
CFERenderer* 	gpoRenderer = NULL;
CFEFont*    	gpoFont;
FEReal			grFPS = _0r;
//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
uint SCREEN_VWIDTH  = 0;
uint SCREEN_VHEIGHT = 0;

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoFont = CFEFontMgr::I()->poLoad("default");

	gpoRenderer->bSetProperty("PRIMITIVE_SORTING",FEPointer(true));

	gpoRenderer->SetScreenVWidth(1280);
	gpoRenderer->SetScreenVHeight(960);

    SCREEN_VWIDTH  = gpoRenderer->uiGetScreenVWidth();
    SCREEN_VHEIGHT = gpoRenderer->uiGetScreenVHeight();
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
    gpoRenderer->Finish();
}
//-----------------------------------------------------------------------------
// Name: UpdateFrame()
// Desc: Decide what needs to be blitted next, wait for flip to complete,
//       then flip the buffers.
//-----------------------------------------------------------------------------
void UpdateFrame()
{
	FEReal rNewFPS = _1r / gpoRenderer->rGetDeltaT();
    if (CFEMath::bIsFinite(rNewFPS))
		grFPS = rNewFPS;

    gpoRenderer->BeginScene();

        const uint MAX_RNDS = 1; // 00;
        
        for (uint j=0;j<MAX_RNDS;j++)
        {
	        // Draw something here
	        gpoRenderer->ResetCamera();
	        gpoRenderer->SetBlendMode(BM_ALPHA);
	        gpoRenderer->SetMaterial(NULL);

	        gpoRenderer->RenderQuad(0,0,SCREEN_VWIDTH,SCREEN_VHEIGHT,CFEColor(1,1,_05r,1));

	        gpoRenderer->SetFont(gpoFont);  
	        /// gpoRenderer->ZoomCamera(4.0f);

	        char szString[256];
	        sprintf(szString,"Test String: %.1f",grFPS);
	        
	        // sprintf(szString,"33.45",(float)(1000.0f * gpoRenderer->rGetDeltaT()) );
			/*
	        const MAX_OPS = 0; // 00;
	        for (uint i=0;i<MAX_OPS;i++)
	        {
	        	FEReal rOpA = CFEMath::rRand()*255;
	        	FEReal rOpB = CFEMath::rRand()*255;
	        	FEReal rOpC = CFEMath::rRand()*255;
	        	FEReal rOpD = CFEMath::rRand()*255;
	        	
	        	FEReal rNum = rOpA*rOpB;
	        	FEReal rDen = rOpC+rOpD;
	        	FEReal rRes = rNum / rDen;
			}        	
			*/

        	// sprintf(szString,"%.2f",(float)(1000.0f * gpoRenderer->rGetDeltaT()) );
        	gpoRenderer->SetTextPointSize(_05r);
        	gpoRenderer->RenderText(szString,0,48,CFEColor(1,0,0,1),THAM_LEFT);
        }

    gpoRenderer->EndScene();
}
//-----------------------------------------------------------------------------
