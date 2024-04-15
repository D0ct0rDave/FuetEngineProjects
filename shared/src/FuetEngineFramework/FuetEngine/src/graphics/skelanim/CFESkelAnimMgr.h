// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimMgrH
#define CFESkelAnimMgrH
//-----------------------------------------------------------------------------
#include "types/CFEResourceMgr.h"
#include "types/CFEString.h"
//-----------------------------------------------------------------------------
class CFESkelAnim;
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFESkelAnimMgr,CFESkelAnim)
{
    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        virtual CFESkelAnim* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        virtual void InvalidateResource(CFESkelAnim* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
