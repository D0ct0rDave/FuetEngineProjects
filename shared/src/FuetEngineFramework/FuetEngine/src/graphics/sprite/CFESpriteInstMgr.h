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
	friend class CFEInstanceMgr<CFESpriteInstMgr, CFESpriteMgrInst>;

    public:

			/// OVERRIDED: Main initialiation procedure.
			static void Init(uint _uiMaxInstances = 256);

			/// OVERRIDED: Main finalization procedure.
			static void Finish();
		
            /// Spawns a sprite automanaged by the manager.
            static void Spawn(const CFEString& _sSpriteModel,uint _uiAction,const CFEVect2& _oPos,const FEReal& _rDepth,const FEReal& _rSize,const FEReal& _rAngle,const CFEColor& _oColor);

            /// Sets the position of a given sprite instance.
            static void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

			/// Gets the position of a given sprite instance.
			static const CFEVect2& oGetPos(FEHandler _hInstance);

            /// Sets the depth of a given sprite instance.
            static void SetDepth(FEHandler _hInstance,const FEReal& _rDepth);

			/// Gets the depth of a given sprite instance.
			static FEReal rGetDepth(FEHandler _hInstance);

            /// Sets the scale of a given sprite instance the same amount in width and height.
            static void SetScale(FEHandler _hInstance,const CFEVect2& _oScale);

            /// Sets the scale of a given sprite instance.
            static void SetScale(FEHandler _hInstance,const FEReal& _rScale);

            /// Retrieves the scale of a given sprite instance.
            static const CFEVect2& oGetScale(FEHandler _hInstance);

            /// Sets the speed multiplier for a given sprite instance.
            static void SetSpeedMult(FEHandler _hInstance,const FEReal& _rSpeedMult);

            /// Retrieves the speed multiplier of a given sprite instance.
            static FEReal rGetSpeedMult(FEHandler _hInstance);

            /// Sets the angle of a given sprite instance.
            static void SetAngle(FEHandler _hInstance,const FEReal& _rAngle);

            /// Retrieves the current angle of a given sprite instance.
            static FEReal rGetAngle(FEHandler _hInstance);

            /// Sets the color of a given sprite instance.
            static void SetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Retrieves the color of a given sprite instance.
            static const CFEColor& oGetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Sets the current action of a given sprite instance by its index.
            static void SetAction(FEHandler _hInstance,uint _uiAction);

            /// Sets the current action of a given sprite instance by its name.
            static void SetAction(FEHandler _hInstance,const CFEString& _sActionName);

            /// Retrieves the current action being played by the sprite instance.
            static uint uiGetAction(FEHandler _hInstance);

            /// Enables the sprite instance.
            static void Enable(FEHandler _hInstance);

            /// Disables the given sprite instance.
            static void Disable(FEHandler _hInstance);

            /// Retrieves whether this sprite is enabled or not.
            static bool bIsEnabled(FEHandler _hInstance);

            /// Retrieves whether this sprite is still playing or has stopped (only for one-shot).
            static bool bIsPlaying(FEHandler _hInstance);

			/// Retrieves the transformed geometry of the given sprite instance.
			static void GetGeometry(FEHandler _hInstance,CFEVect2* _poVXs);

            /// Retrieves the time in the current action this sprite instance is being played.
            static FEReal rGetCurrentActionTime(FEHandler _hInstance);

            /// Sets the time in the current action this sprite instance will be played.
            static void SetCurrentActionTime(FEHandler _hInstance,const FEReal& _rActionTime);

            /// Retrieves the current frame this sprite instance is playing.
            static CFESpriteFrame* poGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the current frame index this sprite instance is playing.
            static uint uiGetCurrentFrame(FEHandler _hInstance);

            /// Retrieves the sprite model used by the given sprite instance.
            static CFESprite* poGetSprite(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            static void ManageRender(FEHandler _hInstance,bool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            static bool bIsRenderManaged(FEHandler _hInstance);

            /// Performs an update step over the given sprite instance, also in case it's disabled.
            static void Update(CFESpriteMgrInst* _poInstance,const FEReal& _rDeltaT);

            /// Performs an update step over the enabled sprite instances in the system.
            static void Update(const FEReal& _rDeltaT);

            /// Performs an update step over the given sprite instance, also in case it's disabled.
            static void Update(FEHandler _hInstance,const FEReal& _rDeltaT);

            /// Renders all the enabled sprite instances in the system.
            static void Render(CFERenderer *_poRenderer);

            /// Renders the given sprite instance, also in case it's disabled.
            static void Render(FEHandler _hInstance,CFERenderer *_poRenderer);

	protected:

			/// OVERRIDED: Retrieves the resource associated to a given name and returns it as a handler.
			static FEHandler hGetResource(const CFEString& _sResource);

			/// OVERRIDED: Sets up a recently aquired instance
			static void SetupInstance(CFESpriteMgrInst* _poInstance,FEHandler _hResource);

	public:	/// WARNING: dirty trick to give access to this function from ParticleMgr to SpriteInstanceMgr
			
			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			static void InvalidateInstance(CFESpriteMgrInst* _poInstance);

			/// OVERRIDED: Creates an instance object of the specific type.
			static CFESpriteMgrInst* poCreateInstance();

			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			static void DestroyInstance(CFESpriteMgrInst* _poInstance);

			/// 
			static CFEArray<CFESpriteMgrInst*> m_oEnabledInsts;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
