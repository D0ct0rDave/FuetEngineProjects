// ----------------------------------------------------------------------------
/*! \class CFESkelAnimNodeUpdater
 *  \brief A generic class to perfrm a process over all the elemnts of a HUD hierarchy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
        static void Update(CFESkelAnimNode* _poNode,FEReal _rTime);
        /// 
        virtual void Visit(CFESkelAnimSprite* _poObj);
        /// 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        /// 
        virtual void Visit(CFESkelAnimBone* _poObj);

	protected:

		static CFESkelAnimNode*     m_poNode;
		static FEReal				m_rTime;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------