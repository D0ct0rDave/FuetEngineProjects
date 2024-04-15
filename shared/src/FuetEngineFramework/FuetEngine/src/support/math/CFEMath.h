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
#ifndef CFEMathH
#define CFEMathH
// ----------------------------------------------------------------------------
#if !defined(__GNUC__) && !defined(__arm__)
#pragma warning( disable : 4244 )	// double to FEReal conversion
#pragma warning( disable : 4305 )	// double to FEReal conversion
#endif
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
// ----------------------------------------------------------------------------
namespace CFEMath
{
	/// Constants

    /// Returns the absolute value of the given number.
    FEReal rAbs(FEReal _rValue);

	/// Returns the absolute of the given value.
    int iAbs(int _iValue);

	/// Returns the minimum between the given values.
    FEReal rMin(FEReal _rA,FEReal _rB);

	/// Returns the minimum between the given values.
    int iMin(int _rA,int _rB);

	/// Returns the maximum between the given values.
    FEReal rMax(FEReal _rA,FEReal _rB);

	/// Returns the maximum between the given values.
    int iMax(int _rA,int _rB);

	/// Clamps the given value between the given range.
    FEReal rClamp(FEReal _rMin,FEReal _rMax,FEReal _rVal);

	/// Clamps the given value between the given range.
    int iClamp(int _iMin,int _iMax,int _iVal);

    /// Returns whether a given number is between a range or not.
    FEBool bBetween(FEReal _rMin,FEReal _rMax,FEReal _rVal);

	/// Returns whether a given number is between a range or not.
	FEBool bBetween(int _iMin,int _iMax,int _iVal);

    /// Returns the sign of the given value.
    FEReal rSign(FEReal _rValue);

    /// Returns the sign of the given value.
    int iSign(int _iValue);

	/// Returns the sinusoidal linear interpolation of the given values.
    FEReal rSlerp(FEReal _rA,FEReal _rB,FEReal _rFact);

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

    /// Returns a random unsigned integer value between (0.._uiMax-1) _uiMax not included.
    uint uiRand(uint _uiMax);

    /// Square root of the given value.
    FEReal rSqrt(FEReal _rValue);

	/// Retrieves the modulus of the division between the numerator and the given denominator.
    FEReal rMod(FEReal _rNum,FEReal _rDen);
    
    /// Retrieves whether the given number is a proper determinated real number.
	FEBool bDeterminate(FEReal _rValue);
	
	/// Retrieves whether the given number is finite or infinite (both positive or negative)
	FEBool bIsFinite(FEReal _rValue);

	/// Retrieves whether the given number is a proper real number or is an invalid representation.
	FEBool bIsNumber(FEReal _rValue); 
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
