//-----------------------------------------------------------------------------
/*! \class CActor
 *  \brief The base class for the actor characters in the game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CActorH
#define CActorH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/CLivingEntity.h"
//-----------------------------------------------------------------------------
class CActor : public CLivingEntity
{
	public:

		CActor();

		/// Retrieves the direction the actor is facing to...
		FEReal rGetDir()
		{
			return(m_rDirMult);
		}

		/// Retrieves the animation instance the actor is facing to...
		FEHandler hGetAnimInst()
		{
			return(m_hAnimInst);
		}

		/// Sets the position of the entity.
		virtual void SetPos(const CFEVect2& _oPos);

	protected:

		// Posible common fields among actors
		FEHandler			m_hAnimInst;
		FEReal				m_rDirMult;
		CFESkelAnimSprite*	m_poFaceSpr;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
