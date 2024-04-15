// ----------------------------------------------------------------------------
/*! \class CFELocal
 *  \brief Basic Localization Facilities. Wrapper of the CFESystem functionality.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFELocal.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
void CFELocal::EnableAutoLoc(FEBool _bEnable)
{
	if (_bEnable)
		CFECore::Local::EnableAutoLoc();
	else
		CFECore::Local::DisableAutoLoc();
}
// ----------------------------------------------------------------------------
void CFELocal::DisableAutoLoc()
{
	CFECore::Local::DisableAutoLoc();
}
// ----------------------------------------------------------------------------
FEBool CFELocal::bIsAutoLocEnabled()
{
	return( CFECore::Local::bIsAutoLocEnabled() );
}
// ----------------------------------------------------------------------------
const CFEString& sGetLocalID()
{	
	return( CFECore::Local::sGetLocalID() );
}
// ----------------------------------------------------------------------------
EFELocalID CFELocal::eGetLocalID()
{
	return( CFECore::Local::eGetLocalID() );
}
// ----------------------------------------------------------------------------
void CFELocal::SetLocalID(EFELocalID _eLocalID)
{
	CFECore::Local::SetLocalID( _eLocalID);
}
// ----------------------------------------------------------------------------
const CFEString& CFELocal::sGetLocalRegionID()
{
	return( CFECore::Local::sGetLocalRegionID() );
}
// ----------------------------------------------------------------------------
EFELocalRegionID CFELocal::eGetLocalRegionID()
{
	return( CFECore::Local::eGetLocalRegionID() );
}
// ----------------------------------------------------------------------------	
void CFELocal::SetLocalRegionID(EFELocalRegionID _eLocalRegionID)
{
	CFECore::Local::SetLocalRegionID(_eLocalRegionID);
}
// ----------------------------------------------------------------------------
void CFELocal::SetLocalIDSupport(EFELocalID _eLocalID,FEBool _bSupport)
{
	CFECore::Local::SetLocalIDSupport(_eLocalID,_bSupport);
}
// ----------------------------------------------------------------------------
FEBool CFELocal::bGetLocalIDSupport(EFELocalID _eLocalID)
{
	return( CFECore::Local::bGetLocalIDSupport(_eLocalID) );
}
// ----------------------------------------------------------------------------
