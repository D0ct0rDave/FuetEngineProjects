//-----------------------------------------------------------------------------
// Include files
//-----------------------------------------------------------------------------
#include "FuetEngine.h"
#include "FuetEngineExt.h"
#include "Profile/CProfileMgr.h"
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

	globals->m_poScreen->SetScreenVWidth(640);
	globals->m_poScreen->SetScreenVHeight(480);
	
	// Create input
	globals->m_oInput.Init(_pParam);
	
	// Registers the profile manager.
	CProfileMgr::Register("KFRM");

	// Initialize managers
	CFESkelAnimInstMgr::Init(64);
	CFESpriteInstMgr::Init(2048);
	CFEParticleSysInstMgr::Init(32);
	CFEParticleMgr::Init(256);

	// Initialize sound system
	CFESoundPlayer::Init(_pParam);

	// Initialize the game.
	CGameLoop::Init( globals->m_poScreen, NULL, &globals->m_oInput);
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

	CFESoundMixer::Update(rDeltaT);
	CFESoundPlayer::Update(rDeltaT);	

	CGameLoop::Loop();
}
// -----------------------------------------------------------------------------
