//-----------------------------------------------------------------------------
/*! \class CGameHelperH
 *  \brief A class which provides helping functionality to the game code.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "Game/CGameGlobals.h"
#include "CGameHelper.h"
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
bool CGameHelper::bVisibleOnScreen(const CFEVect2& _oPos,FEReal _rObjectWidth)
{
    if (
        (_oPos.x >= (goGameGlobals.m_poCamera->oGetPos().x - _rObjectWidth*0.5f))
        && (_oPos.x <= (goGameGlobals.m_poCamera->oGetPos().x + 2*goGameGlobals.m_rScreenMargin + _rObjectWidth*0.5f))
        )
        return(true);
    else
        return(false);
}
//-----------------------------------------------------------------------------