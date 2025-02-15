// ----------------------------------------------------------------------------
/*! \class CFEHUDInstancer
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDInstancerH
#define CFEHUDInstancerH
//-----------------------------------------------------------------------------
#include "CFEHUDVisitor.h"
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFEHUDInstancer : public CFEHUDVisitor
{
    public:

        static CFEHUD* poCreateInstance(CFEHUD* _poHUDModel,bool _bAutoRename = false);

        static CFEHUDElement* poCreateInstance(CFEHUDElement* _poHUDElement,bool _bAutoRename = false);

        static CFEHUDObject* poCreateInstance(CFEHUDObject* _poHUDObject,bool _bAutoRename = false);

        static std::pair<CFEHUDObject*,CFEHUDObjectAction*> poCreateInstance(CFEHUDObject* _poHUDObject,CFEHUDObjectAction* _poHUDAction,bool _bAutoRename = false);
        
        static CFEHUDObjectAction* poCreateInstance(CFEHUDObjectAction* _poHUDAction,bool _bAutoRename = false);
        
        static CFEHUDElementAction* poCreateInstance(CFEHUDElementAction* _poHUDAction,bool _bAutoRename = false);

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
        virtual void Visit(CFEHUDShape* _poObj);
        /// 
        virtual void Visit(CFEHUDElementAction* _poObj);
        /// 
        virtual void Visit(CFEHUDObjectAction* _poObj);
        ///
        virtual const CFEString sGetType() { return("CFEHUDInstancer"); };

	protected:
		
		void CopyCommonProperties(CFEHUDObject* _poDst,CFEHUDObject* _poSrc);

		bool					m_bAutoRename;
		CFEHUDElement*			m_poNewElement;
        CFEHUDObject*			m_poNewObject;
        CFEHUDObjectAction*		m_poNewObjAction;
        CFEHUDElementAction*	m_poNewElemAction;
        
        class CObjectPair
        {
			public:

				CObjectPair() : m_poOrig(NULL), m_poCopy(NULL)
				{
				}

				CObjectPair(CFEHUDObject* _poOrig,CFEHUDObject* _poCopy)
				{
					m_poOrig = _poOrig;
					m_poCopy = _poCopy;
				}

				CFEHUDObject* m_poOrig;
				CFEHUDObject* m_poCopy;
			
        };

        CFEArray<CObjectPair> m_oObjPairs;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------