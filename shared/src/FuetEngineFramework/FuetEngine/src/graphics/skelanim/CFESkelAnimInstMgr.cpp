// ----------------------------------------------------------------------------
/*! \class CFESkelAnimInstMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "graphics/sprite/CFESpriteInstMgr.h"

#include "CFESkelAnimInstMgr.h"
#include "CFESkelAnimInstancer.h"
#include "CFESkelAnim.h"
#include "CFESkelAnimRenderer.h"
#include "CFESkelAnimUpdater.h"
#include "CFESkelAnimNodeUpdater.h"
#include "CFESkelAnimSprite.h"
#include "CFESkelAnimMesh.h"
#include "CFESkelAnimGroup.h"
#include "CFESkelAnimBone.h"
#include "system/CFESystem.h"
#include "types/CFESingleton.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNodeEnabler : public CFESkelAnimNodeVisitor
{
	public:
		CFESkelAnimNodeEnabler(bool _bEnable)
		{
			m_bEnable = _bEnable;
		}
		
		// Do nothing
        virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {

        };

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			if (m_bEnable)
				CFESpriteInstMgr::Enable(_poObj->hGetSprite());
			else
				CFESpriteInstMgr::Disable(_poObj->hGetSprite());
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			if (m_bEnable)
				CFESpriteInstMgr::Enable(_poObj->hGetSprite());
			else
				CFESpriteInstMgr::Disable(_poObj->hGetSprite());
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
            if (_poObj->poGetAttachedNode() != NULL)
                _poObj->poGetAttachedNode()->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumObjs();i++)
				_poObj->poGetNode(i)->Accept(this);
        }

	protected:

		bool	m_bEnable;
};
//-----------------------------------------------------------------------------
class CFESkelAnimNodeActionChanger : public CFESkelAnimNodeVisitor
{
	public:
		CFESkelAnimNodeActionChanger(int _iAction,FEReal _rSpeedMult)
		{
			m_iAction		= _iAction;
			m_rSpeedMult	= _rSpeedMult;
		}

		// Do nothing
        virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {

        };

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			if (m_iAction != -1)
			{
				CFESpriteInstMgr::SetAction(_poObj->hGetSprite(),m_iAction);				
				CFESpriteInstMgr::SetSpeedMult( _poObj->hGetSprite(), m_rSpeedMult);				
			}
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			if (m_iAction != -1)
			{
				CFESpriteInstMgr::SetAction(_poObj->hGetSprite(),m_iAction);				
				CFESpriteInstMgr::SetSpeedMult( _poObj->hGetSprite(), m_rSpeedMult);				
			}
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
            if (_poObj->poGetAttachedNode() != NULL)
                _poObj->poGetAttachedNode()->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumObjs();i++)
				_poObj->poGetNode(i)->Accept(this);
        }

	protected:

		int		m_iAction;
		FEReal	m_rSpeedMult;
};
// ----------------------------------------------------------------------------
class CFESkelAnimMgrInst : public CFESkelAnimInst
{
    public:

        /// Is the current sprite instance being played?
        bool m_bEnabled;

        /// Will the render be managed by the sprite instance manager?
        bool m_bIsRenderManaged;

        /// Position of this instance.
        CFEVect2 m_oPos;

        /// Depth of this instance.
        FEReal m_rDepth;

        /// Scale of this instance.
        CFEVect2 m_oScale;

        /// Angle of this instance.
        FEReal m_rAngle;

        /// Speed multiplier.
        FEReal m_rSpeedMult;

        /// The color of this instance.
        CFEColor m_oColor;

        /// This instance is handled internally.
        bool m_bInternal;
};
// ----------------------------------------------------------------------------
// handlers are instances
// #define ANIMINST(handler) *((CFESpriteInst*)handler)
// handlers are indices
// #define ANIMINST(handler)   m_oSpriteDB[(uint)handler]
// ----------------------------------------------------------------------------
inline CFESkelAnimMgrInst& ANIMINST(FEHandler _hHandler)
{
    return( *(CFESkelAnimMgrInst*)_hHandler );
}
// ----------------------------------------------------------------------------
FEHandler CFESkelAnimInstMgr::hGetResource(const CFEString& _sResource)
{
	return( (FEHandler)CFESkelAnimMgr::poLoad(_sResource) );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetupInstance(CFESkelAnimMgrInst* _poInstance,FEHandler _hResource)
{
	_poInstance->m_poSkelAnim		= (CFESkelAnim*)_hResource;

    _poInstance->m_bEnabled			= false;
    _poInstance->m_rActionTime		= _0r;
    _poInstance->m_rAngle			= _0r;
    _poInstance->m_oColor			= CFEColor(_1r,_1r,_1r,_1r);
    _poInstance->m_oPos				= CFEVect2::ZERO();
    _poInstance->m_rDepth       	= _0r;
    _poInstance->m_oScale			= CFEVect2::ONE();
    _poInstance->m_uiAction			= 0;
    _poInstance->m_rSpeedMult		= _1r;
    _poInstance->m_bInternal		= false;
	_poInstance->m_bIsRenderManaged = true;

    CFESkelAnimInstancer::CreateInstance( (CFESkelAnim*)_hResource, _poInstance);
}
// ----------------------------------------------------------------------------
CFESkelAnimMgrInst* CFESkelAnimInstMgr::poCreateInstance()
{
	return( new CFESkelAnimMgrInst() );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::DestroyInstance(CFESkelAnimMgrInst* _poInstance)
{
	// InvalidateInstance(_poInstance);
	delete _poInstance;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::InvalidateInstance(CFESkelAnimMgrInst* _poInstance)
{
	if (_poInstance->m_poInstAnimNode != NULL)
	{
		delete _poInstance->m_poInstAnimNode;
		_poInstance->m_poInstAnimNode = NULL;
	}

	if (_poInstance->m_poNodeInstTab != NULL)
	{
		delete _poInstance->m_poNodeInstTab;
		_poInstance->m_poNodeInstTab = NULL;
	}
}
// ----------------------------------------------------------------------------
const CFEVect2& CFESkelAnimInstMgr::oGetPos(FEHandler _hInstance)
{
	return( ANIMINST(_hInstance).m_oPos );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetPos(FEHandler _hInstance,const CFEVect2& _oPos)
{
	ANIMINST(_hInstance).m_oPos = _oPos;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetDepth(FEHandler _hInstance,FEReal _rDepth)
{
    ANIMINST(_hInstance).m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetDepth(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_rDepth );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetScale(FEHandler _hInstance,const CFEVect2& _oScale)
{
    ANIMINST(_hInstance).m_oScale = _oScale;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetScale(FEHandler _hInstance,FEReal _rScale)
{
    ANIMINST(_hInstance).m_oScale.x = _rScale;
    ANIMINST(_hInstance).m_oScale.y = _rScale;
}
// ----------------------------------------------------------------------------
const CFEVect2& CFESkelAnimInstMgr::oGetScale(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_oScale );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetAngle(FEHandler _hInstance,FEReal _rAngle)
{
    ANIMINST(_hInstance).m_rAngle = _rAngle;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetAngle(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_rAngle );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetColor(FEHandler _hInstance,const CFEColor& _oColor)
{
    ANIMINST(_hInstance).m_oColor = _oColor;
}
// ----------------------------------------------------------------------------
const CFEColor& CFESkelAnimInstMgr::oGetColor(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_oColor );
}
// ----------------------------------------------------------------------------
/// Setups both action and speed mult at the same time
void CFESkelAnimInstMgr::SetActionAndSpeedMult(FEHandler _hInstance,uint _uiAction,FEReal _rSpeedMult)
{
	// sanity checks
	if (_uiAction >= ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->uiNumActions()) return;
	CFESystemCheck( _uiAction< ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->uiNumActions(),CFEString("Bad action number ") + CFEString(_uiAction) );

    ANIMINST(_hInstance).m_uiAction		= _uiAction;
    ANIMINST(_hInstance).m_rActionTime	= _0r;
	ANIMINST(_hInstance).m_rSpeedMult	= _rSpeedMult;

	CFESkelAnimAction* poAction = ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->poGetAction(_uiAction);
    if (poAction != NULL)
    {
        for (uint i=0;i<poAction->uiNumNodeActions();i++)
        {
		    CFESkelAnimNodeAction* poNodeAction = poAction->poGetNodeAction(i);

		    if ((poNodeAction->iGetNodeActionIdx() != -1) && poNodeAction->bIsVisible())
		    {
		        uint uiNodeIdx = poNodeAction->uiGetNodeIdx();

				CFESkelAnimNodeActionChanger oActionChanger(poNodeAction->iGetNodeActionIdx(),_rSpeedMult);
				CFESkelAnimNode* poNode = ANIMINST(_hInstance).m_poNodeInstTab->at(uiNodeIdx);
				poNode->Accept(&oActionChanger);
		    }
        }
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetAction(FEHandler _hInstance,uint _uiAction)
{
	SetActionAndSpeedMult(_hInstance, _uiAction, ANIMINST(_hInstance).m_rSpeedMult);
}
// ----------------------------------------------------------------------------
uint CFESkelAnimInstMgr::uiGetAction(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_uiAction );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetActionTime(FEHandler _hInstance,FEReal _rActionTime)
{
	if (_hInstance == NULL) return;
	CFESkelAnimMgrInst* poAI 	= &ANIMINST(_hInstance);
    CFESkelAnimAction*  poAction= poAI->m_poSkelAnim->poGetAnimActionSet()->poGetAction(poAI->m_uiAction);
	FEReal rNewTime				= _rActionTime;

	// To prevent overflows
	switch (poAction->eGetPlayMode())
    {
		case SAAPM_ONESHOT:
		rNewTime = CFEMath::rMin(rNewTime,poAction->rGetMaxActionTime());
		break;

		case SAAPM_LOOP:
		rNewTime = CFEMath::rMod(rNewTime,poAction->rGetMaxActionTime());
		break;

		case SAAPM_PINGPONGSTOP:
		rNewTime = CFEMath::rMin(rNewTime,_2r*poAction->rGetMaxActionTime()+_001r);
		break;

		case SAAPM_PINGPONG:
		rNewTime = CFEMath::rMod(rNewTime,_2r*poAction->rGetMaxActionTime());
		break;
    }

	poAI->m_rActionTime = rNewTime;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetActionTime(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_rActionTime );
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetSpeedMult(FEHandler _hInstance)
{
	return( ANIMINST(_hInstance).m_rSpeedMult );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetSpeedMult(FEHandler _hInstance,FEReal _rSpeedMult)
{
	SetActionAndSpeedMult(_hInstance,ANIMINST(_hInstance).m_uiAction,_rSpeedMult);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Enable(FEHandler _hInstance)
{
    ANIMINST(_hInstance).m_bEnabled = true;
    CFESkelAnimNodeEnabler oEnabler(true);
    ANIMINST(_hInstance).m_poInstAnimNode->Accept(&oEnabler);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Disable(FEHandler _hInstance)
{
    ANIMINST(_hInstance).m_bEnabled = false;
    CFESkelAnimNodeEnabler oEnabler(false);
    ANIMINST(_hInstance).m_poInstAnimNode->Accept(&oEnabler);
}
// ----------------------------------------------------------------------------
bool CFESkelAnimInstMgr::bIsEnabled(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
bool CFESkelAnimInstMgr::bIsPlaying(FEHandler _hInstance)
{
	CFESkelAnimAction* poAction = ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->poGetAction(ANIMINST(_hInstance).m_uiAction);

	if ((poAction->eGetPlayMode() == SAAPM_LOOP) || (poAction->eGetPlayMode() == SAAPM_PINGPONG))
		return(true);
	else
		return(ANIMINST(_hInstance).m_rActionTime < poAction->rGetActionTime() );
}
// ----------------------------------------------------------------------------
CFESkelAnim* CFESkelAnimInstMgr::poGetAnim(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_poSkelAnim );
}
// ----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimInstMgr::poGetInstancedNode(FEHandler _hInstance)
{
    return( ANIMINST(_hInstance).m_poInstAnimNode );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::ManageRender(FEHandler _hInstance,bool _bManageRender)
{
	ANIMINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------
bool CFESkelAnimInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
	return( ANIMINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Update(FEHandler _hInstance,FEReal _rDeltaT)
{
    CFESkelAnimMgrInst* poAI     = &ANIMINST(_hInstance);

	SetActionTime(_hInstance,poAI->m_rActionTime+_rDeltaT*poAI->m_rSpeedMult);

	// Update static nodes
	CFESkelAnimNodeUpdater::Update(poAI->m_poInstAnimNode,poAI->m_rActionTime);

	// Update nodes with actions.
	CFESkelAnimUpdater::Update(poAI);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<uiGetMaxInstances();i++)
    {
		CFESkelAnimMgrInst* poInst = poGetResourceInstance(i);

        if ((! bIsInstanceFree(i)) && (poInst->m_bEnabled) && (poInst->m_bIsRenderManaged))
            Update((FEHandler)poInst,_rDeltaT);
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Render(CFERenderer *_poRenderer)
{
    for (uint i=0;i<uiGetMaxInstances();i++)
    {
		CFESkelAnimMgrInst* poInst = poGetResourceInstance(i);

        if ((! bIsInstanceFree(i)) && (poInst->m_bEnabled) && (poInst->m_bIsRenderManaged))
        {
            CFESkelAnimRenderer::Render(_poRenderer,
								        poInst->m_poInstAnimNode,
                                        poInst->m_oPos,
                                        poInst->m_rDepth,
                                        poInst->m_oScale,
                                        poInst->m_rAngle,
                                        poInst->m_oColor);
        }
    }
}
// ----------------------------------------------------------------------------
