// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeUpdaterH
#define CFESkelAnimNodeUpdaterH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimSprite;
class CFESkelAnimBone;
class CFESkelAnimGroup;
//-----------------------------------------------------------------------------
class CFESkelAnimNodeUpdater : public CFESkelAnimNodeVisitor
{
    public:

        /// Update static nodes in the skeletal hierarchy        
        static void Update(CFESkelAnimNode* _poNode,FEReal _rTime,FEReal _rDeltaT);
		
		/// 
		CFESkelAnimNodeUpdater(CFESkelAnimNode* _poNode, FEReal _rTime, FEReal _rDeltaT) : 
			m_poNode(_poNode), 
			m_rTime(_rTime),
			m_rDeltaT(_rDeltaT)
		{
		};

		// avoid override function hiding
		// http://stackoverflow.com/questions/2057823/issues-with-partial-class-function-overrides-in-c
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj);
        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        /// 
        virtual void Visit(CFESkelAnimBone* _poObj);

	protected:

		CFESkelAnimNode*    m_poNode;
		FEReal				m_rTime;
		FEReal				m_rDeltaT;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
