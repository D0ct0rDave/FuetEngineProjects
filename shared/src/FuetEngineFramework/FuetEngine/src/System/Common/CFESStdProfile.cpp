// ----------------------------------------------------------------------------
/*! \class
 *  \brief Profile functionality based on standard filesystem files.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (PROFILE == USE_STDPROFILE)
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include "../CFESystem.h"
#include "../CFESystemGlobals.h"
#include "FEBasicTypes.h"

// #define DEBUG_MESSAGES
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Profile functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Registers and initializes the application profile assigned area.
void CFESystem::Profile::RegisterApplication(const CFEString& _sApplicationName)
{
	FESglobals.m_sApplicationName = _sApplicationName;
}
// ----------------------------------------------------------------------------
/// Saves data to the application profile assigned area.
bool CFESystem::Profile::bSave(FEPointer _pData, uint _uiSize)
{
	#ifdef DEBUG_MESSAGES
	CFESystem::TRC::OpenMessageBox("Creating Savegame Data.\n"
									 "Do not power off the console.",
									 "Ok\nCancel");
	int iRes = 0;
	do
	{
		iRes = CFESystem::TRC::iMessageBoxStep();
	}
	while(iRes == -1);
	do {} while(! CFESystem::TRC::bCloseMessageBox());
	#endif

	CFEString sFilename = FESglobals.m_sApplicationName+".sav";
	FEHandler hHnd = CFESystem::File::hOpen(sFilename,FOM_WRITE);
	if (hHnd!=NULL)
	{
		CFESystem::File::uiWrite(hHnd,_pData,_uiSize);
		CFESystem::File::Close(hHnd);
		return(true);
	}

	return(false);
}
// ----------------------------------------------------------------------------
/// Loads the data from the application profile assigned area.
bool CFESystem::Profile::bLoad(FEPointer _pData, uint _uiSize)
{
	CFEString sFilename = FESglobals.m_sApplicationName+".sav";
	FEHandler hHnd = CFESystem::File::hOpen(sFilename,FOM_READ);
	if (hHnd!=NULL)
	{
		CFESystem::File::uiRead(hHnd,_pData,_uiSize);
		CFESystem::File::Close(hHnd);
		return(true);
	}
	else
	{
		CFESystem::TRC::OpenMessageBox("Creating Savegame Data.\n"
									   "Please, do not power off.",
									   "");
		int iRes = 0;
		FEReal rTime  = _0r;
		uint uiStartTick = CFESystem::uiGetEngineTicks();
		do
		{
			iRes = CFESystem::TRC::iMessageBoxStep();
			uint uiCurTick = CFESystem::uiGetEngineTicks();
			rTime = CFESystem::rGetTickTime(uiCurTick - uiStartTick);
		}
		while(rTime < _4r);
		do {} while(!CFESystem::TRC::bCloseMessageBox());
		
		return( CFESystem::Profile::bSave(_pData,_uiSize) );
	}
}

void CFESystem::Profile::Reset()
{
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
