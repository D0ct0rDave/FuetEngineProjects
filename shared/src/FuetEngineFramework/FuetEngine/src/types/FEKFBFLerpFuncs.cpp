// ----------------------------------------------------------------------------
/*! \class FEKFBFLerpFuncs.h
 *  \brief FE KFBF Lerp Functions.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "FEKFBFLerpFuncs.h"
#include "support/math/CFEMath.h"
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
// ----------------------------------------------------------------------------
FEReal FEKFBFLerpFuncs_rRand(const FEReal& _rFact);
// ----------------------------------------------------------------------------
const uint RANDTABLE_SIZE = 64;
static FEBool gbInitialized = false;
static FEReal grRandomTable[RANDTABLE_SIZE];
//-----------------------------------------------------------------------------
template<>
bool random_lerp(const bool& a, const bool& b,const FEReal& _rFact)
{
	return ( FEKFBFLerpFuncs_rRand(_rFact) > _05r );
}
// ----------------------------------------------------------------------------
template<>
CFEVect2 random_lerp(const CFEVect2& _oA,const CFEVect2& _oB,const FEReal& _rFact)
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
template<>
CFEColor random_lerp(const CFEColor& _oA,const CFEColor& _oB,const FEReal& _rFact)
{
    FEReal rFact = FEKFBFLerpFuncs_rRand(_rFact);
    return ( _oA*(_1r - rFact) + _oB*rFact );
}
//-----------------------------------------------------------------------------
template<>
int random_lerp(const int &_iA,const int&_iB,const FEReal& _rFact)
{
	FEReal rFact = FEKFBFLerpFuncs_rRand(_rFact);
	return ( (int)((FEReal)_iA*(_1r - rFact) + (FEReal)_iB*rFact) );
}
//-----------------------------------------------------------------------------
// Error	28	error C2084: function 'FEBool random_lerp(const FEBool &,const FEBool &,const FEReal &)' already has a body	d:\coderchild\shared\src\fuetengineframework\fuetengine\src\types\fekfbflerpfuncs.h	52	ScriptGenExcel
/*
FEBool random_lerp(const FEBool&,const FEBool&,const FEReal& _rFact)
{
    return ( FEKFBFLerpFuncs_rRand(_rFact) > _05r );
}
*/
// ----------------------------------------------------------------------------
template<>
FEReal random_lerp(const FEReal& _oA,const FEReal& _oB,const FEReal& _rFact)
{
    FEReal rFact = FEKFBFLerpFuncs_rRand(_rFact);
    return ( _oA*(_1r - rFact) + _oB*rFact );
}
// ----------------------------------------------------------------------------
FEReal FEKFBFLerpFuncs_rRand(const FEReal& _rFact)
{
    if (gbInitialized == false)
    {
        // initialize random table
        for (uint i=0;i<RANDTABLE_SIZE;i++)
            grRandomTable[i] = CFEMath::rRand();

        gbInitialized = true;
    }

    uint uiIdx;
    uiIdx = (uint)(_rFact*(FEReal)RANDTABLE_SIZE);
    return ( grRandomTable[ uiIdx ] );
}
// ----------------------------------------------------------------------------
template<> bool default_value(const bool& v) { return(false); };
template<> FEReal default_value(const FEReal& v) { return(_0r); };
template<> int default_value(const int& v) { return(0); };
template<> CFEColor default_value(const CFEColor& v) { return(CFEColor::BLACK()); };
template<> CFEVect2 default_value(const CFEVect2& v) { return(CFEVect2::ZERO()); };
//-----------------------------------------------------------------------------
