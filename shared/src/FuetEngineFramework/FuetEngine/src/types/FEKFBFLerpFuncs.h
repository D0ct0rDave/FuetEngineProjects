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
#ifndef FEKFBFLerpFuncsH
#define FEKFBFLerpFuncsH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFEVect2.h"
#include "CFEColor.h"
//-----------------------------------------------------------------------------
FEReal FEKFBFLerpFuncs_rRand(const FEReal& _rFact); 
#include "CFEKFBFunc.h"
//-----------------------------------------------------------------------------
template<> bool random_lerp<bool>(const bool &_iA,const bool &_iB,const FEReal& _rFact);	// <-- WARNING: no FEBool !!!
template<> FEReal random_lerp<FEReal>(const FEReal& _oA,const FEReal& _oB,const FEReal& _rFact);
template<> int random_lerp<int>(const int &_iA,const int&_iB,const FEReal& _rFact);
template<> CFEVect2 random_lerp<CFEVect2>(const CFEVect2& _oA,const CFEVect2& _oB,const FEReal& _rFact);
template<> CFEColor random_lerp<CFEColor>(const CFEColor& _oA,const CFEColor& _oB,const FEReal& _rFact);
// ----------------------------------------------------------------------------
template<> bool default_value<bool>(const bool& v);
template<> FEReal default_value<FEReal>(const FEReal& v);
template<> int default_value<int>(const int& v);
template<> CFEColor default_value<CFEColor>(const CFEColor& v);
template<> CFEVect2 default_value<CFEVect2>(const CFEVect2& v);
//-----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
