//-----------------------------------------------------------------------------
/*! \class CEntity
 *  \brief A class to derive from for entities with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameGlobalsH
#define CGameGlobalsH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CPlayer.h"
#include "CCamera.h"
//-----------------------------------------------------------------------------
const uint MAX_SKILLS = 4;
//-----------------------------------------------------------------------------
const FEReal CAMERA_ZOOM = 0.4f;
const FEReal CAMERA_YOFS = 10.0f;
//-----------------------------------------------------------------------------
const FEReal GROUND_HEIGHT	= 370.0f;
const FEReal ACTOR_SCALE	= 1.0f / CAMERA_ZOOM;
const FEReal ACTOR_SPEED	= 200.0f;
//-----------------------------------------------------------------------------
class CGameGlobals
{
	public:
		CFEInput*		m_poInput;

        CPlayer*		m_poPlayer;
        CCamera*		m_poCamera;
        CFEMap*			m_poMap;

		FEReal			m_rScreenMargin;
};
//-----------------------------------------------------------------------------
extern CGameGlobals goGameGlobals;
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
