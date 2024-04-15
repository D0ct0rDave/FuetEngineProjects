// ----------------------------------------------------------------------------
/*! \class CFEProfile
 *  \brief Basic profile management.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEProfile.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
/// Registers and initializes the application profile assigned area.
void CFEProfile::RegisterApplication(const CFEString& _sApplicationName,uint _uiApplicationVersion)
{
	CFECore::Profile::RegisterApplication(_sApplicationName,_uiApplicationVersion);
}
// ----------------------------------------------------------------------------
/// Saves data to the application profile assigned area.
FEBool CFEProfile::bSave(FEPointer _pData, uint _uiSize)
{
	return( CFECore::Profile::bSave(_pData,_uiSize));
}
// ----------------------------------------------------------------------------
/// Loads the data from the application profile assigned area.
FEBool CFEProfile::bLoad(FEPointer _pData, uint _uiSize)
{
	return( CFECore::Profile::bLoad(_pData,_uiSize));
}
// ----------------------------------------------------------------------------
/// Cleans'up the profile information from the assigned area.
void CFEProfile::Reset()
{
	CFECore::Profile::Reset();
}
// ----------------------------------------------------------------------------
/// Finalizes the profile system.
void CFEProfile::Finish()
{
	CFECore::Profile::Finish();
}
// ----------------------------------------------------------------------------
uint CFEProfile::uiGetVersion()
{
	return( CFECore::Profile::uiGetVersion() );
}
// ----------------------------------------------------------------------------
