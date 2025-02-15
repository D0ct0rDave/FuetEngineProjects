// -----------------------------------------------------------------------------
/*! \class CKFRMActionLoader
 *  \brief A base class to load actions from a config file.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CKFRMActionLoader.h"
#include "Game/CGameGlobals.h"
// -----------------------------------------------------------------------------
// To add new custom actions:
// 1.- Create the custom action class derived from CAction
// 2.- Implement Update and Reset methods of the custom action
// 3.- Implement a Init method if you need to initialize the class with specific parameters.
// 4.- Create a new poLoadAction_* procedure to create the action and parse the possible parameters.
// 5.- Link the new poLoadAction_* procedure to an specific keyword in the generic poLoad method of
// the custom ActionLoader.
// -----------------------------------------------------------------------------
// KFRM Actions
// -----------------------------------------------------------------------------
#include "CActionTest.h"
#include "CActionMoveEntity.h"
#include "CActionMoveEntityTo.h"
#include "CActionMoveEntityRelTo.h"
#include "CActionSpawnEntity.h"
#include "CActionPlayEntityAnim.h"
#include "CActionPlayHUDAnim.h"
#include "CActionPlayBGM.h"
#include "CActionStopBGM.h"
#include "CActionPerturbCamera.h"
// -----------------------------------------------------------------------------
// Action Loaders
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_Test(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CActionTest* poAction = new CActionTest;
	poAction->Init(goGameGlobals.m_poInput);
	
	return (poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_MoveEntity(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sEntity = _oCfg.sGetString(_sPrefix + ".Entity",0);	

	CFEVect2 oIPos;
	oIPos.x = _oCfg.rGetReal(_sPrefix + ".InitialPos.x",_0r);
	oIPos.y = _oCfg.rGetReal(_sPrefix + ".InitialPos.y",_0r);

	CFEVect2 oFPos;
	oFPos.x = _oCfg.rGetReal(_sPrefix + ".FinalPos.x",oIPos.x);
	oFPos.y = _oCfg.rGetReal(_sPrefix + ".FinalPos.y",oIPos.y);

	FEReal rTime = _oCfg.rGetReal(_sPrefix + ".Time",_0r);

	CActionMoveEntity* poAction = new CActionMoveEntity;
	poAction->Init(sEntity,oIPos,oFPos,rTime);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_MoveEntityTo(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sEntity = _oCfg.sGetString(_sPrefix + ".Entity",0);	

	CFEVect2 oFPos;
	oFPos.x = _oCfg.rGetReal(_sPrefix + ".FinalPos.x",_0r);
	oFPos.y = _oCfg.rGetReal(_sPrefix + ".FinalPos.y",_0r);

	FEReal rTime = _oCfg.rGetReal(_sPrefix + ".Time",_0r);

	CActionMoveEntityTo* poAction = new CActionMoveEntityTo;
	poAction->Init(sEntity,oFPos,rTime);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_MoveEntityRelTo(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sEntity = _oCfg.sGetString(_sPrefix + ".Entity",0);	

	CFEVect2 oFPos;
	oFPos.x = _oCfg.rGetReal(_sPrefix + ".FinalPos.x",_0r);
	oFPos.y = _oCfg.rGetReal(_sPrefix + ".FinalPos.y",_0r);

	FEReal rTime = _oCfg.rGetReal(_sPrefix + ".Time",_0r);

	CActionMoveEntityRelTo* poAction = new CActionMoveEntityRelTo;
	poAction->Init(sEntity,oFPos,rTime);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_SpawnEntity(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sEntity = _oCfg.sGetString(_sPrefix + ".Entity",0);

	CFEVect2 oPos;
	oPos.x = _oCfg.rGetReal(_sPrefix + ".Pos.x",0);
	oPos.y = _oCfg.rGetReal(_sPrefix + ".Pos.y",0);
	bool bLeft = _oCfg.bGetBool(_sPrefix + ".LeftSide",false);

	CActionSpawnEntity* poAction = new CActionSpawnEntity;
	poAction->Init(sEntity,oPos,bLeft);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_PlayEntityAnim(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sEntity = _oCfg.sGetString(_sPrefix + ".Entity",0);	
	CFEString sAnim = _oCfg.sGetString(_sPrefix + ".Anim",0);	

	CActionPlayEntityAnim* poAction = new CActionPlayEntityAnim;
	poAction->Init(sEntity,sAnim);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_PlayHUDAnim(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sAnim = _oCfg.sGetString(_sPrefix + ".Anim",0);	
	
	CActionPlayHUDAnim* poAction = new CActionPlayHUDAnim;
	poAction->Init(sAnim);
	return(poAction);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_StopBGM(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
    return(new CActionStopBGM);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_PlayBGM(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
    return(new CActionPlayBGM);
}
// -----------------------------------------------------------------------------
CFEAction* poLoadAction_PerturbCamera(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	FEReal rXMag = _oCfg.rGetReal(_sPrefix + ".XMagnitude",0);
	FEReal rYMag = _oCfg.rGetReal(_sPrefix + ".YMagnitude",0);
	FEReal rTime = _oCfg.rGetReal(_sPrefix + ".Time",0);

	CActionPerturbCamera* poAction = new CActionPerturbCamera;
	poAction->Init(rTime,rXMag,rYMag);
	return(poAction);
}
// -----------------------------------------------------------------------------
// Generic action loader
// -----------------------------------------------------------------------------
CFEAction* CKFRMActionLoader::poLoad(const CFEConfigFile& _oCfg,const CFEString& _sPrefix)
{
	CFEString sActionType = _oCfg.sGetString(_sPrefix + ".Type","none");
	
	if (sActionType |= "test")
		return( poLoadAction_Test(_oCfg,_sPrefix) );

else if (sActionType |= "SpawnEntity")
		return( poLoadAction_SpawnEntity(_oCfg,_sPrefix) );

else if (sActionType |= "MoveEntity")
		return( poLoadAction_MoveEntity(_oCfg,_sPrefix) );

else if (sActionType |= "MoveEntityTo")
		return( poLoadAction_MoveEntityTo(_oCfg,_sPrefix) );
		
else if (sActionType |= "MoveEntityRelTo")
		return( poLoadAction_MoveEntityRelTo(_oCfg,_sPrefix) );
		
else if (sActionType |= "PlayEntityAnim")
		return( poLoadAction_PlayEntityAnim(_oCfg,_sPrefix) );

else if (sActionType |= "PlayHUDAnim")
		return( poLoadAction_PlayHUDAnim(_oCfg,_sPrefix) );

else if (sActionType |= "StopBGM")
		return( poLoadAction_StopBGM(_oCfg,_sPrefix) );

else if (sActionType |= "PlayBGM")
		return( poLoadAction_PlayBGM(_oCfg,_sPrefix) );

else if (sActionType |= "PerturbCamera")
		return( poLoadAction_PerturbCamera(_oCfg,_sPrefix) );

else
	// Try default action loader.
	return( CFEActionLoader::poLoad(_oCfg,_sPrefix) );
}
// -----------------------------------------------------------------------------
