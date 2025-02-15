// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDInstancer.h"

#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
#include "CFEHUDShape.h"

#include "graphics/sprite/CFESpriteInstMgr.h"
#include "types/CFEKFBFuncUtils.h"
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDInstancer::poCreateInstance(CFEHUD* _poHUDModel,bool _bAutoRename)
{
    // Temporary
    return( _poHUDModel );
}
//-----------------------------------------------------------------------------
CFEHUDElement* CFEHUDInstancer::poCreateInstance(CFEHUDElement* _poHUDElement,bool _bAutoRename)
{
	CFEHUDInstancer* poInstancer = new CFEHUDInstancer;
	poInstancer->m_bAutoRename = _bAutoRename;
	_poHUDElement->Accept(poInstancer);

	CFEHUDElement* poElem = poInstancer->m_poNewElement;
	delete poInstancer;

	return(poElem);
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDInstancer::poCreateInstance(CFEHUDObject* _poHUDObject,bool _bAutoRename)
{
	CFEHUDInstancer* poInstancer = new CFEHUDInstancer;
	poInstancer->m_bAutoRename = _bAutoRename;
	_poHUDObject->Accept(poInstancer);

	CFEHUDObject* poObj = poInstancer->m_poNewObject;
	delete poInstancer;

	return(poObj);
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDInstancer::poCreateInstance(CFEHUDElementAction* _poHUDAction,bool _bAutoRename)
{
	CFEHUDInstancer* poInstancer = new CFEHUDInstancer;
	poInstancer->m_bAutoRename = _bAutoRename;
	_poHUDAction->Accept(poInstancer);

	CFEHUDElementAction* poAction = poInstancer->m_poNewElemAction;
	delete poInstancer;

	return(poAction);
}
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDInstancer::poCreateInstance(CFEHUDObjectAction* _poHUDAction,bool _bAutoRename)
{
	CFEHUDInstancer* poInstancer = new CFEHUDInstancer;
	poInstancer->m_bAutoRename = _bAutoRename;
	_poHUDAction->Accept(poInstancer);

	CFEHUDObjectAction* poAction = poInstancer->m_poNewObjAction;
	delete poInstancer;

	return(poAction);
}
//-----------------------------------------------------------------------------
std::pair<CFEHUDObject*,CFEHUDObjectAction*> CFEHUDInstancer::poCreateInstance(CFEHUDObject* _poHUDObject,CFEHUDObjectAction* _poHUDAction,bool _bAutoRename)
{
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
	/*
    for (uint i=0;i<_poObj->uiNumElements();i++)
        _poObj->poGetElement(i)->Accept(this);
	*/
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
void CFEHUDInstancer::CopyCommonProperties(CFEHUDObject* _poDst,CFEHUDObject* _poSrc)
{
	/// Common properties for object
	if (m_bAutoRename)
		_poDst->SetName(CFEString("Copy Of ")+_poSrc->sGetName() );
	
	_poDst->SetIniPos( _poSrc->oGetIniPos() );
	_poDst->SetIniScale( _poSrc->oGetIniScale() );
	_poDst->SetIniAngle( _poSrc->rGetIniAngle() );
	_poDst->SetIniColor( _poSrc->oGetIniColor() );
	_poDst->SetIniAction( _poSrc->iGetIniAction() );

	_poDst->SetCurPos( _poSrc->oGetCurPos() );
	_poDst->SetCurScale( _poSrc->oGetCurScale() );
	_poDst->SetCurAngle( _poSrc->rGetCurAngle() );
	_poDst->SetCurColor( _poSrc->oGetCurColor() );
	_poDst->SetCurAction( _poSrc->iGetCurAction() );
	
	_poDst->SetDepth( _poSrc->rGetDepth() );
	_poDst->Show( _poSrc->bIsVisible() );
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDGroup* _poObj)
{
	CFEHUDGroup* poGroup = new CFEHUDGroup(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poGroup));

	CopyCommonProperties(poGroup,_poObj);

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
	CopyCommonProperties(poLabel,_poObj);

	poLabel->SetHAlignment( _poObj->eGetHAlignment() );
	poLabel->SetVAlignment( _poObj->eGetVAlignment() );
	poLabel->SetText( _poObj->sGetText() );
	poLabel->SetFont( _poObj->poGetFont() );

	m_poNewObject = poLabel;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDIcon* _poObj)
{
	CFEHUDIcon* poIcon = new CFEHUDIcon(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poIcon));
	CopyCommonProperties(poIcon,_poObj);

	CFESprite* poSprite =CFESpriteInstMgr::poGetSprite(_poObj->hGetIcon());

	poIcon->SetIcon( CFESpriteInstMgr::hGetInstance(poSprite) );
	m_poNewObject = poIcon;
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDRect* _poObj)
{
	CFEHUDRect* poRect = new CFEHUDRect(_poObj->sGetName());
	m_oObjPairs.push_back(CObjectPair(_poObj,poRect));
	CopyCommonProperties(poRect,_poObj);

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
	CopyCommonProperties(poShape,_poObj);

	m_poNewObject = poShape;
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
}
//-----------------------------------------------------------------------------
void CFEHUDInstancer::Visit(CFEHUDObjectAction* _poObj)
{
	m_poNewObjAction = new CFEHUDObjectAction;

	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oPosFunc,_poObj->m_oPosFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oScaleFunc,_poObj->m_oScaleFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_rAngleFunc,_poObj->m_rAngleFunc);
	CFEKFBFuncUtils::CopyKeyframedFunc(m_poNewObjAction->m_oColorFunc,_poObj->m_oColorFunc);
	m_poNewObjAction->SetObjectAction( _poObj->iGetObjectAction() );

	bool bFound = false;
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
