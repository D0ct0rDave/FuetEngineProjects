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
	friend class CFEResourceMgr<CFEFontMgr,CFEFont>;

    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        static CFEFont* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        static void InvalidateResource(CFEFont* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
