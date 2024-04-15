// ----------------------------------------------------------------------------
/*! \class CFESoundMgr
 *  \brief Sound Resource Manager Class declaration
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESoundMgrH
#define CFESoundMgrH
// ----------------------------------------------------------------------------
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFESoundMgr,FEHandler)
{
    public:

        /// Loads the sound represented by the given filename.
        FEHandler hLoad(const CFEString& _sFilename);

        /// Loads the sound represented by the given filename.
        FEHandler hLoad(const CFEString& _sFilename,FEBool _bBGM);

        /// Retrieves a sound property if it exists.
        FEBool bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound property if it exists and it's possible.
        FEBool bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

	protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual FEHandler* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(FEHandler* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
