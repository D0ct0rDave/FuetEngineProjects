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
//         poder spawnear sprites a distinta velocidad de la que fueron diseñados.
//         - En el campo CSpriteFrame, podríamos poner la textura. De ese modo,
//         el sistema permite diferentes texturas por frame.
// ----------------------------------------------------------------------------
#ifndef CFESpriteInstMgrH
#define CFESpriteInstMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "CFESpriteMgr.h"
#include "types/CFEInstanceMgr.h"
// ----------------------------------------------------------------------------
// class CFESpriteInst;
class CFESpriteFrame;
class CFERenderer;
class CFESpriteMgrInst;
// ----------------------------------------------------------------------------
DECLARE_INSTANCE_MANAGER(CFESpriteInstMgr,CFESpriteMgrInst)
{
    public:

			/// OVERRIDED: Main initialiation procedure.
			virtual void Init(uint _uiMaxInstances = 256);

			/// OVERRIDED: Main finalization procedure.
			virtual void Finish();
		
            /// Spawns a sprite automanaged by the manager.
            void Spawn(const CFEString& _sSpriteModel,uint _uiAction,const CFEVect2& _oPos,const FEReal& _rDepth,const FEReal& _rSize,const FEReal& _rAngle,const CFEColor& _oColor);

            /// Sets the position of a given sprite instance.
            void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

			/// Gets the position of a given sprite instance.
			const CFEVect2& oGetPos(FEHandler _hInstance);

            /// Sets the depth of a given sprite instance.
            void SetDepth(FEHandler _hInstance,const FEReal& _rDepth);

			/// Gets the depth of a given sprite instance.
			FEReal rGetDepth(FEHandler _hInstance);

            /// Sets the scale of a given sprite instance the same amount in width and height.
            void SetScale(FEHandler _hInstance,const CFEVect2& _oScale);

            /// Sets the scale of a given sprite instance.
            void SetScale(FEHandler _hInstance,const FEReal& _rScale);

            /// Retrieves the scale of a given sprite instance.
            const CFEVect2& oGetScale(FEHandler _hInstance);

            /// Sets the speed multiplier for a given sprite instance.
            void SetSpeedMult(FEHandler _hInstance,const FEReal& _rSpeedMult);

            /// Retrieves the speed multiplier of a given sprite instance.
            FEReal rGetSpeedMult(FEHandler _hInstance);

            /// Sets the angle of a given sprite instance.
            void SetAngle(FEHandler _hInstance,const FEReal& _rAngle);

            /// Retrieves the current angle of a given sprite instance.
            FEReal rGetAngle(FEHandler _hInstance);

            /// Sets the color of a given sprite instance.
            void SetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Retrieves the color of a given sprite instance.
            const CFEColor& oGetColor(FEHandler _hInstance);

            /// Sets the current action of a given sprite instance by its index.
            void SetAction(FEHandler _hInstance,uint _uiAction);

            /// Sets the current action of a given sprite instance by its name.
            void SetAction(FEHandler _hInstance,const CFEString& _sActionName);

            /// Retrieves the current action being played by the sprite instance.
            uint uiGetAction(FEHandler _hInstance);

            /// Enables the sprite instance.
            void Enable(FEHandler _hInstance);

            /// Disables the given sprite instance.
            void Disable(FEHandler _hInstance);

            /// Retrieves whether this sprite is enabled or not.
            FEBool bIsEnabled(FEHandler _hInstance);

            /// Retrieves whether this sprite is still playing or has stopped (only for one-shot).
            FEBool bIsPlaying(FEHandler _hInstance);

			/// Retrieves the transformed geometry of the given sprite instance.
			void GetGeometry(FEHandler _hInstance,CFEVect2* _poVXs);

			/// Retrieves the random start time value for the current action this sprite instance is playing.
			FEReal rGetCurrentActionRandomStartTime(FEHandler _hInstance);

            /// Retrieves the time in the current action this sprite instance is being played.
            FEReal rGetCurrentActionTime(FEHandler _hInstance);

            /// Sets the time in the current action this sprite instance will be played.
            void SetCurrentActionTime(FEHandler _hInstance,const FEReal& _rActionTime);

            /// Retrieves the current frame this sprite instance is playing.
            CFESpriteFrame* poGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the current frame index this sprite instance is playing.
            uint uiGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the sprite model used by the given sprite instance.
            CFESprite* poGetSprite(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            void ManageRender(FEHandler _hInstance,FEBool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            FEBool bIsRenderManaged(FEHandler _hInstance);

            /// Performs an update step over the given sprite instance, also in case it's disabled.
            void Update(CFESpriteMgrInst* _poInstance,const FEReal& _rDeltaT);

            /// Performs an update step over the enabled sprite instances in the system.
            void Update(const FEReal& _rDeltaT);

            /// Performs an update step over the given sprite instance, also in case it's disabled.
            void Update(FEHandler _hInstance,const FEReal& _rDeltaT);

            /// Renders all the enabled sprite instances in the system.
            void Render(CFERenderer *_poRenderer);

            /// Renders the given sprite instance, also in case it's disabled.
            void Render(FEHandler _hInstance,CFERenderer *_poRenderer);

	protected:

			/// OVERRIDED: Retrieves the resource associated to a given name and returns it as a handler.
			virtual FEHandler hGetResource(const CFEString& _sResource);

			/// OVERRIDED: Sets up a recently aquired instance
			virtual void SetupInstance(CFESpriteMgrInst* _poInstance,FEHandler _hResource);

	public:	/// WARNING: dirty trick to give access to this function from ParticleMgr to SpriteInstanceMgr
			
			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			virtual void InvalidateInstance(CFESpriteMgrInst* _poInstance);

			/// OVERRIDED: Creates an instance object of the specific type.
			virtual CFESpriteMgrInst* poCreateInstance();

			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			virtual void DestroyInstance(CFESpriteMgrInst* _poInstance);

			/// 
			CFEArray<CFESpriteMgrInst*> m_oEnabledInsts;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
