//-----------------------------------------------------------------------------
/*! \class CMapMgr
 *  \brief KFRM Map Loader & Manager
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CMapMgr.h"
#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityFactory.h"
#include "Game/Helpers/CGameHelper.h"
//-----------------------------------------------------------------------------
CFEArray<CEntity*> CMapMgr::m_poMapEntities;
//-----------------------------------------------------------------------------
typedef enum EKFRMControlPointType
{
    KFRMCPT_NONE = 0,
    
    KFRMCPT_START_POINT = 1,
    KFRMCPT_END_POINT = 2,
    KFRMCPT_CINEMATIC = 3,
    
    KFRMCPT_NUMCPS,
};
//-----------------------------------------------------------------------------
void CMapMgr::Init(const CFEString& _sMapName)
{
    Load(_sMapName);
}
//-----------------------------------------------------------------------------
void CMapMgr::Load(const CFEString& _sMapName)
{
 	goGameGlobals.m_poMap = CFEMapLoader::poLoad(_sMapName);
 	goGameGlobals.m_poMap->m_oBV.m_oIni.x += goGameGlobals.m_rScreenMargin;
 	goGameGlobals.m_poMap->m_oBV.m_oEnd.x -= goGameGlobals.m_rScreenMargin;

 	CFEString sKFRMMapName = _sMapName + ".km";
    CFEConfigFile oCfg(sKFRMMapName);
    if (oCfg.bInitialized())
    {
        CFEString sVar;
        uint i;
        // ------------------------------------
        // Retrieve control points
        // ------------------------------------
        sVar = "KFRMMap.ControlPoints";
        uint uiNumCPs = oCfg.iGetInteger(sVar + ".NumCPs",0);
        for (i=0;i<uiNumCPs;i++)
        {
            CFEString sIVar = sVar + CFEString(".CP") + CFEString(i);
            CFEVect2 oPos;
            oPos.x = oCfg.rGetReal(sIVar + ".X",_0r);
            oPos.y = oCfg.rGetReal(sIVar + ".Y",_0r);
            
            uint uiType = oCfg.iGetInteger(sIVar + ".Type",0);

            switch (uiType)
            {
                case KFRMCPT_START_POINT:
                goGameGlobals.m_oStartPos = oPos;
                break;

                case KFRMCPT_END_POINT:
                goGameGlobals.m_oEndPos = oPos;
                break;

                case KFRMCPT_CINEMATIC:
                // add to an array of cinematic points...
                break;
            }
        }
        
        // ------------------------------------
        // Retrieve particle FXs
        // ------------------------------------
        sVar = "KFRMMap.ParticleSystems";
        uint uiNumPSs = oCfg.iGetInteger(sVar + ".NumPSs",0);
        for (i=0;i<uiNumPSs;i++)
        {
            CFEString sIVar = sVar + CFEString(".PS") + CFEString(i);
            CFEVect2 oPos;
            FEReal rDepth;

            oPos.x = oCfg.rGetReal(sIVar + ".X",_0r);
            oPos.y = oCfg.rGetReal(sIVar + ".Y",_0r);
            rDepth = oCfg.rGetReal(sIVar + ".Depth",_0r);
            CFEString sType = oCfg.sGetString(sIVar + ".Type","none");

            CFEParticleSysInstMgr::Spawn(CFEString("data/sfx/")+sType,PSYS_INFINITE_LIFE,oPos,rDepth);
        }
        
        // ------------------------------------
        // Retrieve Entities
        // ------------------------------------
        sVar = "KFRMMap.Entities";
        uint uiEntities = oCfg.iGetInteger(sVar + ".NumEntities",0);
        for (i=0;i<uiEntities;i++)
        {
            CFEString sIVar = sVar + CFEString(".Ent") + CFEString(i);
            CFEVect2 oPos;
            FEReal rDepth;

            oPos.x = oCfg.rGetReal(sIVar + ".X",_0r);
            oPos.y = oCfg.rGetReal(sIVar + ".Y",_0r);
            rDepth = oCfg.rGetReal(sIVar + ".Depth",_0r);
            CFEString sType = oCfg.sGetString(sIVar + ".Type","none");

            CEntity* poEnt = CEntityFactory::poCreate(sType);

            if (poEnt != NULL)
            {
                poEnt->SetDepth(rDepth);
                poEnt->SetPos(oPos);
                m_poMapEntities.push_back(poEnt);
            }
        }
    }    
}
//-----------------------------------------------------------------------------
void CMapMgr::Finish()
{
    // Spawns entities if they are in the view
    delete goGameGlobals.m_poMap;
    goGameGlobals.m_poMap = NULL;
}
//-----------------------------------------------------------------------------
void CMapMgr::Update(FEReal _rDeltaT)
{
    // Spawns entities if they are in the view
    for (uint e=0;e<m_poMapEntities.size();e++)
    {       
        if (CGameHelper::bVisibleOnScreen(m_poMapEntities[e]->oGetPos(),200.0f) && (!m_poMapEntities[e]->bEnabled()))
        {
            m_poMapEntities[e]->Spawn( m_poMapEntities[e]->oGetPos() );
        }
    }
}
//-----------------------------------------------------------------------------
void CMapMgr::Reset()
{
    for (uint e=0;e<m_poMapEntities.size();e++)
        m_poMapEntities[e]->Reset();        
}
//-----------------------------------------------------------------------------

