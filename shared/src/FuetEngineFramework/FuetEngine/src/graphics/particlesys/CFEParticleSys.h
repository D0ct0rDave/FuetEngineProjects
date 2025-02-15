// ----------------------------------------------------------------------------
/*! \class CFEParticleSys
 *  \brief Particle system clases and definitions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
template <typename T>
class CFEPSFunc
{
	public:

		CFEPSRandParam  m_oMult;
		CFEPSRandParam  m_oOfs;
		CFEKFBFunc<T>	m_oFunc;

		T oGetValue(const FEReal& _rT)
		{
			return( m_oFunc.oGetValue(_rT) );
		}
};
//-----------------------------------------------------------------------------
class CFEParticleSys
{
	public:

		/// The sprite used to render the particles.
		CFESprite*			m_poSprite;

		/// Time to live function parameters.
		FEReal				m_rFreq;

		/// Time to live function parameters.
		CFEPSRandParam	    m_rTTL;

		/// Angle function parameters.
		CFEPSFunc<FEReal>	m_rAngle;

		/// Color function parameters.
		CFEPSFunc<CFEColor> m_oColor;

		/// Scale function parameters.
		CFEPSFunc<FEReal>	m_rScale;

		/// Position function parameters.
		CFEPSFunc<FEReal>	m_rXPos;

		/// Position function parameters.
		CFEPSFunc<FEReal>	m_rYPos;
};
//-----------------------------------------------------------------------------
class CFEParticleSysInst
{
    public:

        /// Current life time of this particle system.
        FEReal m_rLifeTime;

        /// by another entity, probably the CGameEntity or the SpriteTile or something like this.
        CFEParticleSys* m_poPS;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
