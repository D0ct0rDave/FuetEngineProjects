// ----------------------------------------------------------------------------
/*! \class
 *  \brief 
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (LOCAL == USE_STDLOCAL)
// ----------------------------------------------------------------------------
#include <string.h>
#include "../CFESystem.h"
#include "../CFESystemGlobals.h"
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Localization related stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
void CFESystem::Local::EnableAutoLoc()
{
	FESglobals.m_bAutoLocEnabled = true;
}
// ----------------------------------------------------------------------------
void CFESystem::Local::DisableAutoLoc()
{
	FESglobals.m_bAutoLocEnabled = false;
}
// ----------------------------------------------------------------------------
bool CFESystem::Local::bIsAutoLocEnabled()
{
	return(FESglobals.m_bAutoLocEnabled);
}
// ----------------------------------------------------------------------------
const CFEString LOCALID_STRINGS[LID_NUM_IDs] = 
{
	CFEString("common"),// LID_COMMON,
	CFEString("es"),	// LID_ES,
	CFEString("en"),	// LID_EN,
	CFEString("fr"),	// LID_FR,
	CFEString("it"),	// LID_IT,
	CFEString("de"),	// LID_DE,
	CFEString("du"),	// LID_DU,
	CFEString("us_es"), // LID_US_ES,
	CFEString("us_en"), // LID_US_EN,
	CFEString("us_fr"), // LID_US_FR,
};
// ----------------------------------------------------------------------------
const CFEString& CFESystem::Local::sGetLocalID()
{
	return( LOCALID_STRINGS[eGetLocalID()] );
}
// ----------------------------------------------------------------------------
const CFEString& CFESystem::Local::sGetLocalID(EFELocalID _eLocalID)
{
	if ((_eLocalID>=LID_COMMON) && (_eLocalID<LID_NUM_IDs))
		return( LOCALID_STRINGS[_eLocalID] );
	else
		return(CFEString::sNULL());
}
// ----------------------------------------------------------------------------
EFELocalID CFESystem::Local::eGetLocalID()
{
	return(FESglobals.m_eLocalID);
}
// ----------------------------------------------------------------------------
void CFESystem::Local::SetLocalID(EFELocalID _eLocalID)
{
	if ((_eLocalID>=LID_COMMON) && (_eLocalID<LID_NUM_IDs))
		FESglobals.m_eLocalID = _eLocalID;
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
