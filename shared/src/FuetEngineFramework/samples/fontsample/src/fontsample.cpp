//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "fontsample.h"		// <- To compile the functions with the external c declaration (no function name mangling)
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Default settings
//-----------------------------------------------------------------------------
CFERenderer* 	gpoRenderer = NULL;
CFEFont*    	gpoFont;
FEHandler		ghHnd = NULL;
//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
uint SCREEN_VWIDTH  = 640;
uint SCREEN_VHEIGHT = 480;

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoFont = CFEFontMgr::I()->poLoad("font");
	ghHnd   = CFEMaterialMgr::I()->hLoad("font");

	gpoRenderer->SetScreenVWidth(SCREEN_VWIDTH);
	gpoRenderer->SetScreenVHeight(SCREEN_VHEIGHT);

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
void LoopEngine()
{
    gpoRenderer->BeginScene();

        // Draw something here
        gpoRenderer->SetBlendMode(BM_ALPHA);

        gpoRenderer->SetMaterial(NULL);
        gpoRenderer->RenderQuad(0,0,SCREEN_VWIDTH,SCREEN_VHEIGHT,CFEColor::BLACK());

        gpoRenderer->SetMaterial(ghHnd);
        gpoRenderer->RenderQuad(SCREEN_VWIDTH/2,SCREEN_VHEIGHT/2,SCREEN_VWIDTH,SCREEN_VHEIGHT,CFEColor(1,1,0,1));

        gpoRenderer->SetFont(gpoFont);
        // gpoRenderer->SetTextPointSize(_2r);

        gpoRenderer->SetTextTracking(_075r);
        gpoRenderer->RenderText("SuperFontSample",SCREEN_VWIDTH*_05r,SCREEN_VHEIGHT*0.5f - 150,CFEColor(1,1,1,1),	THAM_CENTER);

        gpoRenderer->SetTextTracking(_1r);
        gpoRenderer->RenderText("ABCDEFGHI",SCREEN_VWIDTH*_05r,SCREEN_VHEIGHT*0.5f - 100,CFEColor(1,1,1,1),	THAM_CENTER);
        
		gpoRenderer->SetTextTracking(_2r);
        gpoRenderer->RenderText("0123456789",SCREEN_VWIDTH*_05r,SCREEN_VHEIGHT*0.5f - 50,CFEColor(1,1,1,1),	THAM_CENTER);

		gpoRenderer->SetTextTracking(_1r);
        gpoRenderer->RenderText("HELLO WORLD",	  SCREEN_VWIDTH*_05r,SCREEN_VHEIGHT*0.5f + 50,CFEColor(1,1,_05r,1),	THAM_CENTER);

        gpoRenderer->SetTextTracking(_2r);
        gpoRenderer->RenderText("Hello World",	  SCREEN_VWIDTH*_05r,SCREEN_VHEIGHT*0.5f + 100,CFEColor(1,1,_05r,1),	THAM_CENTER);

    gpoRenderer->EndScene();
}
//-----------------------------------------------------------------------------
