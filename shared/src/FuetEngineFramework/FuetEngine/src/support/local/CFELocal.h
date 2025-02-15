// ----------------------------------------------------------------------------
/*! \class CFELocal
 *  \brief Basic Localization Facilities.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFELocalH
#define CFELocalH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
namespace CFELocal
{
	/// Enables the autolocalization system for file loading.
	void EnableAutoLoc(bool _bEnable = true);

	/// Disables the autolocalization system for file loading.
	void DisableAutoLoc();

	/// Retrieves whether the autolocalization system is active or not.
	bool bIsAutoLocEnabled();

	/// Retrieves the localization string ID.
	const CFEString& sGetLocalID();

	/// Retrieves the localization ID as an enumerated type.
	EFELocalID eGetLocalID();
	
	/// Sets the localization ID that will be used by the system.
	void SetLocalID(EFELocalID _eLocalID);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
