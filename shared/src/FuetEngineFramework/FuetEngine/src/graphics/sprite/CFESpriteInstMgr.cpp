// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESpriteInstMgr.h"
// #include "System/FMem.h"
#include "support/file/CFEFile.h"
#include "support/config/CFEConfigFile.h"
#include "support/graphics/CFERenderer.h"
#include "CFESprite.h"
#include "CFESpriteRenderer.h"
#include "types/CFESingleton.h"
// ----------------------------------------------------------------------------
// handlers are instances
// #define SPRITEINST(handler) *((CFESpriteInst*)handler)
// handlers are indices
// #define SPRITEINST(handler)   m_oSpriteDB[(uint)handler]
// ----------------------------------------------------------------------------
inline CFESpriteMgrInst& SPRITEINST(FEHandler _hHandler)
{
    return( *(CFESpriteMgrInst*)_hHandler );
}
// ----------------------------------------------------------------------------
class CFESpriteMgrInst : public CFESpriteInst
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

        /// The color of this instance.
        CFEColor m_oColor;

        /// Speed multiplier for this instance.
        FEReal m_rSpeedMult;

        /// This instance is handled internally.
        bool m_bInternal;

        /// Sets the instance as free to be used later.
        bool m_bFree;
};
// ----------------------------------------------------------------------------
// INSTANCEDDATA to store all the global DATOS.
// ----------------------------------------------------------------------------
DECLARE_SINGLETON(CFESpriteInstMgr_DATA)
{
    public:
        CFEArray<CFESpriteMgrInst*> m_oList;
};

