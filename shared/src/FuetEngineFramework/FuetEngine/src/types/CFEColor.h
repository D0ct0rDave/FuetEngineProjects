// ----------------------------------------------------------------------------
/*! \class CFEColor
 *  \brief Color class
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEColorH
#define CFEColorH
// ----------------------------------------------------------------------------
#include "Support/Math/CFEMath.h"
#include "FEBasicTypes.h"
#include "FEConsts.h"
// ----------------------------------------------------------------------------
/// Class CFEColor
// ----------------------------------------------------------------------------
class CFEColor
{
    public:

        FEReal r;
        FEReal g;
        FEReal b;
        FEReal a;

    public:

        CFEColor(){};
        CFEColor(const FEReal& _r,const FEReal& _g,const FEReal& _b,const FEReal& _a)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        CFEColor(unsigned char _r,unsigned char _g,unsigned char _b)
        {
            r = FEReal(_r) * _1o255r;
            g = FEReal(_g) * _1o255r;
            b = FEReal(_b) * _1o255r;
            a = _1r;
        }

        CFEColor(uint _uiColor)
        {
            r = FEReal( _uiColor        & 0x000000ff) * _1o255r;
            g = FEReal((_uiColor >>  8) & 0x000000ff) * _1o255r;
            b = FEReal((_uiColor >> 16) & 0x000000ff) * _1o255r;
            a = FEReal((_uiColor >> 24) & 0x000000ff) * _1o255r;
        }

        /// Stores the linear interpolation between 2 colors and a given factor
        void Lerp(const CFEColor& _oA,const CFEColor& _oB,FEReal _rFact)
        {
            // x = bx*f + ax(1-f) = bx*f + ax - axf = (bx-ax)*f + ax
            r = _oA.r + (_oB.r - _oA.r)*_rFact;
            g = _oA.g + (_oB.g - _oA.g)*_rFact;
            b = _oA.b + (_oB.b - _oA.b)*_rFact;
            a = _oA.a + (_oB.a - _oA.a)*_rFact;
        }

		// Clamps the internal RGBA values into the normalized range.
		void Clamp()
		{
			if (r>_1r) r=_1r; else if (r<_0r) r=_0r;
			if (g>_1r) g=_1r; else if (g<_0r) g=_0r;
			if (b>_1r) b=_1r; else if (b<_0r) b=_0r;
			if (a>_1r) a=_1r; else if (a<_0r) a=_0r;
		}

        /// Scales a color by a scalar factor.
        CFEColor operator *(const FEReal& _rFactor) const
        {
            return CFEColor(r*_rFactor,g*_rFactor,b*_rFactor,a*_rFactor);
        }
        
        /// Scales the color by given factor
        CFEColor& operator *=(const FEReal& _rFactor)
        {
            r *= _rFactor;
            g *= _rFactor;
            b *= _rFactor;
            a *= _rFactor;
            return *this;
        }
        
        /// Divides a color by a scalar factor.
        CFEColor operator /(const FEReal& _rFactor) const
        {
			if (_rFactor > _0r)
			{
				FEReal rInvFact = _1r / _rFactor;
				return( *this*rInvFact );
			}
			else
				return (WHITE());
        }
        
        /// Divides the color by given factor
        CFEColor& operator /=(const FEReal& _rFactor)
        {
			if (_rFactor > _0r)
			{
				FEReal rInvFact = _1r / _rFactor;
				*this *= rInvFact;
			}
			else
			{
				*this = WHITE();
			}

            return *this;
        }

        /// Returns the sum of the color and a given one.
        CFEColor operator +(const CFEColor& _oColor) const
        {
            return CFEColor(r+_oColor.r,g+_oColor.g,b+_oColor.b,a+_oColor.a);
        }
        
        /// Adds the current color to given color
        CFEColor& operator +=(const CFEColor& _oColor)
        {
            r += _oColor.r;
            g += _oColor.g;
            b += _oColor.b;
            a += _oColor.a;

            return *this;
        }

        /// Returns the substraction of the color and a given one.
        CFEColor operator -(const CFEColor& _oColor) const
        {
            return CFEColor(r-_oColor.r,g-_oColor.g,b-_oColor.b,a-_oColor.a);
        }
        
        /// Substracts the current color to given color
        CFEColor& operator -=(const CFEColor& _oColor)
        {
            r -= _oColor.r;
            g -= _oColor.g;
            b -= _oColor.b;
            a -= _oColor.a;

            return *this;
        }

        /// Returns the modulation of the color and a given one.
        CFEColor operator *(const CFEColor& _oColor) const
        {
            return CFEColor(r*_oColor.r,g*_oColor.g,b*_oColor.b,a*_oColor.a);
        }
        
        /// Modules the current color to given color
        CFEColor& operator *=(const CFEColor& _oColor)
        {
            r *= _oColor.r;
            g *= _oColor.g;
            b *= _oColor.b;
            a *= _oColor.a;

            return *this;
        }

        /// Returns the division of the color and a given one.
        CFEColor operator /(const CFEColor& _oColor) const
        {
            return CFEColor(r/_oColor.r,g/_oColor.g,b/_oColor.b,a/_oColor.a);
        }

        /// Divides the current color to given color
        CFEColor& operator /=(const CFEColor& _oColor)
        {
            r /= _oColor.r;
            g /= _oColor.g;
            b /= _oColor.b;
            a /= _oColor.a;

            return *this;
        }

        FEBool operator |=(const CFEColor& _oColor)
        {
            return( CFEMath::bBetween(r-EPSILON,r+EPSILON,_oColor.r) && 
					CFEMath::bBetween(g-EPSILON,g+EPSILON,_oColor.g) &&
					CFEMath::bBetween(b-EPSILON,b+EPSILON,_oColor.b) &&
					CFEMath::bBetween(a-EPSILON,a+EPSILON,_oColor.a)
					);
        }

        FEBool operator ==(const CFEColor& _oColor)
        {
			return( (r == _oColor.r) && (g == _oColor.g) && (b == _oColor.b) && (a == _oColor.a));
        }

        FEBool operator !=(const CFEColor& _oColor)
        {
			return( (r != _oColor.r) || (g != _oColor.g) || (b != _oColor.b) || (a != _oColor.a));
        }

        /// Returns the full opaque BLACK color.
        static const CFEColor& BLACK();

        /// Returns the full opaque WHITE color.
        static const CFEColor& WHITE();
        
        /// Returns the full transparent BLACK color.
        static const CFEColor& TRANSPBLACK();
        
        /// Returns the full transparent WHITE color.
        static const CFEColor& TRANSPWHITE();
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
