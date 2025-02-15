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
#ifndef CPlayerBonusH
#define CPlayerBonusH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/Entity/Player/CPlayer.h"
//-----------------------------------------------------------------------------
class CPlayerBonus : public CPlayer
{
	public:

		/// This function is implemented for convenience. Just to launch animations.
		/// a different kind of player can override this functuion to launch other
		/// animations different from the standar ones. For instance, a bonus player.
		virtual void PlayAction(ECharacterAction _eAction);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
