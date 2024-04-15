//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
CFERenderer* 	gpoRenderer = NULL;
CFEFont*    	gpoFont = NULL;
CFEInput*		gpoInput = NULL;
CFEVect2		goScrPos(0,0);

float gfAngle = 0.0f;
bool gbLink = true;
const uint WIDTH = 1280;
const uint HEIGHT = 960;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
    //
    gpoRenderer = new CFERenderer;
    gpoRenderer->Init(_pParam);
	gpoRenderer->SetScreenVWidth(WIDTH);
	gpoRenderer->SetScreenVHeight(HEIGHT);

    gpoInput = new CFEInput;
    gpoInput->Init(_pParam);
	gpoInput->SetScreenVWidth(WIDTH);
	gpoInput->SetScreenVHeight(HEIGHT);

    CFESpriteInstMgr::I()->Init(256);
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

		gpoRenderer->SetMaterial(NULL);
		gpoRenderer->SetBlendMode(BM_ALPHA);
		gpoRenderer->RenderQuad(0,0,gpoRenderer->uiGetScreenVWidth(),gpoRenderer->uiGetScreenVHeight(),CFEColor(0,0,0,1));

        // Get DeltaT
        float rDeltaT = gpoRenderer->rGetDeltaT();

        //
        gfAngle += 0.25f*rDeltaT;
		gpoInput->Update(rDeltaT);

        if (gpoInput->bDown(IPB_A))
        {
			CFESpriteInstMgr::I()->Spawn("explosion",0,gpoInput->oCursorCoords(),_0r,(_075r + CFEMath::rRand()*_05r)*_2r,CFEMath::rRand()*_2PIr,CFEColor(1,1,1,1));
        }

		if (gpoInput->bDown(IB_A))
        {
			CFESpriteInstMgr::I()->Spawn("explosion",0,CFEVect2(WIDTH>>1,HEIGHT>>1),_0r,(_075r + CFEMath::rRand()*_05r)*_2r,CFEMath::rRand()*_2PIr,CFEColor(1,1,1,1));			
        }

        CFEVect2 oPos = gpoInput->oCursorCoords();
		gpoRenderer->SetMaterial(NULL);
		gpoRenderer->RenderQuad(oPos.x-10,oPos.y-10,oPos.x+10,oPos.y+10,CFEColor(0,1,0,1));

        // Draw something here
        CFESpriteInstMgr::I()->Update(rDeltaT);
        CFESpriteInstMgr::I()->Render(gpoRenderer);

    gpoRenderer->EndScene();
}
// -----------------------------------------------------------------------------
