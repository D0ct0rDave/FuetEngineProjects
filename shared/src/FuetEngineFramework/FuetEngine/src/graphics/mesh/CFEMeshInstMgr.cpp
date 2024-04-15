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
#include "CFEMeshInstMgr.h"
// #include "System/FMem.h"
#include "support/file/CFEFile.h"
#include "support/config/CFEConfigFile.h"
#include "support/graphics/CFERenderer.h"
#include "CFEMesh.h"
#include "CFEMeshRenderer.h"
#include "types/CFESingleton.h"

// ----------------------------------------------------------------------------
// handlers are instances
// #define MeshINST(handler) *((CFEMeshInst*)handler)
// handlers are indices
// #define MeshINST(handler)   m_oMeshDB[(uint)handler]
// ----------------------------------------------------------------------------
inline CFEMeshMgrInst& MeshINST(FEHandler _hHandler)
{
    return( *(CFEMeshMgrInst*)_hHandler );
}
// ----------------------------------------------------------------------------
class CFEMeshMgrInst : public CFEMeshInst
{
    public:
		CFEMeshMgrInst() :
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

        /// Is the current Mesh instance being played?
        FEBool m_bEnabled;

        /// Will the render be managed by the Mesh instance manager?
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
DECLARE_SINGLETON(CFEMeshInstMgr_DATA)
{
    public:
        CFEArray<CFEMeshMgrInst*> m_oList;
};

#define INSTANCEDDATA CFEMeshInstMgr_DATA::I()
*/
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::Init(uint _uiMaxInstances)
{
	m_oEnabledInsts.reserve(_uiMaxInstances);
	CFEInstanceMgr<CFEMeshInstMgr,CFEMeshMgrInst>::Init(_uiMaxInstances);
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::Finish()
{
	m_oEnabledInsts.clear();
	CFEInstanceMgr<CFEMeshInstMgr,CFEMeshMgrInst>::Finish();
}
// ----------------------------------------------------------------------------
CFEMeshMgrInst* CFEMeshInstMgr::poCreateInstance()
{
	return( new CFEMeshMgrInst() );
};
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::DestroyInstance(CFEMeshMgrInst* _poInstance)
{
	// WARNING: for some reason these are deleted elsewhere in the world!!!!!!!!
	delete _poInstance;
}
// ----------------------------------------------------------------------------
FEHandler CFEMeshInstMgr::hGetResource(const CFEString& _sResource)
{
	return( (FEHandler)CFEMeshMgr::I()->poLoad(_sResource) );
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::SetupInstance(CFEMeshMgrInst* _poInstance,FEHandler _hResource)
{
	CFEMesh* poMesh = (CFEMesh*)_hResource;

    _poInstance->m_poMesh = poMesh;
    _poInstance->m_bEnabled = false;
    _poInstance->m_oVX.reserve( poMesh->m_oUV.size() );
    _poInstance->m_oVX.insert(_poInstance->m_oVX.begin(),poMesh->m_oUV.size(),CFEVect2::ZERO() );

    if ((poMesh!=NULL) && (poMesh->poGetAction(0) != NULL))
        _poInstance->m_rActionTime = CFEMath::rRand() * poMesh->poGetAction(0)->m_rRandStartTime;
    else
        _poInstance->m_rActionTime = _0r;

    _poInstance->m_rAngle = _0r;
    _poInstance->m_oColor = CFEColor(_1r,_1r,_1r,_1r);
    _poInstance->m_oPos = CFEVect2::ZERO();
    _poInstance->m_rDepth = _0r;
    _poInstance->m_oScale= CFEVect2::ONE();
    _poInstance->m_uiMeshAction = 0;
    _poInstance->m_uiCurrentActionFrame = 0;
    _poInstance->m_rSpeedMult= _1r;
    _poInstance->m_bInternal = false;
	_poInstance->m_bIsRenderManaged = true;
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::InvalidateInstance(CFEMeshMgrInst* _poInstance)
{
	if (_poInstance->m_bEnabled)
		Disable(_poInstance);
}
// ----------------------------------------------------------------------------
/// Sets the position of a given Mesh instance.
void CFEMeshInstMgr::SetPos(FEHandler _hInstance,const CFEVect2& _oPos)
{
    if (_hInstance == NULL) return;
	MeshINST(_hInstance).m_oPos = _oPos;
}
// ----------------------------------------------------------------------------
/// Gets the position of a given Mesh instance.
const CFEVect2& CFEMeshInstMgr::oGetPos(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2::ZERO());
	return MeshINST(_hInstance).m_oPos;
}
// ----------------------------------------------------------------------------
/// Sets the depth of a given Mesh instance.
void CFEMeshInstMgr::SetDepth(FEHandler _hInstance,const FEReal& _rDepth)
{
    if (_hInstance == NULL) return;
	MeshINST(_hInstance).m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
/// Gets the position of a given Mesh instance.
FEReal CFEMeshInstMgr::rGetDepth(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return MeshINST(_hInstance).m_rDepth;
}
// ----------------------------------------------------------------------------
/// Sets the scale of a given Mesh instance the same amount in width and height.
void CFEMeshInstMgr::SetScale(FEHandler _hInstance,const CFEVect2& _oScale)
{
    if (_hInstance == NULL) return;
    MeshINST(_hInstance).m_oScale = _oScale;
}
// ----------------------------------------------------------------------------
/// Sets the scale of a given Mesh instance.
void CFEMeshInstMgr::SetScale(FEHandler _hInstance,const FEReal& _rScale)
{
    if (_hInstance == NULL) return;
    MeshINST(_hInstance).m_oScale.x = _rScale;
    MeshINST(_hInstance).m_oScale.y = _rScale;
}
// ----------------------------------------------------------------------------
/// Retrieves the scale of a given Mesh instance.
const CFEVect2& CFEMeshInstMgr::oGetScale(FEHandler _hInstance)
{
    if (_hInstance == NULL) return( CFEVect2::ONE() );
	return(MeshINST(_hInstance).m_oScale);
}
// ----------------------------------------------------------------------------
/// Sets the angle of a given Mesh instance.
void CFEMeshInstMgr::SetAngle(FEHandler _hInstance,const FEReal& _rAngle)
{
    if (_hInstance == NULL) return;
    MeshINST(_hInstance).m_rAngle = _rAngle;
}
// ----------------------------------------------------------------------------
/// Retrieves the current angle of a given Mesh instance.
FEReal CFEMeshInstMgr::rGetAngle(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( MeshINST(_hInstance).m_rAngle );
}
// ----------------------------------------------------------------------------
/// Sets the color of a given Mesh instance.
void CFEMeshInstMgr::SetColor(FEHandler _hInstance,const CFEColor& _oColor)
{
    if (_hInstance == NULL) return;
    MeshINST(_hInstance).m_oColor = _oColor;
}
// ----------------------------------------------------------------------------
/// Retrieves the color of a given Mesh instance.

const CFEColor& CFEMeshInstMgr::oGetColor(FEHandler _hInstance)
{
    static CFEColor soWhite(_1r,_1r,_1r,_1r);
    if (_hInstance == NULL) return(soWhite);
	return MeshINST(_hInstance).m_oColor;
}
// ----------------------------------------------------------------------------
/// Sets the current action of a given Mesh instance.
void CFEMeshInstMgr::SetAction(FEHandler _hInstance,uint _uiAction)
{
    if (_hInstance == NULL) return;
	CFEMeshAction* poAction = MeshINST(_hInstance).m_poMesh->poGetAction(_uiAction);
	if (poAction == NULL) return;

    MeshINST(_hInstance).m_uiMeshAction			= _uiAction;
    MeshINST(_hInstance).m_rActionTime			= CFEMath::rRand() * poAction->m_rRandStartTime;

    if (poAction->m_oSeq.size() > 0)
	    MeshINST(_hInstance).m_uiCurrentActionFrame	= poAction->uiGetFrame( MeshINST(_hInstance).m_rActionTime );
	else
		MeshINST(_hInstance).m_uiCurrentActionFrame	= 0;
}
// ----------------------------------------------------------------------------
/// Sets the current action of a given Mesh instance.
void CFEMeshInstMgr::SetAction(FEHandler _hInstance,const CFEString& _sActionName)
{
    if (_hInstance == NULL) return;
	int iActionIdx = MeshINST(_hInstance).m_poMesh->iGetActionIdx(_sActionName);
	if (iActionIdx == -1) return;

	SetAction(_hInstance,iActionIdx);
}
// ----------------------------------------------------------------------------
uint CFEMeshInstMgr::uiGetAction(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(0);
	return( MeshINST(_hInstance).m_uiMeshAction );
}
// ----------------------------------------------------------------------------
/// Sets the speed multiplier for a given Mesh instance.
void CFEMeshInstMgr::SetSpeedMult(FEHandler _hInstance,const FEReal& _rSpeedMult)
{
    if (_hInstance == NULL) return;
	MeshINST(_hInstance).m_rSpeedMult = _rSpeedMult;
}
// ----------------------------------------------------------------------------
/// Retrieves the speed multiplier of a given Mesh instance.
FEReal CFEMeshInstMgr::rGetSpeedMult(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( MeshINST(_hInstance).m_rSpeedMult );
}
// ----------------------------------------------------------------------------
/// Enables the Mesh instance.
void CFEMeshInstMgr::Enable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    if (! MeshINST(_hInstance).m_bEnabled)
    {
		MeshINST(_hInstance).m_bEnabled = true;
		m_oEnabledInsts.push_back( &MeshINST(_hInstance) );
	}
}
// ----------------------------------------------------------------------------
/// Disables the given Mesh instance.
void CFEMeshInstMgr::Disable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    if (MeshINST(_hInstance).m_bEnabled)
    {
		MeshINST(_hInstance).m_bEnabled = false;
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
/// Retrieves whether this Mesh is enabled or not.
FEBool CFEMeshInstMgr::bIsEnabled(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
    return( MeshINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
/// Retrieves whether this Mesh is still playing or has stopped (only for one-shot).
FEBool CFEMeshInstMgr::bIsPlaying(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	CFEMeshMgrInst* poSI    = &MeshINST(_hInstance);
	CFEMeshAction* poAction = (poSI->m_poMesh->m_poActions[ poSI->m_uiMeshAction ]);

	if ((poAction->m_ePlayMode == MAPM_ONESHOT) || (poAction->m_ePlayMode == MAPM_PINGPONGSTOP))
		return( poSI->m_rActionTime < poAction->m_rActionTime );
	else
		return( false );
}
// ----------------------------------------------------------------------------
/// Retrieves the time in the current action this Mesh instance is being played.
FEReal CFEMeshInstMgr::rGetCurrentActionTime(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
    return( MeshINST(_hInstance).m_rActionTime );
}
// ----------------------------------------------------------------------------
/// Sets the time in the current action this Mesh instance will be played.
void CFEMeshInstMgr::SetCurrentActionTime(FEHandler _hInstance,const FEReal& _rActionTime)
{
	if (_hInstance == NULL) return;
	CFEMeshMgrInst* poSI		= &MeshINST(_hInstance);

	if (poSI->m_poMesh == NULL) return;

	CFEMeshAction* poAction	= (poSI->m_poMesh->m_poActions[ poSI->m_uiMeshAction ]);
	FEReal rNewTime				= _rActionTime;

	// To prevent overflows
	switch (poAction->eGetPlayMode())
    {
		case MAPM_ONESHOT:
		rNewTime = CFEMath::rMin(rNewTime,poAction->rGetMaxActionTime());
		break;

		case MAPM_LOOP:
		rNewTime = CFEMath::rMod(rNewTime,poAction->rGetMaxActionTime());
		break;

		case MAPM_PINGPONGSTOP:
		rNewTime = CFEMath::rMin(rNewTime,_2r*poAction->rGetMaxActionTime()+_001r);
		break;

		case MAPM_PINGPONG:
		rNewTime = CFEMath::rMod(rNewTime,_2r*poAction->rGetMaxActionTime());
		break;
    }

	poSI->m_rActionTime				= rNewTime;
	poSI->m_uiCurrentActionFrame	= poAction->uiGetFrame( rNewTime );
}
// ----------------------------------------------------------------------------
/// Retrieves the current frame this Mesh instance is playing.
CFEMeshFrame* CFEMeshInstMgr::poGetCurrentFrame(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);

    CFEMeshMgrInst* poSI    = &MeshINST(_hInstance);
    CFEMeshAction* poAction = poSI->m_poMesh->m_poActions[ poSI->m_uiMeshAction ];

    return( poAction->m_oSeq[ poSI->m_uiCurrentActionFrame ] );
}
// ----------------------------------------------------------------------------
/// Retrieves the current frame index this Mesh instance is playing.
uint CFEMeshInstMgr::uiGetCurrentFrame(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(0);
    return( MeshINST(_hInstance).m_uiCurrentActionFrame);
}
// ----------------------------------------------------------------------------
/// Retrieves the Mesh model used by the given Mesh instance.
CFEMesh* CFEMeshInstMgr::poGetMesh(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);
    return( MeshINST(_hInstance).m_poMesh );
}
// ----------------------------------------------------------------------------
/// Sets whether the render will be managed by the instance manager or not.
void CFEMeshInstMgr::ManageRender(FEHandler _hInstance,FEBool _bManageRender)
{
    if (_hInstance == NULL) return;
	MeshINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------
/// Tells whether the render is managed by the instance manager or not.
FEBool CFEMeshInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	return( MeshINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
/// Performs an update step over the given Mesh instance, also in case it's disabled.
void CFEMeshInstMgr::Update(CFEMeshMgrInst* _poInstance,const FEReal& _rDeltaT)
{
	SetCurrentActionTime(_poInstance,_poInstance->m_rActionTime+_rDeltaT*_poInstance->m_rSpeedMult);
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::Update(FEHandler _hInstance,const FEReal& _rDeltaT)
{
    if (_hInstance == NULL) return;

    CFEMeshMgrInst* poSI    = &MeshINST(_hInstance);
    Update(poSI,_rDeltaT);

	// Check if this instance should be disabled or not
    if (poSI->m_bInternal)
    {
        CFEMeshAction* poAction = poSI->m_poMesh->m_poActions[ poSI->m_uiMeshAction ];

        if ((poAction->m_ePlayMode != MAPM_PINGPONG) && (poAction->m_ePlayMode != MAPM_LOOP))
        {
            if (
                ((poAction->m_ePlayMode == MAPM_ONESHOT) && (poSI->m_rActionTime >= poAction->m_rActionTime))

                /*
				||
                ((poAction->m_ePlayMode == MAPM_PINGPONGSTOP) && (rFrame >= (2*poAction->m_oSeq.size())) )
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
void CFEMeshInstMgr::Update(const FEReal& _rDeltaT)
{
	for (uint i=0;i<m_oEnabledInsts.size();i++)
	{
		CFEMeshMgrInst* poInst = m_oEnabledInsts[i];
		if (poInst->m_bIsRenderManaged)
		{
			Update(poInst,_rDeltaT);
		}
	}
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::Render(CFERenderer *_poRenderer)
{
	for (uint i=0;i<m_oEnabledInsts.size();i++)
	{
		CFEMeshMgrInst* poInst = m_oEnabledInsts[i];
		if (poInst->m_bIsRenderManaged)
		{
			Render(poInst,_poRenderer);
		}
	}
}
// ----------------------------------------------------------------------------
void CFEMeshInstMgr::Render(FEHandler _hInstance,CFERenderer *_poRenderer)
{
	if (_hInstance == NULL) return;

	CFEMeshMgrInst* poInst = &MeshINST(_hInstance);
    CFEMeshRenderer::Render(_poRenderer,
                              poInst,
                              poInst->m_oPos,
                              poInst->m_rDepth,
                              poInst->m_oScale,
                              poInst->m_rAngle,
                              poInst->m_oColor);
}
// ----------------------------------------------------------------------------
