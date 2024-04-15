// ----------------------------------------------------------------------------
/*! \class CFESkelAnimInstMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "graphics/sprite/CFESprite.h"

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
#include "core/CFECore.h"
#include "types/CFESingleton.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNodeEnabler : public CFESkelAnimNodeVisitor
{
	public:
		CFESkelAnimNodeEnabler(FEBool _bEnable)
		{
			m_bEnable = _bEnable;
		}
	
		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

		// Do nothing
        virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {

        };

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			if (m_bEnable)
				CFESpriteInstMgr::I()->Enable(_poObj->hGetSprite());
			else
				CFESpriteInstMgr::I()->Disable(_poObj->hGetSprite());
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			if (m_bEnable)
				CFESpriteInstMgr::I()->Enable(_poObj->hGetSprite());
			else
				CFESpriteInstMgr::I()->Disable(_poObj->hGetSprite());
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

	protected:

		FEBool	m_bEnable;
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

		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

		// Do nothing
        virtual void Visit(CFESkelAnimSpriteModel* _poObj)
        {

        };

		// Do nothing
        virtual void Visit(CFESkelAnimMeshModel* _poObj)
        {

        };

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
        {
			if (m_iAction != -1)
			{
				#ifdef _DEBUG
				/*
				CFESprite* poSpr = CFESpriteInstMgr::I()->poGetSprite( _poObj->hGetSprite() );
				if (poSpr != NULL)
					CFECore::Log::Print(" ****************     Set Action %s for %s\n",poSpr->poGetAction(m_iAction)->sGetName(),_poObj->sGetName());
					else
					CFECore::Log::Print(" ****************     Set Action %d for %s\n",m_iAction,_poObj->sGetName());
				*/
				#endif

				CFESpriteInstMgr::I()->SetAction(_poObj->hGetSprite(),m_iAction);				
				CFESpriteInstMgr::I()->SetSpeedMult( _poObj->hGetSprite(), m_rSpeedMult);				
			}
        }

        virtual void Visit(CFESkelAnimMesh* _poObj)
        {
			if (m_iAction != -1)
			{
				CFESpriteInstMgr::I()->SetAction(_poObj->hGetSprite(),m_iAction);				
				CFESpriteInstMgr::I()->SetSpeedMult( _poObj->hGetSprite(), m_rSpeedMult);				
			}
        }

        /// 
        virtual void Visit(CFESkelAnimBone* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj)
        {
			for (uint i=0;i<_poObj->uiNumChildren();i++)
				_poObj->poGetChild(i)->Accept(this);
        }

	protected:

		int		m_iAction;
		FEReal	m_rSpeedMult;
};
// ----------------------------------------------------------------------------
class CFESkelAnimMgrInst : public CFESkelAnimInst
{
    public:

        /// Is the current skel anim instance being played?
        FEBool m_bEnabled;

        /// Will the render be managed by the skel anim instance manager?
        FEBool m_bIsRenderManaged;
		
		/// Will the update be managed by the skel anim instance manager?
		FEBool m_bIsUpdateManaged;

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
        FEBool m_bInternal;
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
	return( (FEHandler)CFESkelAnimMgr::I()->poLoad(_sResource) );
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
    _poInstance->m_rDepth       		= _0r;
    _poInstance->m_oScale			= CFEVect2::ONE();
    _poInstance->m_uiAction			= 0;
    _poInstance->m_rSpeedMult		= _1r;
    _poInstance->m_bInternal			= false;
	_poInstance->m_bIsRenderManaged = true;
	_poInstance->m_bIsUpdateManaged	= true;

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

