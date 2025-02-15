// -----------------------------------------------------------------------------
/*! \class CFEActionLoader
 *  \brief A base class to load actions from a config file.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFEActionLoader.h"
// -----------------------------------------------------------------------------
// Common engine actions
// -----------------------------------------------------------------------------
#include "CFEActionSleep.h"
#include "CFEActionPlaySound.h"
#include "CFEActionStopSound.h"
#include "game/script/CFEScriptVarRegistry.h"
#include "FEBasicTypes.h"
#include "support/sound/CFESoundMgr.h"

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
	FEHandler hSound = CFESoundMgr::hLoad(sSound);

	// Add this sound to the registry
	// CFEScriptVarRegistry::uiAdd(sSound,(FEPointer*)hSound);

	FEReal rVol = _oCfg.rGetReal(_sPrefix + ".Vol",_1r);
	FEReal rPan = _oCfg.rGetReal(_sPrefix + ".Pan",_05r);
	bool bLoop = _oCfg.bGetBool(_sPrefix + ".Loop",false);
	bool bWait = _oCfg.bGetBool(_sPrefix + ".Wait",false);

	CFEActionPlaySound* poAction = new CFEActionPlaySound;
	poAction->Init(hSound,SML_FX,rVol,rPan,bLoop,bWait);

	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_StopSound(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sSound = _oCfg.sGetString(_sPrefix + ".Sound",0);
	FEHandler hSound = CFESoundMgr::hLoad(sSound);

	CFEActionStopSound* poAction = new CFEActionStopSound;
	poAction->Init(hSound);

	return(poAction);
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

else if (sActionType |= "sleep")
		return( poLoadAction_Sleep(_oCfg,_sPrefix) );
	
	return(NULL);
}
// -----------------------------------------------------------------------------
