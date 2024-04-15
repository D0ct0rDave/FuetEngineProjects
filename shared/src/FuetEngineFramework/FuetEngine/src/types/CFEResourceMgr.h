// ----------------------------------------------------------------------------
/*! \class CFEResourceMgr
 *  \brief A generic template definition to manage resources.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
#include "Support/misc/CFEStringUtils.h"
//-----------------------------------------------------------------------------
#define DECLARE_RESOURCE_MANAGER(CLASSNAME,RESOURCETYPE)\
class CLASSNAME : public CFEResourceMgr<CLASSNAME,RESOURCETYPE>
//-----------------------------------------------------------------------------
template <class B, class T>
class CFEResourceMgr
{
	// -------------------
	// BEGIN: singleton code
    // -------------------
	public:

        static B* I()
        {
            if (m_spoInstance == NULL)
                m_spoInstance = new B;

            return(m_spoInstance);
        }

        static void Finish()
        {
            if (m_spoInstance != NULL)
            {
                delete m_spoInstance;
                m_spoInstance = NULL;
            }
        }

    protected:

        static B* m_spoInstance;
	
    // -------------------
	// END: singleton code
	// -------------------
	
    public:

        virtual ~CFEResourceMgr()
        {
			Reset();
        };

	    /// Loads the resource identified by the given filename and returns it.
        T* poLoad(const CFEString& _sFilename)
        {
			CFEString sFilename = CFEStringUtils::sGetCanonicalPath(_sFilename);

		    // Search for the object
            CFEResourceObj<T>* poResEntry = m_oResDB.poGet(sFilename);
            if (poResEntry == NULL)
            {
                // Loads the resource through sFilename virtual implementation of the function.
                T* poRes = poLoadResource(sFilename);

                // Adds the object to the database
                if (poRes != NULL)
					Register(poRes,sFilename,true);

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
        void Register(T* _poRes,const CFEString& _sResourceName,FEBool _bFirstReference = false)
        {
			// Adds the object to the database
            CFEResourceObj<T>* poResEntry = new CFEResourceObj<T>;
            poResEntry->m_poRes		= _poRes;
            poResEntry->m_uiCtxID	= CFEResourceContextMgr::uiGetContextID();
            poResEntry->m_uiRefCount= _bFirstReference?1:0;

            m_oResDB.uiAdd(_sResourceName,poResEntry);
        }

        /// Function for reload the full contents of the resource manager.
        void Reload()
        {
			for (uint i=0;i<m_oResDB.uiNumElems();i++)
            {
                const CFEString& sFilename = m_oResDB.sGetVariable(i);
                T* poNewRes = poLoadResource(sFilename);

                if (poNewRes != NULL)
                {
                    CFEResourceObj<T>* poResEntry = m_oResDB.poGetAt(i);

                    // We have to copy new resource data into old one to prevent objects having invalid references by just
                    // substituting the contents of the element in the resource array.
					*poResEntry->m_poRes = *poNewRes;
                }
            }
        }

        /// Resets to the initial state manager.
        void Reset()
        {
			for (uint i=0;i<m_oResDB.uiNumElems();i++)
			{
                this->InvalidateResource( m_oResDB.poGetAt(i)->m_poRes );
				delete m_oResDB.poGetAt(i);
			}

            m_oResDB.Clear();
        }

        /// Releases the given resource, and invalidates it if it's not being used anymore.
		void ReleaseResource(T* _poRes)
		{
            for (uint i=0;i<m_oResDB.uiNumElems();i++)
            {
				// Found ?
				if (m_oResDB.poGetAt(i)->m_poRes == _poRes)
				{
					m_oResDB.poGetAt(i)->m_uiRefCount--;

					if (m_oResDB.poGetAt(i)->m_uiRefCount == 0)
					{
						InvalidateResource(m_oResDB.poGetAt(i)->m_poRes);
						delete m_oResDB.poGetAt(i);

						m_oResDB.Delete(i);
					}

					return;
				}
			}
		}

        /// Releases the resources allocated at the given or greater context.
        void ReleaseContextResources(uint _uiContextID)
        {
			for (uint i=0;i<m_oResDB.uiNumElems();)
            {
				if (m_oResDB.poGetAt(i)->m_uiCtxID >= _uiContextID)
				{
					InvalidateResource(m_oResDB.poGetAt(i)->m_poRes);
					delete m_oResDB.poGetAt(i);			

					m_oResDB.Delete(i);
				}
				else
					i++;
			}
        }

        /// Retrieves the resource name associated with the given resource.
        const CFEString& sGetResourceName(T* _poRes)
        {

            // Adds (or replaces) the object in the database
            for (uint i=0;i<m_oResDB.uiNumElems();i++)
                if (m_oResDB.poGetAt(i)->m_poRes == _poRes)
                    return( m_oResDB.sGetVariable(i) );

            return( CFEString::sNULL() );
        }

        /// Tells wether a given resource exists or not
        FEBool bExists(const CFEString& _sResourceName)
        {
			return( m_oResDB.poGet(_sResourceName) != NULL );
        }

	protected:

        /// Function to override for every type Resource
        /// virtual T* poLoadResource(const CFEString& _sFilename)
        virtual T* poLoadResource(const CFEString&) { return(NULL); };    	// <- avoid unused parameter warning

        /// Function to override for every type Resource
        /// Here the user can free resources or handlers belonging to this object
        /// virtual void InvalidateResource(T* _poRes)
        virtual void InvalidateResource(T*){}; 								// <- avoid unused parameter warning

	protected:

		typedef CFEResourceObj<T> CResourceEntry;
        CFELookUpDatabase<CResourceEntry> m_oResDB;
};

template <class B, class T>
B* CFEResourceMgr<B,T>::m_spoInstance = NULL;

//-----------------------------------------------------------------------------
#endif // CFEResourceMgrH
//-----------------------------------------------------------------------------
