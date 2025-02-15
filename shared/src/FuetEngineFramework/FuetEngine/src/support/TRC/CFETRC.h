// ----------------------------------------------------------------------------
/*! \class CFETRC
 *  \brief Basic Localization Facilities.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFETRCH
#define CFETRCH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "FEEnums.h"
#include "types/CFEString.h"
// ----------------------------------------------------------------------------
namespace CFETRC
{
	typedef void(*TOpenMessageBoxFunc)(const CFEString& _sMessage,const CFEString& _sOptions);
	typedef int(*TMessageBoxStepFunc)();
	typedef bool(*TCloseMessageBoxFunc)();

	/// Initializes the TRC Technical Requirements system
	void Init();

	/// Performs a check step in the TRC system.
	void CheckStep();

	/// Registers a the function to open a message box.
	void RegisterOpenMessageBoxFunc(TOpenMessageBoxFunc _pFunc);

	/// Registers a the function to write a message in a message box.
	void RegisterMessageBoxStepFunc(TMessageBoxStepFunc _pFunc);

	/// Registers a the function to close a message box.
	void RegisterCloseMessageBoxFunc(TCloseMessageBoxFunc _pFunc);

	/// Finalizes the TRC system.
	void Finish();
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
