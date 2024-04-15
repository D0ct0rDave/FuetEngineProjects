// -----------------------------------------------------------------------------
/*! \class CFEActionLoader
 *  \brief A base class to load actions from a config file.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEActionLoader.h"
// -----------------------------------------------------------------------------
// Common engine actions
// -----------------------------------------------------------------------------
#include "CFEActionSleep.h"
#include "CFEActionWaitInput.h"
#include "CFESoundAction.h"
#include "CFEActionPlaySound.h"
#include "CFEActionStopSound.h"
#include "CFEActionFadeSoundLine.h"
#include "game/script/CFEScriptVarRegistry.h"
#include "FEBasicTypes.h"
#include "support/system/CFESystem.h"
#include "support/sound/CFESoundMgr.h"
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_WaitInput(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEActionWaitInput* poAction = new CFEActionWaitInput;
	poAction->Init();

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_Sleep(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	FEReal rSleepTime = _oCfg.rGetReal(_sPrefix + ".Time",0);

	CFEActionSleep* poAction = new CFEActionSleep;
	poAction->Init(rSleepTime);

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_PlaySound(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sSound = _oCfg.sGetString(_sPrefix + ".Sound",0);
	FEHandler hSound = CFESoundMgr::I()->hLoad(sSound);

	FEReal rVol   = _oCfg.rGetReal(_sPrefix + ".Vol",_1r);
	FEReal rPan   = _oCfg.rGetReal(_sPrefix + ".Pan",_05r);
	int iLine     = _oCfg.iGetInteger(_sPrefix + ".Line",3);
	FEBool bLoop  = _oCfg.bGetBool(_sPrefix + ".Loop",false);
	FEBool bWait  = _oCfg.bGetBool(_sPrefix + ".Wait",false);

	CFEActionPlaySound* poAction = new CFEActionPlaySound;
	poAction->Init(hSound,(EFESoundMixerLine)iLine,rVol,rPan,bLoop,bWait);

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_StopSound(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sSound = _oCfg.sGetString(_sPrefix + ".Sound",0);
	if (! CFESoundMgr::I()->bExists(sSound)) return(NULL);

	// Trick: Load (It was on memory, so it won't be loaded again). Then get the handle.	
	FEHandler hSound = CFESoundMgr::I()->hLoad(sSound);
	// WARNING: Reference counter has been increased!!! we'll have to decrease it!
	CFESoundMgr::I()->ReleaseResource((FEHandler*)hSound);

	CFEActionStopSound* poAction = new CFEActionStopSound;
	poAction->Init(hSound);

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_FadeSoundLine(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	int iLine     = _oCfg.iGetInteger(_sPrefix + ".Line",3);
	FEReal rVol   = _oCfg.rGetReal(_sPrefix + ".Vol",_1r);
	FEReal rTime  = _oCfg.rGetReal(_sPrefix + ".Time",_1r);
	FEBool bWait  = _oCfg.bGetBool(_sPrefix + ".Wait",false);
	 
	CFEActionFadeSoundLine* poAction = new CFEActionFadeSoundLine;
	poAction->Init((EFESoundMixerLine)iLine,rVol,rTime,bWait);

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEActionLoader::CFEActionLoader()
{
	CFESoundActionQueue::I()->Clear();
}
// -----------------------------------------------------------------------------
CFEAction* CFEActionLoader::poLoad(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sActionType = _oCfg.sGetString(_sPrefix + ".Type","none");
	
	if (sActionType |= "none")
		return(NULL);

else if (sActionType |= "playsound")
		return(poLoadAction_PlaySound(_oCfg,_sPrefix) );

else if (sActionType |= "stopsound")
		return(poLoadAction_StopSound(_oCfg,_sPrefix) );

else if (sActionType |= "fadesoundline")
		return(poLoadAction_FadeSoundLine(_oCfg,_sPrefix) );

else if (sActionType |= "sleep")
		return( poLoadAction_Sleep(_oCfg,_sPrefix) );
	
else if (sActionType |= "waitinput")
		return( poLoadAction_WaitInput(_oCfg,_sPrefix) );
else 
	CFESystem::Print("WARNING: Unknown GameAction %s\n",sActionType.szString());

	return(NULL);
}
// -----------------------------------------------------------------------------
