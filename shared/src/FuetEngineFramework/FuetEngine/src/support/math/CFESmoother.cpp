// ----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par GammaE License
 */
// ----------------------------------------------------------------------------
#include "CFESmoother.h"
#include "CFEMath.h"
// ----------------------------------------------------------------------------
void CFESmoother::Init(FEReal _rSmoothFact)
{
    m_rAccum = 0.0f;
    m_rSmoothFact = CFEMath::rClamp(0.0f,1.0f,_rSmoothFact);
}
// ----------------------------------------------------------------------------
FEReal CFESmoother::rGetValue(FEReal _rInputVal,FEReal _rDeltaT)
{
    if ( m_rSmoothFact > 100.0f )
    {
        int A = 0;
    }

    if ( m_rAccum > 100.0f )
    {
        int A = 0;
    }
    // FERealfNewVal = fClamp(0.0f,1.0f,_rInputVal);

    FEReal rRet = CFEMath::rLerp(m_rAccum,_rInputVal,m_rSmoothFact);
    m_rAccum = (m_rAccum + rRet) * 0.5f;

    return(rRet);
}
// ----------------------------------------------------------------------------
