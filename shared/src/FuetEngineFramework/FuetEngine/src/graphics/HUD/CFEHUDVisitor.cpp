// ----------------------------------------------------------------------------
/*! \class CHUDVisitor
 *  \brief A generic class to perform a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDGroup.h"
#include "CFEHUDAction.h"
//-----------------------------------------------------------------------------
void CFEHUDVisitor::Visit(CFEHUD* _poObj)
{
    for (uint i=0;i<_poObj->uiNumElements();i++)
		_poObj->poGetElement(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDVisitor::Visit(CFEHUDElement* _poObj)
{
    for (uint i=0;i<_poObj->uiNumLayers();i++)
		if (_poObj->poGetLayer(i) != NULL)
			_poObj->poGetLayer(i)->Accept(this);

    for (uint i=0;i<_poObj->uiNumActions();i++)
		if (_poObj->poGetAction(i) != NULL)
			_poObj->poGetAction(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDVisitor::Visit(CFEHUDElementAction* _poObj)
{
	for (uint i=0;i<_poObj->uiNumActions();i++)
		if (_poObj->poGetAction(i) != NULL)
			_poObj->poGetAction(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFEHUDVisitor::Visit(CFEHUDGroup* _poObj)
{
	Visit((CFEHUDObject*)_poObj);

	for (uint i=0;i<_poObj->uiNumObjs();i++)
		if (_poObj->poGetObject(i) != NULL)
			_poObj->poGetObject(i)->Accept(this);        
}
//-----------------------------------------------------------------------------
