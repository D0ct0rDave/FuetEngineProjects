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
#ifndef CFESkelAnimGroupH
#define CFESkelAnimGroupH
//-----------------------------------------------------------------------------
#include "types/CFEArray.h"
#include "CFESkelAnimNode.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimGroup : public CFESkelAnimNode
{
	public:

		CFESkelAnimGroup(const CFEString& _sName) : CFESkelAnimNode(_sName)
		{
		}

	    /// Destructor of the class
		virtual ~CFESkelAnimGroup();

		/// Adds a new oode to the group.
		uint uiAddChild(CFESkelAnimNode* _poNode)
		{
		    if (_poNode == NULL) return(0xffffffff);

			m_oChild.push_back(_poNode);
			return(m_oChild.size()-1);
		}

		/// Retrieves the index of the given child (if exists)
		int iGetChildIndex(CFESkelAnimNode* _poNode)
		{
			for (uint i=0;i<m_oChild.size();i++)
				if (m_oChild[i] == _poNode)
					return((int)i);

			return(-1);
		}

		/// Swaps the two given elements
		void Swap(uint _uiIdxA,uint _uiIdxB)
		{
			CFESkelAnimNode* poAux = m_oChild[_uiIdxA];
			m_oChild[_uiIdxA] = m_oChild[_uiIdxB];
			m_oChild[_uiIdxB] = poAux;
		}

		/// Deletes an element identified by the given index of this group.
		void DeleteChild(uint _uiIdx)
		{
			m_oChild.erase( m_oChild.begin() + (long)_uiIdx );
		}

		/// Retrieves an element identified by the given index of this group.
		CFESkelAnimNode* poGetChild(uint _uiIdx)
		{
			return( m_oChild[_uiIdx]);
		}

		/// Sets an element identified by the given index of this group.
		void SetChild(uint _uiIdx,CFESkelAnimNode* _poNode)
		{
			if (_uiIdx>=uiNumChildren()) return;
			m_oChild[_uiIdx] = _poNode;
		}

		/// Retrieves the number of children belonging to this group.
		uint uiNumChildren()
		{
			return ( m_oChild.size() );
		}
		
		/// Perform processing over the oode
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	private:

		CFEArray<CFESkelAnimNode*> m_oChild;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
