//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"

//-----------------------------------------------------------------------------
// Local data
//-----------------------------------------------------------------------------
class CAppGlobals
{
    public:
		CAppGlobals()
		{
			m_poScreen0 = &m_oUpperScreen;
			m_poScreen1 = &m_oLowerScreen;
			m_rScr0Y = 0;
			m_rScr1Y = 480;
		}

		void SwapScreens()
		{
			CFERenderer* poAux = m_poScreen0;
			m_poScreen0 = m_poScreen1;
			m_poScreen1 = poAux;
		}
		
		CFERenderer		m_oUpperScreen;
        CFERenderer		m_oLowerScreen;

        CFERenderer*	m_poScreen0;
        CFERenderer*	m_poScreen1;
        CFEInput		m_oInput;

        CFEFont*    	m_poFont;
        FEReal			m_rScr0Y;
        FEReal			m_rScr1Y;
};

static CAppGlobals* globals;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
const uint WIDTH	= 1280;
const uint HEIGHT	= 960;

void InitEngine(void* _pParam1,void* _pParam2)
{
	// Create the Renderers and initialize them.
	globals = new CAppGlobals;

	// Create the Renderers and initialize them.
	globals->m_poScreen0->Init(_pParam1);
	globals->m_poScreen0->SetScreenVWidth(WIDTH);
	globals->m_poScreen0->SetScreenVHeight(HEIGHT);

	globals->m_poScreen1->Init(_pParam2);
	globals->m_poScreen1->SetScreenVWidth(WIDTH);
	globals->m_poScreen1->SetScreenVHeight(HEIGHT);

	// Create input
	globals->m_oInput.Init(_pParam1);

	globals->m_poFont = CFEFontMgr::I()->poLoad("font");
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
	globals->m_oUpperScreen.Finish();
	globals->m_oLowerScreen.Finish();	

	globals->m_poScreen0 = NULL;
	globals->m_poScreen1 = NULL;
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
	const FEReal rSpeed = 100;

	if (globals->m_poScreen0 == NULL) return;

	FEReal rDeltaT = globals->m_poScreen0->rGetDeltaT();
	globals->m_oInput.Update(rDeltaT);

	if (globals->m_oInput.bDown(IPB_A))
	    globals->SwapScreens();

	globals->m_poScreen0->BeginScene();
        
        globals->m_poScreen0->SetBlendMode(BM_ALPHA);

		globals->m_poScreen0->RenderQuad(0,0,WIDTH,HEIGHT,CFEColor(0.5f,0,0,1));
        globals->m_poScreen0->SetFont(globals->m_poFont);

		globals->m_rScr0Y = globals->m_rScr0Y+rDeltaT*rSpeed;
		if (globals->m_rScr0Y >= (FEReal)HEIGHT) globals->m_rScr0Y = 0;

        globals->m_poScreen0->RenderText("Red Screen!",WIDTH>>1,globals->m_rScr0Y,CFEColor(1,1,1,1),THAM_CENTER);
	globals->m_poScreen0->EndScene();
	
	globals->m_poScreen1->BeginScene();

        globals->m_poScreen1->SetBlendMode(BM_ALPHA);
		globals->m_poScreen1->RenderQuad(0,0,WIDTH,HEIGHT,CFEColor(0,0,0.5f,1));
        
        globals->m_rScr1Y = globals->m_rScr1Y-rDeltaT*rSpeed;
		if (globals->m_rScr1Y <= 0.0f) globals->m_rScr1Y = HEIGHT;

        globals->m_poScreen1->SetFont(globals->m_poFont);
        globals->m_poScreen1->RenderText("Blue Screen!",WIDTH>>1,globals->m_rScr1Y,CFEColor(1,1,1,1),THAM_CENTER);
        
	globals->m_poScreen1->EndScene();


/*
	CFERenderer* poRnd = globals->m_poScreen0;
		poRnd->BeginScene();
		poRnd->EndScene();

		poRnd = globals->m_poScreen1;

		poRnd->BeginScene();

			poRnd->ResetCamera();
			poRnd->SetMaterial(NULL);
			poRnd->RenderQuad(0,0,poRnd->uiGetScreenVWidth(),poRnd->uiGetScreenVHeight(),CFEColor(0.5f,0,0,1));
			poRnd->SetBlendMode(BM_ALPHA);
			poRnd->SetFont(globals->m_poFont);
			poRnd->RenderText("loading",1220.0,900.0,CFEColor::WHITE(),THAM_RIGHT,TVAM_BOTTOM);
			CFEScreenFX::Render(poRnd);

		poRnd->EndScene();
*/
		
	/*
	globals->m_poScreen0->BeginScene();
		globals->m_poScreen0->RenderQuad(0,0,WIDTH,HEIGHT,0,0,1,1,CFEColor(0.5f,0,0,1));
	globals->m_poScreen0->EndScene();

	globals->m_poScreen1->BeginScene();
		globals->m_poScreen1->RenderQuad(0,0,WIDTH,HEIGHT,0,0,1,1,CFEColor(0,0,0.5f,1));
	globals->m_poScreen1->EndScene();
	*/
}
// -----------------------------------------------------------------------------
/*

*/