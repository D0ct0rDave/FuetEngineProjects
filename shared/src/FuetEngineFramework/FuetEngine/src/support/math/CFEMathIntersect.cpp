// ----------------------------------------------------------------------------
/*! \class CFEMathIntersect
 *  \brief Math Intersection tests.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMathIntersect.h"
#include "core/CFECore.h"
// ----------------------------------------------------------------------------
FEBool CFEMath::bOverlap(const CFERect& _oA,const CFERect& _oB)
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
FEBool CFEMath::bInside(const CFERect& _oRect,const CFEVect2& _oPoint)
{
    return ( CFEMath::bBetween(_oRect.m_oIni.x,_oRect.m_oEnd.x,_oPoint.x) && CFEMath::bBetween(_oRect.m_oIni.y,_oRect.m_oEnd.y,_oPoint.y) );
}
// ----------------------------------------------------------------------------
FEReal CFEMath::rIntersect(const CFESegment& _oSeg,const CFESegment& _oPnt)
{
	// TAKEN FROM 2D RALLY project

	/*
	E = B-A = ( Bx-Ax, By-Ay )
	F = D-C = ( Dx-Cx, Dy-Cy ) 
	P = ( -Ey, Ex )
	h = ( (A-C) * P ) / ( F * P )
	*/
	
	CFEVect2 E,F,P;
	FEReal h;

	E =	_oSeg.m_oEnd - _oSeg.m_oIni;
	F = _oPnt.m_oEnd - _oPnt.m_oIni;

	P.x = -E.y;	// la normal !?!?
	P.y = E.x;

	FEReal rFP = F.rDot(P);

	if (rFP != 0.0f)
	{
		CFEVect2 AC = _oSeg.m_oIni - _oPnt.m_oIni;
		h = AC.rDot(P) / rFP;
		
		if ((h>=0.0f) && (h<=1.0f))
			return(h);			
	}

	return(-_1r);
	
	/*
	float fLambdaDen = ((_roSeg.m_oDir.y / _roPnt.m_oDir.y) - (_roSeg.m_oDir.x / _roPnt.m_oDir.x));
	float fLambdaNum = ((_roSeg.m_oA.y / _roPnt.m_oA.y) - (_roSeg.m_oA.x / _roPnt.m_oA.x)); 
	float fLambda    = fLambdaNum / fLambdaDen;

	float fMu = ((_roSeg.m_oA.x - _roPnt.m_oA.x) - (fLambda*_roSeg.m_oDir.x)) / _roPnt.m_oDir.x; 

	oRes.first  = ((fLambda>=0.0f) && (fLambda<=1.0f)) && ((fMu>=0.0f) && (fMu<=1.0f));
	oRes.second = fMu;
	*/
	/*
	CFEVect2 oSAPA = _oPnt.m_oIni - _oSeg.m_oIni;
	CFEVect2 oSAPB = _oPnt.m_oEnd - _oSeg.m_oIni;

	FEReal rDotPA = oSAPA.rDot(&_oSeg.m_oNormal);
	FEReal rDotPB = oSAPB.rDot(&_oSeg.m_oNormal);

	CFEVect2 oPASA = _oSeg.m_oIni - _oPnt.m_oIni;
	CFEVect2 oPASB = _oSeg.m_oEnd - _oPnt.m_oIni;

	FEReal rDotSA = oPASA.rDot(&_oPnt.m_oNormal);
	FEReal rDotSB = oPASB.rDot(&_oPnt.m_oNormal);

	// Both dot products should have different sign then the multiply should give a negative number
	FEReal rRes = ((rDotPA*rDotPB) < 0.0f) && ((rDotSA*rDotSB)<0.0f); 
	oRes.second = 0.0f;
	return (oRes);
	*/
}
