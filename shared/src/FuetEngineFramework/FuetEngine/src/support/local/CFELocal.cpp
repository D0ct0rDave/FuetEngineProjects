// ----------------------------------------------------------------------------
/*! \class CFELocal
 *  \brief Basic Localization Facilities. Wrapper of the CFESystem functionality.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFELocal.h"
#include "system/CFESystem.h"
// ----------------------------------------------------------------------------
void CFELocal::EnableAutoLoc(bool _bEnable)
{
	if (_bEnable)
		CFESystem::Local::EnableAutoLoc();
	else
		CFESystem::Local::DisableAutoLoc();
}
// ----------------------------------------------------------------------------
void CFELocal::DisableAutoLoc()
{
	CFESystem::Local::DisableAutoLoc();
}
// ----------------------------------------------------------------------------
bool CFELocal::bIsAutoLocEnabled()
{
	return( CFESystem::Local::bIsAutoLocEnabled() );
}
// ----------------------------------------------------------------------------
const CFEString& sGetLocalID()
{	
	return( CFESystem::Local::sGetLocalID() );
}
// ----------------------------------------------------------------------------
EFELocalID CFELocal::eGetLocalID()
{
	return( CFESystem::Local::eGetLocalID() );
}
// ----------------------------------------------------------------------------
void CFELocal::SetLocalID(EFELocalID _eLocalID)
{
	CFESystem::Local::SetLocalID( _eLocalID);
}
// ----------------------------------------------------------------------------
