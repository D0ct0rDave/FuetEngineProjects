// ----------------------------------------------------------------------------
/*! \class CFELocal
 *  \brief Basic Localization Facilities.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
	void EnableAutoLoc(FEBool _bEnable = true);

	/// Disables the autolocalization system for file loading.
	void DisableAutoLoc();

	/// Retrieves whether the autolocalization system is active or not.
	FEBool bIsAutoLocEnabled();

	/// Retrieves the localization string ID.
	const CFEString& sGetLocalID();

	/// Retrieves the localization ID as an enumerated type.
	EFELocalID eGetLocalID();
	
	/// Sets the localization ID that will be used by the system.
	void SetLocalID(EFELocalID _eLocalID);

	/// Sets whether a specific local ID is supported or not.
	void SetLocalIDSupport(EFELocalID _eLocalID,FEBool _bSupport);

	/// Retrieves whether the specific local ID is supported or not.
	FEBool bGetLocalIDSupport(EFELocalID _eLocalID);

	/// Retrieves the local region string ID.
	const CFEString& sGetLocalRegionID();

	/// Retrieves the local region ID as an enumerated type.
	EFELocalRegionID eGetLocalRegionID();	
	
	/// Sets the localization ID that will be used by the system.
	void SetLocalRegionID(EFELocalRegionID _eLocalRegionID);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
