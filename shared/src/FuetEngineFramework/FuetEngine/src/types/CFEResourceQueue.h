// -----------------------------------------------------------------------------
/*! \class CFEResourceQueue
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEResourceQueueH
#define CFEResourceQueueH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
// ----------------------------------------------------------------------------
template <typename T>
class CFEResourceQueue
{
	protected:
		template <typename T>
		class CFEResourceElem
		{
			public:

				T*				m_poElem;
				bool			m_bFree;
		};

	public:
		
		/// Default constructor of the template class.
		CFEResourceQueue() : m_uiCurElem(0)
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
			m_oList.clear();
			m_uiCurElem  = 0;
		}

		/// Adds a new element to the queue. Use this to fed the buffer with free resources.
		int iAdd(T* _poElem)
		{
			CFEResourceElem<T> oResEntry;
		
			oResEntry.m_bFree = true;
			oResEntry.m_poElem = _poElem;

			m_oList.push_back( oResEntry);

			return(uiGetNumElems()-1);
		}

		/// Retrieves the resource at the given array position.
		T* poGet(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return(NULL);
			return( m_oList[_uiIdx].m_poElem );
		}

		/// Retrieves the resource at the given array position.
		bool bIsFree(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return(false);
			return( m_oList[_uiIdx].m_bFree );
		}

		/// Gets a free resource in the system.
		T* poGetResource()
		{
			if (uiGetNumElems()==0) return(NULL);

			// handle circular queue
			uint uiTestElems = 0;
	
			do {

				if ( m_oList[m_uiCurElem].m_bFree)
				{
					m_oList[m_uiCurElem].m_bFree = false;
					return( m_oList[m_uiCurElem].m_poElem );
				}

				m_uiCurElem = (m_uiCurElem + 1) % uiGetNumElems();
				uiTestElems++;

			} while ( uiTestElems < uiGetNumElems() );

			// No free resources ...
			return(NULL);
		}

		/// Releases a used resource in the system.
		void Release(T* _poElem)
		{		
		    for (uint i=0;i<uiGetNumElems();i++)
		    {
			    if ( m_oList[i].m_poElem == _poElem)
			    {
					Release(i);
				    return;
				}
			}
		}

		/// Releases a used resource in the system.
		void Release(uint _uiIdx)
		{
			if (_uiIdx >= uiGetNumElems()) return;
			m_oList[_uiIdx].m_bFree  = true;
		}

		/// Releases all the used resources in the system.
		void ReleaseAll()
		{
		    for (uint i=0;i<uiGetNumElems();i++)
				m_oList[i].m_bFree = true;
		}

		/// Retrieves the number of elements the resource queue is currently holding.
		uint uiGetNumElems()
		{
			return(m_oList.size());
		}

   protected:
		
		// The resource queue.
		typedef CFEResourceElem<T> CResourceElemEntry;
		CFEArray<CResourceElemEntry> m_oList;

		// Current seek position in the queue.
		uint	m_uiCurElem;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
