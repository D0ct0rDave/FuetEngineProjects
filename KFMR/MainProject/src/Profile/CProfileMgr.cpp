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
#include "CProfileMgr.h"
#include "CLibConfigFileWriter.h"
//-----------------------------------------------------------------------------
CProfile* CProfileMgr::m_oSlots = NULL;
//-----------------------------------------------------------------------------
void CProfileMgr::Register(const CFEString& _sGameName)
{
    m_oSlots = new CProfile[MAX_PROFILE_SLOTS];
    Load();
}
//-----------------------------------------------------------------------------
void CProfileMgr::LoadProfile(const CFEConfigFile& _oCfg,uint _uiProfile)
{
    CFEString sPrefix = CFEString("Profiles.Slot") + CFEString(_uiProfile);

    m_oSlots[_uiProfile].m_uiCurLevel = _oCfg.iGetInteger(sPrefix + ".Level",0);
    m_oSlots[_uiProfile].m_uiCurSkill = _oCfg.iGetInteger(sPrefix + ".Skill",0);
    m_oSlots[_uiProfile].m_uiScore    = _oCfg.iGetInteger(sPrefix + ".Score",0);
}
//-----------------------------------------------------------------------------
void CProfileMgr::SaveProfile(const CLibConfigFileWriter& _oCfg,uint _uiProfile)
{
    CLibConfigFileWriter* poCfg = (CLibConfigFileWriter*)&_oCfg;
    poCfg->OpenSection("Slot",_uiProfile);
    
        poCfg->AddVar("Level",m_oSlots[_uiProfile].m_uiCurLevel);
        poCfg->AddVar("Skill",m_oSlots[_uiProfile].m_uiCurSkill);
        poCfg->AddVar("Score",m_oSlots[_uiProfile].m_uiScore);

    poCfg->CloseSection();
}
//-----------------------------------------------------------------------------
void CProfileMgr::Load()
{
    // Load config
    CFEConfigFile oCfg;
    if (oCfg.bLoad("data/kfrm.cfg"))
    {
        for (uint i=0;i<MAX_PROFILE_SLOTS;i++)
            LoadProfile(oCfg,i);
    }
}
//-----------------------------------------------------------------------------
void CProfileMgr::Save()
{
    // Load config
    CLibConfigFileWriter oCfg;
    if (oCfg.bCreate("data/kfrm.cfg"))
    {
        oCfg.OpenSection("Profiles");
            
            for (uint i=0;i<MAX_PROFILE_SLOTS;i++)
                SaveProfile(oCfg,i);

        oCfg.CloseSection();
        oCfg.Write();
    }
}
//-----------------------------------------------------------------------------
