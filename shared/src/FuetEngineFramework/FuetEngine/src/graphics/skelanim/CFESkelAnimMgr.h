// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
    public:

        static CFESkelAnim* poLoadResource(const CFEString& _sFilename);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
