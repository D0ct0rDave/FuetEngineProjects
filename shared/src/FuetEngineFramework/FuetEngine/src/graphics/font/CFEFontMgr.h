// ----------------------------------------------------------------------------
/*! \class CFEFontMgr
 *  \brief Class responsible of font object management
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual CFEFont* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(CFEFont* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
