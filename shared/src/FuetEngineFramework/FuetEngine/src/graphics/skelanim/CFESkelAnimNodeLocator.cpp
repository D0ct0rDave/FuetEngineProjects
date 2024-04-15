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
#include "CFESkelAnimNodeLocator.h"

#include "CFESkelAnimNode.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
//-----------------------------------------------------------------------------
CFESkelAnimNode*    CFESkelAnimNodeLocator::m_poNode = NULL;
CFEString	        CFESkelAnimNodeLocator::m_sName;
//-----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimNodeLocator::poLocateNode(CFESkelAnimNode* _poNode,const CFEString& _sName)
{
    CFESkelAnimNodeLocator oLocator;

	m_sName = _sName;
	m_poNode = NULL;
	_poNode->Accept( &oLocator );

	return(m_poNode);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeLocator::Visit(CFESkelAnimNode* _poObj)
{
	if (_poObj->sGetName() == m_sName)
		m_poNode = _poObj;
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeLocator::Visit(CFESkelAnimBone* _poObj)
{
	if (_poObj->sGetName() == m_sName)
	{
		m_poNode = _poObj;
		return;
	}

	for (uint i=0;i<_poObj->uiNumChildren();i++)
    {
        _poObj->poGetChild(i)->Accept(this);
        if (m_poNode != NULL) return;
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeLocator::Visit(CFESkelAnimGroup* _poObj)
{
	if (_poObj->sGetName() == m_sName)
	{
		m_poNode = _poObj;
		return;
	}

	for (uint i=0;i<_poObj->uiNumChildren();i++)
    {
        _poObj->poGetChild(i)->Accept(this);
        if (m_poNode != NULL) return;
	}
}
//-----------------------------------------------------------------------------