	if (_poInstance->m_poNodeInstMask != NULL)
	{
		delete _poInstance->m_poNodeInstMask;
		_poInstance->m_poNodeInstMask = NULL;
	}
}
// ----------------------------------------------------------------------------
const CFEVect2& CFESkelAnimInstMgr::oGetPos(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(CFEVect2::ZERO());
	return( ANIMINST(_hInstance).m_oPos );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetPos(FEHandler _hInstance,const CFEVect2& _oPos)
{
	if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_oPos = _oPos;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetDepth(FEHandler _hInstance,FEReal _rDepth)
{
    if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetDepth(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( ANIMINST(_hInstance).m_rDepth );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetScale(FEHandler _hInstance,const CFEVect2& _oScale)
{
	if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_oScale = _oScale;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetScale(FEHandler _hInstance,FEReal _rScale)
{
	if (_hInstance == NULL) return;
    ANIMINST(_hInstance).m_oScale.x = _rScale;
    ANIMINST(_hInstance).m_oScale.y = _rScale;
}
// ----------------------------------------------------------------------------
const CFEVect2& CFESkelAnimInstMgr::oGetScale(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2::ONE());
    return( ANIMINST(_hInstance).m_oScale );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetAngle(FEHandler _hInstance,FEReal _rAngle)
{
	if (_hInstance == NULL) return;
    ANIMINST(_hInstance).m_rAngle = _rAngle;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetAngle(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(_0r);
    return( ANIMINST(_hInstance).m_rAngle );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetColor(FEHandler _hInstance,const CFEColor& _oColor)
{
	if (_hInstance == NULL) return;
    ANIMINST(_hInstance).m_oColor = _oColor;
}
// ----------------------------------------------------------------------------
const CFEColor& CFESkelAnimInstMgr::oGetColor(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(CFEColor::WHITE());
    return( ANIMINST(_hInstance).m_oColor );
}
// ----------------------------------------------------------------------------
/// Setups both action and speed mult at the same time
void CFESkelAnimInstMgr::SetActionAndSpeedMult(FEHandler _hInstance,uint _uiAction,FEReal _rSpeedMult)
{
	ANIMINST(_hInstance).m_rSpeedMult	= _rSpeedMult;

	// sanity checks
	if (ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet() == NULL) return;
	if (_uiAction >= ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->uiNumActions()) return;
	CFECoreCheck( _uiAction< ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->uiNumActions(),CFEString("Bad action number ") + CFEString(_uiAction) );

    ANIMINST(_hInstance).m_uiAction		= _uiAction;
    ANIMINST(_hInstance).m_rActionTime	= _0r;

	
	CFESkelAnimUpdater::RestartActions( &(ANIMINST(_hInstance)) );

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

	// Update initial state.
	Update(_hInstance,_0r);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::SetAction(FEHandler _hInstance,uint _uiAction)
{
	if (_hInstance == NULL) return;
	SetActionAndSpeedMult(_hInstance, _uiAction, ANIMINST(_hInstance).m_rSpeedMult);
}
// ----------------------------------------------------------------------------
uint CFESkelAnimInstMgr::uiGetAction(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(0);
    return( ANIMINST(_hInstance).m_uiAction );
}
// ----------------------------------------------------------------------------
// IMPORTANT NOTE: MaxActionTime now takes into account the sprite animation time
// of sprite nodes. This may affect loops on skeletal animations nodes also
// which may affect what the user sees.
// ----------------------------------------------------------------------------
FEBool bIsNAN(FEReal* _prReal)
{
	return( *((uint*)_prReal) == 0xffc00000 );
}

void CFESkelAnimInstMgr::SetActionTime(FEHandler _hInstance,FEReal _rActionTime)
{
	if (_hInstance == NULL) return;
	if (ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet() == NULL) return;

	CFESkelAnimMgrInst* poAI 	= &ANIMINST(_hInstance);
    CFESkelAnimAction*  poAction= poAI->m_poSkelAnim->poGetAnimActionSet()->poGetAction(poAI->m_uiAction);
    if (poAction == NULL) return;

	FEReal rNewTime				= _rActionTime;
	FEReal rMaxActionTime		= poAction->rGetMaxActionTime();

	// To prevent overflows
	if (rMaxActionTime>_0r)
	{
		switch (poAction->eGetPlayMode())
		{
			case SAAPM_ONESHOT:
			rNewTime = CFEMath::rMin(rNewTime,rMaxActionTime);
			break;

			// loop anims...
			default:
			{
				// WARNING: This is not valid, if one bone has a loop animation of 2 seconds and other of 3,
				// when time = 3 it will restart all the animations
				// m_rTime = CFEMath::rMod(m_rTime,rMaxActionTime);
				rNewTime = CFEMath::rMod(rNewTime,1000.0f);
			}
			break;

			/*
			case SAAPM_LOOP:
			rNewTime = CFEMath::rMod(rNewTime,rMaxActionTime);
			break;

			case SAAPM_PINGPONGSTOP:
			rNewTime = CFEMath::rMin(rNewTime,_2r*rMaxActionTime+_001r);
			break;

			case SAAPM_PINGPONG:
			rNewTime = CFEMath::rMod(rNewTime,_2r*rMaxActionTime);
			break;
			*/
		}
	}

	if (bIsNAN(&rNewTime))
	{
		int a = 0;
	}		

	poAI->m_rActionTime = rNewTime;
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetActionTime(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(_0r);
    return( ANIMINST(_hInstance).m_rActionTime );
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetActionMaxTime(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(_0r);

	CFESkelAnimMgrInst* poAI 	= &ANIMINST(_hInstance);
    CFESkelAnimAction*  poAction= poAI->m_poSkelAnim->poGetAnimActionSet()->poGetAction(poAI->m_uiAction);

	return(poAction->rGetMaxActionTime());
}
// ----------------------------------------------------------------------------
FEReal CFESkelAnimInstMgr::rGetSpeedMult(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(_0r);
	return( ANIMINST(_hInstance).m_rSpeedMult );
}
// ----------------------------------------------------------------------------
FEBool bIsInfinite(FEReal* _prReal)
{
	return( *((uint*)_prReal) == 0x7f800000 );
}

void CFESkelAnimInstMgr::SetSpeedMult(FEHandler _hInstance,FEReal _rSpeedMult)
{
	if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_rSpeedMult = _rSpeedMult;
	if (bIsInfinite(&_rSpeedMult))
	{
		int a = 0;
	}
	// SetActionAndSpeedMult(_hInstance,ANIMINST(_hInstance).m_uiAction,_rSpeedMult);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Enable(FEHandler _hInstance)
{
	if (_hInstance == NULL) return;
    ANIMINST(_hInstance).m_bEnabled = true;
    CFESkelAnimNodeEnabler oEnabler(true);
    ANIMINST(_hInstance).m_poInstAnimNode->Accept(&oEnabler);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Disable(FEHandler _hInstance)
{
	if (_hInstance == NULL) return;
    ANIMINST(_hInstance).m_bEnabled = false;
    CFESkelAnimNodeEnabler oEnabler(false);
    ANIMINST(_hInstance).m_poInstAnimNode->Accept(&oEnabler);
}
// ----------------------------------------------------------------------------
FEBool CFESkelAnimInstMgr::bIsEnabled(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
    return( ANIMINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
FEBool CFESkelAnimInstMgr::bIsPlaying(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	CFESkelAnimAction* poAction = ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->poGetAction(ANIMINST(_hInstance).m_uiAction);
	if (poAction == NULL) return(false);

	if ((poAction->eGetPlayMode() == SAAPM_LOOP) || (poAction->eGetPlayMode() == SAAPM_PINGPONG))
		return(true);
	else
		return(ANIMINST(_hInstance).m_rActionTime < poAction->rGetActionTime() );
}
// ----------------------------------------------------------------------------
int CFESkelAnimInstMgr::iGetNodeMaskIndex(FEHandler _hInstance, const CFEString& _sNodeName)
{
	if (_hInstance != NULL)
	{
		for (uint i = 0; i < ANIMINST(_hInstance).m_poNodeInstTab->size(); i++)
		{
			if (ANIMINST(_hInstance).m_poNodeInstTab->at(i)->sGetName() == _sNodeName)
				return(i);
		}
	}

	return(-1);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::UnmaskNode(FEHandler _hInstance,int _iNodeMaskIndex)
{
	if ((_hInstance != NULL) && (_iNodeMaskIndex!=-1))
		ANIMINST(_hInstance).m_poNodeInstMask[_iNodeMaskIndex] = false;
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::MaskNode(FEHandler _hInstance,int _iNodeMaskIndex)
{
	if ((_hInstance != NULL) && (_iNodeMaskIndex!=-1))
		ANIMINST(_hInstance).m_poNodeInstMask[_iNodeMaskIndex] = true;
}
// ----------------------------------------------------------------------------
FEBool CFESkelAnimInstMgr::bIsNodeMasked(FEHandler _hInstance,int _iNodeMaskIndex)
{
	if ((_hInstance != NULL) && (_iNodeMaskIndex!=-1))
		return(ANIMINST(_hInstance).m_poNodeInstMask[_iNodeMaskIndex]);
	else
		return(false);
}
// ----------------------------------------------------------------------------
FEBool* CFESkelAnimInstMgr::pbGetNodeTableMask(FEHandler _hInstance)
{
	if (_hInstance != NULL)
		return(ANIMINST(_hInstance).m_poNodeInstMask);
	else
		return(NULL);
}
// ----------------------------------------------------------------------------
const CFEArray<CFESkelAnimNode*>* CFESkelAnimInstMgr::poGetNodeTable(FEHandler _hInstance)
{
	if (_hInstance != NULL)
		return(ANIMINST(_hInstance).m_poNodeInstTab);
	else
		return(NULL);
}
// ----------------------------------------------------------------------------
CFESkelAnim* CFESkelAnimInstMgr::poGetAnim(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(NULL);
    return( ANIMINST(_hInstance).m_poSkelAnim );
}
// ----------------------------------------------------------------------------
CFESkelAnimNode* CFESkelAnimInstMgr::poGetInstancedNode(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);
    return( ANIMINST(_hInstance).m_poInstAnimNode );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::ManageRender(FEHandler _hInstance,FEBool _bManageRender)
{
	if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------
FEBool CFESkelAnimInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(false);
	return( ANIMINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::ManageUpdate(FEHandler _hInstance,FEBool _bManagedUpdate)
{
	if (_hInstance == NULL) return;
	ANIMINST(_hInstance).m_bIsUpdateManaged = _bManagedUpdate;
}
// ----------------------------------------------------------------------------
FEBool CFESkelAnimInstMgr::bIsUpdateManaged(FEHandler _hInstance)
{
	if (_hInstance == NULL) return(false);
	return( ANIMINST(_hInstance).m_bIsUpdateManaged );
}
// ----------------------------------------------------------------------------
/// Sets the animation isntance to a specific moment in time.
void CFESkelAnimInstMgr::SetTime(FEHandler _hInstance,FEReal _rTime)
{
    if (_hInstance == NULL) return;
    CFESkelAnimMgrInst* poAI     = &ANIMINST(_hInstance);

	FEReal rPrevTime = poAI->m_rActionTime;

	SetActionTime(_hInstance,_rTime*poAI->m_rSpeedMult);

	// This hack instead of (_rDeltaT*poAI->m_rSpeedMult) will be useful also for big DeltaT (like in the ActorEditor)
	FEReal rDeltaT = poAI->m_rActionTime - rPrevTime;

	// Update static nodes
	CFESkelAnimNodeUpdater::Update(poAI->m_poInstAnimNode,poAI->m_rActionTime,rDeltaT);

	// Update nodes with actions.
	CFESkelAnimUpdater::Update(poAI);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Update(FEHandler _hInstance,FEReal _rDeltaT)
{	
	if (_hInstance == NULL) return;
    CFESkelAnimMgrInst* poAI = &ANIMINST(_hInstance);

	SetActionTime(_hInstance,poAI->m_rActionTime+_rDeltaT*poAI->m_rSpeedMult);

	// Update static nodes
	CFESkelAnimNodeUpdater::Update(poAI->m_poInstAnimNode,poAI->m_rActionTime,_rDeltaT);

	// Update nodes with actions.
	CFESkelAnimUpdater::Update(poAI);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<uiGetMaxInstances();i++)
    {
		CFESkelAnimMgrInst* poInst = poGetResourceInstance(i);

        if ((! bIsInstanceFree(i)) && (poInst->m_bEnabled) && (poInst->m_bIsUpdateManaged))
            Update((FEHandler)poInst,_rDeltaT);
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Render(CFERenderer *_poRenderer)
{
	#ifdef _DEBUG
	uint uiNumRenderedInstances = 0;
	#endif

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
			#ifdef _DEBUG
			uiNumRenderedInstances++;
			#endif
        }
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Render(FEHandler _hInstance,CFERenderer *_poRenderer)
{
	CFESkelAnimMgrInst& oInst = ANIMINST(_hInstance);
	CFESkelAnimRenderer::Render(_poRenderer,
								oInst.m_poInstAnimNode,
                                oInst.m_oPos,
                                oInst.m_rDepth,
                                oInst.m_oScale,
                                oInst.m_rAngle,
                                oInst.m_oColor);
}
// ----------------------------------------------------------------------------
