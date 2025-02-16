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
#include "CFESkelAnimSprite.h"
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
            int a = 0;
        };

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj)
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

        /// Sets the instance as free to be used later.
        bool m_bFree;
};
// ----------------------------------------------------------------------------
//
DECLARE_SINGLETON(CFESkelAnimInstMgr_DATA)
{
    public:
        
        CFEArray<CFESkelAnimMgrInst*>   m_oList;
};
#define INSTANCEDDATA CFESkelAnimInstMgr_DATA::I()
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
void CFESkelAnimInstMgr::Init(uint _uiMaxInstances)
{
    for (uint i=0;i<_uiMaxInstances;i++)
    {
        CFESkelAnimMgrInst* poInst = new CFESkelAnimMgrInst;
        poInst->m_bFree     = true;
        poInst->m_bEnabled  = false;
        poInst->m_bInternal = false;
		poInst->m_rSpeedMult= _1r;
        poInst->m_bIsRenderManaged = true;

        INSTANCEDDATA->m_oList.push_back(poInst);
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Finish()
{
    Reset();

    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
        delete INSTANCEDDATA->m_oList[i];

    INSTANCEDDATA->m_oList.clear();
    INSTANCEDDATA->Finish();
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Reset()
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (! INSTANCEDDATA->m_oList[i]->m_bFree)
        {
            if (INSTANCEDDATA->m_oList[i]->m_poInstAnimNode != NULL)
            {
                delete INSTANCEDDATA->m_oList[i]->m_poInstAnimNode;
                INSTANCEDDATA->m_oList[i]->m_poInstAnimNode = NULL;
            }

            if (INSTANCEDDATA->m_oList[i]->m_poNodeInstTab != NULL)
            {
                delete INSTANCEDDATA->m_oList[i]->m_poNodeInstTab;
                INSTANCEDDATA->m_oList[i]->m_poInstAnimNode = NULL;
            }
        }

        INSTANCEDDATA->m_oList[i]->m_bFree     = true;
        INSTANCEDDATA->m_oList[i]->m_bEnabled  = false;
        INSTANCEDDATA->m_oList[i]->m_bInternal = false;
		INSTANCEDDATA->m_oList[i]->m_rSpeedMult= _1r;
        INSTANCEDDATA->m_oList[i]->m_bIsRenderManaged = true;        
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Load(const CFEString& _sSkelAnim)
{
    CFESkelAnimMgr::poLoad(_sSkelAnim);
}
// ----------------------------------------------------------------------------
FEHandler CFESkelAnimInstMgr::hGetInstance(const CFEString& _sSkelAnim)
{
    // Retrieve the sprite resource
    CFESkelAnim* poAnim = CFESkelAnimMgr::poLoad(_sSkelAnim);

    // Unable to load sprite
    if (poAnim == NULL) return(NULL);

    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (INSTANCEDDATA->m_oList[i]->m_bFree)
        {
            CFESkelAnimMgrInst* poInst = INSTANCEDDATA->m_oList[i];           
            CFESkelAnimInstancer::CreateInstance(poAnim,poInst);

            poInst->m_poSkelAnim    = poAnim;

            poInst->m_bFree			= false;
            poInst->m_bEnabled		= false;
            poInst->m_rActionTime	= _0r;
            poInst->m_rAngle		= _0r;
            poInst->m_oColor		= CFEColor(_1r,_1r,_1r,_1r);
            poInst->m_oPos			= CFEVect2::oZERO();
            poInst->m_rDepth        = _0r;
            poInst->m_oScale		= CFEVect2::oONE();
            poInst->m_uiAction		= 0;
            poInst->m_rSpeedMult	= _1r;
            poInst->m_bInternal		= false;
			poInst->m_bIsRenderManaged= true;

            return((FEHandler)poInst);    // we can also return the index in case.
        }
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::DeleteInstance(FEHandler _hInstance)
{
    ANIMINST(_hInstance).m_bFree    = true;
    ANIMINST(_hInstance).m_bEnabled = false;
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
	CFESystem::Check( _uiAction< ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->uiNumActions(),CFEString("Bad action number ") + CFEString(_uiAction) );
	
    ANIMINST(_hInstance).m_uiAction		= _uiAction;
    ANIMINST(_hInstance).m_rActionTime	= _0r;
	ANIMINST(_hInstance).m_rSpeedMult	= _rSpeedMult;

	CFESkelAnimAction* poAction = ANIMINST(_hInstance).m_poSkelAnim->poGetAnimActionSet()->poGetAction(_uiAction);
    if (poAction != NULL)
    {
        for (uint i=0;i<poAction->uiNumNodeActions();i++)
        {
		    CFESkelAnimNodeAction* poNodeAction = poAction->poGetNodeAction(i);

		    if (poNodeAction->iGetNodeActionIdx() != -1)
		    {
		        uint uiNodeIdx = poNodeAction->uiGetNodeIdx();

			    CFESkelAnimSprite* poAnimNodeSprite = (CFESkelAnimSprite*)ANIMINST(_hInstance).m_poNodeInstTab->at(uiNodeIdx);
			    CFESpriteInstMgr::SetAction( poAnimNodeSprite->hGetSprite(), poNodeAction->iGetNodeActionIdx() );
			    CFESpriteInstMgr::SetSpeedMult( poAnimNodeSprite->hGetSprite(), _rSpeedMult);
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
    ANIMINST(_hInstance).m_rActionTime = _rActionTime;
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
    CFESkelAnimMgrInst* poAI    = &ANIMINST(_hInstance);
    CFESkelAnimAction*  poAction = poAI->m_poSkelAnim->poGetAnimActionSet()->poGetAction(poAI->m_uiAction);
    poAI->m_rActionTime		 += _rDeltaT * poAI->m_rSpeedMult;

	CFESkelAnimUpdater::Update(poAI);
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {   
        if (INSTANCEDDATA->m_oList[i]->m_bEnabled)
            Update((FEHandler)INSTANCEDDATA->m_oList[i],_rDeltaT);
    }
}
// ----------------------------------------------------------------------------
void CFESkelAnimInstMgr::Render(CFERenderer *_poRenderer)
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if ( INSTANCEDDATA->m_oList[i]->m_bEnabled && INSTANCEDDATA->m_oList[i]->m_bIsRenderManaged)
        {
			CFESkelAnimMgrInst* poAnimInst = INSTANCEDDATA->m_oList[i];
            CFESkelAnimRenderer::Render(_poRenderer,
								        poAnimInst->m_poInstAnimNode,
                                        poAnimInst->m_oPos,
                                        poAnimInst->m_rDepth,
                                        poAnimInst->m_oScale,
                                        poAnimInst->m_rAngle,
                                        poAnimInst->m_oColor);
        }
    }
}
// ----------------------------------------------------------------------------
