// ----------------------------------------------------------------------------
/*! \class CFEVect2
 *  \brief 2D Vector class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEVect2.h"
// ----------------------------------------------------------------------------
static CFEVect2 gsoZERO(_0r,_0r);
static CFEVect2 gsoONE(_1r,_1r);
static CFEVect2 gsoX(_1r,_0r);
static CFEVect2 gsoY(_0r,_1r);
// ----------------------------------------------------------------------------
/// Returns the ZERO vector.
const CFEVect2& CFEVect2::ZERO()
{
    return(gsoZERO);
}
// ----------------------------------------------------------------------------
/// Returns the ONE vector.
const CFEVect2& CFEVect2::ONE()
{
    return(gsoONE);
}
// ----------------------------------------------------------------------------
/// Returns the X vector.
const CFEVect2& CFEVect2::X()
{
    return(gsoX);
}
// ----------------------------------------------------------------------------
/// Returns the Y vector.
const CFEVect2& CFEVect2::Y()
{
    return(gsoY);
}
// ----------------------------------------------------------------------------
