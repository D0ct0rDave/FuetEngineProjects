// ----------------------------------------------------------------------------
/*! \class
 *  \brief Image library functionality through FreeImage.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "../FEConfig.h"
#if (MATH == USE_STDMATH)
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <math.h>

#include "../CFESystem.h"
#include "FEBasicTypes.h"
// ------------------------------------------------------------------------
// http://www.numberempire.com/primenumbers.php
// ------------------------------------------------------------------------
namespace CFESystem
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
FEReal CFESystem::Math::rSin(FEReal _rAngle)
{
    return( sinf(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFESystem::Math::rCos(FEReal _rAngle)
{
    return( cosf(_rAngle) );
}
// ----------------------------------------------------------------------------
FEReal CFESystem::Math::rRand()
{
	RND_CUR_STATE = (RND_Mult*RND_CUR_STATE + RND_Add) & MAX_RANGE_MOD;
	return( (FEReal) ((float)((double)RND_CUR_STATE/(double)MAX_RANGE)) );
}
// ----------------------------------------------------------------------------
FEReal CFESystem::Math::rSqrt(FEReal _rValue)
{
    return( sqrtf(_rValue) );
}
// ----------------------------------------------------------------------------
FEReal CFESystem::Math::rMod(FEReal _rNum,FEReal _rDen)
{
	return( fmodf(_rNum,_rDen) );
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
