//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"
#include "FuetEngineExt.h"
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
			m_poScreen = &m_oRenderer;
		};

		CFERenderer		m_oRenderer;
        CFERenderer*	m_poScreen;
        CFEInput		m_oInput;
};

static CAppGlobals* globals = NULL;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam)
{
	// Create the Renderers and initialize them.
	globals = new CAppGlobals;
	globals->m_poScreen->Init(_pParam);

	globals->m_poScreen->SetScreenVWidth(256);
	globals->m_poScreen->SetScreenVHeight(192);
	
	// Create input
	globals->m_oInput.Init(_pParam);
	
	// Initialize managers
	CFESkelAnimInstMgr::Init(64);
	CFESpriteInstMgr::Init(1024);
	
	// Initialize sound system
	CFESoundPlayer::Init(_pParam);
	
	// Initialize the game.
	CGameLoop::Init( globals->m_poScreen, &globals->m_oInput);
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
	CGameLoop::Finish();
	globals->m_oRenderer.Finish();
	globals->m_poScreen = NULL;
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
	if (globals->m_poScreen == NULL) return;

	FEReal rDeltaT = globals->m_poScreen->rGetDeltaT();
	globals->m_oInput.Update(rDeltaT);
	CFESoundPlayer::Update(rDeltaT);
	CFESoundMixer::Update(rDeltaT);
	
	CGameLoop::Loop();
}
// -----------------------------------------------------------------------------
