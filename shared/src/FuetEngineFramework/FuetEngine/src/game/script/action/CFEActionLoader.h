// -----------------------------------------------------------------------------
/*! \class CFEActionLoader
 *  \brief A base class to load actions from a config file.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEActionLoaderH
#define CFEActionLoaderH
// -----------------------------------------------------------------------------
#include "CFEAction.h"
#include "support/config/CFEConfigFile.h"
#include "types/CFEString.h"
// -----------------------------------------------------------------------------
class CFEActionLoader
{
	public:

		/// Loads an action from the given config file.
		virtual CFEAction* poLoad(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
