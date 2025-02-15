// ----------------------------------------------------------------------------
/*! \class CFEProfile
 *  \brief Basic profile management.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEProfile.h"
#include "system/CFESystem.h"
// ----------------------------------------------------------------------------
/// Registers and initializes the application profile assigned area.
void CFEProfile::RegisterApplication(const CFEString& _sApplicationName)
{
	CFESystem::Profile::RegisterApplication(_sApplicationName);
}
// ----------------------------------------------------------------------------
/// Saves data to the application profile assigned area.
bool CFEProfile::bSave(FEPointer _pData, uint _uiSize)
{
	return( CFESystem::Profile::bSave(_pData,_uiSize));
}
// ----------------------------------------------------------------------------
/// Loads the data from the application profile assigned area.
bool CFEProfile::bLoad(FEPointer _pData, uint _uiSize)
{
	return( CFESystem::Profile::bLoad(_pData,_uiSize));
}
// ----------------------------------------------------------------------------
/// Cleans'up the profile information from the assigned area.
void CFEProfile::Reset()
{
	CFESystem::Profile::Reset();
}
// ----------------------------------------------------------------------------