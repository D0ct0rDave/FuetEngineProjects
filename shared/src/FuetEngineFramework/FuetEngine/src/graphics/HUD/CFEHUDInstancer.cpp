// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include <utility>

#include "CFEHUDInstancer.h"

#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"
#include "CFEHUDPSys.h"

#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
#include "graphics/particlesys/CFEParticleSysInstMgr.h"
#include "types/CFEKFBFuncUtils.h"
//-----------------------------------------------------------------------------
CFEHUDInstancer::CFEHUDInstancer()
{
	m_bAutoRename		= false;
	m_bInstanceObjects	= true;

	m_poNewHUD			= NULL;
	m_poNewElement		= NULL;
    m_poNewObject		= NULL;
    m_poNewObjAction	= NULL;
    m_poNewElemAction	= NULL;
}
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDInstancer::poCreateInstance(CFEHUD* _poHUDModel,FEBool _bInstanceObjects,FEBool _bAutoRename)
{
	if (_poHUDModel == NULL) return(NULL);

	CFEHUDInstancer oInstancer;
	oInstancer.m_bInstanceObjects = _bInstanceObjects;
	oInstancer.m_bAutoRename = _bAutoRename;
	_poHUDModel->Accept(&oInstancer);

	return(oInstancer.m_poNewHUD);
}
//-----------------------------------------------------------------------------
CFEHUDElement* CFEHUDInstancer::poCreateInstance(CFEHUDElement* _poHUDElement,FEBool _bAutoRename)
{
	if (_poHUDElement == NULL) return(NULL);

	CFEHUDInstancer oInstancer;
	oInstancer.m_bAutoRename = _bAutoRename;
	_poHUDElement->Accept(&oInstancer);

	return(oInstancer.m_poNewElement);
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDInstancer::poCreateInstance(CFEHUDObject* _poHUDObject,FEBool _bAutoRename)
{
	if (_poHUDObject == NULL) return(NULL);

	CFEHUDInstancer oInstancer;
	oInstancer.m_bAutoRename = _bAutoRename;
	_poHUDObject->Accept(&oInstancer);

	return(oInstancer.m_poNewObject);
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDInstancer::poCreateInstance(CFEHUDElementAction* _poHUDAction,FEBool _bAutoRename)
{
	if (_poHUDAction == NULL) return(NULL);

	CFEHUDInstancer oInstancer;
	oInstancer.m_bAutoRename = _bAutoRename;
	_poHUDAction->Accept(&oInstancer);

	return(oInstancer.m_poNewElemAction);
}
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDInstancer::poCreateInstance(CFEHUDObjectAction* _poHUDAction,FEBool _bAutoRename)
{
	if (_poHUDAction == NULL) return(NULL);

	CFEHUDInstancer oInstancer;
	oInstancer.m_bAutoRename = _bAutoRename;
	_poHUDAction->Accept(&oInstancer);

	return(oInstancer.m_poNewObjAction);
}
//-----------------------------------------------------------------------------
std::pair<CFEHUDObject*,CFEHUDObjectAction*> CFEHUDInstancer::poCreateInstance(CFEHUDObject* _poHUDObject,CFEHUDObjectAction* _poHUDAction,FEBool _bAutoRename)
{
	if ((_poHUDAction == NULL) && (_poHUDAction == NULL))
	{
		return( std::pair<CFEHUDObject*,CFEHUDObjectAction*>((CFEHUDObject*)NULL,(CFEHUDObjectAction*)NULL) );
	}

	CFEHUDInstancer* poInstancer = new CFEHUDInstancer;
	poInstancer->m_bAutoRename = _bAutoRename;
	
	_poHUDObject->Accept(poInstancer);
	_poHUDAction->Accept(poInstancer);
	
	std::pair<CFEHUDObject*,CFEHUDObjectAction*> oPair;
	oPair.first = poInstancer->m_poNewObject;
	oPair.second = poInstancer->m_poNewObjAction;

	delete poInstancer;
	return(oPair);
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUD* _poObj)
{
	m_poNewHUD = new CFEHUD;

	for (uint i=0;i<_poObj->uiNumElements();i++)
	{
        _poObj->poGetElement(i)->Accept(this);
		m_poNewHUD->uiAddElement(m_poNewElement);
	}
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDElement* _poObj)
{
	m_poNewElement = new CFEHUDElement(m_bAutoRename?CFEString("Copy Of ")+_poObj->sGetName():_poObj->sGetName());

    for (uint i=0;i<_poObj->uiNumLayers();i++)
    {
        _poObj->poGetLayer(i)->Accept(this);
        m_poNewElement->uiAddLayer(m_poNewObject);
	}

	for (uint a=0;a<_poObj->uiNumActions();a++)
	{
		_poObj->poGetAction(a)->Accept(this);
		m_poNewElement->uiAddAction(m_poNewElemAction);
	}
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDObject* _poObj)
{
    _poObj->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::CopyCommonProperties(CFEHUDObject* _poDst,CFEHUDObject* _poSrc,FEBool _bAutoRename)
{
	/// Common properties for object
	if (_bAutoRename)
		_poDst->SetName(CFEString("Copy Of ")+_poSrc->sGetName() );

	_poDst->SetTAG( _poSrc->sGetTAG() );

	_poDst->SetIniPos( _poSrc->oGetIniPos() );
	_poDst->SetIniScale( _poSrc->oGetIniScale() );
	_poDst->SetIniAngle( _poSrc->rGetIniAngle() );
	_poDst->SetIniDepth( _poSrc->rGetIniDepth() );
	_poDst->SetIniColor( _poSrc->oGetIniColor() );	
	_poDst->SetIniAction( _poSrc->iGetIniAction() );
	_poDst->SetIniVis( _poSrc->bGetIniVis() );
	// _poDst->ShowObj( _poSrc->bIsObjVisible() );

	_poDst->SetCurPos( _poSrc->oGetCurPos() );
	_poDst->SetCurScale( _poSrc->oGetCurScale() );
	_poDst->SetCurAngle( _poSrc->rGetCurAngle() );
	_poDst->SetCurDepth( _poSrc->rGetCurDepth() );
	_poDst->SetCurColor( _poSrc->oGetCurColor() );
	_poDst->SetCurAction( _poSrc->iGetCurAction() );
	_poDst->SetCurVis( _poSrc->bGetCurVis() );
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDGroup* _poObj)
{
	CFEHUDGroup* poGroup = new CFEHUDGroup(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poGroup));

	CopyCommonProperties(poGroup,_poObj,m_bAutoRename);
	poGroup->SetDepthFact(_poObj->rGetDepthFact());

    for (uint i=0;i<_poObj->uiNumObjs();i++)
    {
        _poObj->poGetObject(i)->Accept(this);
        poGroup->uiAddObject(m_poNewObject);
	}

	m_poNewObject = poGroup;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDLabel* _poObj)
{
	CFEHUDLabel* poLabel = new CFEHUDLabel(_poObj->sGetName());	
	m_oObjPairs.push_back(CObjectPair(_poObj,poLabel));
	CopyCommonProperties(poLabel,_poObj,m_bAutoRename);

	poLabel->SetHAlignment( _poObj->eGetHAlignment() );
	poLabel->SetVAlignment( _poObj->eGetVAlignment() );
	poLabel->SetText( _poObj->sGetText() );
	poLabel->SetFont( _poObj->poGetFont() );
	
	poLabel->SetTracking( _poObj->rGetTracking() );
	poLabel->SetPointSize( _poObj->rGetPointSize() );
	poLabel->SetInterlining( _poObj->rGetInterlining() );
	poLabel->SetAdjustmentWidth( _poObj->rGetAdjustmentWidth() );
	
	m_poNewObject = poLabel;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDIcon* _poObj)
{
	CFEHUDIcon* poIcon = new CFEHUDIcon(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poIcon));
	CopyCommonProperties(poIcon,_poObj,m_bAutoRename);

	if (m_bInstanceObjects)
	{
		CFESprite* poSprite = CFESpriteInstMgr::I()->poGetSprite(_poObj->hGetIcon());
		FEHandler hNewInstance = CFESpriteInstMgr::I()->hGetInstance(poSprite);

		CFESpriteInstMgr::I()->SetAction(hNewInstance,_poObj->iGetAction());
		CFESpriteInstMgr::I()->Enable(hNewInstance);
		CFESpriteInstMgr::I()->ManageRender(hNewInstance,false);

		poIcon->SetIcon( hNewInstance );
	}
	else
		poIcon->SetIcon( _poObj->hGetIcon() );

	m_poNewObject = poIcon;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDRect* _poObj)
{
	CFEHUDRect* poRect = new CFEHUDRect(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poRect));
	CopyCommonProperties(poRect,_poObj,m_bAutoRename);

	poRect->SetWidth( _poObj->rGetWidth() );
	poRect->SetHeight( _poObj->rGetHeight() );
	poRect->SetPivot( _poObj->oGetPivot() );
	for (uint i=0;i<4;i++)
		poRect->SetCornerColor(i,_poObj->oGetCornerColor(i));

	m_poNewObject = poRect;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDShape* _poObj)
{
	CFEHUDShape* poShape = new CFEHUDShape(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poShape));
	CopyCommonProperties(poShape,_poObj,m_bAutoRename);
	
	if (m_bInstanceObjects)
	{
		CFEMesh* poMesh = CFEMeshInstMgr::I()->poGetMesh( _poObj->hGetMesh() );
		poShape->SetMesh( CFEMeshInstMgr::I()->hGetInstance( poMesh ) );
	}
	else
		poShape->SetMesh( _poObj->hGetMesh() );

	m_poNewObject = poShape;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDPSys* _poObj)
{
	CFEHUDPSys* poPSys = new CFEHUDPSys(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poPSys));
	CopyCommonProperties(poPSys,_poObj,m_bAutoRename);

	if (m_bInstanceObjects)
	{
		CFEParticleSys* poParticleSystem = CFEParticleSysInstMgr::I()->poGetParticleSys( _poObj->hGetPSys() );
		poPSys->SetPSys(  CFEParticleSysInstMgr::I()->hGetInstance( poParticleSystem ) );
	}
	else
		poPSys->SetPSys( _poObj->hGetPSys() );


	m_poNewObject = poPSys;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDElementAction* _poObj)
{
	m_poNewElemAction = new CFEHUDElementAction(_poObj->sGetName());	

	for (uint i=0;i<_poObj->uiNumActions();i++)
	{
		_poObj->poGetAction(i)->Accept(this);
		m_poNewElemAction->uiAddAction(m_poNewObjAction);
	}

	m_poNewElemAction->SetActionTime(_poObj->rGetActionTime());
	m_poNewElemAction->SetMaxActionTime(_poObj->rGetMaxActionTime());
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDObjectAction* _poObj)
{
	m_poNewObjAction = new CFEHUDObjectAction;

	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_rXFunc,_poObj->m_rXFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_rYFunc,_poObj->m_rYFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oScaleFunc,_poObj->m_oScaleFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_rAngleFunc,_poObj->m_rAngleFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_rDepthFunc,_poObj->m_rDepthFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oColorFunc,_poObj->m_oColorFunc);	
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_iActionFunc,_poObj->m_iActionFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_bVisFunc,_poObj->m_bVisFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oEventFunc,_poObj->m_oEventFunc);
	
	FEBool bFound = false;
	for (uint i=0;i<m_oObjPairs.size();i++)
	{
		if (m_oObjPairs[i].m_poOrig == _poObj->poGetHUDObject())
		{
			// assign copy object to the new action
			m_poNewObjAction->SetHUDObject( m_oObjPairs[i].m_poCopy );
			bFound = true;
			break;
		}
	}

	if (! bFound)
	{
		// just reference the same object
		m_poNewObjAction->SetHUDObject( _poObj->poGetHUDObject() );
	}
}
//-----------------------------------------------------------------------------
