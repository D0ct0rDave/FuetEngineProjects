// ----------------------------------------------------------------------------
/*! \class CFEMathIntersect
 *  \brief Basic Intersection tests.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMathIntersectH
#define CFEMathIntersectH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFERect.h"
// ----------------------------------------------------------------------------
namespace CFEMath
{
	/// Checks whether two given rects overlap or not.
	bool bOverlap(const CFERect& _oA,const CFERect& _oB);

	/// Checks whether the given point falls inside the given rectangle or not.
	bool bInside(const CFERect& _oRect,const CFEVect2& _oPoint);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
