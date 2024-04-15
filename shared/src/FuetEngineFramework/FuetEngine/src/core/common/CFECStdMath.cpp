// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (MATH == USE_STDMATH)
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include "../CFECore.h"
#include "FEBasicTypes.h"
// ------------------------------------------------------------------------
// http://www.numberempire.com/primenumbers.php
// ------------------------------------------------------------------------
namespace CFECore
{
	namespace Math
	{
		const uint LARGE_PRIME_A 	= 0x100002B;
		const uint LARGE_PRIME_B 	= 0x1000049;
		const uint INITIAL_SEED	 	= 0x100004B;
		const uint MAX_RANGE_MOD 	= 0x7fffffff;
		const uint MAX_RANGE	 	= MAX_RANGE_MOD;

		const uint RND_Mult			= LARGE_PRIME_A;		// Multipliplier
		const uint RND_Add			= LARGE_PRIME_B;		// Aditive constant
		static uint RND_Z0			= INITIAL_SEED;			// Z0 Seed 
		static uint RND_CUR_STATE	= RND_Z0;				// X(n-1)
	}
};
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// Basic Math Functionality
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
FEReal CFECore::Math::rSin(FEReal _rAngle)
{
    return( sinf(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFECore::Math::rCos(FEReal _rAngle)
{
    return( cosf(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFECore::Math::rRand()
{
	RND_CUR_STATE = (RND_Mult*RND_CUR_STATE + RND_Add) & MAX_RANGE_MOD;
	return( (FEReal) ((float)((double)RND_CUR_STATE/(double)MAX_RANGE)) );
}
// ----------------------------------------------------------------------------
int CFECore::Math::iRand(int _iMin,int _iMax)
{
	/*
	int iModulus = (_iMax-_iMin+1);
	if (iModulus == 0) return(0);
	RND_CUR_STATE = (RND_Mult*RND_CUR_STATE + RND_Add) & MAX_RANGE_MOD;
	return(_iMin + (RND_CUR_STATE % iModulus) );
	*/

	int iRes = (FEReal)_iMin + rRand()*(FEReal)(_iMax-_iMin+1);

	if (iRes <= _iMax) 
		return(iRes );
	else 
		return(_iMax);
}
// ----------------------------------------------------------------------------
FEReal CFECore::Math::rSqrt(FEReal _rValue)
{
    return( sqrtf(_rValue) );
}
// ----------------------------------------------------------------------------
FEReal CFECore::Math::rMod(FEReal _rNum,FEReal _rDen)
{
	return( fmodf(_rNum,_rDen) );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
