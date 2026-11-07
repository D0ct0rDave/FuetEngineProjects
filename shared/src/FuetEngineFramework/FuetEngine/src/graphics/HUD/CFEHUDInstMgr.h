// ----------------------------------------------------------------------------
/*! \class CFEHUDInstMgr
 *  \brief A class to manage all the HUD instances and functionalities over the HUD elements of an instance.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDInstMgrH
#define CFEHUDInstMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFERenderer;
class CFEHUDInstance;
class CFEHUDElementAction;
class CFEHUDObject;
class CFEHUD;
//-----------------------------------------------------------------------------
class CFEHUDInstMgr
{
	public:

        /// Retrieves a HUD instance of a given HUD resource.
        static FEHandler hGetInstance(const CFEString& _sHUDModel);

        // Updates all the HUD instances in the system.
        static void Update(FEReal _rDeltaT);

        // Updates all the HUD instances in the system.
        static void SetActionTime(FEReal _rDeltaT);

        /// Renders the HUD instance into the given renderer.
        static void Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer);

        /// Retrieves an element action from the given HUD instance.
        static CFEHUDElementAction* poGetElementAction(FEHandler _hHUDInstance,const CFEString& _sActionName);

        /// Retrieves an object from the given HUD instance.
        static CFEHUDObject* poGetObject(FEHandler _hHUDInstance,const CFEString& _sObjectName);

        /// Retrieves the HUD object associated to the given instance.
        static CFEHUD* poGetHUD(FEHandler _hHUDInstance);

        // Sets up the elements of a HUD element, associated with the given element action.
        static void SetActionTime(CFEHUDElementAction* _poAction,FEReal _rActionTime);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
