// ----------------------------------------------------------------------------
/*! \class CFESkelAnimBone
 *  \brief A class to store bone information in a skeletal hierachy.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimBoneH
#define CFESkelAnimBoneH
// ----------------------------------------------------------------------------
#include "CFESkelAnimGroup.h"
// ----------------------------------------------------------------------------
class CFESkelAnimBone : public CFESkelAnimGroup
{
    public:

        CFESkelAnimBone(const CFEString& _sName) : CFESkelAnimGroup(_sName), m_oBonePos(CFEVect2::ZERO())
        {
        };

	    /// Destructor of the class
		/*
		virtual ~CFESkelAnimBone()
		{
			delete m_poAttachedNode;
		}
		*/

        /// Sets the position of this bone in the skeleton.
        void SetBonePos(const CFEVect2& _oBonePos)
        {
            m_oBonePos = _oBonePos;
        }

        /// Retrieves the position of this bone in the skeleton.
        const CFEVect2& oGetBonePos()
        {
            return(m_oBonePos);
        }

		/// Perform processing over the object.
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}
    
	public:
        
        ///        
        CFEVect2    m_oBonePos;
        
        /// 
        /// CFESkelAnimNode* m_poAttachedNode;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
