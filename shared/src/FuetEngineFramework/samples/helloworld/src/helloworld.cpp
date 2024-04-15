//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"	// <- To compile the functions with the external c declaration (no function name mangling)
#include "helloworld.h"
//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
CFERenderer*     gpoRender;
FEHandler        ghMat;

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    //
    gpoRender = new CFERenderer;
    gpoRender->Init(_pParam);
    gpoRender->SetScreenVWidth(1280);
    gpoRender->SetScreenVHeight(960);
	ghMat = CFEMaterialMgr::I()->poLoad("helloworld");
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine() 
{
    gpoRender->Finish();
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
    gpoRender->BeginScene();

        uint uiW = gpoRender->uiGetScreenVWidth();
        uint uiH = gpoRender->uiGetScreenVHeight();
        
        gpoRender->ResetCamera();
//        gpoRender->TranslateCamera(150,50);
        
        // Draw something here
        gpoRender->SetMaterial(ghMat);
		// gpoRender->SetMaterial(NULL);

        for (uint i=0;i<5;i++)
        {
            float rXMargin = (gpoRender->uiGetScreenVWidth()  - uiW)*0.5f;
            float rYMargin = (gpoRender->uiGetScreenVHeight() - uiH)*0.5f;
            float rX0 = rXMargin;
            float rY0 = rYMargin;
            float rX1 = gpoRender->uiGetScreenVWidth() - rXMargin;
            float rY1 = gpoRender->uiGetScreenVHeight() - rYMargin;

	        gpoRender->RenderQuad(rX0,rY0,rX1,rY1,CFEColor(1,1,1.0f / (FEReal)(i+1),1));
            uiW >>= 1;
            uiH >>= 1;        	
        }

    gpoRender->EndScene();
}
// -----------------------------------------------------------------------------
