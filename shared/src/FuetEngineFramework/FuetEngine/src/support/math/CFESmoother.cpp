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
    m_rAccum = _0r;
    m_rSmoothFact = CFEMath::rClamp(_0r,_1r,_rSmoothFact);
}
// ----------------------------------------------------------------------------
FEReal CFESmoother::rGetValue(FEReal _rInputVal,FEReal _rDeltaT)
{
    FEReal rRet = CFEMath::rLerp(m_rAccum,_rInputVal,m_rSmoothFact);
    m_rAccum = (m_rAccum + rRet) * _05r;

    return(rRet);
}
// ----------------------------------------------------------------------------
