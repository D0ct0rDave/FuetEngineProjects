// -----------------------------------------------------------------------------
/*! \class CFEScriptVarRegistry
 *  \brief A class to store values assigned to script variables that can be retrieved later.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEScriptVarRegistryH
#define CFEScriptVarRegistryH
// -----------------------------------------------------------------------------
// Includes
#include "FEBasicTypes.h"
#include "types/CFEResourceMgr.h"
// -----------------------------------------------------------------------------
static class CFEScriptVarRegistry : public CFELookUpDatabase<FEPointer>
{
    public:
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
