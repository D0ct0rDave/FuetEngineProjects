// ----------------------------------------------------------------------------
/*! \class CFEHUDElemLocator
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDElemLocatorH
#define CFEHUDElemLocatorH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDElemLocator : public CFEHUDVisitor
{
    public:

		/// Retrieves the element action that matches with the specified name.
		CFEHUDElementAction* poLocateElemAction(CFEHUD* _poHUDModel,const CFEString& _sName);

		/// Retrieves the object action that matches with the specified names.
        CFEHUDObjectAction* poLocateObjAction(CFEHUD* _poHUDModel,const CFEString& _sName);

        /// Retrieves the object that matches with the specified name.
        CFEHUDObject* poLocateHUDObject(CFEHUD* _poHUDModel,const CFEString& _sName);

        /// Retrieves the element that matches with the specified name.
        CFEHUDElement* poLocateHUDElement(CFEHUD* _poHUDModel,const CFEString& _sName);

		/// Retrieves the element action that matches with the specified name.
		CFEHUDElementAction* poLocateElemAction(CFEHUDElement* _poElem,const CFEString& _sName);

		/// Retrieves the object action that matches with the specified names.
        CFEHUDObjectAction* poLocateObjAction(CFEHUDElement* _poElem,const CFEString& _sName);

        /// Retrieves the object that matches with the specified name.
        CFEHUDObject* poLocateHUDObject(CFEHUDElement* _poElem,const CFEString& _sName);

        /// 
        virtual void Visit(CFEHUD* _poObj);
        /// 
        virtual void Visit(CFEHUDElement* _poObj);
        /// 
        virtual void Visit(CFEHUDObject* _poObj);
        /// 
        virtual void Visit(CFEHUDGroup* _poObj);
        /// 
        virtual void Visit(CFEHUDLabel* _poObj);
        /// 
        virtual void Visit(CFEHUDIcon* _poObj);
        /// 
        virtual void Visit(CFEHUDRect* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        /// 
        virtual void Visit(CFEHUDObjectAction* _poObj);        

	protected:
		
		uint		m_uiSearchType;
		void*		m_poHUDElem;
		CFEString	m_sName;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------