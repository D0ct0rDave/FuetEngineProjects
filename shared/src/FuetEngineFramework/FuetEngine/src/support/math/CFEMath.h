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
#ifndef CFEMathH
#define CFEMathH
// ----------------------------------------------------------------------------
#pragma warning( disable : 4244 )	// double to FEReal conversion
#pragma warning( disable : 4305 )	// double to FEReal conversion
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
namespace CFEMath
{
	/// Constants

    /// Returns the absolute value of the given number.
    FEReal rAbs(FEReal _rValue);
    
	/// Returns the minimum between the given values.
    FEReal rMin(FEReal _rA,FEReal _rB);

	/// Returns the maximum between the given values.
    FEReal rMax(FEReal _rA,FEReal _rB);

	/// Clamps the given value between the given range.
    FEReal rClamp(FEReal _rMin,FEReal _rMax,FEReal _rVal);

    /// Returns whether a given number is between a range or not.
    bool bBetween(FEReal _rMin,FEReal _rMax,FEReal _rVal);

	/// Returns the absolute of the given value.
    FEReal rAbs(FEReal _rValue);

	/// Returns the linear interpolation of the given values.
    FEReal rLerp(FEReal _rA,FEReal _rB,FEReal _rFact);

    /// Returns the sine of the given angle.
    FEReal rSin(FEReal _rAngle);

    /// Returns the cosine of the given angle.
    FEReal rCos(FEReal _rAngle);

    /// Returns a random real value between (0..1).
    FEReal rRand();

    /// Returns a random real value between (_rMin.._rMax).
    FEReal rRand(FEReal _rMin,FEReal _rMax);

    /// Returns a random integer value between (_iMin .. _iMax) both included.
    int iRand(int _iMin,int _iMax);

    /// Returns a random unsigned integer value between (0.._uiMax) _uiMax included.
    uint uiRand(uint _uiMax);

    /// Square root of the given value.
    FEReal rSqrt(FEReal _rValue);   

	/// Retrieves the modulus of the division between the numerator and the given denominator.
    FEReal rMod(FEReal _rNum,FEReal _rDen);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
