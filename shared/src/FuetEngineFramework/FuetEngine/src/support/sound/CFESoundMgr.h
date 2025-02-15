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
DECLARE_RESOURCE_MANAGER(CFESoundMgr,FEHandler)
{
	friend class CFEResourceMgr<CFESoundMgr,FEHandler>;

    public:

        /// Loads the sound represented by the given filename.
        static FEHandler hLoad(const CFEString& _sFilename);

        /// Loads the sound represented by the given filename.
        static FEHandler hLoad(const CFEString& _sFilename,bool _bBGM);

        /// Retrieves a sound property if it exists.
        static bool bGetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

        /// Sets a sound property if it exists and it's possible.
        static bool bSetSoundProperty(FEHandler _hSound,const CFEString& _sProperty,FEPointer _pParam);

	protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        static FEHandler* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        static void InvalidateResource(FEHandler* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
