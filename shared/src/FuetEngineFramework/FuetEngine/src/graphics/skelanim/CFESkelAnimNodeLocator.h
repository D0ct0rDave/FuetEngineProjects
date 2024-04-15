// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeLocator
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeLocatorH
#define CFESkelAnimNodeLocatorH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
class CFESkelAnimGroup;
//-----------------------------------------------------------------------------
class CFESkelAnimNodeLocator : public CFESkelAnimNodeVisitor
{
    public:

        /// Retrieves the object that matches with the specified name.
        static CFESkelAnimNode* poLocateNode(CFESkelAnimNode* _poNode,const CFEString& _sName);
		
		// avoid override function hiding
		// http://stackoverflow.com/questions/2057823/issues-with-partial-class-function-overrides-in-c
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        virtual void Visit(CFESkelAnimNode* _poObj);
        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        /// 
        virtual void Visit(CFESkelAnimBone* _poObj);

	protected:

		static CFESkelAnimNode*     m_poNode;
		static CFEString	        m_sName;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
