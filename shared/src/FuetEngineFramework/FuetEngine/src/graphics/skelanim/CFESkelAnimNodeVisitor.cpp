// ----------------------------------------------------------------------------
/*! \class CFESkelAnimVisitor
 *  \brief A generic class to perform a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
//-----------------------------------------------------------------------------
#include "CFESkelAnimNodeVisitor.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
//-----------------------------------------------------------------------------
void CFESkelAnimNodeVisitor::Visit(CFESkelAnimGroup* _poObj)
{
	Visit( (CFESkelAnimNode*)_poObj );

	for (uint i=0;i<_poObj->uiNumChildren();i++)
		if (_poObj->poGetChild(i) != NULL)
			_poObj->poGetChild(i)->Accept(this);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeVisitor::Visit(CFESkelAnimBone* _poObj)
{
	Visit((CFESkelAnimNode*)_poObj);

	for (uint i=0;i<_poObj->uiNumChildren();i++)
		if (_poObj->poGetChild(i) != NULL)
			_poObj->poGetChild(i)->Accept(this);
}
//-----------------------------------------------------------------------------
