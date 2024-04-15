// ----------------------------------------------------------------------------
/*! \class CFESkelAnimUpdater
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimUtils.h"

#include "CFESkelAnim.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimMesh.h"
#include "CFESkelAnimBone.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimSpriteModel.h"
#include "CFESkelAnimMeshModel.h"

#include "CFESkelAnim.h"

#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESprite.h"

#include "types/FEKFBFLerpFuncs.h"
// ----------------------------------------------------------------------------		
CFESkelAnimNodeTypeGetter::CFESkelAnimNodeTypeGetter() :  m_eType(SANT_NONE)
{

}
// ----------------------------------------------------------------------------
EFESkelAnimNodeType CFESkelAnimNodeTypeGetter::eGetType(CFESkelAnimNode* _poNode)
{
	CFESkelAnimNodeTypeGetter oObj;
	oObj.m_eType = SANT_NONE;

	_poNode->Accept(&oObj);

	return(oObj.m_eType);
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimGroup* _poNode)
{
	m_eType = SANT_GROUP;
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimMeshModel* _poNode)
{
	m_eType = SANT_MESH_MODEL;
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimMesh* _poNode)
{
	m_eType = SANT_MESH;
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimSpriteModel* _poNode)
{
	m_eType = SANT_SPRITE_MODEL;
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimSprite* _poNode)
{
	m_eType = SANT_SPRITE;	
}

void CFESkelAnimNodeTypeGetter::Visit(CFESkelAnimBone* _poNode)
{
	m_eType = SANT_BONE;
}
//-----------------------------------------------------------------------------
ESAAPlayMode CFESkelAnimUtils::eGetActionPlayMode(const CFEString& _sPlayMode)
{
    if (_sPlayMode |= "Loop")
        return(SAAPM_LOOP);

else if (_sPlayMode |= "PingPong")
        return(SAAPM_PINGPONG);

else if (_sPlayMode |= "FinalValue")
        return(SAAPM_ONESHOT);

	return(SAAPM_ONESHOT);
}
//-----------------------------------------------------------------------------
class CFESkelAnimNodeActionTimeGetter : public CFESkelAnimNodeVisitor
{
	public:

		static FEReal rGetTime(CFESkelAnimNode* _poNode,unsigned int _uiAction,FEBool _bMaxTime)
		{
			CFESkelAnimNodeActionTimeGetter oObj;
			oObj.m_uiAction	= _uiAction;
			oObj.m_rMaxTime	= -_1r;
			oObj.m_rTime	= _0r;
			oObj.m_bMaxTime = _bMaxTime;
			
			_poNode->Accept(&oObj);
			return(oObj.m_rMaxTime);
		}

		/*
		virtual void Visit(CFESkelAnimSprite* _poNode)
		{
			CFESprite* poSprite = CFESpriteInstMgr::poGetSprite( _poNode->hGetSprite() );

			if ((poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_LOOP) && (poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_PINGPONG))
				m_fTime = poSprite->poGetAction(m_uiAction)->m_rActionTime;
		}
        */

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

		virtual void Visit(CFESkelAnimGroup* _poNode)
		{
			for (uint i=0;i<_poNode->uiNumChildren();i++)
			{
				
				CFESkelAnimNode* poChildNode = _poNode->poGetChild(i);
				if (poChildNode != NULL)
					poChildNode->Accept(this);
			}
		}

		virtual void Visit(CFESkelAnimBone* _poNode)
		{
			for (uint i=0;i<_poNode->uiNumChildren();i++)
			{
				CFESkelAnimNode* poChildNode = _poNode->poGetChild(i);
				if (poChildNode != NULL)
					poChildNode->Accept(this);
			}
		}

		virtual void Visit(CFESkelAnimSpriteModel* _poNode)
		{
			if (_poNode->sGetSprite() == "") return;

			CFESprite* poSprite = CFESpriteMgr::I()->poLoad( _poNode->sGetSprite() );
			if (poSprite == NULL) return;

			if (	(poSprite->poGetAction(m_uiAction)!=NULL) 
					&& 
					(
					    (m_bMaxTime)
					 || ((poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_LOOP) && (poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_PINGPONG))
					)
				)
				m_rTime = poSprite->poGetAction(m_uiAction)->m_rActionTime;

			if ((m_rTime != -_1r) && (m_rTime > m_rMaxTime))
				m_rMaxTime = m_rTime;

			CFESpriteMgr::I()->ReleaseResource(poSprite);
		}

		virtual void Visit(CFESkelAnimMeshModel* _poNode)
		{
			if (_poNode->sGetSprite() == "") return;

			CFESprite* poSprite = CFESpriteMgr::I()->poLoad( _poNode->sGetSprite() );
			if (poSprite == NULL) return;

			if (	(poSprite->poGetAction(m_uiAction)!=NULL) 
					&& 
					(
					    (m_bMaxTime)
					 || ((poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_LOOP) && (poSprite->poGetAction(m_uiAction)->m_ePlayMode != SFSPM_PINGPONG))
					)
				)
				m_rTime = poSprite->poGetAction(m_uiAction)->m_rActionTime;

			if ((m_rTime != -_1r) && (m_rTime > m_rMaxTime))
				m_rMaxTime = m_rTime;

			CFESpriteMgr::I()->ReleaseResource(poSprite);
		}

	protected:

		FEReal			m_rTime;
		FEReal			m_rMaxTime;
		unsigned int	m_uiAction;
		FEBool			m_bMaxTime;
};
//-----------------------------------------------------------------------------
FEReal CFESkelAnimUtils::rGetNodeActionTime(CFESkelAnim* _poAnim,CFESkelAnimNodeAction* _poAction)
{
	CFESkelAnimNodeAction* poNodeAction = _poAction;
	EFEKFBFuncWrapMode eWrapMode;
	FEReal rActionTime = -_1r;
	FEReal rTime = _0r;

	eWrapMode = poNodeAction->m_oPosFunc.eGetWrapMode();
	rTime = poNodeAction->m_oPosFunc.rGetMaxLimit();
	if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;
	
	#ifdef USE_SKELANIM_SCALE_COLOR
	eWrapMode = poNodeAction->m_oScaleFunc.eGetWrapMode();
	rTime = poNodeAction->m_oScaleFunc.rGetMaxLimit();
	if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

	eWrapMode = poNodeAction->m_oColorFunc.eGetWrapMode();
	rTime = poNodeAction->m_oColorFunc.rGetMaxLimit();
	if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;
	#endif

	eWrapMode = poNodeAction->m_rAngleFunc.eGetWrapMode();
	rTime = poNodeAction->m_rAngleFunc.rGetMaxLimit();
	if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG) && (rTime>_0r)&& (rActionTime < rTime)) rActionTime = rTime;

	// Retrieve the action to be played on this node (only for sprites)
	if ((_poAnim!=NULL) && (poNodeAction->uiGetNodeIdx() >=0) && (poNodeAction->iGetNodeActionIdx() != -1))
	{
		CFESkelAnimNode* poNode = _poAnim->m_oNodeTab[poNodeAction->uiGetNodeIdx()];
		if (poNode != NULL)
		{
			rTime = CFESkelAnimNodeActionTimeGetter::rGetTime(poNode,poNodeAction->iGetNodeActionIdx(),false);
			if ((rTime>_0r) && (rActionTime < rTime)) rActionTime = rTime;
		}
	}

	return(rActionTime);
}
//-----------------------------------------------------------------------------
FEReal CFESkelAnimUtils::rGetActionTime(CFESkelAnim* _poAnim,CFESkelAnimAction* _poAction)
{
	uint i;
	FEReal rActionTime = -_1r;
    for (i=0;i<_poAction->uiNumNodeActions();i++)
    {
		FEReal rTime = rGetNodeActionTime(_poAnim,_poAction->poGetNodeAction(i));
		if (rActionTime < rTime) rActionTime = rTime;
	}

    return(rActionTime);
}
//-----------------------------------------------------------------------------
FEReal CFESkelAnimUtils::rGetNodeMaxActionTime(CFESkelAnim* _poAnim,CFESkelAnimNodeAction* _poAction)
{
	FEReal rMaxActionTime = -_1r;
	CFESkelAnimNodeAction* poNodeAction = _poAction;
	FEReal rMaxTime;

	rMaxTime = poNodeAction->m_oPosFunc.rGetMaxLimit();
	if ((rMaxTime>_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

	#ifdef USE_SKELANIM_SCALE_COLOR
	rMaxTime = poNodeAction->m_oScaleFunc.rGetMaxLimit();
	if ((rMaxTime>_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

	rMaxTime = poNodeAction->m_oColorFunc.rGetMaxLimit();
	if ((rMaxTime>_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;
	#endif

	rMaxTime = poNodeAction->m_rAngleFunc.rGetMaxLimit();
	if ((rMaxTime>_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

	// Retrieve the action to be played on this node (only for sprites)
	if ((_poAnim!=NULL) && (poNodeAction->uiGetNodeIdx() >=0) && (poNodeAction->iGetNodeActionIdx() != -1))
	{
		CFESkelAnimNode* poNode = _poAnim->m_oNodeTab[poNodeAction->uiGetNodeIdx()];
		if (poNode != NULL)
		{
			rMaxTime = CFESkelAnimNodeActionTimeGetter::rGetTime(poNode,poNodeAction->iGetNodeActionIdx(),true);
			if ((rMaxTime>_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;
		}
	}

	return(rMaxActionTime);
}
//-----------------------------------------------------------------------------
FEReal CFESkelAnimUtils::rGetMaxActionTime(CFESkelAnim* _poAnim,CFESkelAnimAction* _poAction)
{
	uint i;
	FEReal rMaxActionTime = -_1r;
    for (i=0;i<_poAction->uiNumNodeActions();i++)
    {
		FEReal rTime = rGetNodeMaxActionTime(_poAnim,_poAction->poGetNodeAction(i));
		if (rMaxActionTime < rTime) rMaxActionTime = rTime;
	}

    return(rMaxActionTime);
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// END: HELPERS
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
