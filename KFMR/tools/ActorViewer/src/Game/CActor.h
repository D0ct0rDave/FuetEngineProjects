//-----------------------------------------------------------------------------
/*! \class CActor
 *  \brief The base class for the actor characters in the game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CActorH
#define CActorH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CLivingEntity.h"
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

		/// Sets the direction the actor will face to...
		void SetDir(FEReal _rDirMult)
		{
			m_rDirMult = _rDirMult;
		}

		/// Retrieves the animation instance the actor is facing to...
		FEHandler hGetAnimInst()
		{
			return(m_hAnimInst);
		}
        
        /// Retrieves the actor face sprite node.
		CFESkelAnimSprite* poGetActorFace()
		{
		    return(m_poFace);
		}

        /// Set the actor face sprite node.
		void SetActorFace(CFESkelAnimSprite* _poActorFace)
		{
		    m_poFace = _poActorFace;
		}

		/// Sets the position of the entity.
		virtual void SetPos(const CFEVect2& _oPos);

        /// Set actor scale.
        void SetScale(FEReal _rScale)
        {
            m_rScale = _rScale;
        }

        /// Retrieve actor scale.
        FEReal rGetScale()
        {
            return(m_rScale);
        }

	protected:

        /// Scale
		FEReal	            m_rScale;

		// Posible common fields among actors
		FEHandler			m_hAnimInst;
		FEReal				m_rDirMult;
		CFESkelAnimSprite*	m_poFace;	
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
