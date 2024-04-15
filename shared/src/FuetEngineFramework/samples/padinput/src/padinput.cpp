//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "Padinput.h"		// <- To compile the functions with the external c declaration (no function name mangling)
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
uint SCREEN_VWIDTH  = 0;
uint SCREEN_VHEIGHT = 0;
CFERenderer* 	gpoRenderer = NULL;
CFEFont*    	gpoFont = NULL;
CFEInput*		gpoInput = NULL;
CFEVect2		goScrPos(0,0);

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
	// Create input
	gpoInput = new CFEInput;
	gpoInput->Init(_pParam);

    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoFont = CFEFontMgr::I()->poLoad("font");

    SCREEN_VWIDTH  = gpoRenderer->uiGetScreenVWidth();
    SCREEN_VHEIGHT = gpoRenderer->uiGetScreenVHeight();
    
    goScrPos.x = SCREEN_VWIDTH/2;
    goScrPos.y = SCREEN_VHEIGHT/2;
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
    gpoRenderer->Finish();
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
    gpoRenderer->BeginScene();

        // Draw something here
        gpoRenderer->ResetCamera();
        gpoRenderer->SetBlendMode(BM_ALPHA);
        gpoRenderer->SetMaterial(NULL);
        gpoRenderer->RenderQuad(0,0,SCREEN_VWIDTH,SCREEN_VHEIGHT,CFEColor(1,1,1,1));
        
        gpoRenderer->SetFont(gpoFont);        
        gpoRenderer->ZoomCamera(1.0);
        
		
		// Do something with the input        
        gpoInput->Update(gpoRenderer->rGetDeltaT());
        
        
        // goScrPos.
        if (gpoInput->bDown(IPB_A))
        {
			goScrPos.x = gpoInput->oCursorCoords().x;
			goScrPos.y = gpoInput->oCursorCoords().y;
        }

        EFETextHAlignmentMode eHAlign = THAM_CENTER;
        EFETextVAlignmentMode eVAlign = TVAM_CENTER;

        if (gpoInput->bPressed(IB_LEFT))
			eHAlign = THAM_LEFT;
else if (gpoInput->bPressed(IB_RIGHT))
			eHAlign = THAM_RIGHT;

        if (gpoInput->bPressed(IB_UP))
			eVAlign = TVAM_TOP;
else if (gpoInput->bPressed(IB_DOWN))
			eVAlign = TVAM_BOTTOM;
        
		char szString[256];
		sprintf_s(szString,256,"(%.1f , %.1f)",goScrPos.x,goScrPos.y);
        // gpoRenderer->RenderText("FuetEngine",goScrPos.x,goScrPos.y,CFEColor(0,0,1,1),eHAlign,eVAlign);

		gpoRenderer->RenderText(szString,goScrPos.x,goScrPos.y,CFEColor(0,0,1,1),eHAlign,eVAlign);

    gpoRenderer->EndScene();
}
//-----------------------------------------------------------------------------
