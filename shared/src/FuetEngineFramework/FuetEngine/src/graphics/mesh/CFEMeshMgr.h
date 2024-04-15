// ----------------------------------------------------------------------------
/*! \class CFEMeshMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMeshMgrH
#define CFEMeshMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
class CFEMesh;
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFEMeshMgr,CFEMesh)
{
	protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual CFEMesh* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(CFEMesh* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
