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
#include "FEKFBFLerpFuncs.h"
#include "support/math/CFEMath.h"
// ----------------------------------------------------------------------------
const uint RANDTABLE_SIZE = 64;
static bool gbInitialized = false;
static FEReal grRandomTable[RANDTABLE_SIZE];
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
//-----------------------------------------------------------------------------
