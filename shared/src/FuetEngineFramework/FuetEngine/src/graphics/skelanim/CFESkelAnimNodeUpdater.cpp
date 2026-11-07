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
void CFESkelAnimNodeUpdater::Update(CFESkelAnimNode* _poNode,FEReal _rTime,FEReal _rDeltaT)
{
	CFESkelAnimNodeUpdater oUpdater(_poNode,_rTime, _rDeltaT);
	_poNode->Accept( &oUpdater );
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimGroup* _poObj)
{
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		if (_poObj->poGetChild(i)!=NULL)
			_poObj->poGetChild(i)->Accept(this);
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimBone* _poObj)
{
	for (uint i=0;i<_poObj->uiNumChildren();i++)
	{
		if (_poObj->poGetChild(i)!=NULL)
			_poObj->poGetChild(i)->Accept(this);
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimNodeUpdater::Visit(CFESkelAnimSprite* _poObj)
{
	if (_poObj->hGetSprite()!=NULL)
	{
		FEHandler hHnd = _poObj->hGetSprite();

		if (CFESpriteInstMgr::I()->rGetCurrentActionRandomStartTime(hHnd) == _0r)
			CFESpriteInstMgr::I()->SetCurrentActionTime(hHnd,m_rTime);
		else
			CFESpriteInstMgr::I()->Update(hHnd, m_rDeltaT );
	}
}
//-----------------------------------------------------------------------------
