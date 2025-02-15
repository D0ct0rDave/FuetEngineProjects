// ----------------------------------------------------------------------------
/*! \class CFEResourceMgr
 *  \brief A generic template definition to manage resources.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEResourceMgrH
#define CFEResourceMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFEString.h"
#include "CFELookupDatabase.h"
#include "CFESingleton.h"
#include "Support/Mem/CFEResourceContextMgr.h"
//-----------------------------------------------------------------------------
#define DECLARE_RESOURCE_MANAGER(ClassName,ResourceType)\
class ClassName : public CFEResourceMgr<ClassName,ResourceType>
//-----------------------------------------------------------------------------
template <typename B, typename T>
class CFEResourceMgr
{	
	friend class CFEResourceMgr<B,T>;

    public:

        /// Loads the resource identified by the given filename and returns it.
        static T* poLoad(const CFEString& _sFilename)
        {
			m_bInitialized = true;

            // Search for the object
            CFEResourceObj<T>* poResEntry = CFEResDB::I()->m_oResDB.poGet(_sFilename);
            if (poResEntry == NULL)
            {
                // Loads the resource through the virtual implementation of the function.
                T* poRes = B::poLoadResource(_sFilename);

                // Adds the object to the database
                if (poRes != NULL)
					Register(poRes,_sFilename);

                return( poRes );
            }
            else
            {
				poResEntry->m_uiRefCount++;
                return ( poResEntry->m_poRes );
            }
        }

        /// Registers an external resource and associates it to a name.
        /// Replaces the contents in case the resource is already in the database
        static void Register(T* _poRes,const CFEString& _sResourceName)
        {
			m_bInitialized = true;

			// Adds the object to the database
            CFEResourceObj<T>* poResEntry = new CFEResourceObj<T>;
            poResEntry->m_poRes		= _poRes;
            poResEntry->m_uiCtxID	= CFEResourceContextMgr::uiGetContextID();
            poResEntry->m_uiRefCount= 0;

            CFEResDB::I()->m_oResDB.uiAdd(_sResourceName,poResEntry);
        }

        /// Function for reload the full contents of the resource manager.
        static void Reload()
        {
			if (! m_bInitialized) return;

            for (uint i=0;i<CFEResDB::I()->m_oResDB.uiNumElems();i++)
            {
                const CFEString& sFilename = CFEResDB::I()->m_oResDB.sGetVariable(i);
                T* poNewRes = B::poLoadResource(sFilename);

                if (poNewRes != NULL)
                {
                    CFEResourceObj<T>* poResEntry = CFEResDB::I()->m_oResDB.poGet(i);

                    // We have to copy new resource data into old one to prevent objects having invalid references by just
                    // substituting the contents of the element in the resource array.
					*poResEntry->m_poRes = *poNewRes;                    
                }
            }
        }

        /// Resets to the initial state manager.
        static void Reset()
        {
			if (! m_bInitialized) return;

            for (uint i=0;i<CFEResDB::I()->m_oResDB.uiNumElems();i++)
                B::InvalidateResource( CFEResDB::I()->m_oResDB.poGet(i)->m_poRes );

            CFEResDB::I()->m_oResDB.Clear();
        }

        /// Releases the given resource, and invalidates it if it's not being used anymore.
		static void ReleaseResource(T* _poRes)
		{
			if (! m_bInitialized) return;
			
			bool bFound = false;
            for (uint i=0;((i<CFEResDB::I()->m_oResDB.uiNumElems()) && (! bFound));i++)
            {
				if (CFEResDB::I()->m_oResDB.poGet(i)->m_poRes == _poRes)
				{
					CFEResDB::I()->m_oResDB.poGet(i)->m_uiRefCount--;

					if (CFEResDB::I()->m_oResDB.poGet(i)->m_uiRefCount == 0)
					{
						B::InvalidateResource(CFEResDB::I()->m_oResDB.poGet(i)->m_poRes);
						delete CFEResDB::I()->m_oResDB.poGet(i);

						CFEResDB::I()->m_oResDB.Delete(i);
					}

					bFound = true;
				}
			}

			// To avoid init/finish stage we should do something like this.
			if ((bFound) && (CFEResDB::I()->m_oResDB.uiNumElems()==0))
				DestroySingleton();
		}

        /// Releases the resources allocated at the given or greater context.
        static void ReleaseContextResources(uint _uiContextID)
        {
			if (! m_bInitialized) return;

            for (uint i=0;i<CFEResDB::I()->m_oResDB.uiNumElems();)
            {
				if (CFEResDB::I()->m_oResDB.poGet(i)->m_uiCtxID >= _uiContextID)
				{
					B::InvalidateResource(CFEResDB::I()->m_oResDB.poGet(i)->m_poRes);
					delete CFEResDB::I()->m_oResDB.poGet(i);
					CFEResDB::I()->m_oResDB.Delete(i);
				}
				else
					i++;
			}
			
			// To avoid init/finish stage we should do something like this.
			if (CFEResDB::I()->m_oResDB.uiNumElems()==0)
				DestroySingleton();
        }

        /// Retrieves the resource name associated with the given resource.
        static const CFEString& sGetResourceName(T* _poRes)
        {
			if (m_bInitialized)
			{
				// Adds (or replaces) the object in the database
				for (uint i=0;i<CFEResDB::I()->m_oResDB.uiNumElems();i++)
					if (CFEResDB::I()->m_oResDB.poGet(i)->m_poRes == _poRes)
						return( CFEResDB::I()->m_oResDB.sGetVariable(i) );
			}

            return( CFEString::sNULL() );
        }

        /// Tells wether a given resource exists or not
        static bool bExists(const CFEString& _sResourceName)
        {
			if (! m_bInitialized) return(false);
            return( CFEResDB::I()->m_oResDB.poGet(_sResourceName) != NULL );
        }

	protected:

        /// Function to override for every type Resource
        static T* poLoadResource(const CFEString& _sFilename) { return(NULL); };

        /// Function to override for every type Resource
        /// Here the user can free resources or handlers belonging to this object
        static void InvalidateResource(T* _poRes){};

	protected:
		
		///
		static void DestroySingleton()
		{
			CFEResDB::I()->Finish();
			m_bInitialized = false;
		}

    protected:

		DECLARE_SINGLETON(CFEResDB)
		{
			public:
				~CFEResDB()
				{
					m_oResDB.Clear();
				}
				
			typedef CFEResourceObj<T> CResourceEntry;
			CFELookUpDatabase<CResourceEntry> m_oResDB;				
		};

		/// 		
		static bool	m_bInitialized;
};
//-----------------------------------------------------------------------------
template <typename B, typename T>
bool CFEResourceMgr<B,T>::m_bInitialized = false;
//-----------------------------------------------------------------------------
#endif // CFEResourceMgrH
//-----------------------------------------------------------------------------
