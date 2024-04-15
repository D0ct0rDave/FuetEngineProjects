// ----------------------------------------------------------------------------
/*! \class
 *  \brief 
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "core/FEConfig.h"
#if (LOCAL == USE_STDLOCAL)
// ----------------------------------------------------------------------------
#include "core/CFECore.h"
#include "core/CFECoreGlobals.h"
// ----------------------------------------------------------------------------
#include <string.h>
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
/// Localization related stuff.
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// by default we support all the languages
static FEBool m_bSupportedLanguage[LID_NUM_IDs] =
{
	true, // LID_COMMON = 0,

	true, // LID_ES,						// spanish
	true, // LID_EN,						// english
	true, // LID_FR,						// french
	true, // LID_IT,						// italian
	true, // LID_DE,						// deutch (german)
	true, // LID_DU,						// dutch
	true, // LID_PT,						// portuguese

	true, // LID_US_ES,						// us spanish
	true, // LID_US_EN,						// us english
	true, // LID_US_FR,						// us french
	true, // LID_US_PT,						// us portuguese
	
	true, // LID_JP,						// japanish
};
// ----------------------------------------------------------------------------
void CFECore::Local::EnableAutoLoc()
{
	FECglobals.m_bAutoLocEnabled = true;
}
// ----------------------------------------------------------------------------
void CFECore::Local::DisableAutoLoc()
{
	FECglobals.m_bAutoLocEnabled = false;
}
// ----------------------------------------------------------------------------
FEBool CFECore::Local::bIsAutoLocEnabled()
{
	return(FECglobals.m_bAutoLocEnabled);
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
	CFEString("pt"),	// LID_PT

	CFEString("us_es"), // LID_US_ES,
	CFEString("us_en"), // LID_US_EN,
	CFEString("us_fr"), // LID_US_FR,
	CFEString("us_pt"),	// LID_US_PT

	CFEString("jp"),	// LID_JP
};
// ----------------------------------------------------------------------------
const CFEString& CFECore::Local::sGetLocalID()
{
	return( LOCALID_STRINGS[eGetLocalID()] );
}
// ----------------------------------------------------------------------------
const CFEString& CFECore::Local::sGetLocalID(EFELocalID _eLocalID)
{
	if ((_eLocalID>=LID_COMMON) && (_eLocalID<LID_NUM_IDs))
		return( LOCALID_STRINGS[_eLocalID] );
	else
		return(CFEString::sNULL());
}
// ----------------------------------------------------------------------------
EFELocalID CFECore::Local::eGetLocalID()
{
	return(FECglobals.m_eLocalID);
}
// ----------------------------------------------------------------------------
void CFECore::Local::SetLocalIDSupport(EFELocalID _eLocalID,FEBool _bSupport)
{
	if ((_eLocalID>LID_COMMON) && (_eLocalID<LID_NUM_IDs))	// note > LID_COMMON
	{
		m_bSupportedLanguage[_eLocalID] = _bSupport;
		
		// If the current local ID not supported
		if ((eGetLocalID() == _eLocalID) && (! m_bSupportedLanguage[_eLocalID]))
		{
			if (FECglobals.m_eLocalRegionID == LRID_EU)
				// for Europe set English as default
				FECglobals.m_eLocalID = LID_DEFAULT;
			else
				// for US set US English as default.
				FECglobals.m_eLocalID = LID_US_DEFAULT;
		}
	}
}
// ----------------------------------------------------------------------------
FEBool CFECore::Local::bGetLocalIDSupport(EFELocalID _eLocalID)
{
	if ((_eLocalID>=LID_COMMON) && (_eLocalID<LID_NUM_IDs))	// note >= LID_COMMON
		return(m_bSupportedLanguage[_eLocalID]);
	else
		return(false);
}
// ----------------------------------------------------------------------------
void CFECore::Local::SetLocalID(EFELocalID _eLocalID)
{
	if ((_eLocalID>=LID_COMMON) && (_eLocalID<LID_NUM_IDs))
	{
		if (m_bSupportedLanguage[_eLocalID])
		{
			FECglobals.m_eLocalID = _eLocalID;
		}
		else
		{
			if (FECglobals.m_eLocalRegionID == LRID_EU)
				// for Europe set English as default
				FECglobals.m_eLocalID = LID_DEFAULT;
			else
				// for US set US English as default.
				FECglobals.m_eLocalID = LID_US_DEFAULT;
		}
	}
}
// ----------------------------------------------------------------------------
const CFEString LOCALREGIONID_STRINGS[LRID_NUM_IDs] = 
{
	CFEString("CM"),	// LRID_COMMON,
	CFEString("US"),	// LRID_US,
	CFEString("EU"),	// LRID_EU,
	CFEString("JP"),	// LRID_FR,
	CFEString("AS"),	// LRID_ASS,
};
// ----------------------------------------------------------------------------
const CFEString& CFECore::Local::sGetLocalRegionID()
{
	return( LOCALREGIONID_STRINGS[eGetLocalRegionID()] );	
}
// ----------------------------------------------------------------------------
const CFEString& CFECore::Local::sGetLocalRegionID(EFELocalRegionID _eLocalRegionID)
{
	if ((_eLocalRegionID>=LRID_COMMON) && (_eLocalRegionID<LRID_NUM_IDs))
		return( LOCALREGIONID_STRINGS[_eLocalRegionID] );
	else
		return(CFEString::sNULL());
}
// ----------------------------------------------------------------------------
EFELocalRegionID CFECore::Local::eGetLocalRegionID()
{
	return(FECglobals.m_eLocalRegionID);
}
// ----------------------------------------------------------------------------
void CFECore::Local::SetLocalRegionID(EFELocalRegionID _eLocalRegionID)
{
	if ((_eLocalRegionID>=LRID_COMMON) && (_eLocalRegionID<LRID_NUM_IDs))
	{
		// if (FECglobals.m_eLocalRegionID != _eLocalRegionID) // DO NOT PERFORM THIS TEST: FORCE SETUP
		{
			FECglobals.m_eLocalRegionID = _eLocalRegionID;
			switch (_eLocalRegionID)
			{
				// ------------------------------				
				// Force US Region
				// ------------------------------
				case LRID_US:
				{
					// perform conversion between region locales
					switch (FECglobals.m_eLocalID)
					{
						case LID_ES:
						case LID_US_ES:
						{
							FECglobals.m_eLocalID = LID_US_ES;
						}
						break;

						case LID_EN:
						case LID_US_EN:
						{
							FECglobals.m_eLocalID = LID_US_EN;
						}
						break;
						
						case LID_FR:
						case LID_US_FR:
						{
							FECglobals.m_eLocalID = LID_US_FR;
						}
						break;

						case LID_PT:
						case LID_US_PT:
						{
							FECglobals.m_eLocalID = LID_US_PT;
						}	
						break;
						
						// Rest of this region unsupported locales
						// LID_IT,						// italian
						// LID_DE,						// deutch (german)
						// LID_DU,						// dutch
						// LID_JP,						// japanish
						default:
							FECglobals.m_eLocalID = LID_US_EN;
					}

					// If the selected local is not supported
					if (! m_bSupportedLanguage[FECglobals.m_eLocalID])
					{
						// fall back to default language
						FECglobals.m_eLocalID = LID_US_DEFAULT;
					}
				}
				break;

				// ------------------------------
				// Force EU
				// ------------------------------
				case LRID_EU:
				{
					// perform conversion between region locales
					switch (FECglobals.m_eLocalID)
					{
						case LID_ES:
						case LID_US_ES:
						{
							FECglobals.m_eLocalID = LID_ES;
						}
						break;

						case LID_EN:
						case LID_US_EN:
						{
							FECglobals.m_eLocalID = LID_EN;
						}
						break;
						
						case LID_FR:
						case LID_US_FR:
						{
							FECglobals.m_eLocalID = LID_FR;
						}
						break;

						case LID_PT:
						case LID_US_PT:
						{
							FECglobals.m_eLocalID = LID_PT;
						}
						break;
						
						// Rest of this region unsupported locales
						case LID_JP:
						{
							FECglobals.m_eLocalID = LID_EN;
						}
						break;

						// Leave as is...
						default:
						{
							// LID_IT,						// italian
							// LID_DE,						// deutch (german)
							// LID_DU,						// dutch
							// do nothing
						}
					}

					// If the selected local is not supported
					if (! m_bSupportedLanguage[FECglobals.m_eLocalID])
					{
						// fallback to default language
						FECglobals.m_eLocalID = LID_DEFAULT;
					}
				}
				break;

				// ------------------------------
				// Force JP
				// ------------------------------
				case LRID_JP:
				{
					FECglobals.m_eLocalID = LID_JP;
				}
				break;

				default:
				{
					// do nothing??
				}
			}
		}
	}
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
