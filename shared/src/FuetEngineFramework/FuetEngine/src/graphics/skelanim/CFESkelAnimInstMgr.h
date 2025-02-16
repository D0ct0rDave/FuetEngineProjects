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
#ifndef CFESkelAnimInstMgrH
#define CFESkelAnimInstMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
#include "CFESkelAnimMgr.h"
// ----------------------------------------------------------------------------
class CFESkelAnim;
class CFESkelAnimNode;
class CFESkelAnimMgrInst;
class CFERenderer;
// ----------------------------------------------------------------------------
class CFESkelAnimInstMgr
{
    public:

            /// Main initialiation procedure.
            static void Init(uint _uiMaxSkelAnimInstances = 256);

            /// Main finalization procedure.
            static void Finish();

            /// Invalidates all the instaced animations inside the manager.
            static void Reset();

            /// Forces the loading of a skeleton animation resource.
            static void Load(const CFEString& _sSkelAnim);

            /// Retrieves a skel animation instance of a given skeleton animation resource.
            static FEHandler hGetInstance(const CFEString& _sSkelAnim);

            /// Deletes a given skeleton animation instance.
            static void DeleteInstance(FEHandler _hInstance);

            /// Sets the position of a given skeleton animation instance.
            static void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

            /// Sets the depth of a given skeleton animation instance.
            static void SetDepth(FEHandler _hInstance,FEReal _rAngle);

            /// Retrieves the depth of a given skeleton animation instance.
            static FEReal rGetDepth(FEHandler _hInstance);

			/// Gets the position of a given skeleton animation instance.
			static const CFEVect2& oGetPos(FEHandler _hInstance);

            /// Sets the scale of a given skeleton animation instance the same amount in width and height.
            static void SetScale(FEHandler _hInstance,const CFEVect2& _oScale);

            /// Sets the scale of a given skeleton animation instance.
            static void SetScale(FEHandler _hInstance,FEReal _rScale);

            /// Sets the scale of a given skeleton animation instance.
            static const CFEVect2& oGetScale(FEHandler _hInstance);

            /// Sets the angle of a given skeleton animation instance.
            static void SetAngle(FEHandler _hInstance,FEReal _rAngle);

            /// Retrieves the angle of a given skeleton animation instance.
            static FEReal rGetAngle(FEHandler _hInstance);

            /// Sets the color of a given skeleton animation instance.
            static void SetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Retrieves the color of a given skeleton animation instance.
            static const CFEColor& oGetColor(FEHandler _hInstance);

            /// Sets the current action of a given skeleton animation instance.
            static void SetAction(FEHandler _hInstance,uint _uiAction);

            /// Retrieves the current action setup in the given instance.
            static uint uiGetAction(FEHandler _hInstance);

            /// Sets the current time of the animation being played by the given instance.
            static void SetActionTime(FEHandler _hInstance,FEReal _rActionTime);

            /// Retrieves the current time of the animation being played by the given instance.
            static FEReal rGetActionTime(FEHandler _hInstance);

            /// Sets the speed multiplier for a given animation instance.
            static void SetSpeedMult(FEHandler _hInstance,FEReal _rSpeedMult);

            /// Retrieves the speed multiplier of a given animation instance.
            static FEReal rGetSpeedMult(FEHandler _hInstance);

            /// Enables the skeleton animation instance.
            static void Enable(FEHandler _hInstance);

            /// Disables the given skeleton animation instance.
            static void Disable(FEHandler _hInstance);

            /// Retrieves whether this skeleton animation is enabled or not.
            static bool bIsEnabled(FEHandler _hInstance);

            /// Retrieves whether this skeleton animationis still playing or has stopped.
            static bool bIsPlaying(FEHandler _hInstance);
			
			/// Retrieves the skeleton animation used by the given instance.
			static CFESkelAnim* poGetAnim(FEHandler _hInstance);
			
			/// Retrieves the instanced node hierarchy of the animation used by this instance.
			static CFESkelAnimNode* poGetInstancedNode(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            static void ManageRender(FEHandler _hInstance,bool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            static bool bIsRenderManaged(FEHandler _hInstance);

            /// Performs an update step over the given skeleton animation instance, also in case it's disabled.
            static void Update(FEHandler _hInstance,FEReal _rDeltaT);

            /// Performs an update step over the enabled skeleton animation instances in the system.
            static void Update(FEReal _rDeltaT);

            /// Renders all the enabled skeleton animation instances in the system.
            static void Render(CFERenderer *_poRenderer);

    protected:
            
            /// Setups both action and speed mult at the same time
            static void SetActionAndSpeedMult(FEHandler _hInstance,uint _uiAction,FEReal _rSpeedMult);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
