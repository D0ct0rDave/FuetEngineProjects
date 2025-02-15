// ----------------------------------------------------------------------------
/*! \class CFEInstanceMgr
 *  \brief A generic template definition to manage instances to resources.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEInstanceMgrH
#define CFEInstanceMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFEString.h"
#include "CFESingleton.h"
#include "CFEResourceQueue.h"

#include "Support/Mem/CFEResourceContextMgr.h"
//-----------------------------------------------------------------------------
#define DECLARE_INSTANCE_MANAGER(ClassName,InstanceType)\
class ClassName : public CFEInstanceMgr<ClassName, InstanceType>
//-----------------------------------------------------------------------------
template <typename B, typename T>
class CFEInstanceMgr
{
	friend class CFEInstanceMgr<B,T>;
	
    public:

        /// Main initialiation procedure.
        static void Init(uint _uiMaxInstances = 256)
		{
			for (uint i=0;i<_uiMaxInstances;i++)
			{
				CFEResourceObj<T>* poRes = new CFEResourceObj<T>;

				poRes->m_poRes   = B::poCreateInstance();
				poRes->m_uiCtxID = CFEResourceContextMgr::uiGetContextID();

		        CFEInstDB::I()->m_oInstDB.iAdd( poRes );
			}

			m_bInitialized = true;
		}

        /// Main finalization procedure.
        static void Finish()
        {
			if (! m_bInitialized) return;

			B::Reset();
			
			// Destroy all the allocated data during intialization
			for (uint i=0;i<uiGetMaxInstances();i++)
			{
				B::DestroyInstance(CFEInstDB::I()->m_oInstDB.poGet(i)->m_poRes);
				CFEInstDB::I()->m_oInstDB.poGet(i)->m_poRes = NULL;
				delete CFEInstDB::I()->m_oInstDB.poGet(i);
			}

			CFEInstDB::I()->m_oInstDB.Reset();
			CFEInstDB::I()->Finish();
			m_bInitialized = false;
        }
        
        /// Resets to the initial state manager.
        static void Reset()
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
				if (! bIsInstanceFree(i))
					B::ReleaseInstance( CFEInstDB::I()->m_oInstDB.poGet(i)->m_poRes );

			CFEInstDB::I()->m_oInstDB.ReleaseAll();
        }

        /// Preloads the given resource before instancing it.
        static void Preload(const CFEString& _sResource)
        {
			B::hGetResource(_sResource);
        }

		/// Retrieves a instance of a specific resource.
		static FEHandler hGetInstance(FEHandler _hResource)
		{
			if (! m_bInitialized) return(NULL);
			if (_hResource == NULL) return(NULL);

			// Retrieves a free instance
			T* poInstance = B::poGetFreeInstance();

			// Sets up the instance data.
			if (poInstance!=NULL)
				B::SetupInstance(poInstance,_hResource);

			return((FEHandler)poInstance);
		};

        /// Retrieves a instance of a specific resource given its resource name.
        static FEHandler hGetInstance(const char* _szResource)
        {
			return( B::hGetInstance( CFEString(_szResource)) );
        }

        /// Retrieves a instance of a specific resource given its resource name.
        static FEHandler hGetInstance(const CFEString& _sResource)
        {
			return( B::hGetInstance(B::hGetResource(_sResource)) );
        }

        /// Releases a given instance.
        static void ReleaseInstance(FEHandler _hInstance)
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
		    {
				if ((! bIsInstanceFree(i)) && ((FEHandler)CFEInstDB::I()->m_oInstDB.poGet(i)->m_poRes == _hInstance))
				{
					B::InvalidateInstance( CFEInstDB::I()->m_oInstDB.poGet(i)->m_poRes );
					CFEInstDB::I()->m_oInstDB.Release(i);
					return;
				}
			}	
        }

        /// Releases the instances allocated at the given or greater context.
        static void ReleaseContextInstances(uint _uiContextID)
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
		    {	
				CFEResourceObj<T>* poInst = CFEInstDB::I()->m_oInstDB.poGet(i);

				if ((! bIsInstanceFree(i)) && (poInst->m_uiCtxID>=_uiContextID))
				{
					// Prefer calling this instead of directly copy the code contained withing the
					// ReleaseInstance function. In case the function is overrided it will also work.
					B::ReleaseInstance(poInst->m_poRes);
					CFEInstDB::I()->m_oInstDB.Release(i);
				}
			}
		}

	protected:

		// Function to override for every type of instance.
		/// Retrieves the resource associated to a given name and returns it as a handler.
		static FEHandler hGetResource(const CFEString& _sResource){ return(NULL); };

		// Function to override for every type of instance.
		/// Sets up a recently aquired instance
		static void SetupInstance(T* _poInstance,FEHandler _hResource){};

		// Function to override for every type of instance.
		/// Creates an instance object of the specific type.
        static T* poCreateInstance() { return(NULL); };

		// Function to override for every type of instance.
		/// Destroys an instance object of the specific type.
        static void DestroyInstance(T* _poInstance) {  };

	protected:

        // Function to override for every type Resource
        /// Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
        static void InvalidateInstance(T* _poInstance){};

	protected:

		/// Retrieves the instance object associated to a given index in the resource queue.
		static T* poGetResourceInstance(uint _uiIdx)
		{
			if (! m_bInitialized) return(NULL);

			if (_uiIdx >= uiGetMaxInstances()) return(NULL);
			return(CFEInstDB::I()->m_oInstDB.poGet(_uiIdx)->m_poRes);
		}

		/// Retrieves the maximun number of allocatables instances by the instace manager.
		static uint uiGetMaxInstances()
		{
			if (! m_bInitialized) return(0);
			return(CFEInstDB::I()->m_oInstDB.uiGetNumElems());
		}

		/// Retrieves the maximun number of allocatables instances by the instace manager.
		static bool bIsInstanceFree(uint _uiIdx)
		{
			if (! m_bInitialized) return(false);
			return( CFEInstDB::I()->m_oInstDB.bIsFree(_uiIdx) );
		}
	
	private:

        /// Retrieves a free instance from the resource queue.
        static T* poGetFreeInstance()
        {
			CFEResourceObj<T>* poRes = CFEInstDB::I()->m_oInstDB.poGetResource();
			poRes->m_uiCtxID = CFEResourceContextMgr::uiGetContextID();
			return(poRes->m_poRes);
		};

	protected:

		DECLARE_SINGLETON(CFEInstDB)
		{
			public:
				typedef CFEResourceObj<T> CResourceEntry;
				CFEResourceQueue<CResourceEntry> m_oInstDB;
				~CFEInstDB()
				{
				}
		};
		
		/// 		
		static bool	m_bInitialized;
};
//-----------------------------------------------------------------------------
template <typename B, typename T>
bool CFEInstanceMgr<B,T> ::m_bInitialized = false;
//-----------------------------------------------------------------------------
#endif // CFEInstanceMgrH
//-----------------------------------------------------------------------------