#define INSTANCEDDATA CFESpriteInstMgr_DATA::I()
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Init(uint _uiMaxSpriteInstances)
{
    for (uint i=0;i<_uiMaxSpriteInstances;i++)
    {
        CFESpriteMgrInst* poInst = new CFESpriteMgrInst;
        
        INSTANCEDDATA->m_oList.push_back(poInst);
    }

    Reset();
}
// ----------------------------------------------------------------------------
/// Main finalization procedure.
void CFESpriteInstMgr::Finish()
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
        delete INSTANCEDDATA->m_oList[i];

    INSTANCEDDATA->m_oList.clear();
    INSTANCEDDATA->Finish();
}           
// ----------------------------------------------------------------------------
/// Resets to the initial state manager.
void CFESpriteInstMgr::Reset()
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        INSTANCEDDATA->m_oList[i]->m_poSprite  = NULL;
        INSTANCEDDATA->m_oList[i]->m_bFree     = true;
        INSTANCEDDATA->m_oList[i]->m_bEnabled  = false;
        INSTANCEDDATA->m_oList[i]->m_bInternal = false;
		INSTANCEDDATA->m_oList[i]->m_rSpeedMult= _1r;
        INSTANCEDDATA->m_oList[i]->m_bIsRenderManaged = true;        
    }
}
// ----------------------------------------------------------------------------
/// Forces the loading of a sprite resource.
void CFESpriteInstMgr::Load(const CFEString& _sSpriteModel)
{
    CFESpriteMgr::poLoad(_sSpriteModel);
}
// ----------------------------------------------------------------------------
/// Retrieves a sprite instance of a given sprite resource.
FEHandler CFESpriteInstMgr::hGetInstance(const CFEString& _sSpriteModel)
{
    // Retrieve the sprite resource
    return ( hGetInstance( CFESpriteMgr::poLoad(_sSpriteModel)) );
}
// ----------------------------------------------------------------------------
/// Retrieves a sprite instance of a given sprite object.
FEHandler CFESpriteInstMgr::hGetInstance(CFESprite* _poSprite)
{
	// Check if the sprite object exists.
    if (_poSprite == NULL) return(NULL);

    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (INSTANCEDDATA->m_oList[i]->m_bFree)
        {
            CFESpriteMgrInst* poInst = INSTANCEDDATA->m_oList[i];
            poInst->m_poSprite = _poSprite;
            poInst->m_bFree    = false;
            poInst->m_bEnabled = false;

            if (_poSprite->poGetAction(0) != NULL)
                poInst->m_rActionTime = CFEMath::rRand() * _poSprite->poGetAction(0)->m_rRandStartTime;
            else
                poInst->m_rActionTime = _0r;

            poInst->m_rAngle = _0r;
            poInst->m_oColor = CFEColor(_1r,_1r,_1r,_1r);
            poInst->m_oPos = CFEVect2::oZERO();
            poInst->m_rDepth = _0r;
            poInst->m_oScale= CFEVect2::oONE();
            poInst->m_uiSpriteAction = 0;
            poInst->m_uiCurrentActionFrame = 0;
            poInst->m_rSpeedMult= _1r;
            poInst->m_bInternal = false;
			poInst->m_bIsRenderManaged = true;

            return((FEHandler)poInst);    // we can also return the index in case.
        }
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
/// Spawns a sprite automanaged by the manager.
void CFESpriteInstMgr::Spawn(const CFEString& _sSpriteModel,uint _uiAction,const CFEVect2& _oPos,const FEReal _rDepth,FEReal _rScale,FEReal _rAngle,const CFEColor& _oColor)
{
    FEHandler hHandler = hGetInstance(_sSpriteModel);
    if (hHandler != NULL)
    {
        CFESpriteMgrInst* poInst = (CFESpriteMgrInst*)hHandler;

        poInst->m_rAngle = _rAngle;
        poInst->m_oColor = _oColor;
        poInst->m_oPos   = _oPos;
        poInst->m_oScale = CFEVect2(_rScale,_rScale);
        poInst->m_uiSpriteAction = _uiAction;
        poInst->m_bEnabled  = true;
        poInst->m_bInternal = true;
    }
}
// ----------------------------------------------------------------------------
/// Deletes a given sprite instance.
void CFESpriteInstMgr::DeleteInstance(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_bFree    = true;
    SPRITEINST(_hInstance).m_bEnabled = false;
}
// ----------------------------------------------------------------------------
/// Retrieves the transformed geometry of the given sprite instance.
void CFESpriteInstMgr::GetGeometry(FEHandler _hInstance,CFEVect2* _poVXs)
{
    if (_hInstance == NULL) return;
	CFESpriteMgrInst* poInst = &SPRITEINST(_hInstance);
	CFESpriteRenderer::GetSpriteGeometry(poInst,poInst->m_oPos,poInst->m_oScale,poInst->m_rAngle,_poVXs);
}
// ----------------------------------------------------------------------------
/// Sets the position of a given sprite instance.
void CFESpriteInstMgr::SetPos(FEHandler _hInstance,const CFEVect2& _oPos)
{
    if (_hInstance == NULL) return;
	SPRITEINST(_hInstance).m_oPos = _oPos;
}
// ----------------------------------------------------------------------------
/// Gets the position of a given sprite instance.
CFEVect2 CFESpriteInstMgr::oGetPos(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2(_0r,_0r) );
	return SPRITEINST(_hInstance).m_oPos;
}
// ----------------------------------------------------------------------------
/// Sets the depth of a given sprite instance.
void CFESpriteInstMgr::SetDepth(FEHandler _hInstance,FEReal _rDepth)
{
    if (_hInstance == NULL) return;
	SPRITEINST(_hInstance).m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
/// Gets the position of a given sprite instance.
FEReal CFESpriteInstMgr::rGetDepth(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return SPRITEINST(_hInstance).m_rDepth;
}
// ----------------------------------------------------------------------------
/// Sets the scale of a given sprite instance the same amount in width and height.
void CFESpriteInstMgr::SetScale(FEHandler _hInstance,const CFEVect2& _oScale)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_oScale = _oScale;
}
// ----------------------------------------------------------------------------
/// Sets the scale of a given sprite instance.
void CFESpriteInstMgr::SetScale(FEHandler _hInstance,FEReal _rScale)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_oScale.x = _rScale;
    SPRITEINST(_hInstance).m_oScale.y = _rScale;
}
// ----------------------------------------------------------------------------
/// Retrieves the scale of a given sprite instance.
const CFEVect2& CFESpriteInstMgr::oGetScale(FEHandler _hInstance)
{
    if (_hInstance == NULL) return( CFEVect2::oONE() );
	return(SPRITEINST(_hInstance).m_oScale);
}
// ----------------------------------------------------------------------------
/// Sets the angle of a given sprite instance.
void CFESpriteInstMgr::SetAngle(FEHandler _hInstance,FEReal _rAngle)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_rAngle = _rAngle;
}
// ----------------------------------------------------------------------------
/// Retrieves the current angle of a given sprite instance.
FEReal CFESpriteInstMgr::rGetAngle(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( SPRITEINST(_hInstance).m_rAngle );
}
// ----------------------------------------------------------------------------
/// Sets the color of a given sprite instance.
void CFESpriteInstMgr::SetColor(FEHandler _hInstance,const CFEColor& _oColor)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_oColor = _oColor;
}
// ----------------------------------------------------------------------------
/// Retrieves the color of a given sprite instance.

