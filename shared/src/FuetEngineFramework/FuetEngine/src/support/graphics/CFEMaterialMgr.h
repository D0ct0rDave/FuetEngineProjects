// ----------------------------------------------------------------------------
/*! \class CFEMaterialMgr
 *  \brief Material Manager Class implementation
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMaterialMgrH
#define CFEMaterialMgrH
// ----------------------------------------------------------------------------
#include "types/CFEResourceMgr.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFEMaterialMgr,FEHandler)
{
    public:

        /// Loads the material represented by the given filename.
        FEHandler hLoad(const CFEString& _sFilename)
        {
            return ( (FEHandler)poLoad(_sFilename) );
        }

        /// Function for reload the full contents of the resource manager. Overrided from base class.
        void Reload();

        /// Retrieves a material property if it exists.
        FEBool bGetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a material property if it exists and it's possible.
        FEBool bSetMaterialProperty(FEHandler _hMaterial,const CFEString& _sProperty,FEPointer _pParam);

	protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual FEHandler* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(FEHandler* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
