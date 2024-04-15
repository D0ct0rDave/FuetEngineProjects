// ----------------------------------------------------------------------------
/*! \class CFEMathIntersect
 *  \brief Basic Intersection tests.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMathIntersectH
#define CFEMathIntersectH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFERect.h"
#include "types/CFESegment.h"
// ----------------------------------------------------------------------------
namespace CFEMath
{
	/// Checks whether two given rects overlap or not.
	FEBool bOverlap(const CFERect& _oA,const CFERect& _oB);

	/// Checks whether the given point falls inside the given rectangle or not.
	FEBool bInside(const CFERect& _oRect,const CFEVect2& _oPoint);
	
	/// Checks whether 2 given segments intersect or not. -1 do not intersect (0..1) intersect
	FEReal rIntersect(const CFESegment& _oSeg,const CFESegment& _oPnt);
}
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
