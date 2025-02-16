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
#include "CFEHUDElemLocator.h"

#include "CFEHUDAction.h"
#include "CFEHUD.h"
#include "CFEHUDelement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDLabel.h"
#include "CFEHUDIcon.h"
#include "CFEHUDRect.h"
//-----------------------------------------------------------------------------
const uint ST_ELEMACTION = 0;
const uint ST_OBJACTION  = 1;
const uint ST_HUDELEM    = 2;
const uint ST_HUDOBJ     = 3;
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDElemLocator::poLocateElemAction(CFEHUD* _poHUDModel,const CFEString& _sName)
{
    if ((_poHUDModel == NULL) ||(_sName == "")) return(NULL);

	m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_ELEMACTION;
	_poHUDModel->Accept(this);
	return((CFEHUDElementAction*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDElemLocator::poLocateObjAction(CFEHUD* _poHUDModel,const CFEString& _sName)
{
    if ((_poHUDModel == NULL) ||(_sName == "")) return(NULL);

	m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_OBJACTION;

	_poHUDModel->Accept(this);
	return((CFEHUDObjectAction*)m_poHUDElem);
}
//-----------------------------------------------------------------------------        
CFEHUDObject* CFEHUDElemLocator::poLocateHUDObject(CFEHUD* _poHUDModel,const CFEString& _sName)
{
    if ((_poHUDModel == NULL) ||(_sName == "")) return(NULL);

	m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_HUDOBJ;

	_poHUDModel->Accept(this);
	return((CFEHUDObject*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
CFEHUDElement* CFEHUDElemLocator::poLocateHUDElement(CFEHUD* _poHUDModel,const CFEString& _sName)
{
    if ((_poHUDModel == NULL) ||(_sName == "")) return(NULL);

	m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_HUDELEM;
	
	_poHUDModel->Accept(this);
	return((CFEHUDElement*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDElemLocator::poLocateElemAction(CFEHUDElement* _poElem,const CFEString& _sName)
{
    if ((_poElem == NULL) ||(_sName == "")) return(NULL);

    m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_ELEMACTION;

    _poElem->Accept(this);
    return((CFEHUDElementAction*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
CFEHUDObjectAction* CFEHUDElemLocator::poLocateObjAction(CFEHUDElement* _poElem,const CFEString& _sName)
{
    if ((_poElem == NULL) ||(_sName == "")) return(NULL);

    m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_OBJACTION;

    _poElem->Accept(this);
    return((CFEHUDObjectAction*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDElemLocator::poLocateHUDObject(CFEHUDElement* _poElem,const CFEString& _sName)
{
    if ((_poElem == NULL) ||(_sName == "")) return(NULL);

    m_sName = _sName;
	m_poHUDElem = NULL;
	m_uiSearchType = ST_HUDOBJ;

    _poElem->Accept(this);
    return((CFEHUDObject*)m_poHUDElem);
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUD* _poObj)
{
    for (uint i=0;i<_poObj->uiNumElements();i++)
	{
		_poObj->poGetElement(i)->Accept(this);
		if (m_poHUDElem != NULL) return;
	}
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDElement* _poObj)
{
	switch(m_uiSearchType)
	{
		case  ST_HUDELEM:
		{
			if (_poObj->sGetName() == m_sName)
				m_poHUDElem = (void*)_poObj;
		}
		break;

		case  ST_HUDOBJ:
		{
			for (uint i=0;i<_poObj->uiNumLayers();i++)
			{
				_poObj->poGetLayer(i)->Accept(this);
				if (m_poHUDElem != NULL) return;
			}
		}
		break;

		case  ST_ELEMACTION:
		{
			for (uint i=0;i<_poObj->uiNumActions();i++)
			{
				_poObj->poGetAction(i)->Accept(this);
				if (m_poHUDElem != NULL) return;
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDObject* _poObj)
{
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDGroup* _poObj)
{
	if ((_poObj->sGetName() == m_sName) && (m_uiSearchType == ST_HUDOBJ))
	{
		m_poHUDElem = (void*)_poObj;
		return;		
	}

	for (uint i=0;i<_poObj->uiNumObjs();i++)
    {
        _poObj->poGetObject(i)->Accept(this);
        if (m_poHUDElem != NULL) return;
	}
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDLabel* _poObj)
{
	if ((_poObj->sGetName() == m_sName) && (m_uiSearchType == ST_HUDOBJ))
		m_poHUDElem = (void*)_poObj;
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDIcon* _poObj)
{
	if ((_poObj->sGetName() == m_sName) && (m_uiSearchType == ST_HUDOBJ))
		m_poHUDElem = (void*)_poObj;
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDRect* _poObj)
{
	if ((_poObj->sGetName() == m_sName) && (m_uiSearchType == ST_HUDOBJ))
		m_poHUDElem = (void*)_poObj;
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDElementAction* _poObj)
{
	switch(m_uiSearchType)
	{
		case ST_ELEMACTION:
		{
			if (_poObj->sGetName() == m_sName)
				m_poHUDElem = (void*)_poObj;
		}
		break;

		case ST_OBJACTION:
		{	
			for (uint i=0;i<_poObj->uiNumActions();i++)
			{
				_poObj->poGetAction(i)->Accept(this);
				if (m_poHUDElem != NULL) return;
			}
		}
		break;
	}
}
//-----------------------------------------------------------------------------
void CFEHUDElemLocator::Visit(CFEHUDObjectAction* _poObj)
{
	if ((_poObj->poGetHUDObject()) && (_poObj->poGetHUDObject()->sGetName() == m_sName) && (m_uiSearchType == ST_OBJACTION))
		m_poHUDElem = (void*)_poObj;
}
//-----------------------------------------------------------------------------
