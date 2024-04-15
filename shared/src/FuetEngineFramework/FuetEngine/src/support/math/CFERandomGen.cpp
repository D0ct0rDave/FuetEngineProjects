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
#include "CFERandomGen.h"
#include "CFEMath.h"
// ----------------------------------------------------------------------------
const uint LARGE_PRIME_A 	= 0x100002B;
const uint LARGE_PRIME_B 	= 0x1000049;
const uint INITIAL_SEED	 	= 0x100004B;
const uint MAX_RANGE_MOD 	= 0x7fffffff;
const uint MAX_RANGE	 	= MAX_RANGE_MOD;

const uint RND_Mult			= LARGE_PRIME_A;		// Multipliplier
const uint RND_Add			= LARGE_PRIME_B;		// Aditive constant
// ----------------------------------------------------------------------------
CFERandomGen::CFERandomGen()
{
	SetSeed(INITIAL_SEED);
}
// ----------------------------------------------------------------------------
void CFERandomGen::SetSeed(uint _uiSeed)
{
	m_uiSeed	= _uiSeed;
	m_uiSt		= m_uiSeed;
}
// ----------------------------------------------------------------------------
FEReal CFERandomGen::rRand()
{
	m_uiSt = (RND_Mult*m_uiSt + RND_Add) & MAX_RANGE_MOD;
	return( FEReal(float((double)m_uiSt/(double)MAX_RANGE)) );
}
// ----------------------------------------------------------------------------
FEReal CFERandomGen::rRand(FEReal _rMin,FEReal _rMax)
{
    return (_rMin + rRand()*(_rMax-_rMin));
}
// ----------------------------------------------------------------------------
int CFERandomGen::iRand(int _iMin,int _iMax)
{
	/// Retrieves a random value between _iMin and _iMax.
	int iVal = _iMin + int(rRand()*FEReal(_iMax-_iMin+1));
    if (iVal > _iMax) iVal = _iMax;
    return ( iVal );
}
// ----------------------------------------------------------------------------
uint CFERandomGen::uiRand(uint _uiMax)
{
    return ( iRand(0,_uiMax-1) );
}
// ----------------------------------------------------------------------------
