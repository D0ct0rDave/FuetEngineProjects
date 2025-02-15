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
#include "CFESkelAnimNodeUpdater.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
FEReal	CFESkelAnimNodeUpdater::m_rTime = _0r;
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Update(CFESkelAnimNode* _poNode,FEReal _rTime)
{
	CFESkelAnimNodeUpdater oUpdater;
	m_rTime = _rTime;
	_poNode->Accept( &oUpdater );
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimGroup* _poObj)
{
	for (uint i=0;i<_poObj->uiNumObjs();i++)
	{
		if (_poObj->poGetNode(i)!=NULL)
			_poObj->poGetNode(i)->Accept(this);
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimBone* _poObj)
{
	if (_poObj->poGetAttachedNode()!=NULL)
		_poObj->poGetAttachedNode()->Accept(this);	
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimSprite* _poObj)
{
	if (_poObj->hGetSprite()!=NULL)
		CFESpriteInstMgr::SetCurrentActionTime(_poObj->hGetSprite(),m_rTime);
}
//-----------------------------------------------------------------------------
