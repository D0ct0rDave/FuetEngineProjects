//-----------------------------------------------------------------------------
#include "CSFXHelper.h"
//-----------------------------------------------------------------------------
CSFXHelper::TSFXData            CSFXHelper::m_oSFXDataTable[SFX_NUM_SFXS];
CFEArray<CSFXHelper::TSFXInfo>  CSFXHelper::m_oActiveFX;      
//-----------------------------------------------------------------------------
void CSFXHelper::Init(EGameSFX _eSFXs[])
{
    uint i = 0;
    while (_eSFXs[i] != SFX_NONE)
    {
        // Try loading sounds one by one.
        CFEParticleSys* poPS = NULL;
        uint j = 0;

        do{
            CFEString sSFX;
            sSFX.Format("data/sfx/%s_%d",GAME_SFX[ _eSFXs[i] ], j+1);

            poPS = CFEParticleSysMgr::poLoad( sSFX );
            if (poPS != NULL)
            {
                m_oSFXDataTable[ _eSFXs[i] ].m_poSFX[j] = poPS;
                j++;
            }
        }while (poPS != NULL);
        
        m_oSFXDataTable[ _eSFXs[i] ].m_uiNumSFX = j;
        i++;
    }
}
//-----------------------------------------------------------------------------
void CSFXHelper::Finish()
{
    memset(m_oSFXDataTable,0,sizeof(m_oSFXDataTable));
    m_oActiveFX.clear();    
    CFEParticleSysMgr::Reset();
}
//-----------------------------------------------------------------------------
void CSFXHelper::Reset()
{
    m_oActiveFX.clear();
}
//-----------------------------------------------------------------------------
void CSFXHelper::SpawnParticle(EGameSFX _eSFX,const CFEVect2& _oPos,FEReal _rDepth)
{
    if (m_oSFXDataTable[_eSFX].m_uiNumSFX > 0)
    {
        uint uiIdx = CFEMath::iRand(0,m_oSFXDataTable[_eSFX].m_uiNumSFX-1);
        CFEParticleSysInstMgr::SpawnParticle(m_oSFXDataTable[_eSFX].m_poSFX[uiIdx],_oPos,_rDepth);
    }
}
//-----------------------------------------------------------------------------
/// Creates and returns a particle system.
FEHandler CSFXHelper::hSpawnPSys(EGameSFX _eSFX)
{
    if (m_oSFXDataTable[_eSFX].m_uiNumSFX > 0)
    {
        uint uiIdx = CFEMath::iRand(0,m_oSFXDataTable[_eSFX].m_uiNumSFX-1);
        return ( CFEParticleSysInstMgr::hGetInstance(m_oSFXDataTable[_eSFX].m_poSFX[uiIdx]) );
    }

    return (NULL);
}
//-----------------------------------------------------------------------------
void CSFXHelper::SpawnPSys(EGameSFX _eSFX,const CFEVect2& _oPos,FEReal _rDepth,FEReal _rTime)
{
    if (m_oSFXDataTable[_eSFX].m_uiNumSFX > 0)
    {
        uint uiIdx = CFEMath::iRand(0,m_oSFXDataTable[_eSFX].m_uiNumSFX-1);
        CFEParticleSysInstMgr::Spawn(m_oSFXDataTable[_eSFX].m_poSFX[uiIdx],_rTime,_oPos,_rDepth);
    }
}
//-----------------------------------------------------------------------------
void CSFXHelper::SpawnFollowerFX(EGameSFX _eSFX,CEntity* _poEnt,const CFEVect2& _oOffset)
{
    if (_poEnt == NULL) return;

    if (m_oSFXDataTable[_eSFX].m_uiNumSFX > 0)
    {
        uint uiIdx = CFEMath::iRand(0,m_oSFXDataTable[_eSFX].m_uiNumSFX-1);
        TSFXInfo oPS;
        oPS.m_poEnt = _poEnt;
        oPS.m_hSFX = CFEParticleSysInstMgr::hGetInstance(m_oSFXDataTable[_eSFX].m_poSFX[uiIdx]);
        oPS.m_oOfs = _oOffset;

        CFEParticleSysInstMgr::Enable( oPS.m_hSFX );
        CFEParticleSysInstMgr::SetDepth( oPS.m_hSFX ,0.5f);
        CFEParticleSysInstMgr::SetPos( oPS.m_hSFX, _poEnt->oGetPos() + oPS.m_oOfs);

        m_oActiveFX.push_back(oPS);
    }
}
//-----------------------------------------------------------------------------
void CSFXHelper::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<m_oActiveFX.size();)
    {
        if (m_oActiveFX[i].m_poEnt->bEnabled())
        {
            CFEParticleSysInstMgr::SetPos( m_oActiveFX[i].m_hSFX, m_oActiveFX[i].m_poEnt->oGetPos() + m_oActiveFX[i].m_oOfs);
            i++;
        }
        else
        {
            CFEParticleSysInstMgr::ReleaseInstance( m_oActiveFX[i].m_hSFX );
            m_oActiveFX.Delete(i);
        }
    }
}
//-----------------------------------------------------------------------------
