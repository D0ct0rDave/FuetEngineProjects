// ----------------------------------------------------------------------------
/*! \class CFESkelAnimInstMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimInstMgrH
#define CFESkelAnimInstMgrH
// ----------------------------------------------------------------------------
#include "types/CFEInstanceMgr.h"

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
DECLARE_INSTANCE_MANAGER(CFESkelAnimInstMgr,CFESkelAnimMgrInst)
{
    public:

            /// Sets the position of a given skeleton animation instance.
            void SetPos(FEHandler _hInstance,const CFEVect2& _oPos);

            /// Sets the depth of a given skeleton animation instance.
            void SetDepth(FEHandler _hInstance,FEReal _rAngle);

            /// Retrieves the depth of a given skeleton animation instance.
            FEReal rGetDepth(FEHandler _hInstance);

			/// Gets the position of a given skeleton animation instance.
			const CFEVect2& oGetPos(FEHandler _hInstance);

            /// Sets the scale of a given skeleton animation instance the same amount in width and height.
            void SetScale(FEHandler _hInstance,const CFEVect2& _oScale);

            /// Sets the scale of a given skeleton animation instance.
            void SetScale(FEHandler _hInstance,FEReal _rScale);

            /// Sets the scale of a given skeleton animation instance.
            const CFEVect2& oGetScale(FEHandler _hInstance);

            /// Sets the angle of a given skeleton animation instance.
            void SetAngle(FEHandler _hInstance,FEReal _rAngle);

            /// Retrieves the angle of a given skeleton animation instance.
            FEReal rGetAngle(FEHandler _hInstance);

            /// Sets the color of a given skeleton animation instance.
            void SetColor(FEHandler _hInstance,const CFEColor& _oColor);

            /// Retrieves the color of a given skeleton animation instance.
            const CFEColor& oGetColor(FEHandler _hInstance);

            /// Sets the current action of a given skeleton animation instance.
            void SetAction(FEHandler _hInstance,uint _uiAction);

            /// Retrieves the current action setup in the given instance.
            uint uiGetAction(FEHandler _hInstance);

            /// Sets the current time of the animation being played by the given instance.
            void SetActionTime(FEHandler _hInstance,FEReal _rActionTime);

            /// Retrieves the current time of the animation being played by the given instance.
            FEReal rGetActionTime(FEHandler _hInstance);

            /// Retrieves total time of a single cycle of the current action animation.
            FEReal rGetActionMaxTime(FEHandler _hInstance);

            /// Sets the speed multiplier for a given animation instance.
            void SetSpeedMult(FEHandler _hInstance,FEReal _rSpeedMult);

            /// Retrieves the speed multiplier of a given animation instance.
            FEReal rGetSpeedMult(FEHandler _hInstance);

            /// Enables the skeleton animation instance.
            void Enable(FEHandler _hInstance);

            /// Disables the given skeleton animation instance.
            void Disable(FEHandler _hInstance);

            /// Retrieves whether this skeleton animation is enabled or not.
            FEBool bIsEnabled(FEHandler _hInstance);

            /// Retrieves whether this skeleton animation is still playing or has stopped.
            FEBool bIsPlaying(FEHandler _hInstance);

			/// Retrieves the index in the mask table of a given node
			int iGetNodeMaskIndex(FEHandler _hInstance, const CFEString& _sNodeName);

			/// Unmasks a given node of a given skeleton animation instance for updating.
            void UnmaskNode(FEHandler _hInstance,int _iNodeMaskIndex);

            /// Masks a given node of a given skeleton animation instance for updating.
            void MaskNode(FEHandler _hInstance,int _iNodeMaskIndex);

            /// Retrieves whether a given node of a given animation instance is masked o unmasked for updating.
            FEBool bIsNodeMasked(FEHandler _hInstance,int _iNodeMaskIndex);

			/// Retrieves the node table mask for the given instance.
			FEBool* pbGetNodeTableMask(FEHandler _hInstance);

			/// Retrieves the node table for the given instance.
			const CFEArray<CFESkelAnimNode*>* poGetNodeTable(FEHandler _hInstance);

			/// Retrieves the skeleton animation used by the given instance.
			CFESkelAnim* poGetAnim(FEHandler _hInstance);

			/// Retrieves the instanced node hierarchy of the animation used by this instance.
			CFESkelAnimNode* poGetInstancedNode(FEHandler _hInstance);

            /// Sets whether the render will be managed by the instance manager or not.
            void ManageRender(FEHandler _hInstance,FEBool _bManageRender);

			/// Tells whether the render is managed by the instance manager or not.
            FEBool bIsRenderManaged(FEHandler _hInstance);

            /// Sets whether the update process will be managed by the instance manager or not.
            void ManageUpdate(FEHandler _hInstance,FEBool _bManagedUpdate);

			/// Tells whether the update process is managed by the instance manager or not.
            FEBool bIsUpdateManaged(FEHandler _hInstance);

            /// Performs an update step over the given skeleton animation instance, also in case it's disabled.
            void Update(FEHandler _hInstance,FEReal _rDeltaT);

            /// Sets the animation isntance to a specific moment in time.
            void SetTime(FEHandler _hInstance,FEReal _rTime);

            /// Performs an update step over the enabled skeleton animation instances in the system.
            void Update(FEReal _rDeltaT);

            /// Renders all the enabled skeleton animation instances in the system.
            void Render(CFERenderer *_poRenderer);

            /// Renders all a single instance. Helper function for non managed rendering instances.
            void Render(FEHandler _hInstance,CFERenderer *_poRenderer);

    protected:

			/// OVERRIDED: Retrieves the resource associated to a given name and returns it as a handler.
			virtual FEHandler hGetResource(const CFEString& _sResource);

			/// OVERRIDED: Sets up a recently aquired instance
			virtual void SetupInstance(CFESkelAnimMgrInst* _poInstance,FEHandler _hResource);

			/// OVERRIDED: Creates an instance object of the specific type.
			virtual CFESkelAnimMgrInst* poCreateInstance();

			/// OVERRIDED: Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
			virtual void DestroyInstance(CFESkelAnimMgrInst* _poInstance);

			/// OVERRIDED: Here the user can free the possible allocated instance data.
			virtual void InvalidateInstance(CFESkelAnimMgrInst* _poInstance);

			/// Setups both action and speed mult at the same time
            virtual void SetActionAndSpeedMult(FEHandler _hInstance,uint _uiAction,FEReal _rSpeedMult);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
