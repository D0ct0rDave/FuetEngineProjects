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
#include "CFEParticleSysMgr.h"
#include "CFEParticleSysInstMgr.h"
#include "CFEParticleMgr.h"

// #include "System/FMem.h"
#include "Support/File/CFEFile.h"
#include "Support/Config/CFEConfigFile.h"
#include "Support/Graphics/CFERenderer.h"
#include "types/CFESingleton.h"
#include "CFEParticleSys.h"
// ----------------------------------------------------------------------------
class CFEParticleSysMgrInst : public CFEParticleSysInst
{
    public:
		CFEParticleSysMgrInst() : 
			m_bEnabled(false), 
			m_bIsRenderManaged(true),
			m_oPos(CFEVect2::ZERO()),
			m_oSpeed(CFEVect2::ZERO()),
			m_rDepth(_0r),
			m_bInternal(false),
			m_rAccParts(_0r)
		{

		}

        /// Is the current ParticleSys instance being played?
        FEBool m_bEnabled;

        /// Will the render be managed by the ParticleSys instance manager?
        FEBool m_bIsRenderManaged;

        /// Position of this instance.
        CFEVect2 m_oPos;

        /// Speed of this emitter.
        CFEVect2 m_oSpeed;

        /// Depth of this instance.
        FEReal m_rDepth;

        /// This instance is handled internally.
        FEBool m_bInternal;

