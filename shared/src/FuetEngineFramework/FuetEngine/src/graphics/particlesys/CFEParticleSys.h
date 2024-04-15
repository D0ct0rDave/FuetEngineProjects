// ----------------------------------------------------------------------------
/*! \class CFEParticleSys
 *  \brief Particle system clases and definitions.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEParticleSysH
#define CFEParticleSysH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"

#include "support/Math/CFEMath.h"
#include "types/CFEKFBFunc.h"
//-----------------------------------------------------------------------------
class CFESprite;
//-----------------------------------------------------------------------------
class CFEPSRandParam
{
	public:

		FEReal			m_rRndMin;
		FEReal			m_rRndMax;

		FEReal rGetValue()
		{
			return( CFEMath::rRand(m_rRndMin,m_rRndMax) );
		}
};
//-----------------------------------------------------------------------------
enum PSYS_FLAGS
{
	PSF_TANGENTIAL_EMISION = 1,
};
//-----------------------------------------------------------------------------
class CFEParticleSys
{
	public:

		/// Default constructor of the class
		CFEParticleSys() :
			m_poSprite(NULL),
			m_rFreq(_0r),
			m_rTTL(_0r),
			m_rEmisorAccel(_0r),
			m_rEmisorXAccel(_0r),
			m_rEmisorYAccel(_0r),
			m_rPartSizeAccel(_0r),
			m_rPartAngleAccel(_0r),
			m_uiFlags(0)
		{
		}

		/// The sprite used to render the particles.
		CFESprite*			m_poSprite;

		/// Time to live function parameters.
		FEReal				m_rFreq;

		/// Time to live function parameters.
		FEReal				m_rTTL;

		/// Emisor radius
		CFEPSRandParam		m_oEmisorRad;

		/// Emisor phase
		CFEPSRandParam		m_oEmisorPhase;

		/// Emisor speed
		CFEPSRandParam		m_oEmisorSpeed;

		/// Emisor accel
		FEReal				m_rEmisorAccel;
		
		/// Emisor X Offset
		CFEPSRandParam		m_oEmisorXOfs;

		/// Emisor Y Offset
		CFEPSRandParam		m_oEmisorYOfs;

		/// Emisor X Offset
		CFEPSRandParam		m_oEmisorXSpeed;

		/// Emisor Y Offset
		CFEPSRandParam		m_oEmisorYSpeed;

		/// Emisor X accel
		FEReal				m_rEmisorXAccel;

		/// Emisor Y accel
		FEReal				m_rEmisorYAccel;

		/// Particle Size
		CFEPSRandParam		m_oPartSize;

		/// Particle Size Speed
		CFEPSRandParam		m_oPartSizeSpeed;

		/// Particle Size Accel
		FEReal				m_rPartSizeAccel;

		/// Particle Angle
		CFEPSRandParam		m_oPartAngle;

		/// Particle Angle Speed
		CFEPSRandParam		m_oPartAngleSpeed;

		/// Particle Angle Accel
		FEReal				m_rPartAngleAccel;

		/// Color function parameters.
		CFEKFBFunc<CFEColor> m_oColor;
		
		/// Particle System Flags
		uint				m_uiFlags;
};
//-----------------------------------------------------------------------------
class CFEParticleSysInst
{
    public:

		/// Default constructor of the class
		CFEParticleSysInst() :
			m_rLifeTime(_0r),
			m_poPS(NULL)
		{
		}

        /// Current life time of this particle system.
        FEReal			m_rLifeTime;

        /// by another entity, probably the CGameEntity or the SpriteTile or something like this.
        CFEParticleSys* m_poPS;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
