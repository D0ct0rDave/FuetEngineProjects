// ----------------------------------------------------------------------------
/*! \class CFESpriteMgr
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
    public:

        static CFESprite* poLoadResource(const CFEString& _sFilename);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