        /// Number of accumulated particles
        FEReal  m_rAccParts;
};
// ----------------------------------------------------------------------------
// handlers are instances
// #define PSINST(handler) *((CFEPSINST*)handler)
// handlers are indices
// #define PSINST(handler)   m_oParticleSysDB[(uint)handler]
// ----------------------------------------------------------------------------
inline CFEParticleSysMgrInst& PSINST(FEHandler _hHandler)
{
    return( *(CFEParticleSysMgrInst*)_hHandler );
}
// ----------------------------------------------------------------------------
FEHandler CFEParticleSysInstMgr::hGetResource(const CFEString& _sResource)
{
	return( (FEHandler)CFEParticleSysMgr::I()->poLoad(_sResource) );
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::SetupInstance(CFEParticleSysMgrInst* _poInstance,FEHandler _hResource)
{
	_poInstance->m_poPS      = (CFEParticleSys*)_hResource;	
	
	_poInstance->m_oPos      = CFEVect2::ZERO();
	_poInstance->m_oSpeed	 = CFEVect2::ZERO();
	_poInstance->m_rDepth    = _0r;
	_poInstance->m_rLifeTime = _0r;
	_poInstance->m_bInternal = false;

	_poInstance->m_bIsRenderManaged = true;
	_poInstance->m_rAccParts = _0r;

	_poInstance->m_bEnabled  = false;
}
// ----------------------------------------------------------------------------
CFEParticleSysMgrInst* CFEParticleSysInstMgr::poCreateInstance()
{
	return( new CFEParticleSysMgrInst );
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::DestroyInstance(CFEParticleSysMgrInst* _poInstance)
{
	delete _poInstance;
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(const CFEString& _sParticleSysModel,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth)
{
    FEHandler hHandler = hGetInstance(_sParticleSysModel);
    if (hHandler != NULL) Spawn(hHandler,_rLifeTime,_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(CFEParticleSys* _poPS,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth)
{
    FEHandler hHandler = hGetInstance(_poPS);
    if (hHandler != NULL) Spawn(hHandler,_rLifeTime,_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(FEHandler _hInst,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth)
{
    CFEParticleSysMgrInst* poInst = (CFEParticleSysMgrInst*)_hInst;

    poInst->m_oPos      = _oPos;
	poInst->m_oSpeed		= CFEVect2::ZERO();
    poInst->m_rDepth    = _rDepth;
    poInst->m_rLifeTime = _rLifeTime;
    poInst->m_bInternal = true;

	poInst->m_bEnabled  = true;
}
// ----------------------------------------------------------------------------
/// Spawns a ParticleSys automanaged by the manager.
void CFEParticleSysInstMgr::SpawnParticle(const CFEString& _sParticleSysModel,const CFEVect2& _oPos,const FEReal& _rDepth)
{
    // Retrieve the ParticleSys resource
    SpawnParticle(CFEParticleSysMgr::I()->poLoad(_sParticleSysModel),_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
/// Spawns a ParticleSys automanaged by the manager.
void CFEParticleSysInstMgr::SpawnParticle(CFEParticleSys* _poPS,const CFEVect2& _oPos,const FEReal& _rDepth)
{
	// Check if the ParticleSys object exists.
    if (_poPS == NULL) return;

	//
    CFEParticleMgr::CreateParticles(1,_oPos,_rDepth,CFEVect2::ZERO(),_poPS,NULL);
}
// ----------------------------------------------------------------------------
/// Gets the poPSItion of a given ParticleSys instance.
const CFEVect2& CFEParticleSysInstMgr::oGetPos(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2::ZERO());
	return PSINST(_hInstance).m_oPos;
}// ----------------------------------------------------------------------------
/// Sets the poPSItion of a given ParticleSys instance.
void CFEParticleSysInstMgr::SetPos(FEHandler _hInstance,const CFEVect2& _oPos)
{
    if (_hInstance == NULL) return;
	PSINST(_hInstance).m_oPos = _oPos;
}
// ----------------------------------------------------------------------------
/// Sets the depth of a given ParticleSys instance.
void CFEParticleSysInstMgr::SetDepth(FEHandler _hInstance,const FEReal& _rDepth)
{
    if (_hInstance == NULL) return;
	PSINST(_hInstance).m_rDepth = _rDepth;
}
// ----------------------------------------------------------------------------
/// Gets the depth of a given ParticleSys instance.
FEReal CFEParticleSysInstMgr::rGetDepth(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(_0r);
	return( PSINST(_hInstance).m_rDepth );
}
// ----------------------------------------------------------------------------
/// Sets the particle system emitter speed.
void CFEParticleSysInstMgr::SetSpeed(FEHandler _hInstance,const CFEVect2& _oSpeed)
{
	if (_hInstance == NULL) return;
	PSINST(_hInstance).m_oSpeed = _oSpeed;
}
// ----------------------------------------------------------------------------
/// Gets the particle system emitter speed.
const CFEVect2& CFEParticleSysInstMgr::oGetSpeed(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2::ZERO());
	return( PSINST(_hInstance).m_oSpeed);
}
// ----------------------------------------------------------------------------
/// Enables the ParticleSys instance.
void CFEParticleSysInstMgr::Enable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    PSINST(_hInstance).m_bEnabled = true;
}
// ----------------------------------------------------------------------------
/// Disables the given ParticleSys instance.
void CFEParticleSysInstMgr::Disable(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    PSINST(_hInstance).m_bEnabled = false;
}
// ----------------------------------------------------------------------------
/// Retrieves whether this ParticleSys is enabled or not.
FEBool CFEParticleSysInstMgr::bIsEnabled(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
    return( PSINST(_hInstance).m_bEnabled );
}
// ----------------------------------------------------------------------------
/// Retrieves the ParticleSys model used by the given ParticleSys instance.
CFEParticleSys* CFEParticleSysInstMgr::poGetParticleSys(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(NULL);
    return( PSINST(_hInstance).m_poPS );
}
// ----------------------------------------------------------------------------
/// Sets whether the render will be managed by the instance manager or not.
void CFEParticleSysInstMgr::ManageRender(FEHandler _hInstance,FEBool _bManageRender)
{
    if (_hInstance == NULL) return;
	PSINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------
/// Tells whether the render is managed by the instance manager or not.
FEBool CFEParticleSysInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	return( PSINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Update(FEHandler _hInstance,const FEReal& _rDeltaT)
{
    if (_hInstance == NULL) return;

    CFEParticleSysMgrInst* poPSI = &PSINST(_hInstance);
    if ((poPSI->m_bInternal) && (poPSI->m_rLifeTime!=PSYS_INFINITE_LIFE))
    {
        poPSI->m_rLifeTime -= _rDeltaT;

        if (poPSI->m_rLifeTime <= _0r)
        {
            ReleaseInstance(_hInstance);
            return;
        }
    }

    FEReal rNumParts = poPSI->m_poPS->m_rFreq*_rDeltaT + poPSI->m_rAccParts;
    uint uiNumParts = (uint)rNumParts;

    if (uiNumParts>0)
        CFEParticleMgr::CreateParticles(uiNumParts,poPSI->m_oPos,poPSI->m_rDepth,poPSI->m_oSpeed,poPSI->m_poPS,_hInstance);

    // update accumulation of particles.
    poPSI->m_rAccParts = rNumParts - FEReal(uiNumParts);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Update(const FEReal& _rDeltaT)
{
    for (uint i=0;i<uiGetMaxInstances();i++)
    {
		CFEParticleSysMgrInst* poInst = poGetResourceInstance(i);

        if ((! bIsInstanceFree(i)) && (poInst->m_bEnabled) && (poInst->m_bIsRenderManaged))
            Update((FEHandler)poInst,_rDeltaT);
    }

    CFEParticleMgr::Update(_rDeltaT);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Render(CFERenderer *_poRenderer)
{
	CFEParticleMgr::Render(_poRenderer,NULL);
}
// ----------------------------------------------------------------------------
