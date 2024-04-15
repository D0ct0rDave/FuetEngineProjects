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
#include "CFESmoother.h"
#include "CFEMath.h"
// ----------------------------------------------------------------------------
void CFESmoother::Init(FEReal _rSmoothFact)
{
    m_rAccum = _0r;
    m_rSmoothFact = CFEMath::rClamp(_0r,_1r,_rSmoothFact);
}
// ----------------------------------------------------------------------------
void CFESmoother::SetValue(FEReal _rValue)
{
	m_rAccum = _rValue;
}
// ----------------------------------------------------------------------------
FEReal CFESmoother::rGetValue(FEReal _rInputVal,FEReal _rDeltaT)
{
	FEReal rFact = CFEMath::rMin(60.0f*_rDeltaT*m_rSmoothFact,_1r);
    FEReal rRet  = CFEMath::rLerp(m_rAccum,_rInputVal,rFact);
    m_rAccum = rRet;

    return(rRet);
}
// ----------------------------------------------------------------------------
