// ----------------------------------------------------------------------------
/*! \class CFEFontMgr
 *  \brief Class responsible of font object management
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEFontMgrH
#define CFEFontMgrH
// ----------------------------------------------------------------------------
#include "CFEFont.h"
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFEFontMgr,CFEFont)
{
    public:
        static CFEFont* poLoadResource(const CFEString& _sFilename);
        static void InvalidateResource(CFEFont* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
