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

		/// Creates the buffer to store the elements of the queue.
		void Init(uint _uiMaxElems)
		{
			m_oList = new CFEResourceElem<T>[_uiMaxElems];
			m_uiMaxElems = _uiMaxElems;

			Reset();
		}

		/// Frees up stored resources
		void Finish()
		{
			delete []m_oList;
		}

		/// Cleans the whole buffer.
		void Reset()
		{
			for (uint i=0;i<m_uiMaxElems;i++)
			{
				m_oList[i].m_bFree  = true;
				m_oList[i].m_poElem = NULL;
			}

			m_uiNumElems = 0;			
			m_uiCurElem  = 0;
		}

		/// Adds a new element to the queue. Use this to fed the buffer with free resources.
		int iAdd(T* _poElem)
		{
			if (m_uiNumElems == m_uiMaxElems) return(-1);

			m_oList[m_uiNumElems].m_bFree  = true;
			m_oList[m_uiNumElems].m_poElem = _poElem;
			m_uiNumElems++;

			return(m_uiNumElems-1);
		}

		/// Retrieves the resource at the given array position.
		T* poGet(uint _uiIdx)
		{
			if (_uiIdx == m_uiNumElems) return(NULL);
			return( m_oList[_uiIdx].m_poElem );
		}

		/// Gets a free resource in the system.
		T* poGetResource()
		{
		   // handle circular queue
		   uint uiTestElems = 0;

		   do {

			   if ( m_oList[m_uiCurElem].m_bFree)
			   {
					m_oList[m_uiCurElem].m_bFree = false;
					return( m_oList[m_uiCurElem].m_poElem );
			   }

			   m_uiCurElem = (m_uiCurElem + 1) % m_uiNumElems;
			   uiTestElems++;

		   } while ( uiTestElems < m_uiNumElems );

			// No free resources ...
		   return(NULL);
		}

		/// Releases a used resource in the system.
		void Release(T* _poElem)
		{
		    for (uint i=0;i<m_uiNumElems;i++)
			    if ( m_oList[i].m_poElem == _poElem)
			    {
					m_oList[i].m_bFree = true;
				    return;
				}
		}

		/// Releases all the used resources in the system.
		void ReleaseAll()
		{
		    for (uint i=0;i<m_uiNumElems;i++)
				m_oList[i].m_bFree = true;
		}

   public:
		
		// The resource queue.
		CFEResourceElem<T>* m_oList;

		// Maximum elements that can be stored in the queue.
		uint	m_uiMaxElems;

		// Number of elements stored in the queue.
		uint	m_uiNumElems;

		// Current seek position in the queue.
		uint	m_uiCurElem;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
