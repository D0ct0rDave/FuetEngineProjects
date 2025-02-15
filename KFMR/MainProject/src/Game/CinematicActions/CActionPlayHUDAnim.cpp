// -----------------------------------------------------------------------------
/*! \class CActionPlayHUDAnim
 *  \brief The base class for script actions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CActionPlayHUDAnim.h"
#include "Game/Entity/CEntityRegistry.h"
#include "Game/Helpers/CHUDMgr.h"
// -----------------------------------------------------------------------------
void CActionPlayHUDAnim::Init(const CFEString& _sActionName)
{
    m_eHUDAction = HA_NONE;
    for (uint i=0;i<HA_NUM_HUD_ACTIONS;i++)
    {
        if (HUD_ACTIONS[i].m_sActionName |= _sActionName)   
        {
            m_eHUDAction = (EHUDAction)i;
            return;
        }
    }
}
// -----------------------------------------------------------------------------
bool CActionPlayHUDAnim::bUpdate(FEReal _rDeltaT)
{
    CHUDMgr::Play(m_eHUDAction);
    return(false);
}
// -----------------------------------------------------------------------------
