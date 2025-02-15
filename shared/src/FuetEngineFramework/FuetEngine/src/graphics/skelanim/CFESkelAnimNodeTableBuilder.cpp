// ----------------------------------------------------------------------------
/*! \class CFESkelAnimGroup
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimNodeTableBuilder.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimBone.h"
#include "CFESkelAnimGroup.h"
//-----------------------------------------------------------------------------
CFEArray<CFESkelAnimNode*>*	CFESkelAnimNodeTableBuilder::m_poNodeTable = NULL;
//-----------------------------------------------------------------------------
void CFESkelAnimNodeTableBuilder::BuildTable(CFEArray<CFESkelAnimNode*>* _poNodeTable, CFESkelAnimNode* _poNode)
{
    CFESkelAnimNodeTableBuilder oTableBuilder;
	m_poNodeTable = _poNodeTable;

	_poNode->Accept(&oTableBuilder);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeTableBuilder::Visit(CFESkelAnimNode* _poObj)
{
	m_poNodeTable->push_back(_poObj);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeTableBuilder::Visit(CFESkelAnimBone* _poObj)
{
	m_poNodeTable->push_back(_poObj);

    if (_poObj->poGetAttachedNode() != NULL)
        _poObj->poGetAttachedNode()->Accept(this);
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeTableBuilder::Visit(CFESkelAnimGroup* _poObj)
{
	m_poNodeTable->push_back(_poObj);

	for (uint i=0;i<_poObj->uiNumObjs();i++)
        _poObj->poGetNode(i)->Accept(this);
}
//-----------------------------------------------------------------------------
