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

static CAppGlobals* globals;
// -----------------------------------------------------------------------------
// Init Engine
// -----------------------------------------------------------------------------
void InitEngine(void* _pParam1,void* _pParam2)
{
	// Create the Renderers and initialize them.
	globals = new CAppGlobals;

	// Create the Renderers and initialize them.
	globals->m_poScreen0->Init(_pParam1);
	globals->m_poScreen1->Init(_pParam2);

	globals->m_poScreen0->SetScreenVWidth(640);
	globals->m_poScreen0->SetScreenVHeight(480);
	globals->m_poScreen1->SetScreenVWidth(640);
	globals->m_poScreen1->SetScreenVHeight(480);
	
	// Create input
	globals->m_oInput.Init(_pParam1);
	
	// Registers the profile manager.
	CProfileMgr::Register("KFRM");

	// Initialize managers
	CFESkelAnimInstMgr::Init(64);
	CFESpriteInstMgr::Init(2048);
	CFEParticleSysInstMgr::Init(32);
	CFEParticleMgr::Init(256);

	// Initialize sound system
	CFESoundPlayer::Init(_pParam1);

	// Initialize the game.
	CGameLoop::Init( globals->m_poScreen0, globals->m_poScreen1, &globals->m_oInput);
}
// -----------------------------------------------------------------------------
// FinishApplication
// -----------------------------------------------------------------------------
void FinishEngine()
{
	CGameLoop::Finish();

	globals->m_oUpperScreen.Finish();
	globals->m_oLowerScreen.Finish();	

	globals->m_poScreen0 = NULL;
	globals->m_poScreen1 = NULL;
}
//-----------------------------------------------------------------------------
void LoopEngine()
{
	if (globals->m_poScreen0 == NULL) return;

	FEReal rDeltaT = globals->m_poScreen0->rGetDeltaT();
	globals->m_oInput.Update(rDeltaT);

	CFESoundMixer::Update(rDeltaT);
	CFESoundPlayer::Update(rDeltaT);	

	CGameLoop::Loop();
}
// -----------------------------------------------------------------------------
