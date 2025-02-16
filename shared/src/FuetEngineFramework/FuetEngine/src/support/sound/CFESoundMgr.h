// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Resource Manager Class declaration
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESoundMgrH
#define CFESoundMgrH
// ----------------------------------------------------------------------------
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
// class CFESoundMgr : public CFEResourceMgr<CFEMaterial,CFESoundMgr>
DECLARE_RESOURCE_MANAGER(CFESoundMgr,FEHandler)
{
    public:

        /// Loads the sound represented by the given filename.
        static FEHandler hLoad(const CFEString& _sFilename)
        {
            return ( (FEHandler)poLoad(_sFilename) );
        }

        /// Retrieves a sound property if it exists.
        static bool bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound property if it exists and it's possible.
        static bool bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        // Implementation of the poLoadResource function.
        static FEHandler* poLoadResource(const CFEString& _sFilename);

        // Implementation of the InvalidateResource function.
        static void InvalidateResource(FEHandler* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
