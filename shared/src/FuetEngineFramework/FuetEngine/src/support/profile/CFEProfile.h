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
#ifndef CFEProfileH
#define CFEProfileH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
namespace CFEProfile
{
	/// Registers and initializes the application profile assigned area.
	void RegisterApplication(const CFEString& _sApplicationName);

	/// Saves data to the application profile assigned area.
	bool bSave(FEPointer _pData, uint _uiSize);

	/// Loads the data from the application profile assigned area.
	bool bLoad(FEPointer _pData, uint _uiSize);

	/// Cleans'up the profile information from the assigned area.
	void Reset();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
