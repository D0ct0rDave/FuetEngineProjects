// ----------------------------------------------------------------------------
/*! \class CFESkelAnimBone
 *  \brief A class to store bone information in a skeletal hierachy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimBoneH
#define CFESkelAnimBoneH
// ----------------------------------------------------------------------------
#include "CFESkelAnimNode.h"
// ----------------------------------------------------------------------------
class CFESkelAnimBone : public CFESkelAnimNode
{
    public:

        CFESkelAnimBone(const CFEString& _sName) : CFESkelAnimNode(_sName), m_poAttachedNode(NULL)
        {
        };

	    /// Destructor of the class
		virtual ~CFESkelAnimBone()
		{
			delete m_poAttachedNode;
		}

        /// Perform processing over the object.
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
		    _poVisitor->Visit(this);
		}

        /// Sets the absolute position of this bone.
        void SetAbsPos(const CFEVect2& _oAbsPos)
        {
            m_oAbsPos = _oAbsPos;
        }

        /// Retrieves the absolute position of this bone.
        const CFEVect2& oGetAbsPos()
        {
            return(m_oAbsPos);
        }

        /// Sets the attached node of this bone.
        void SetAttachedNode(CFESkelAnimNode* _poAttachedNode)
        {
            m_poAttachedNode = _poAttachedNode;
        }

        /// Retrieves the attached to this bone.
        CFESkelAnimNode* poGetAttachedNode()
        {
            return(m_poAttachedNode);
        }

    public:
        
        ///        
        CFEVect2    m_oAbsPos;
        
        /// 
        CFESkelAnimNode* m_poAttachedNode;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
