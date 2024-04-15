// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#ifndef CFESmootherH
#define CFESmootherH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
class CFESmoother
{
    public:

        CFESmoother(FEReal _rSmoothFact = 1.0f) : m_rAccum(0.0f)
        {
            Init(_rSmoothFact);
        }

        /// Inializes the smoother object using the initial smooth factor
        void Init(FEReal _rSmoothFact);

        /// Retrieves the a smooth value given the raw input value
        FEReal rGetValue(FEReal _rInputVal,FEReal _rDeltaT = _1r);

        /// Sets the "stable" value of the smoother
        void SetValue(FEReal _rValue);

        /// Sets the smooth factor.
        void SetSmoothFact(FEReal _rSmoothFact)
        {
			m_rSmoothFact = _rSmoothFact;
        }

    protected:

        FEReal m_rSmoothFact;
        FEReal m_rAccum;
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
