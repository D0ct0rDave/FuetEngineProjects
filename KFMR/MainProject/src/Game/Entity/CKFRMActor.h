//-----------------------------------------------------------------------------
/*! \class CKFRMActor
 *  \brief The base class for all the KFRM actors.
 *  \brief You can put here anything specific to the KFRM game characters.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CKFRMActorH
#define CKFRMActorH
//-----------------------------------------------------------------------------
#include "CActor.h"
#include "Game/Entity/EntityAttacks.h"

const uint ACTOR_CINEMATIC_STATE = 0xa5a5a5a5;
//-----------------------------------------------------------------------------
class CKFRMActor : public CActor
{
	public:

		CKFRMActor();

    protected:

        // Stores the inflicted attacks.
        TEntityAttack       m_oAttack;

		// Stores the received attacks.
		TEntityAttack*	    m_poReceivedAttack;		

		// Update while a cinematic is playing
		// bool				m_bUpdateInCinem;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