const CFEColor& CFESpriteInstMgr::oGetColor(FEHandler _hInstance,const CFEColor& _oColor)
{
    static CFEColor soWhite(_1r,_1r,_1r,_1r);
    if (_hInstance == NULL) return(soWhite);
	return SPRITEINST(_hInstance).m_oColor;
}
// ----------------------------------------------------------------------------
/// Sets the cu rrent action of a given sprite instance.
void CFESpriteInstMgr::SetAction(FEHandler _hInstance,uint _uiAction)
{
    if (_hInstance == NULL) return;
	CFESpriteAction* poSprAct = SPRITEINST(_hInstance).m_poSprite->poGetAction(_uiAction);
	if (poSprAct == NULL) return;

    SPRITEINST(_hInstance).m_uiSpriteAction			= _uiAction;
    SPRITEINST(_hInstance).m_rActionTime			= CFEMath::rRand() * poSprAct->m_rRandStartTime;
    SPRITEINST(_hInstance).m_uiCurrentActionFrame	= 0;
}
// ----------------------------------------------------------------------------
/// Sets the current action of a given sprite instance.
void CFESpriteInstMgr::SetAction(FEHandler _hInstance,const CFEString& _sActionName)
{
    if (_hInstance == NULL) return;
	int iActionIdx = SPRITEINST(_hInstance).m_poSprite->iGetActionIdx(_sActionName);
	if (iActionIdx == -1) return;

	SetAction(_hInstance,iActionIdx);
}
// ----------------------------------------------------------------------------
uint CFESpriteInstMgr::uiGetAction(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(0);
	return( SPRITEINST(_hInstance).m_uiSpriteAction );
}
// ----------------------------------------------------------------------------
/// Sets the speed multiplier for a given sprite instance.
void CFESpriteInstMgr::SetSpeedMult(FEHandler _hInstance,FEReal _rSpeedMult)
{
    if (_hInstance == NULL) return;
	SPRITEINST(_hInstance).m_rSpeedMult = _rSpeedMult;
}
// ----------------------------------------------------------------------------
/// Retrieves the speed multiplier of a given sprite instance.
FEReal CFESpriteInstMgr::rGetSpeedMult(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( SPRITEINST(_hInstance).m_rSpeedMult );
}
// ----------------------------------------------------------------------------
/// Enables the sprite instance.
void CFESpriteInstMgr::Enable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_bEnabled = true;
}
// ----------------------------------------------------------------------------
/// Disables the given sprite instance.
void CFESpriteInstMgr::Disable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_bEnabled = false;
}
// ----------------------------------------------------------------------------
/// Retrieves whether this sprite is enabled or not.
bool CFESpriteInstMgr::bIsEnabled(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
    return( SPRITEINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
/// Retrieves whether this sprite is still playing or has stopped (only for one-shot).
bool CFESpriteInstMgr::bIsPlaying(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	CFESpriteMgrInst* poSI    = &SPRITEINST(_hInstance);
	CFESpriteAction* poAction = &(poSI->m_poSprite->m_oActions[ poSI->m_uiSpriteAction ]);

	if ((poAction->m_ePlayMode == SFSPM_ONESHOT) || (poAction->m_ePlayMode == SFSPM_PINGPONGSTOP))
		return( poSI->m_rActionTime < poAction->m_rActionTime );
	else
		return( false );
}
// ----------------------------------------------------------------------------
/// Retrieves the sprite model used by the given sprite instance.
CFESprite* CFESpriteInstMgr::poGetSprite(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);
    return( SPRITEINST(_hInstance).m_poSprite );
}
// ----------------------------------------------------------------------------
/// Sets whether the render will be managed by the instance manager or not.
void CFESpriteInstMgr::ManageRender(FEHandler _hInstance,bool _bManageRender)
{
    if (_hInstance == NULL) return;
	SPRITEINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------            
/// Tells whether the render is managed by the instance manager or not.
bool CFESpriteInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	return( SPRITEINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
/// Performs an update step over the given sprite instance, also in case it's disabled.
void CFESpriteInstMgr::Update(CFESpriteInst* _poInstance,FEReal _rDeltaT)
{
    CFESpriteAction* poAction = &(_poInstance->m_poSprite->m_oActions[ _poInstance->m_uiSpriteAction ]);

    _poInstance->m_rActionTime		    += _rDeltaT;
    _poInstance->m_uiCurrentActionFrame	 = poAction->uiGetFrame( _poInstance->m_rActionTime );
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Update(FEHandler _hInstance,FEReal _rDeltaT)
{
    if (_hInstance == NULL) return;
    
    CFESpriteMgrInst* poSI    = &SPRITEINST(_hInstance);
    Update(poSI,_rDeltaT*poSI->m_rSpeedMult);
    
	// Check if this instance should be disabled or not
    if (poSI->m_bInternal)
    {
        CFESpriteAction* poAction = &(poSI->m_poSprite->m_oActions[ poSI->m_uiSpriteAction ]);

        if ((poAction->m_ePlayMode != SFSPM_PINGPONG) && (poAction->m_ePlayMode != SFSPM_LOOP))
        {
            if (
                ((poAction->m_ePlayMode == SFSPM_ONESHOT) && (poSI->m_rActionTime >= poAction->m_rActionTime))

                /*
				||
                ((poAction->m_ePlayMode == SFSPM_PINGPONGSTOP) && (rFrame >= (2*poAction->m_oSeq.size())) )
                */
                )
                #pragma message(__FUNCTION__"/"__FILE__)
                #pragma message("#################")
                #pragma message("To be implemented")
                #pragma message("#################")
            {
                poSI->m_bEnabled  = false;
                poSI->m_bFree     = true;
                poSI->m_bInternal = false;
            }
        }
    }
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (INSTANCEDDATA->m_oList[i]->m_bEnabled)
            Update((FEHandler)INSTANCEDDATA->m_oList[i],_rDeltaT);
    }
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Render(CFERenderer *_poRenderer)
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
    	CFESpriteMgrInst* poInst = INSTANCEDDATA->m_oList[i];

        if ( poInst->m_bEnabled && poInst->m_bIsRenderManaged)
        {
            CFESpriteRenderer::Render(  _poRenderer,
                                        poInst,
                                        poInst->m_oPos,
                                        poInst->m_rDepth,
                                        poInst->m_oScale,
                                        poInst->m_rAngle,
                                        poInst->m_oColor);
        }
    }
}
// ----------------------------------------------------------------------------
