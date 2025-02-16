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

        /// Is the current ParticleSys instance being played?
        bool m_bEnabled;

        /// Will the render be managed by the ParticleSys instance manager?
        bool m_bIsRenderManaged;

        /// Position of this instance.
        CFEVect2 m_oPos;

        /// Depth of this instance.
        FEReal m_rDepth;

        /// This instance is handled internally.
        bool m_bInternal;

        /// Sets the instance as free to be used later.
        bool m_bFree;

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
DECLARE_SINGLETON(CFEParticleSysInstMgr_DATA)
{
    public:
        CFEArray<CFEParticleSysMgrInst*> m_oList;
};
#define INSTANCEDDATA CFEParticleSysInstMgr_DATA::I()
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Init(uint _uiMaxPSINSTances)
{
    for (uint i=0;i<_uiMaxPSINSTances;i++)
    {
        CFEParticleSysMgrInst* poInst = new CFEParticleSysMgrInst;
        INSTANCEDDATA->m_oList.push_back(poInst);
    }

    Reset();
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Finish()
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
        delete INSTANCEDDATA->m_oList[i];

    INSTANCEDDATA->m_oList.clear();
    INSTANCEDDATA->Finish();
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Reset()
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        INSTANCEDDATA->m_oList[i]->m_bFree     = true;
        INSTANCEDDATA->m_oList[i]->m_bEnabled  = false;
        INSTANCEDDATA->m_oList[i]->m_bInternal = false;
        INSTANCEDDATA->m_oList[i]->m_bIsRenderManaged = true;
    }
}
// ----------------------------------------------------------------------------
/// Forces the loading of a ParticleSys resource.
void CFEParticleSysInstMgr::Load(const CFEString& _sParticleSysModel)
{
    CFEParticleSysMgr::poLoad(_sParticleSysModel);
}
// ----------------------------------------------------------------------------
/// Retrieves a ParticleSys instance of a given ParticleSys resource.
FEHandler CFEParticleSysInstMgr::hGetInstance(const CFEString& _sParticleSysModel)
{
    // Retrieve the ParticleSys resource
    return ( hGetInstance( CFEParticleSysMgr::poLoad(_sParticleSysModel)) );
}
// ----------------------------------------------------------------------------
/// Retrieves a ParticleSys instance of a given ParticleSys object.
FEHandler CFEParticleSysInstMgr::hGetInstance(CFEParticleSys* _poParticleSys)
{
	// Check if the ParticleSys object exists.
    if (_poParticleSys == NULL) return(NULL);

    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (INSTANCEDDATA->m_oList[i]->m_bFree)
        {
            CFEParticleSysMgrInst* poInst = INSTANCEDDATA->m_oList[i];
            poInst->m_poPS      = _poParticleSys;
            poInst->m_bFree     = false;
            poInst->m_bEnabled  = false;
            poInst->m_oPos      = CFEVect2::oZERO();
            poInst->m_rDepth    = _0r;
            poInst->m_bInternal = false;
			poInst->m_bIsRenderManaged = true;
            poInst->m_rAccParts = _0r;
            poInst->m_rLifeTime = _0r;

            return((FEHandler)poInst);    // we can also return the index in case.
        }
    }

    return(NULL);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(const CFEString& _sParticleSysModel,FEReal _rLifeTime,const CFEVect2& _oPos,FEReal _rDepth)
{
    FEHandler hHandler = hGetInstance(_sParticleSysModel);
    if (hHandler != NULL) Spawn(hHandler,_rLifeTime,_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(CFEParticleSys* _poPS,FEReal _rLifeTime,const CFEVect2& _oPos,FEReal _rDepth)
{
    FEHandler hHandler = hGetInstance(_poPS);
    if (hHandler != NULL) Spawn(hHandler,_rLifeTime,_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Spawn(FEHandler _hInst,FEReal _rLifeTime,const CFEVect2& _oPos,FEReal _rDepth)
{
    CFEParticleSysMgrInst* poInst = (CFEParticleSysMgrInst*)_hInst;

    poInst->m_oPos      = _oPos;
    poInst->m_rDepth    = _rDepth;
    poInst->m_rLifeTime = _rLifeTime;
    poInst->m_bEnabled  = true;
    poInst->m_bInternal = true;
}
// ----------------------------------------------------------------------------
/// Spawns a ParticleSys automanaged by the manager.
void CFEParticleSysInstMgr::SpawnParticle(const CFEString& _sParticleSysModel,const CFEVect2& _oPos,FEReal _rDepth)
{
    // Retrieve the ParticleSys resource
    SpawnParticle(CFEParticleSysMgr::poLoad(_sParticleSysModel),_oPos,_rDepth);
}
// ----------------------------------------------------------------------------
/// Spawns a ParticleSys automanaged by the manager.
void CFEParticleSysInstMgr::SpawnParticle(CFEParticleSys* _poPS,const CFEVect2& _oPos,FEReal _rDepth)
{
	// Check if the ParticleSys object exists.
    if (_poPS == NULL) return;

	// 
    CFEParticleMgr::CreateParticles(1,_oPos,_rDepth,_poPS);
}
// ----------------------------------------------------------------------------
/// Deletes a given ParticleSys instance.
void CFEParticleSysInstMgr::DeleteInstance(FEHandler _hInstance)
{
    if (_hInstance == NULL) return;
    PSINST(_hInstance).m_bFree    = true;
    PSINST(_hInstance).m_bEnabled = false;
}
// ----------------------------------------------------------------------------
/// Gets the poPSItion of a given ParticleSys instance.
CFEVect2 CFEParticleSysInstMgr::oGetPos(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(CFEVect2(_0r,_0r) );
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
void CFEParticleSysInstMgr::SetDepth(FEHandler _hInstance,FEReal _rDepth)
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
bool CFEParticleSysInstMgr::bIsEnabled(FEHandler _hInstance)
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
void CFEParticleSysInstMgr::ManageRender(FEHandler _hInstance,bool _bManageRender)
{
    if (_hInstance == NULL) return;
	PSINST(_hInstance).m_bIsRenderManaged = _bManageRender;
}
// ----------------------------------------------------------------------------            
/// Tells whether the render is managed by the instance manager or not.
bool CFEParticleSysInstMgr::bIsRenderManaged(FEHandler _hInstance)
{
    if (_hInstance == NULL) return(false);
	return( PSINST(_hInstance).m_bIsRenderManaged );
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Update(FEHandler _hInstance,FEReal _rDeltaT)
{
    if (_hInstance == NULL) return;

    CFEParticleSysMgrInst* poPSI = &PSINST(_hInstance);
    if ((poPSI->m_bInternal) && (poPSI->m_rLifeTime!=PSYS_INFINITE_LIFE))
    {
        poPSI->m_rLifeTime -= _rDeltaT;

        if (poPSI->m_rLifeTime <= _0r)
        {
            DeleteInstance(_hInstance);
            return;
        }
    }

    FEReal rNumParts = poPSI->m_poPS->m_rFreq*_rDeltaT + poPSI->m_rAccParts;
    uint uiNumParts = (uint)rNumParts;
    
    if (uiNumParts>0)
        CFEParticleMgr::CreateParticles(uiNumParts,poPSI->m_oPos,poPSI->m_rDepth,poPSI->m_poPS);

    // update accumulation of particles.  
    poPSI->m_rAccParts = rNumParts - FEReal(uiNumParts);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oList.size();i++)
    {
        if (INSTANCEDDATA->m_oList[i]->m_bEnabled)
            Update((FEHandler)INSTANCEDDATA->m_oList[i],_rDeltaT);
    }

    CFEParticleMgr::Update(_rDeltaT);
}
// ----------------------------------------------------------------------------
void CFEParticleSysInstMgr::Render(CFERenderer *_poRenderer)
{
    CFEParticleMgr::Render(_poRenderer);
}
// ----------------------------------------------------------------------------
