//-----------------------------------------------------------------------------
/*! \class CEntity
 *  \brief A class to derive from for entities with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameGlobalsH
#define CGameGlobalsH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/Enemy/CEnemy.h"
#include "Game/Entity/Player/CPlayer.h"
#include "Game/Entity/Camera/CCamera.h"
#include "Profile/CProfile.h"
//-----------------------------------------------------------------------------
const uint MAX_SKILLS = 4;
//-----------------------------------------------------------------------------
const FEReal CAMERA_ZOOM = _1r; // 0.4f;
const FEReal CAMERA_YOFS = 10.0f;
//-----------------------------------------------------------------------------
const FEReal ACTOR_SCALE	= 1.0f / CAMERA_ZOOM;
const FEReal ACTOR_SPEED	= 200.0f;
//-----------------------------------------------------------------------------
const FEReal SLOW_MOTION_ENABLE_SPEED	= 2.5f;
const FEReal SLOW_MOTION_DISABLE_SPEED	= 5.0f;
const FEReal SLOW_MOTION_MAX_FACTOR_1	= 0.2f;
//-----------------------------------------------------------------------------
class CGameGlobals
{
	public:

		CFEInput*		m_poInput;
		CFERenderer*	m_poRenderer[2];

        CPlayer*		m_poPlayer;
        CEnemy*		    m_poBoss;
        CCamera*		m_poCamera;
        CFEMap*			m_poMap;

		FEReal			m_rScreenMargin;
		
        CFELogger       m_oLogger;
        CFEChrono       m_oChrono;

        CProfile*       m_poCurProfile;

        CFEVect2        m_oStartPos;
        CFEVect2        m_oEndPos;

		// Slow Motion
		bool			m_bSlowMotionActivated;
		FEReal			m_fSlowMotionTimer;
		FEReal			m_fSlowMotionFactor;

		void			SlowMotion(bool _bActivate = true, FEReal _fEndTimer = 5.0f) 
		{ 
			m_bSlowMotionActivated =_bActivate; 
			m_fSlowMotionTimer = _fEndTimer;
		}

		void			SlowMotionReset() 
		{ 
			m_bSlowMotionActivated = 0; 
			m_fSlowMotionFactor = 1; 
			m_fSlowMotionTimer = 0;
		}

		bool			bSlowMotionNeedsUpdate() 
		{ 
			return (m_bSlowMotionActivated || (m_fSlowMotionFactor < 1)); 
		}

		void			SlowMotionUpdate()
		{
			if(m_bSlowMotionActivated)
			{
				if(m_fSlowMotionFactor > SLOW_MOTION_MAX_FACTOR_1)
				{
					m_fSlowMotionFactor -= SLOW_MOTION_ENABLE_SPEED * m_poRenderer[0]->rGetDeltaT();
					if(m_fSlowMotionFactor < SLOW_MOTION_MAX_FACTOR_1)
					{
						 m_fSlowMotionFactor = SLOW_MOTION_MAX_FACTOR_1;
					}
				}
				m_fSlowMotionTimer -= m_poRenderer[0]->rGetDeltaT();
				if(m_fSlowMotionTimer < 0) 
				{
					m_bSlowMotionActivated = false;
				}
			}
			else
			{
				if(m_fSlowMotionFactor < 1)
				{
					m_fSlowMotionFactor += SLOW_MOTION_DISABLE_SPEED * m_poRenderer[0]->rGetDeltaT();
					if(m_fSlowMotionFactor > 1)
					{
						m_fSlowMotionFactor = 1;
					}
				}
			}
		}

};
//-----------------------------------------------------------------------------
extern CGameGlobals goGameGlobals;
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
