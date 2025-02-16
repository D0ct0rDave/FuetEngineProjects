//-----------------------------------------------------------------------------
#ifndef CFEResourceMgrH
#define CFEResourceMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
#include "CFELookUpDatabase.h"
//-----------------------------------------------------------------------------
#define DECLARE_RESOURCE_MANAGER(ClassName,ResourceType)\
class ClassName : public CFEResourceMgr<ResourceType,ClassName>
//-----------------------------------------------------------------------------
template <typename T,typename B>
class CFEResourceMgr
{
    public:

        /// Loads the resource identified by the given filename and returns it.
        static T* poLoad(const CFEString& _sFilename)
        {
            // Search for the object
            T* poRes = m_oResDB.poGet(_sFilename);
            if (poRes == NULL)
            {
                // Loads the resource through the virtual implementation of the function.
                poRes = B::poLoadResource(_sFilename);

                // Adds the object to the database
                m_oResDB.uiAdd(_sFilename,poRes);
                return( poRes );
            }
            else
            {
                return ( poRes );
            }
        }

        /// Function for reload the full contents of the resource manager.
        static void Reload()
        {
            for (uint i=0;i<m_oResDB.uiNumElems();i++)
            {
                const CFEString& sFilename = m_oResDB.sGetVariable(i);
                T* poNewRes = B::poLoadResource(sFilename);

                if (poNewRes != NULL)
                {
                    T* poRes = m_oResDB.poGet(i);

                    //
                    B::InvalidateResource(poRes);

                    // We have to copy new resource data into old one to prevent objects having invalid references by just
                    // substituting the contents of the element in the resource array.
                    *poRes = *poNewRes;
                }
            }
        }

        /// Invalidates the full contents of the resource manager
        static void Clear()
        {
            for (uint i=0;i<m_oResDB.uiNumElems();i++)
                InvalidateResource( m_oResDB.poGet(i) );

            m_oResDB.Clear();
        }
        
        // Retrieves the resource name associated with the given resource.
        static const CFEString& sGetResourceName(T* _poRes)
        {
            // Adds (or replaces) the object in the database
            return( m_oResDB.sGetVariable(_poRes) );
        }

        /// Registers an external resource and associates it to a name.
        /// Replaces the contents in case the resource is already in the database
        static void Register(T* _poRes,const CFEString& _sResourceName)
        {
            // Adds (or replaces) the object in the database
            m_oResDB.uiAdd(_sResourceName,_poRes);
        }

        /// Tells wether a given resource exists or not
        static bool bExists(const CFEString& _sResourceName)
        {
            return( m_oResDB.poGet(_sResourceName) != NULL );
        }

        /// Function to override for every type Resource
        static T* poLoadResource(const CFEString& _sFilename) { return(NULL); };

        /// Function to override for every type Resource
        /// Here the user can free resources or handlers belonging to this object
        static void InvalidateResource(T* _poRes){};

    // private:
    public:

        static CFELookUpDatabase<T> m_oResDB;
};

//
template <typename T,typename B>
CFELookUpDatabase<T> CFEResourceMgr<T,B> ::m_oResDB;

//-----------------------------------------------------------------------------
#endif // CFEResourceMgrH
//-----------------------------------------------------------------------------
