// ----------------------------------------------------------------------------
/*! \class CFEResourceContextMgr
 *  \brief Resource Context Management class to help the automatic deallocation of system resources.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEResourceContextMgrH
#define CFEResourceContextMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
//-----------------------------------------------------------------------------
template <typename T>
class CFEResourceObj
{
	public:
		/// The resource itself
		T*		m_poRes;

		/// The reference count of this resource.
		uint	m_uiRefCount;

		/// The context this resource was allocated.
		uint 	m_uiCtxID;		
};
//-----------------------------------------------------------------------------
namespace CFEResourceContextMgr
{   
    /// Initializes the resource context manager. Specify the maximum number of memory contexts the system will handle.
    void Init(uint _uiMaxMemContexts);

    /// Finalizes the resource context manager.
    void Finish();

    /// Push the current resource context.
    void PushContext();

    /// Pops the current context. Frees all the resources associated to the previous context.
    void PopContext();

    /// Retrieves the current context id.
    uint uiGetContextID();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
