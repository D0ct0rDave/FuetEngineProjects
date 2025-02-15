//-----------------------------------------------------------------------------
/*! \class CProfileMgr
 *  \brief This class manages the loading and saving of the different profiles.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CProfileMgrH
#define CProfileMgrH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CLibConfigFileWriter.h"
#include "CProfile.h"
//-----------------------------------------------------------------------------
const uint MAX_PROFILE_SLOTS = 3;
//-----------------------------------------------------------------------------
class CProfileMgr
{
	public:

		/// Registers the game for using the profile manager. 
		/// And creates save data in case it is not created.
		static void Register(const CFEString& _sGameName);

		/// Loads the profile data into the profile slots
		static void Load();
		
		/// Save the slot data in the profile slots.
		static void Save();

		/// Profile slots
		static CProfile* m_oSlots;
    
    protected:
        
        static void LoadProfile(const CFEConfigFile& _oCfg,uint _uiProfile);
        static void SaveProfile(const CLibConfigFileWriter& _oCfg,uint _uiProfile);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
