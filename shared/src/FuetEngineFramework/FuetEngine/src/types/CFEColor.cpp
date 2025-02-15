// ----------------------------------------------------------------------------
/*! \class CFEColor
 *  \brief Color class
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEColor.h"
// ----------------------------------------------------------------------------
static CFEColor gsoTRANSPBLACK(_0r,_0r,_0r,_0r);
static CFEColor gsoTRANSPWHITE(_1r,_1r,_1r,_0r);
static CFEColor gsoBLACK(_0r,_0r,_0r,_1r);
static CFEColor gsoWHITE(_1r,_1r,_1r,_1r);
// ----------------------------------------------------------------------------
/// Returns the BLACK color.
const CFEColor& CFEColor::BLACK()
{
	return(gsoBLACK);
}
// ----------------------------------------------------------------------------
/// Returns the Transparent BLACK color.
const CFEColor& CFEColor::TRANSPBLACK()
{
	return(gsoTRANSPBLACK);
}
// ----------------------------------------------------------------------------
/// Returns the WHITE color.
const CFEColor& CFEColor::WHITE()
{
	return(gsoWHITE);
}
// ----------------------------------------------------------------------------
/// Returns the Transparent WHITE color.
const CFEColor& CFEColor::TRANSPWHITE()
{
	return(gsoTRANSPWHITE);
}
// ----------------------------------------------------------------------------
