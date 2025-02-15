//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"
#include "FuetEngineExt.h"
#include "Game/Player/CPlayer.h"
#include "Game/CGameGlobals.h"
#include "Game/CGameLoop.h"
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
};

static CAppGlobals globals;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam1,void* _pParam2)
{
	// Create the Renderers and initialize them.
	globals.m_poScreen0->Init(_pParam1);
	globals.m_poScreen1->Init(_pParam2);

	globals.m_poScreen0->SetScreenVWidth(256);
	globals.m_poScreen0->SetScreenVHeight(192);
	globals.m_poScreen1->SetScreenVWidth(256);
	globals.m_poScreen1->SetScreenVHeight(192);
	
	// Create input
	globals.m_oInput.Init(_pParam1);
	
	// Initialize managers
	CFESkelAnimInstMgr::Init(64);
	CFESpriteInstMgr::Init(1024);
	
	// Initialize the game.
	CGameLoop::Init( globals.m_poScreen0, &globals.m_oInput);
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
	CGameLoop::Finish();

	globals.m_oUpperScreen.Finish();
	globals.m_oLowerScreen.Finish();	

	globals.m_poScreen0 = NULL;
	globals.m_poScreen1 = NULL;
}
//-----------------------------------------------------------------------------
void Update(FEReal _rDeltaT)
{
}
//-----------------------------------------------------------------------------
void Render()
{
	globals.m_poScreen1->BeginScene();
	
	    globals.m_poScreen0->SetMaterial(NULL);

		globals.m_poScreen1->RenderQuad(0,0,globals.m_poScreen1->uiGetScreenVWidth(),globals.m_poScreen1->uiGetScreenVHeight(),0,0,1,1,CFEColor(0.0,0.0,0.25,1.0));
		globals.m_poScreen0->RenderRect(goGameGlobals.m_poCamera->oGetPos().x-5 + 100,goGameGlobals.m_poCamera->oGetPos().y-5 + 100,goGameGlobals.m_poCamera->oGetPos().x+5 + 100,goGameGlobals.m_poCamera->oGetPos().y+5 + 100,CFEColor(1,1,1,1));
		
	globals.m_poScreen1->EndScene();	
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
	if (globals.m_poScreen0 == NULL) return;

	FEReal rDeltaT = globals.m_poScreen0->rGetDeltaT();
	globals.m_oInput.Update(rDeltaT);
	CGameLoop::Loop();

	// 
	Update(rDeltaT);
	Render();
}
// -----------------------------------------------------------------------------
