// ----------------------------------------------------------------------------
/*! \class CFEHUDUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDUpdaterH
#define CFEHUDUpdaterH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "FEBasicTypes.h"
//-----------------------------------------------------------------------------
class CFEHUDElementAction;
class CFEHUDObjectAction;
//-----------------------------------------------------------------------------
class CFEHUDUpdater /* : public CFEHUDVisitor*/
{
    public:

		/// Updates the whole HUD object
        static void Update(CFEHUD* _poHUD,FEReal _rDeltaT);
        
		/// Updates an element of the HUD
        static void Update(CFEHUDElement* _poElement,FEReal _rDeltaT);        

        /// 
        static void Process(CFEHUDElementAction* _poObj,FEReal _rTime);

        /// 
        static void Process(CFEHUDObjectAction* _poObj,FEReal _rTime);

		///
		static void SetActionDefaultValues(CFEHUDObject* _poObj);

		///
		static void RestartActions(CFEHUDElementAction* _poObj);

		/// Reset the all the HUD elements and components to its initial values.
		static void ResetHUD(CFEHUD* _poHUD);

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
