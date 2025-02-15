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
#ifndef CFEColorH
#define CFEColorH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
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

        /// Stores the linear interpolation between 2 colors and a given factor
        void Lerp(const CFEColor& _oA,const CFEColor& _oB,FEReal _rFact)
        {
            // x = bx*f + ax(1-f) = bx*f + ax - axf = (bx-ax)*f + ax
            r = _oA.r + (_oB.r - _oA.r)*_rFact;
            g = _oA.g + (_oB.g - _oA.g)*_rFact;
            b = _oA.b + (_oB.b - _oA.b)*_rFact;
            a = _oA.a + (_oB.a - _oA.a)*_rFact;
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
