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
#define DECLARE_INSTANCE_MANAGER(CLASSNAME,INSTANCETYPE)\
class CLASSNAME : public CFEInstanceMgr<CLASSNAME,INSTANCETYPE>
//-----------------------------------------------------------------------------
template <class B, class T>
class CFEInstanceMgr
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
				B::m_spoInstance->FinishManager();
                delete m_spoInstance;
                m_spoInstance = NULL;
            }
        }

    protected:

		CFEInstanceMgr() : m_bInitialized(false)
		{
		}
		
		~CFEInstanceMgr()
		{
			
		}

        static B* m_spoInstance;
	
    // -------------------
	// END: singleton code
	// -------------------
	public:
	
        /// Main initialiation procedure.
        virtual void Init(uint _uiMaxInstances = 256)
		{
			for (uint i=0;i<_uiMaxInstances;i++)
			{
				CFEResourceObj<T>* poRes = new CFEResourceObj<T>;

				poRes->m_poRes   = this->poCreateInstance();
				poRes->m_uiCtxID = CFEResourceContextMgr::uiGetContextID();

		        m_oInstDB.iAdd( poRes );
			}

			m_bInitialized = true;
		}

        /// Main finalization procedure.
        virtual void FinishManager()
        {
			if (! m_bInitialized) return;

			Reset();
			
			// Destroy all the allocated data during intialization
			for (uint i=0;i<uiGetMaxInstances();i++)
			{
				this->DestroyInstance(m_oInstDB.poGet(i)->m_poRes);
				m_oInstDB.poGet(i)->m_poRes = NULL;
				delete m_oInstDB.poGet(i);
			}

			m_oInstDB.Reset();
			m_bInitialized = false;
        }
        
        /// Resets to the initial state manager.
        void Reset()
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
				if (! bIsInstanceFree(i))
					ReleaseInstance( m_oInstDB.poGet(i)->m_poRes );

			m_oInstDB.ReleaseAll();
        }

        /// Preloads the given resource before instancing it.
        void Preload(const CFEString& _sResource)
        {
			this->hGetResource(_sResource);
        }

		/// Retrieves a instance of a specific resource.
		FEHandler hGetInstance(FEHandler _hResource)
		{
			if (! m_bInitialized) return(NULL);
			if (_hResource == NULL) return(NULL);

			// Retrieves a free instance
			T* poInstance = poGetFreeInstance();

			// Sets up the instance data.
			if (poInstance!=NULL)
				this->SetupInstance(poInstance,_hResource);

			return((FEHandler)poInstance);
		};

        /// Retrieves a instance of a specific resource given its resource name.
        FEHandler hGetInstance(const char* _szResource)
        {
			return( hGetInstance( CFEString(_szResource)) );
        }

        /// Retrieves a instance of a specific resource given its resource name.
        FEHandler hGetInstance(const CFEString& _sResource)
        {
			return( hGetInstance(this->hGetResource(_sResource)) );
        }

        /// Releases a given instance.
        void ReleaseInstance(FEHandler _hInstance)
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
		    {
				if ((! bIsInstanceFree(i)) && ((FEHandler)m_oInstDB.poGet(i)->m_poRes == _hInstance))
				{
					InvalidateInstance( m_oInstDB.poGet(i)->m_poRes );
					m_oInstDB.Release(i);
					return;
				}
			}	
        }

        /// Releases the instances allocated at the given or greater context.
        void ReleaseContextInstances(uint _uiContextID)
        {
			if (! m_bInitialized) return;

			for (uint i=0;i<uiGetMaxInstances();i++)
		    {	
				CFEResourceObj<T>* poInst = m_oInstDB.poGet(i);

				if ((! bIsInstanceFree(i)) && (poInst->m_uiCtxID>=_uiContextID))
				{
					// Prefer calling this instead of directly copy the code contained withing the
					// ReleaseInstance function. In case the function is overrided it will also work.
					ReleaseInstance(poInst->m_poRes);
					m_oInstDB.Release(i);
				}
			}
		}

	protected:

		// Function to override for every type of instance.
		/// Retrieves the resource associated to a given name and returns it as a handler.
		/// virtual FEHandler hGetResource(const CFEString& _sResource)
		virtual FEHandler hGetResource(const CFEString&) {return(NULL);};	// <- avoid unused parameter warning
	
		// Function to override for every type of instance.
		/// Sets up a recently aquired instance
		/// virtual void SetupInstance(T* _poInstance,FEHandler _hResource){};
		virtual void SetupInstance(T*,FEHandler){};							// <- avoid unused parameter warning

		// Function to override for every type of instance.
		/// Creates an instance object of the specific type.
        virtual T* poCreateInstance() { return(NULL); };

		// Function to override for every type of instance.
		/// Destroys an instance object of the specific type.
        /// virtual void DestroyInstance(T* _poInstance)
        virtual void DestroyInstance(T*) {};								// <- avoid unused parameter warning

	protected:

        // Function to override for every type Resource
        /// Here the programmer can free the possible allocated data when the instance is retrieved through hGetInstance methods.
        /// virtual void InvalidateInstance(T* _poInstance)
        virtual void InvalidateInstance(T*) {};								// <- avoid unused parameter warning

	protected:

		/// Retrieves the instance object associated to a given index in the resource queue.
		T* poGetResourceInstance(uint _uiIdx)
		{
			if (! m_bInitialized) return(NULL);

			if (_uiIdx >= uiGetMaxInstances()) return(NULL);
			return(m_oInstDB.poGet(_uiIdx)->m_poRes);
		}

		/// Retrieves the maximun number of allocatables instances by the instace manager.
		uint uiGetMaxInstances()
		{
			if (! m_bInitialized) return(0);
			return(m_oInstDB.uiGetNumElems());
		}

		/// Retrieves the maximum number of allocatables instances by the instace manager.
		FEBool bIsInstanceFree(uint _uiIdx)
		{
			if (! m_bInitialized) return(false);
			return( m_oInstDB.bIsFree(_uiIdx) );
		}
	
	private:

        /// Retrieves a free instance from the resource queue.
        T* poGetFreeInstance()
        {
			CFEResourceObj<T>* poRes = m_oInstDB.poGetResource();
			if (poRes == NULL)
				return(NULL);

 			poRes->m_uiCtxID = CFEResourceContextMgr::uiGetContextID();
			return(poRes->m_poRes);
		};

	protected:

		typedef CFEResourceObj<T> CResourceEntry;
		CFEResourceQueue<CResourceEntry> m_oInstDB;

		/// 		
		FEBool	m_bInitialized;
};


template <class B, class T>
B* CFEInstanceMgr<B,T>::m_spoInstance = NULL;

//-----------------------------------------------------------------------------
#endif // CFEInstanceMgrH
//-----------------------------------------------------------------------------
