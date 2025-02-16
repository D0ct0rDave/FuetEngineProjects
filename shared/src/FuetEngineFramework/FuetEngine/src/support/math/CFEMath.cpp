// ----------------------------------------------------------------------------
/*! \class CFEMath
 *  \brief Basic Math Functionality.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMath.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
FEReal CFEMath::rSin(FEReal _rAngle)
{
    return ( CFESystem::Math::rSin(_rAngle) );}
// ----------------------------------------------------------------------------
FEReal CFEMath::rCos(FEReal _rAngle)
{
    return ( CFESystem::Math::rCos(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rRand()
{
    return ( CFESystem::Math::rRand() );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rRand(FEReal _rMin,FEReal _rMax)
{
    return ( _rMin + CFESystem::Math::rRand()*(_rMax-_rMin));
}
// ----------------------------------------------------------------------------
int CFEMath::iRand(int _iMin,int _iMax)
{
    // What happens if _iMin > _iMax ???
    int iRange = _iMax - _iMin + 1;
    int iVal   = _iMin + (int)(CFESystem::Math::rRand()*(FEReal)iRange);
    if (iVal > _iMax) iVal = _iMax;

    return ( iVal );
}
// ----------------------------------------------------------------------------
uint CFEMath::uiRand(uint _uiMax)
{
    uint uiVal = (uint)(CFESystem::Math::rRand() * ((FEReal)_uiMax + _1r));
    if (uiVal > _uiMax) uiVal = _uiMax;
    return ( uiVal );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rSqrt(FEReal _rValue)
{
    return ( CFESystem::Math::rSqrt(_rValue) );
}

// ----------------------------------------------------------------------------
FEReal CFEMath::rMin(FEReal _rA,FEReal _rB)
{
	if (_rA < _rB) return(_rA); else return(_rB);
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rMax(FEReal _rA,FEReal _rB)
{
	if (_rA > _rB) return(_rA); else return(_rB);
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rClamp(FEReal _rMin,FEReal _rMax,FEReal _rVal)
{
	if (_rVal < _rMin) return(_rMin); else if (_rVal > _rMax) return(_rMax); else return(_rVal);
}
// ----------------------------------------------------------------------------
bool CFEMath::bBetween(FEReal _rMin,FEReal _rMax,FEReal _rVal)
{
	return ( (_rVal >= _rMin) && (_rVal <= _rMax) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rAbs(FEReal _rValue)
{
	if (_rValue < _0r)
		return(-_rValue);
	else 
		return(_rValue);
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rLerp(FEReal _rA,FEReal _rB,FEReal _rFact)
{
	return( _rA + (_rB - _rA)*_rFact );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rMod(FEReal _rNum,FEReal _rDen)
{
	return ( CFESystem::Math::rMod(_rNum,_rDen) );
}
// ----------------------------------------------------------------------------
