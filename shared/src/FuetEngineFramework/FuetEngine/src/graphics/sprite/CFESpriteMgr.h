// ----------------------------------------------------------------------------
/*! \class CFESpriteMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESpriteMgrH
#define CFESpriteMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
class CFESprite;
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFESpriteMgr,CFESprite)
{
    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual CFESprite* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(CFESprite* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
