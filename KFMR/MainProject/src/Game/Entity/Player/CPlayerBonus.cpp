//-----------------------------------------------------------------------------
/*! \class CPlayerBonus
 *  \brief The class that handles all the player procedures at bonus stage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CPlayerBonus.h"
#include "Game/Helpers/CAnimHelper.h"
//-----------------------------------------------------------------------------
void CPlayerBonus::PlayAction(ECharacterAction _eAction)
{
    switch (_eAction)
    {
        case ACTION_ACTOR_CROUCH_PUNCH:
        case ACTION_ACTOR_CROUCH_KICK:
        // PlayAction(_eAction); // grab item crouch
        CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_GRAB_CROUCH);
        break;
        
        case ACTION_ACTOR_KICK:
        case ACTION_ACTOR_PUNCH:
        CAnimHelper::Play(m_hAnimInst,ACTION_ACTOR_GRAB);
        // PlayAction(_eAction); // grab item
        break;

        // ...
        default:
        CPlayer::PlayAction(_eAction);
        break;
    }
}
//-----------------------------------------------------------------------------
