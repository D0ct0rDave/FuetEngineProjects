// -----------------------------------------------------------------------------
/*! \class CKFRMActionLoader
 *  \brief A base class to load actions from a config file.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CKFRMActionLoaderH
#define CKFRMActionLoaderH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
// -----------------------------------------------------------------------------
class CKFRMActionLoader : public CFEActionLoader
{
	public:

		/// Loads an action from the given config file.
		virtual CFEAction* poLoad(const CFEConfigFile& _oCfg,const CFEString& _sPrefix);
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
