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
// TODO: - Spawn method.
//         - Estudiar si es interesante poner un control de velocidad variable.
//         poder spawnear ParticleSyss a distinta velocidad de la que fueron diseñados.
//         - En el campo CParticleSysFrame, podríamos poner la textura. De ese modo,
//         el sistema permite diferentes texturas por frame.
// ----------------------------------------------------------------------------
#ifndef CFEParticleSysInstMgrH
#define CFEParticleSysInstMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEVect2.h"
#include "Types/CFEColor.h"
#include "CFEParticleSysMgr.h"
#include "types/CFEInstanceMgr.h"
// ----------------------------------------------------------------------------
const FEReal PSYS_INFINITE_LIFE = -_1r;
// ----------------------------------------------------------------------------
class CFEParticleSysMgrInst;
class CFERenderer;
// ----------------------------------------------------------------------------
DECLARE_INSTANCE_MANAGER(CFEParticleSysInstMgr,CFEParticleSysMgrInst)
{
	friend CFEInstanceMgr<CFEParticleSysInstMgr,CFEParticleSysMgrInst>;
 
    public:

            /// Spawns a ParticleSys automanaged by the manager.
            static void Spawn(const CFEString& _sParticleSysModel,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth = _0r);

            /// Spawns a ParticleSys automanaged by the manager.
            static void Spawn(CFEParticleSys* _poPS,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth = _0r);

            /// Spawns a one particle of a the given ParticleSys.
            static void SpawnParticle(const CFEString& _sParticleSysModel,const CFEVect2& _oPos,const FEReal& _rDepth = _0r);

            /// Spawns a one particle of a the given ParticleSys.
            static void SpawnParticle(CFEParticleSys* _poPS,const CFEVect2& _oPos,const FEReal& _rDepth = _0r);

            /// Sets the position of a given ParticleSys instance.
            static void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

			/// Gets the position of a given ParticleSys instance.
			static CFEVect2 oGetPos(FEHandler _hInstance);

            /// Sets the depth of a given ParticleSys instance.
            static void SetDepth(FEHandler _hInstance,const FEReal& _rDepth);

			/// Gets the depth of a given ParticleSys instance.
			static FEReal rGetDepth(FEHandler _hInstance);

            /// Enables the ParticleSys instance.
            static void Enable(FEHandler _hInstance);

            /// Disables the given ParticleSys instance.
            static void Disable(FEHandler _hInstance);

            /// Retrieves whether this ParticleSys is enabled or not.
            static bool bIsEnabled(FEHandler _hInstance);

            /// Retrieves the ParticleSys model used by the given ParticleSys instance.
            static CFEParticleSys* poGetParticleSys(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            static void ManageRender(FEHandler _hInstance,bool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            static bool bIsRenderManaged(FEHandler _hInstance);

            /// Performs an update step over the given ParticleSys instance, also in case it's disabled.
            static void Update(FEHandler _hInstance,const FEReal& _rDeltaT);

            /// Performs an update step over the enabled ParticleSys instances in the system.
            static void Update(const FEReal& _rDeltaT);

            /// Renders all the enabled ParticleSys instances in the system.
            static void Render(CFERenderer *_poRenderer);

    protected:

			/// OVERRIDED: Retrieves the resource associated to a given name and returns it as a handler.
			static FEHandler hGetResource(const CFEString& _sResource);

			/// OVERRIDED: Sets up a recently aquired instance
			static void SetupInstance(CFEParticleSysMgrInst* _poInstance,FEHandler _hResource);

			/// OVERRIDED: Creates an instance object of the specific type.
			static CFEParticleSysMgrInst* poCreateInstance();
			
			/// OVERRIDED: Destroys an instance object of the specific type.
			static void DestroyInstance(CFEParticleSysMgrInst* _poInstance);

            /// 
            static void Spawn(FEHandler _hInstl,const FEReal& _rLifeTime,const CFEVect2& _oPos,const FEReal& _rDepth);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
