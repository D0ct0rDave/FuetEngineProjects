// ----------------------------------------------------------------------------
/*! \class CFESkelAnimGroup
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimNodeTableBuilderH
#define CFESkelAnimNodeTableBuilderH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimNode;
class CFESkelAnimSpriteModel;
class CFESkelAnimSprite;
class CFESkelAnimGroup;
class CFESkelAnimBone;
class CFESkelAnimMesh;
//-----------------------------------------------------------------------------
class CFESkelAnimNodeTableBuilder : public CFESkelAnimNodeVisitor
{
    public:
        
        static void BuildTable(CFEArray<CFESkelAnimNode*>* _poNodeTable, CFESkelAnimNode* _poNode);
		
		// avoid override function hiding
		// http://www.parashift.com/c++-faq-lite/hiding-rule.html
		using CFESkelAnimNodeVisitor::Visit;

        //
        virtual void Visit(CFESkelAnimNode* _poObj);
        // 
        virtual void Visit(CFESkelAnimGroup* _poObj);
        // 
        virtual void Visit(CFESkelAnimBone* _poObj);

	protected:

		static CFEArray<CFESkelAnimNode*>*	m_poNodeTable;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
