//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
CFERenderer* gpoRenderer;
FEHandler ghSpriteInst;
FEHandler ghSpriteInst2; 

float gfAngle = 0.0f;
bool gbLink = true;

// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    /*
    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
	
	gpoRenderer->SetScreenVWidth(1280);
	gpoRenderer->SetScreenVHeight(960);
	
	ghSpriteInst = CFEMaterialMgr::hLoad("key-1-A");
  */

    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
    gpoRenderer->SetScreenVWidth(1280);
    gpoRenderer->SetScreenVHeight(960);   

    CFESpriteInstMgr::I()->Init(1024);
    CFEVect2 goPos(gpoRenderer->uiGetScreenVWidth()/2,gpoRenderer->uiGetScreenVHeight()/2);

    ghSpriteInst = CFESpriteInstMgr::I()->hGetInstance("explosion");
    CFESpriteInstMgr::I()->SetPos(ghSpriteInst,goPos);
    CFESpriteInstMgr::I()->Enable(ghSpriteInst);
    CFESpriteInstMgr::I()->SetAction(ghSpriteInst,0);
    CFESpriteInstMgr::I()->SetScale(ghSpriteInst,6.0f);
    CFESpriteInstMgr::I()->SetColor(ghSpriteInst,CFEColor(1,1,1,1.0f));
    
}
// -----------------------------------------------------------------------------
// Finish Engine
// -----------------------------------------------------------------------------
void FinishEngine()
{
    gpoRenderer->Finish();
}
//-----------------------------------------------------------------------------
// Update Frame
//-----------------------------------------------------------------------------
void LoopEngine()
{
    gpoRenderer->BeginScene();
		
		gpoRenderer->SetMaterial(NULL);
        gpoRenderer->RenderQuad(0,0,gpoRenderer->uiGetScreenVWidth(),gpoRenderer->uiGetScreenVHeight(),CFEColor(0,0,0,1));

        FEReal rDeltaT = gpoRenderer->rGetDeltaT();        
        gfAngle += 0.25f*rDeltaT;

        // Draw something here
        CFESpriteInstMgr::I()->SetAngle(ghSpriteInst,gfAngle);
        CFESpriteInstMgr::I()->Update(rDeltaT);
        CFESpriteInstMgr::I()->Render(gpoRenderer);
		
    gpoRenderer->EndScene();
}
// -----------------------------------------------------------------------------
