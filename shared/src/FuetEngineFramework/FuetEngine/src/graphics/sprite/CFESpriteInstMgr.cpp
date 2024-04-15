// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#define SPRITEINST(_hInstance) (* (reinterpret_cast<CFESpriteMgrInst*>(_hInstance)))
/*
inline CFESpriteMgrInst& SPRITEINST(FEHandler _hHandler)
{
    return( *(CFESpriteMgrInst*)_hHandler );
}
*/
// ----------------------------------------------------------------------------
class CFESpriteMgrInst : public CFESpriteInst
{
    public:
		CFESpriteMgrInst() :
			m_bEnabled(false),
			m_bIsRenderManaged(false),
			m_oPos(CFEVect2::ZERO()),
			m_rDepth(_0r),
			m_oScale(CFEVect2::ONE()),
			m_rAngle(_0r),
			m_oColor(CFEColor::WHITE()),
			m_rSpeedMult(_1r),
			m_bInternal(false)
		{
		}

        /// Is the current sprite instance being played?
        FEBool m_bEnabled;

        /// Will the render be managed by the sprite instance manager?
        FEBool m_bIsRenderManaged;

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
        FEBool m_bInternal;
};
// ----------------------------------------------------------------------------
// INSTANCEDDATA to store all the global DATOS.
// ----------------------------------------------------------------------------
/*
DECLARE_SINGLETON(CFESpriteInstMgr_DATA)
{
    public:
        CFEArray<CFESpriteMgrInst*> m_oList;
};

#define INSTANCEDDATA CFESpriteInstMgr_DATA::I()
*/
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Init(uint _uiMaxInstances)
{
	m_oEnabledInsts.reserve(_uiMaxInstances);
	CFEInstanceMgr<CFESpriteInstMgr,CFESpriteMgrInst>::Init(_uiMaxInstances);
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Finish()
{
	m_oEnabledInsts.clear();
	CFEInstanceMgr<CFESpriteInstMgr,CFESpriteMgrInst>::Finish();
}
// ----------------------------------------------------------------------------
CFESpriteMgrInst* CFESpriteInstMgr::poCreateInstance()
{
	return( new CFESpriteMgrInst() );
};
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::DestroyInstance(CFESpriteMgrInst* _poInstance)
{
	// WARNING: for some reason these are deleted elsewhere in the world!!!!!!!!
	delete _poInstance;
}
// ----------------------------------------------------------------------------
FEHandler CFESpriteInstMgr::hGetResource(const CFEString& _sResource)
{
	return( (FEHandler)CFESpriteMgr::I()->poLoad(_sResource) );
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::SetupInstance(CFESpriteMgrInst* _poInstance,FEHandler _hResource)
{
	CFESprite* poSprite = (CFESprite*)_hResource;

    _poInstance->m_poSprite = poSprite;
    _poInstance->m_bEnabled = false;

    if ((poSprite!=NULL) && (poSprite->poGetAction(0) != NULL))
        _poInstance->m_rActionTime = CFEMath::rRand() * poSprite->poGetAction(0)->m_rRandStartTime;
    else
        _poInstance->m_rActionTime = _0r;

    _poInstance->m_rAngle = _0r;
    _poInstance->m_oColor = CFEColor(_1r,_1r,_1r,_1r);
    _poInstance->m_oPos = CFEVect2::ZERO();
    _poInstance->m_rDepth = _0r;
    _poInstance->m_oScale= CFEVect2::ONE();
    _poInstance->m_uiSpriteAction = 0;
    _poInstance->m_uiCurrentActionFrame = 0;
    _poInstance->m_rSpeedMult= _1r;
    _poInstance->m_bInternal = false;
	_poInstance->m_bIsRenderManaged = true;
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::InvalidateInstance(CFESpriteMgrInst* _poInstance)
{
	if (_poInstance->m_bEnabled)
		Disable(_poInstance);
}
// ----------------------------------------------------------------------------
/// Spawns a sprite automanaged by the manager.
void CFESpriteInstMgr::Spawn(const CFEString& _sSpriteModel,uint _uiAction,const CFEVect2& _oPos,const FEReal& _rDepth,const FEReal& _rScale,const FEReal& _rAngle,const CFEColor& _oColor)
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
        poInst->m_bInternal = true;

		Enable((FEHandler)poInst);
    }
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
const CFEVect2& CFESpriteInstMgr::oGetPos(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2::ZERO());
	return SPRITEINST(_hInstance).m_oPos;
}
// ----------------------------------------------------------------------------
/// Sets the depth of a given sprite instance.
void CFESpriteInstMgr::SetDepth(FEHandler _hInstance,const FEReal& _rDepth)
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
void CFESpriteInstMgr::SetScale(FEHandler _hInstance,const FEReal& _rScale)
{
    if (_hInstance == NULL) return;
    SPRITEINST(_hInstance).m_oScale.x = _rScale;
    SPRITEINST(_hInstance).m_oScale.y = _rScale;
}
// ----------------------------------------------------------------------------
/// Retrieves the scale of a given sprite instance.
const CFEVect2& CFESpriteInstMgr::oGetScale(FEHandler _hInstance)
{
    if (_hInstance == NULL) return( CFEVect2::ONE() );
	return(SPRITEINST(_hInstance).m_oScale);
}
// ----------------------------------------------------------------------------
/// Sets the angle of a given sprite instance.
void CFESpriteInstMgr::SetAngle(FEHandler _hInstance,const FEReal& _rAngle)
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

const CFEColor& CFESpriteInstMgr::oGetColor(FEHandler _hInstance)
{
    static CFEColor soWhite(_1r,_1r,_1r,_1r);
    if (_hInstance == NULL) return(soWhite);
	return SPRITEINST(_hInstance).m_oColor;
}
// ----------------------------------------------------------------------------
/// Sets the current action of a given sprite instance.
void CFESpriteInstMgr::SetAction(FEHandler _hInstance,uint _uiAction)
{
    if (_hInstance == NULL) return;
	CFESpriteAction* poAction = SPRITEINST(_hInstance).m_poSprite->poGetAction(_uiAction);
	if (poAction == NULL) return;

    SPRITEINST(_hInstance).m_uiSpriteAction			= _uiAction;
    SPRITEINST(_hInstance).m_rActionTime			= CFEMath::rRand() * poAction->m_rRandStartTime;

    if (poAction->m_oSeq.size() > 0)
	    SPRITEINST(_hInstance).m_uiCurrentActionFrame	= poAction->uiGetFrame( SPRITEINST(_hInstance).m_rActionTime );
	else
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
void CFESpriteInstMgr::SetSpeedMult(FEHandler _hInstance,const FEReal& _rSpeedMult)
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
    if (! SPRITEINST(_hInstance).m_bEnabled)
    {
		SPRITEINST(_hInstance).m_bEnabled = true;
		m_oEnabledInsts.push_back( &SPRITEINST(_hInstance) );
	}
}
// ----------------------------------------------------------------------------
/// Disables the given sprite instance.
void CFESpriteInstMgr::Disable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    if (SPRITEINST(_hInstance).m_bEnabled)
    {
		SPRITEINST(_hInstance).m_bEnabled = false;
		for (uint i=0;i<m_oEnabledInsts.size();i++)
		{
			if (m_oEnabledInsts[i] == _hInstance)
			{
				m_oEnabledInsts.Delete(i);
				break;
			}
		}
	}
}
// ----------------------------------------------------------------------------
/// Retrieves whether this sprite is enabled or not.
FEBool CFESpriteInstMgr::bIsEnabled(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
    return( SPRITEINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
/// Retrieves whether this sprite is still playing or has stopped (only for one-shot).
FEBool CFESpriteInstMgr::bIsPlaying(FEHandler _hInstance)
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
/// Retrieves the time in the current action this sprite instance is being played.
FEReal CFESpriteInstMgr::rGetCurrentActionTime(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
    return( SPRITEINST(_hInstance).m_rActionTime );
}
// ----------------------------------------------------------------------------
/// Retrieves the random start time value for the current action this sprite instance is playing.
FEReal CFESpriteInstMgr::rGetCurrentActionRandomStartTime(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
    if (SPRITEINST(_hInstance).m_poSprite == NULL) return(_0r);

    CFESpriteAction* poAction = SPRITEINST(_hInstance).m_poSprite->poGetAction( SPRITEINST(_hInstance).m_uiSpriteAction );
    
    if (poAction == NULL) return(_0r);
	return (  poAction->m_rRandStartTime );
}
// ----------------------------------------------------------------------------
/// Sets the time in the current action this sprite instance will be played.
void CFESpriteInstMgr::SetCurrentActionTime(FEHandler _hInstance,const FEReal& _rActionTime)
{
	if (_hInstance == NULL) return;
	CFESpriteMgrInst* poSI		= &SPRITEINST(_hInstance);

	if (poSI->m_poSprite == NULL) return;

	CFESpriteAction* poAction	= &(poSI->m_poSprite->m_oActions[ poSI->m_uiSpriteAction ]);
	FEReal rNewTime				= _rActionTime;

	// To prevent overflows
	switch (poAction->eGetPlayMode())
    {
		case SFSPM_ONESHOT:
		rNewTime = CFEMath::rMin(rNewTime,poAction->rGetMaxActionTime());
		break;

		case SFSPM_LOOP:
		{
			if (poAction->m_rRandStartTime>_0r)
			{
				rNewTime = CFEMath::rMod(rNewTime,poAction->rGetMaxActionTime());

				if (rNewTime<_rActionTime)
					//loop detected
					rNewTime = rNewTime + CFEMath::rRand() * poAction->m_rRandStartTime;
			}
			else
				rNewTime = CFEMath::rMod(rNewTime,poAction->rGetMaxActionTime());
		}
		break;

		case SFSPM_PINGPONGSTOP:
		rNewTime = CFEMath::rMin(rNewTime,_2r*poAction->rGetMaxActionTime()+_001r);
		break;

		case SFSPM_PINGPONG:
		rNewTime = CFEMath::rMod(rNewTime,_2r*poAction->rGetMaxActionTime());
		break;
    }

	poSI->m_rActionTime				= rNewTime;
	poSI->m_uiCurrentActionFrame	= poAction->uiGetFrame( rNewTime );
}
// ----------------------------------------------------------------------------
/// Retrieves the current frame this sprite instance is playing.
CFESpriteFrame* CFESpriteInstMgr::poGetCurrentFrame(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);

    CFESpriteMgrInst* poSI    = &SPRITEINST(_hInstance);
    CFESpriteAction* poAction = &(poSI->m_poSprite->m_oActions[ poSI->m_uiSpriteAction ]);

    return( &poAction->m_oSeq[ poSI->m_uiCurrentActionFrame ] );
}
// ----------------------------------------------------------------------------
/// Retrieves the current frame index this sprite instance is playing.
uint CFESpriteInstMgr::uiGetCurrentFrame(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(0);
    return( SPRITEINST(_hInstance).m_uiCurrentActionFrame);
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
void CFESpriteInstMgr::ManageRender(FEHandler _hInstance,FEBool _bManageRender)
{
    if (_hInstance == NULL) return;
	SPRITEINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------
/// Tells whether the render is managed by the instance manager or not.
FEBool CFESpriteInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	return( SPRITEINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
/// Performs an update step over the given sprite instance, also in case it's disabled.
void CFESpriteInstMgr::Update(CFESpriteMgrInst* _poInstance,const FEReal& _rDeltaT)
{
	if (_poInstance == NULL) return;
	SetCurrentActionTime(_poInstance,_poInstance->m_rActionTime+_rDeltaT*_poInstance->m_rSpeedMult);
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Update(FEHandler _hInstance,const FEReal& _rDeltaT)
{
    if (_hInstance == NULL) return;

    CFESpriteMgrInst* poSI    = &SPRITEINST(_hInstance);
    Update(poSI,_rDeltaT);

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
                // #pragma message(__FUNCTION__"/"__FILE__)
                #pragma message("#################")
                #pragma message("To be implemented")
                #pragma message("#################")
            {
                poSI->m_bEnabled  = false;
                poSI->m_bInternal = false;
            }
        }
    }
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Update(const FEReal& _rDeltaT)
{
	for (uint i=0;i<m_oEnabledInsts.size();i++)
	{
		CFESpriteMgrInst* poInst = m_oEnabledInsts[i];
		if (poInst->m_bIsRenderManaged)
		{
			Update(m_oEnabledInsts[i],_rDeltaT);
		}
	}
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Render(CFERenderer *_poRenderer)
{
	for (uint i=0;i<m_oEnabledInsts.size();i++)
	{
		CFESpriteMgrInst* poInst = m_oEnabledInsts[i];
		if (poInst->m_bIsRenderManaged)
		{
			Render(poInst,_poRenderer);
		}
	}
}
// ----------------------------------------------------------------------------
void CFESpriteInstMgr::Render(FEHandler _hInstance,CFERenderer *_poRenderer)
{
	if (_hInstance == NULL) return;

	CFESpriteMgrInst* poInst = &SPRITEINST(_hInstance);
	CFESpriteRenderer::Render(_poRenderer,
							  poInst,
							  poInst->m_oPos,
							  poInst->m_rDepth,
							  poInst->m_oScale,
							  poInst->m_rAngle,
							  poInst->m_oColor);
}
// ----------------------------------------------------------------------------
