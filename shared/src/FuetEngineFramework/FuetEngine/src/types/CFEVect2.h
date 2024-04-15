// ---------------------------------------------------------------------------
/*! \class CFEVect2
 *  \brief 2D Vector class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEVect2H
#define CFEVect2H
// ----------------------------------------------------------------------------
#include "Support/Math/CFEMath.h"
#include "FEBasicTypes.h"
#include "FEConsts.h"
// ----------------------------------------------------------------------------
class CFEVect2
{
    public:
        /// Default constructor for the class.
        CFEVect2(){};

        FEReal x;
        FEReal y;

    public:

		/// Linearly interpolates 2 vectors by a given factor.
        void Lerp(const CFEVect2& _oA,const CFEVect2& _oB,FEReal _rFact)
        {
            // x = bx*f + ax(1-f) = bx*f + ax - axf = (bx-ax)*f + ax
            x = _oA.x + (_oB.x - _oA.x)*_rFact;
            y = _oA.y + (_oB.y - _oA.y)*_rFact;
        }

		/// Rotates the vector by a given angle (in radians).
        void Rotate(FEReal _rAngle)
        {
            FEReal rCosA = CFEMath::rCos(_rAngle);
            FEReal rSinA = CFEMath::rSin(_rAngle);

            CFEVect2 oPos(*this);

            x  = oPos.x*rCosA + oPos.y*rSinA;
            y = -oPos.x*rSinA + oPos.y*rCosA;
        }

		/// Builds the ortogonal vector of the current one.
        void Orthogonalize()
        {
            FEReal r = x;
            x = -y;
            y = r;
        }

		/// Retrieves the distance to a given vector.
        FEReal rDistance(const CFEVect2& _oV) const
        {
            CFEVect2 oD = _oV - *this;
            return ( oD.rLength() );
        }

		/// Retrieves the squared distance to a given vector.
        FEReal rSqDistance(const CFEVect2& _oV) const
        {
            CFEVect2 oD = _oV - *this;
            return ( oD.rSqLength() );
        }

        /// Retrieves the squared length of the vector
        FEReal rSqLength() const
        {
            return(x*x + y*y);
        }

		/// Retrieves the length of the vector
        FEReal rLength() const
        {
            return ( CFEMath::rSqrt( rSqLength() ) );
        }

		/// Retrieves the dot product with the given vector.
        FEReal rDot(const CFEVect2& _oV) const
        {
            return ( x*_oV.x + y*_oV.y );
        }

		/// Makes the vector unitary.
        void Normalize()
        {
        	#ifdef DS
        	// Dirty trick for DS fixed point math:_(
        	if ((x>_64r) || (y>_64r))
        	{
	       		x *= _0001r;
        		y *= _0001r;
        	}
        	#endif
        	
			FEReal rLen = rLength();
			if (rLen == _0r)
			{
				x = _INFr;
				y = _INFr;
				return;
			}
			
			FEReal r1_OVER_LEN = _1r / rLen;
            x *= r1_OVER_LEN;
            y *= r1_OVER_LEN;
        }

        /// Constructor with initialization.
        CFEVect2(FEReal _rX,FEReal _rY)
        {
            x = _rX;
            y = _rY;
        }

        /// Copy constructor.
        CFEVect2(const CFEVect2& _oSrc)
        {
            x = _oSrc.x;
            y = _oSrc.y;
        }

        CFEVect2 operator-() const
        {
            return( CFEVect2(-x,-y) );
        }

        CFEVect2 operator -(const CFEVect2& v) const
        {
            return CFEVect2(x-v.x,y-v.y);
        }

        CFEVect2& operator -=(const CFEVect2& v)
        {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        CFEVect2 operator +(const CFEVect2& v) const
        {
            return( CFEVect2(x+v.x,y+v.y) );
        }

        CFEVect2& operator +=(const CFEVect2& v)
        {
            x += v.x;
            y += v.y;
            return *this;
        }

        CFEVect2 operator *(const FEReal& _rFactor) const
        {
            return CFEVect2(x*_rFactor,y*_rFactor);
        }

        CFEVect2& operator *=(const FEReal& _rFactor)
        {
            x *= _rFactor;
            y *= _rFactor;
            return *this;
        }

        CFEVect2 operator /(const FEReal& _rFactor) const
        {
            FEReal rFactor = 1.0f / _rFactor;
            return CFEVect2(x*rFactor,y*rFactor);
        }

        CFEVect2& operator /=(const FEReal& _rFactor)
        {
            FEReal rFactor = 1.0f / _rFactor;
            x *= rFactor;
            y *= rFactor;

            return *this;
        }

        CFEVect2 operator *(const CFEVect2& v) const
        {
            return CFEVect2(x*v.x,y*v.y);
        }

        CFEVect2& operator *=(const CFEVect2& v)
        {
            x *= v.x;
            y *= v.y;
            return *this;
        }

        CFEVect2 operator /(const CFEVect2& v) const
        {
            return CFEVect2(x/v.x,y/v.y);
        }

        CFEVect2& operator /=(const CFEVect2& v)
        {
            x /= v.x;
            y /= v.y;

            return *this;
        }

        FEBool operator |=(const CFEVect2& v)
        {
            return( CFEMath::bBetween(x-EPSILON,x+EPSILON,v.x) && CFEMath::bBetween(y-EPSILON,y+EPSILON,v.y));
        }

        FEBool operator ==(const CFEVect2& v)
        {
			return( (x == v.x) && (y == v.y));
        }

        FEBool operator ==(const CFEVect2& v) const
        {
			return( (x == v.x) && (y == v.y));
        }

        FEBool operator !=(const CFEVect2& v)
        {
			return( (x != v.x) || (y != v.y));
        }
		
		FEReal &operator[](const uint index)
		{
			if (index == 0) return(x);
			return(y);
		}

        FEReal operator[](const uint index) const
        {
			if (index == 0) return(x);
			return(y);
        }

        /// Returns the ZERO vector.
        static const CFEVect2& ZERO();

        /// Returns the ONE vector.
        static const CFEVect2& ONE();

        /// Returns the X vector.
        static const CFEVect2& X();

        /// Returns the Y vector.
        static const CFEVect2& Y();
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
