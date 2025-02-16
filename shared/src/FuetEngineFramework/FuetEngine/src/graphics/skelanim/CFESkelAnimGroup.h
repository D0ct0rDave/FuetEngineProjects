// ----------------------------------------------------------------------------
/*! \class CFESkelAnimGroup
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimGroupH
#define CFESkelAnimGroupH
//-----------------------------------------------------------------------------
#include "CFESkelAnimNode.h"
#include "CFESkelAnimNodeVisitor.h"
//-----------------------------------------------------------------------------
class CFESkelAnimGroup : public CFESkelAnimNode
{
	public:

		CFESkelAnimGroup(const CFEString& _sName) : CFESkelAnimNode(_sName)
		{
		}

		/// Adds a new oode to the group.
		uint uiAddNode(CFESkelAnimNode* _poNode)
		{
		    if (_poNode == NULL) return(0xffffffff);

			m_oNodes.push_back(_poNode);
			return(m_oNodes.size()-1);
		}

		/// Retrieves the index of the given oode (if exists)
		int iGetNodeIndex(CFESkelAnimNode* _poNode)
		{
			for (uint i=0;i<m_oNodes.size();i++)
				if (m_oNodes[i] == _poNode)
					return(i);

			return(-1);
		}

		/// Swaps the two given elements
		void Swap(uint _uiIdxA,uint _uiIdxB)
		{
			CFESkelAnimNode* poAux = m_oNodes[_uiIdxA];
			m_oNodes[_uiIdxA] = m_oNodes[_uiIdxB];
			m_oNodes[_uiIdxB] = poAux;
		}

		/// Deletes an element identified by the given index of this group.
		void DeleteNode(uint _uiIdx)
		{
			m_oNodes.erase( m_oNodes.begin() + _uiIdx );
		}

		/// Retrieves an element identified by the given index of this group.
		CFESkelAnimNode* poGetNode(uint _uiIdx)
		{
			return( m_oNodes[_uiIdx]);
		}

		/// Retrieves the number of subojects belonging to this group.
		uint uiNumObjs()
		{
			return ( m_oNodes.size() );
		}
		
		/// Perform processing over the oode
		virtual void Accept(CFESkelAnimNodeVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	private:

		CFEArray<CFESkelAnimNode*> m_oNodes;
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
