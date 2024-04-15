// -----------------------------------------------------------------------------
/*! \class CFEResourceQueue
 *  \brief A class to manage all the entities of the system.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEResourceQueueH
#define CFEResourceQueueH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include <list>
// ----------------------------------------------------------------------------
template <typename T>
class CFEResourceQueue
{
	protected:

		class CFEResourceElem
		{
			public:

				CFEResourceElem(T* _pT) :
					 m_poElem(_pT)
					,m_bFree(true)
				{
				}

				T*				m_poElem;
				FEBool			m_bFree;
		};

	public:

		/// Default constructor of the template class.
		CFEResourceQueue()
		{
		}

		/// Destructor of the class.
		virtual ~CFEResourceQueue()
		{
			Reset();
		}

		/// Cleans the whole buffer.
		void Reset()
		{
			m_oAll.clear();
			m_oUsed.clear();
			m_oFree.clear();
		}

		/// Adds a new element to the queue. Use this to fed the buffer with free resources.
		int iAdd(T* _poElem)
		{
			CFEResourceElem oResEntry(_poElem);

			m_oAll.push_back( oResEntry );
			m_oFree.push_back( m_oAll.size()-1);

			return( (int)m_oAll.size()-1 );
		}

		/// Retrieves the resource at the given array position.
		T* poGetAt(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return(NULL);
			return( m_oAll[_uiIdx].m_poElem );
		}

		/// Retrieves the resource at the given array position.
		FEBool bIsFree(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return(false);
			return(m_oAll[_uiIdx].m_bFree);
		}

		/// Gets a free resource in the system.
		T* poGetResource()
		{
			if (m_oFree.size() == 0) return(NULL);

			uint uiIdx = m_oFree.back();
			m_oFree.pop_back();
			m_oUsed.push_back(uiIdx);

			m_oAll[uiIdx].m_bFree = false;
			return(m_oAll[uiIdx].m_poElem);
		}

		/// Releases a used resource in the system.
		void Release(T* _poElem)
		{
			for(std::list<uint>::iterator it = m_oUsed.begin();it != m_oUsed.end() ; it++)
			{
				uint uiIdx = *it;
				if (m_oAll[uiIdx].m_poElem == _poElem)
				{
					// remove from the used list
					m_oUsed.erase(it);

					// add to the free list
					m_oFree.push_back(uiIdx);
					
					m_oAll[uiIdx].m_bFree = true;
					return;
				}
			}
		}

		/// Releases a used resource in the system.
		void Release(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return;
			Release( m_oAll[_uiIdx].m_poElem );
		}

		/// Releases all the used resources in the system.
		void ReleaseAll()
		{
			m_oFree.clear();
			m_oUsed.clear();

		    for (uint i=0;i<m_oAll.size();i++)	
				m_oFree.push_back(i);
		}

		/// Retrieves the number of elements the resource queue is currently holding.
		uint uiGetNumElems()
		{
			return(m_oAll.size());
		}

   protected:

		// The resource queue.
		CFEArray<CFEResourceElem> m_oAll;

		std::list<uint> m_oFree;
		std::list<uint> m_oUsed;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
