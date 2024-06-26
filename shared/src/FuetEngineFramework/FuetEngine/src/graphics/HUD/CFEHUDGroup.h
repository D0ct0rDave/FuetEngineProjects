// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDGroupH
#define CFEHUDGroupH
//-----------------------------------------------------------------------------
#include "types/CFEArray.h"
#include "CFEHUDObject.h"
#include "CFEHUDVisitor.h"
//-----------------------------------------------------------------------------
class CFEHUDGroup : public CFEHUDObject
{
	public:

			/// Default constructor of the class.
		    CFEHUDGroup(const CFEString& _sName) : 
				CFEHUDObject(_sName), 
				m_rDepthFactor(_01r)
			{
			}

			/// Default constructor of the class.
		    CFEHUDGroup(const CFEString& _sName,uint _uiReserveSize) : 
				CFEHUDObject(_sName), 
				m_rDepthFactor(_01r)
			{
				m_oObjs.reserve(_uiReserveSize);
			}

			/// Destructor of the class.
			virtual ~CFEHUDGroup();

			/// Adds a new object to the group.
			uint uiAddObject(CFEHUDObject* _poObject)
			{
				m_oObjs.push_back(_poObject);
				return(m_oObjs.size()-1);
			}

			/// Inserts a new object into the group right after the given position.
			void InsertObject(CFEHUDObject* _poObject,uint _uiIdx)
			{
				m_oObjs.insert(m_oObjs.begin() + (long)_uiIdx, _poObject);
			}

			/// Sets the a given object.
			void SetObject(uint _uiIdx,CFEHUDObject* _poObject)
			{
				m_oObjs[_uiIdx] = _poObject;
			}

            /// Retrieves the index of the given object (if exists)
            int iGetObjectIndex(CFEHUDObject* _poObject)
            {
                for (uint i=0;i<m_oObjs.size();i++)
					if (m_oObjs[i] == _poObject)
						return((int)i);

                return(-1);
            }

            /// Swaps the two given elements
            void Swap(uint _uiIdxA,uint _uiIdxB)
            {
                CFEHUDObject* poAux = m_oObjs[_uiIdxA];
                m_oObjs[_uiIdxA] = m_oObjs[_uiIdxB];
                m_oObjs[_uiIdxB] = poAux;
            }

			/// Deletes an element identified by the given index of this group.
			void DeleteObject(uint _uiIdx)
			{
				m_oObjs.erase( m_oObjs.begin() + (long)_uiIdx );
			}

			/// Retrieves an element identified by the given index of this group.
			CFEHUDObject* poGetObject(uint _uiIdx)
			{
				return( m_oObjs[_uiIdx]);
			}

			/// Retrieves the number of subojects belonging to this group.
			uint uiNumObjs()
			{
				return ( m_oObjs.size() );
			}
			
			/// Sets the depth factor used with the subobjects of this group.
            void SetDepthFact(FEReal _rDepthFactor)
			{
				m_rDepthFactor = _rDepthFactor;
			}
			
			/// Retrieves the depth factor for this group.
			FEReal rGetDepthFact()
			{
				return(m_rDepthFactor);
			}

			/// Perform processing over the object
    		virtual void Accept(CFEHUDVisitor* _poVisitor)
    		{
    		    _poVisitor->Visit(this);
    		}

		private:

			CFEArray<CFEHUDObject*> m_oObjs;
			FEReal	m_rDepthFactor;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
