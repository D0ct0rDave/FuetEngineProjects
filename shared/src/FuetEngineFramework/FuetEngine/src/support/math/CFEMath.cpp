// ----------------------------------------------------------------------------
/*! \class CFEMath
 *  \brief Basic Math Functionality.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMath.h"
#include "core/CFECore.h"
#include <float.h>
// ----------------------------------------------------------------------------
FEReal CFEMath::rSin(FEReal _rAngle)
{
    return ( CFECore::Math::rSin(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rCos(FEReal _rAngle)
{
    return ( CFECore::Math::rCos(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rRand()
{
    return ( CFECore::Math::rRand() );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rRand(FEReal _rMin,FEReal _rMax)
{
    return ( _rMin + CFECore::Math::rRand()*(_rMax-_rMin));
}
// ----------------------------------------------------------------------------
int CFEMath::iRand(int _iMin,int _iMax)
{
	/// Retrieves a random value between _iMin and _iMax.
	int iVal = CFECore::Math::iRand(_iMin,_iMax);
    if (iVal > _iMax) iVal = _iMax;
    return ( iVal );
}
// ----------------------------------------------------------------------------
uint CFEMath::uiRand(uint _uiMax)
{
    return ( CFECore::Math::iRand(0,(int)_uiMax-1) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rSqrt(FEReal _rValue)
{
    return ( CFECore::Math::rSqrt(_rValue) );
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
FEBool CFEMath::bBetween(FEReal _rMin,FEReal _rMax,FEReal _rVal)
{
	return ( (_rVal >= _rMin) && (_rVal <= _rMax) );
}
// ----------------------------------------------------------------------------
int CFEMath::iMin(int _iA,int _iB)
{
	if (_iA < _iB) return(_iA); else return(_iB);
}
// ----------------------------------------------------------------------------
int CFEMath::iMax(int _iA,int _iB)
{
	if (_iA > _iB) return(_iA); else return(_iB);
}
// ----------------------------------------------------------------------------
int CFEMath::iClamp(int _iMin,int _iMax,int _iVal)
{
	if (_iVal < _iMin) return(_iMin); else if (_iVal > _iMax) return(_iMax); else return(_iVal);
}
// ----------------------------------------------------------------------------
FEBool CFEMath::bBetween(int _iMin,int _iMax,int _iVal)
{
	return ( (_iVal >= _iMin) && (_iVal <= _iMax) );
}
// ----------------------------------------------------------------------------
int CFEMath::iAbs(int _iValue)
{
	/*
	// This is only valid for FLOATS!!!!!!!!!!!!
	uint* _puiValue = (uint*)&_rValue;
	*_puiValue &= 0x7fffffff;

	return(_rValue);
	*/

	if (_iValue < 0)	
		return(-_iValue);
	else 
		return(_iValue);
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rAbs(FEReal _rValue)
{
	/*
	// This is only valid for FLOATS!!!!!!!!!!!!
	uint* _puiValue = (uint*)&_rValue;
	*_puiValue &= 0x7fffffff;

	return(_rValue);
	*/

	if (_rValue < _0r)	
		return(-_rValue);
	else 
		return(_rValue);

}
// ----------------------------------------------------------------------------
FEReal CFEMath::rSign(FEReal _rValue)
{
	/// Zero can be considered both positive or negative: http://mathforum.org/library/drmath/view/57215.html
	if (_rValue<_0r)
		return(-_1r);
	else
		return(_1r);
}
// ----------------------------------------------------------------------------
int CFEMath::iSign(int _iValue)
{
	/// Zero can be considered both positive or negative: http://mathforum.org/library/drmath/view/57215.html
	if (_iValue<0)
		return(-1);
	else
		return(1);
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rSlerp(FEReal _rA,FEReal _rB,FEReal _rFact)
{
	return( _rA + (_rB - _rA) * CFEMath::rSin(_rFact*_PI2r) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rLerp(FEReal _rA,FEReal _rB,FEReal _rFact)
{
	return( _rA + (_rB - _rA)*_rFact );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rMod(FEReal _rNum,FEReal _rDen)
{
	return ( CFECore::Math::rMod(_rNum,_rDen) );
}
// ----------------------------------------------------------------------------
FEBool CFEMath::bDeterminate(FEReal _rValue)
{
	return ( *(unsigned int*)(&_rValue) != 0xffc00000 );
}
// ----------------------------------------------------------------------------
FEBool CFEMath::bIsFinite(FEReal _rValue) 
{
	return (_rValue <= DBL_MAX && _rValue >= -DBL_MAX); 
} 
// ----------------------------------------------------------------------------
FEBool CFEMath::bIsNumber(FEReal _rValue) 
{
    // This looks like it should always be true, 
    // but it's false if x is a NaN.
	return (_rValue == _rValue); 
}
// ----------------------------------------------------------------------------
