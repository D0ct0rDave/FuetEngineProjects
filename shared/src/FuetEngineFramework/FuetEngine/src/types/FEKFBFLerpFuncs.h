// ----------------------------------------------------------------------------
/*! \class FEKFBFLerpFuncs.h
 *  \brief FE KFBF Lerp Functions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEKFBFLerpFuncsH
#define FEKFBFLerpFuncsH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
// ----------------------------------------------------------------------------
extern FEReal FEKFBFLerpFuncs_rRand(const FEReal& _rFact);
// ----------------------------------------------------------------------------
inline FEReal random_lerp(const FEReal& _oA,const FEReal& _oB,const FEReal& _rFact)
{
    FEReal rDelta = _oB - _oA;
    return ( _oA + rDelta*(_2r * FEKFBFLerpFuncs_rRand(_rFact) - _1r) );
}
//-----------------------------------------------------------------------------
inline CFEVect2 random_lerp(const CFEVect2& _oA,const CFEVect2& _oB,const FEReal& _rFact)
{
    CFEVect2 oDelta = _oB - _oA;

    CFEVect2 oV;
    oV.x = _oA.x + oDelta.x*(_2r * FEKFBFLerpFuncs_rRand(_rFact) - _1r);
	
	FEReal rFixedFact;
    if (_rFact > _05r) rFixedFact = _rFact - _05r; else rFixedFact = _rFact + _05r;
    oV.y = _oA.y + oDelta.y*(_2r * FEKFBFLerpFuncs_rRand(rFixedFact) - _1r);

    return(oV);
}
//-----------------------------------------------------------------------------
inline CFEColor random_lerp(const CFEColor& _oA,const CFEColor& _oB,const FEReal& _rFact)
{
    FEReal rFact = FEKFBFLerpFuncs_rRand(_rFact);
    return ( _oA*(_1r - rFact) + _oB*rFact );
}
//-----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
