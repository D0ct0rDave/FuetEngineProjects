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
// TODO: - Spawn method.
//         - Estudiar si es interesante poner un control de velocidad variable.
//         poder spawnear Meshs a distinta velocidad de la que fueron diseñados.
//         - En el campo CMeshFrame, podríamos poner la textura. De ese modo,
//         el sistema permite diferentes texturas por frame.
// ----------------------------------------------------------------------------
#ifndef CFEMeshInstMgrH
#define CFEMeshInstMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "CFEMeshMgr.h"
#include "types/CFEInstanceMgr.h"
// ----------------------------------------------------------------------------
// class CFEMeshInst;
class CFEMeshFrame;
class CFERenderer;
class CFEMeshMgrInst;
// ----------------------------------------------------------------------------
DECLARE_INSTANCE_MANAGER(CFEMeshInstMgr,CFEMeshMgrInst)
{
    public:

			/// OVERRIDED: Main initialiation procedure.
			virtual void Init(uint _uiMaxInstances = 256);

			/// OVERRIDED: Main finalization procedure.
			virtual void Finish();
		
            /// Sets the position of a given Mesh instance.
            void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

			/// Gets the position of a given Mesh instance.
			const CFEVect2& oGetPos(FEHandler _hInstance);

            /// Sets the depth of a given Mesh instance.
            void SetDepth(FEHandler _hInstance,const FEReal& _rDepth);

			/// Gets the depth of a given Mesh instance.
			FEReal rGetDepth(FEHandler _hInstance);

            /// Sets the scale of a given Mesh instance the same amount in width and height.
            void SetScale(FEHandler _hInstance,const CFEVect2& _oScale);

            /// Sets the scale of a given Mesh instance.
            void SetScale(FEHandler _hInstance,const FEReal& _rScale);

            /// Retrieves the scale of a given Mesh instance.
            const CFEVect2& oGetScale(FEHandler _hInstance);

            /// Sets the speed multiplier for a given Mesh instance.
            void SetSpeedMult(FEHandler _hInstance,const FEReal& _rSpeedMult);

            /// Retrieves the speed multiplier of a given Mesh instance.
            FEReal rGetSpeedMult(FEHandler _hInstance);

            /// Sets the angle of a given Mesh instance.
            void SetAngle(FEHandler _hInstance,const FEReal& _rAngle);

            /// Retrieves the current angle of a given Mesh instance.
            FEReal rGetAngle(FEHandler _hInstance);

            /// Sets the color of a given Mesh instance.
            void SetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Retrieves the color of a given Mesh instance.
            const CFEColor& oGetColor(FEHandler _hInstance);

            /// Sets the current action of a given Mesh instance by its index.
            void SetAction(FEHandler _hInstance,uint _uiAction);

            /// Sets the current action of a given Mesh instance by its name.
            void SetAction(FEHandler _hInstance,const CFEString& _sActionName);

            /// Retrieves the current action being played by the Mesh instance.
            uint uiGetAction(FEHandler _hInstance);

            /// Enables the Mesh instance.
            void Enable(FEHandler _hInstance);

            /// Disables the given Mesh instance.
            void Disable(FEHandler _hInstance);

            /// Retrieves whether this Mesh is enabled or not.
            FEBool bIsEnabled(FEHandler _hInstance);

            /// Retrieves whether this Mesh is still playing or has stopped (only for one-shot).
            FEBool bIsPlaying(FEHandler _hInstance);

            /// Retrieves the time in the current action this Mesh instance is being played.
            FEReal rGetCurrentActionTime(FEHandler _hInstance);

            /// Sets the time in the current action this Mesh instance will be played.
            void SetCurrentActionTime(FEHandler _hInstance,const FEReal& _rActionTime);

            /// Retrieves the current frame this Mesh instance is playing.
            CFEMeshFrame* poGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the current frame index this Mesh instance is playing.
            uint uiGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the Mesh model used by the given Mesh instance.
            CFEMesh* poGetMesh(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            void ManageRender(FEHandler _hInstance,FEBool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            FEBool bIsRenderManaged(FEHandler _hInstance);

            /// Performs an update step over the given Mesh instance, also in case it's disabled.
            void Update(CFEMeshMgrInst* _poInstance,const FEReal& _rDeltaT);

            /// Performs an update step over the enabled Mesh instances in the system.
            void Update(const FEReal& _rDeltaT);

            /// Performs an update step over the given Mesh instance, also in case it's disabled.
            void Update(FEHandler _hInstance,const FEReal& _rDeltaT);

            /// Renders all the enabled Mesh instances in the system.
            void Render(CFERenderer *_poRenderer);

            /// Renders the given Mesh instance, also in case it's disabled.
            void Render(FEHandler _hInstance,CFERenderer *_poRenderer);

	protected:

			/// OVERRIDED: Retrieves the resource associated to a given name and returns it as a handler.
			virtual FEHandler hGetResource(const CFEString& _sResource);

			/// OVERRIDED: Sets up a recently aquired instance
			virtual void SetupInstance(CFEMeshMgrInst* _poInstance,FEHandler _hResource);

	public:	/// WARNING: dirty trick to give access to this function from ParticleMgr to MeshInstanceMgr
			
			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			virtual void InvalidateInstance(CFEMeshMgrInst* _poInstance);

			/// OVERRIDED: Creates an instance object of the specific type.
			virtual CFEMeshMgrInst* poCreateInstance();

			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			virtual void DestroyInstance(CFEMeshMgrInst* _poInstance);

			/// 
			CFEArray<CFEMeshMgrInst*> m_oEnabledInsts;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
