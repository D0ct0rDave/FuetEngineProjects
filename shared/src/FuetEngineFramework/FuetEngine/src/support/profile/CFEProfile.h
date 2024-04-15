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
#ifndef CFEProfileH
#define CFEProfileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
namespace CFEProfile
{
	/// Registers and initializes the application profile assigned area.
	void RegisterApplication(const CFEString& _sApplicationName,uint _uiApplicationVersion = 0);

	/// Saves data to the application profile assigned area.
	FEBool bSave(FEPointer _pData, uint _uiSize);

	/// Loads the data from the application profile assigned area.
	FEBool bLoad(FEPointer _pData, uint _uiSize);

	/// Retrieves the application version.
	uint uiGetVersion();

	/// Cleans'up the profile information from the assigned area.	
	void Reset();
	
	/// Finalizes the profile system.
	void Finish();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
