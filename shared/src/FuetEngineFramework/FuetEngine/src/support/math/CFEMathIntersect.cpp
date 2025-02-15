// ----------------------------------------------------------------------------
/*! \class CFEMathIntersect
 *  \brief Math Intersection tests.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMathIntersect.h"
#include "System/CFESystem.h"
// ----------------------------------------------------------------------------
bool CFEMath::bOverlap(const CFERect& _oA,const CFERect& _oB)
{
	return(
			// overlap code check
			!(
				// separating axis test

				// over x
				(_oA.m_oIni.x > _oB.m_oEnd.x)
			 || (_oA.m_oEnd.x < _oB.m_oIni.x)

				// over y
				|| (_oA.m_oIni.y > _oB.m_oEnd.y)
				|| (_oA.m_oEnd.y < _oB.m_oIni.y)
			)
				
			||

			// inclusion code check (dst inside the src)
			(
			   (_oA.m_oIni.x <= _oB.m_oIni.x) && (_oA.m_oEnd.x >= _oB.m_oEnd.x)
			&& (_oA.m_oIni.y <= _oB.m_oIni.y) && (_oA.m_oEnd.y >= _oB.m_oEnd.y)
			)
			||
			(
				   (_oA.m_oIni.x >= _oB.m_oIni.x) && (_oA.m_oEnd.x <= _oB.m_oEnd.x)
				&& (_oA.m_oIni.y >= _oB.m_oIni.y) && (_oA.m_oEnd.y <= _oB.m_oEnd.y)
			)

			);
}
// ----------------------------------------------------------------------------
bool CFEMath::bInside(const CFERect& _oRect,const CFEVect2& _oPoint)
{
    return ( CFEMath::bBetween(_oRect.m_oIni.x,_oRect.m_oEnd.x,_oPoint.x) && CFEMath::bBetween(_oRect.m_oIni.y,_oRect.m_oEnd.y,_oPoint.y) );
}
// ----------------------------------------------------------------------------
