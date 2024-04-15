// -------------------------------------------------------------------------------------------
#ifndef CRealClass_GenericH
#define CRealClass_GenericH
// -------------------------------------------------------------------------------------------
/*
Copyright (c) 2006 Henry Strickland & Ryan Seto

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

        (* http://www.opensource.org/licenses/mit-license.php *)
*/

// -------------------------------------------------------------------------------------------
// #define DOUBLE_SUPPORT
#define FLOAT_SUPPORT
// -------------------------------------------------------------------------------------------
typedef long long s64;
#define INTERNAL_TYPE int
const static INTERNAL_TYPE SHORTMASK = 0x0000ffff;

const static unsigned int BP		= 18;		// how many low bits are right of Binary Point
const static unsigned int BP2		= BP*2;		// how many low bits are right of Binary Point
const static unsigned int BPhalf	= BP/2;		// how many low bits are right of Binary Point

// const static unsigned int FX32TOREALSHIFT = BP-FX32_SHIFT;

#if defined(DOUBLE_SUPPORT)
	const float DOUBLE_STEP = 1.0 / (1<<BP);
#endif

#if defined (FLOAT_SUPPORT)
	const float FLOAT_STEP = 1.0F / (1<<BP);
#endif
// -------------------------------------------------------------------------------------------
// For private construction via guts, or for some platform to hardcode construct.
enum CRealClassRAW { RCR_RAW };

class CRealClass {

	private:

		INTERNAL_TYPE g; // the guts
		float f;

		#if defined(DOUBLE_SUPPORT)
		static double dSTEP() { return DOUBLE_STEP; }  // smallest step we can represent
		#endif

		#if defined (FLOAT_SUPPORT)
		static float fSTEP() { return FLOAT_STEP; }
		#endif

	public:

		// Internal special constructor
		CRealClass(CRealClassRAW, INTERNAL_TYPE guts) : g(guts),f() {}
		
		// Default constructor
		CRealClass() : g(0), f(0.0f) {}

		// Constructors
		CRealClass(const CRealClass& a) : g( a.g ), f(a.f) {}
		CRealClass(int a) : g( a << BP ),f((float)a) {}
		CRealClass(unsigned int a) : g( (INTERNAL_TYPE)(a << BP) ),f((float)a) {}
		CRealClass(long a) : g( a << BP ),f((float)a) {}

		// copy constructors
		CRealClass& operator =(const CRealClass& a) { g = a.g; f = a.f; return *this; }
		CRealClass& operator =(int a) { g = CRealClass(a).g; f = CRealClass(a).f; return *this; }
		CRealClass& operator =(unsigned int a) { g = CRealClass(a).g; f = CRealClass(a).f; return *this; }
		CRealClass& operator =(long a) { g = CRealClass(a).g; f = CRealClass(a).f; return *this; }
		
		// cast operators
		operator int() { return int(g>>BP); }
		operator unsigned int() { return unsigned int(g>>BP); }
		operator long() { return long(g>>BP); }

		// short support
		CRealClass(short a) : g( INTERNAL_TYPE(a) << BP ), f(a) {}
		CRealClass& operator =(short a)
		{
			g = INTERNAL_TYPE(a) << BP;
			f = CRealClass(a).f; 
			return *this;
		}

		operator short() { return (short)((g >> BP) & SHORTMASK); }
		operator const short() const { return (short)((g >> BP) & SHORTMASK); }

		// unsigned short support
		CRealClass(unsigned short a) : g( INTERNAL_TYPE(a) << BP ), f(a) {}
		CRealClass& operator =(unsigned short a)
		{
			g= INTERNAL_TYPE(a) << BP;
			f = CRealClass(a).f; 
			return *this;
		}

		operator unsigned short() { return( (unsigned short)((g >> BP) & SHORTMASK) ); }
		operator const unsigned short() const { return (unsigned short)((g >> BP) & SHORTMASK); }

		// arithmetic operators
		CRealClass operator +() const
		{
			CRealClass rRes(RCR_RAW,g);
			rRes.f = f;
			return rRes;
		}
		
		CRealClass operator -() const
		{		
			CRealClass rRes(RCR_RAW,-g);
			rRes.f = f;
			return rRes;
		}

		CRealClass operator +(const CRealClass& a) const
		{
			int iA = int(a.g>>BP);
			int iB = int(this->g>>BP);
			int iSum = iA+iB;

			CRealClass rRes = CRealClass(RCR_RAW, g + a.g);
			rRes.f = f + a.f;

			int iRes = rRes.g >> BP;
			
			// to detect overflows
			if (iSum*iRes < 0)
			{
				int a = 0;
				// _asm { int 3 };
			}

			return rRes; 
		}

		CRealClass operator -(const CRealClass& a) const
		{
			CRealClass rRes(RCR_RAW,g - a.g);
			rRes.f = f - a.f;
			return rRes;
		}

	#if 1
		// more acurate, using long long
		CRealClass operator *(const CRealClass& a) const
		{
			CRealClass rRes(RCR_RAW,  INTERNAL_TYPE((long long(g) * long long(a.g)) >> BP));
			rRes.f = f * a.f;
			return rRes;
		}
	#else
		// faster, but with only half as many bits right of binary point
		CRealClass operator *(const CRealClass& a) const { return CRealClass(RCR_RAW, (g>>BPhalf) * (a.g>>BPhalf) ); }
	#endif
		CRealClass operator /(const CRealClass& a) const
		{
			CRealClass rRes(RCR_RAW, INTERNAL_TYPE((( (s64(g)) << BP) / ((s64(a.g))))));
			rRes.f = f / a.f;
			return rRes;
		}

		CRealClass& operator +=(CRealClass a) { return *this = *this + a; return *this; }
		CRealClass& operator -=(CRealClass a) { return *this = *this - a; return *this; }
		CRealClass& operator *=(CRealClass a) { return *this = *this * a; return *this; }
		CRealClass& operator /=(CRealClass a) { return *this = *this / a; return *this; }

		CRealClass& operator +=(int a) { return *this = *this + (CRealClass)a; return *this; }
		CRealClass& operator -=(int a) { return *this = *this - (CRealClass)a; return *this; }
		CRealClass& operator *=(int a) { return *this = *this * (CRealClass)a; return *this; }
		CRealClass& operator /=(int a) { return *this = *this / (CRealClass)a; return *this; }

		CRealClass& operator +=(unsigned int a) { return *this = *this + (CRealClass)a; return *this; }
		CRealClass& operator -=(unsigned int a) { return *this = *this - (CRealClass)a; return *this; }
		CRealClass& operator *=(unsigned int a) { return *this = *this * (CRealClass)a; return *this; }
		CRealClass& operator /=(unsigned int a) { return *this = *this / (CRealClass)a; return *this; }

		CRealClass& operator +=(long a) { return *this = *this + (CRealClass)a; return *this; }
		CRealClass& operator -=(long a) { return *this = *this - (CRealClass)a; return *this; }
		CRealClass& operator *=(long a) { return *this = *this * (CRealClass)a; return *this; }
		CRealClass& operator /=(long a) { return *this = *this / (CRealClass)a; return *this; }

		bool operator ==(const CRealClass& a) const { return g == a.g; }
		bool operator !=(const CRealClass& a) const { return g != a.g; }
		bool operator <=(const CRealClass& a) const { return g <= a.g; }
		bool operator >=(const CRealClass& a) const { return g >= a.g; }
		bool operator  <(const CRealClass& a) const { return g  < a.g; }
		bool operator  >(const CRealClass& a) const { return g  > a.g; }


		#ifdef FLOAT_SUPPORT
		CRealClass(float a) : g( INTERNAL_TYPE(a / fSTEP()) ), f(a) {}
		CRealClass& operator =(float a) { g = CRealClass(a).g; f=a; return *this; }
		operator float() { return float(g) * fSTEP(); }
		operator const float() const { return float(g) * fSTEP(); }

		CRealClass operator +(float a) const
		{
			CRealClass rRes(RCR_RAW, g + CRealClass(a).g);
			rRes.f = f + a;
			return rRes;
		}
		
		CRealClass operator -(float a) const
		{
			CRealClass rRes(RCR_RAW, g - CRealClass(a).g);
			rRes.f = f - a;
			return rRes;
		}		

		CRealClass operator *(float a) const
		{
			CRealClass rRes(RCR_RAW, (g>>BPhalf) * (CRealClass(a).g>>BPhalf) );
			rRes.f = f * a;
			return rRes;
		}

		CRealClass operator /(float a) const
		{
			// return CRealClass(RCR_RAW, INTERNAL_TYPE( FX_DivFx64c((fx32)g,(fx32)CRealClass(a).g) >> (FX64C_SHIFT-BP)) );
			// return CRealClass(RCR_RAW, INTERNAL_TYPE( ((s64(g) << BP2) / (s64(CRealClass(a).g))) >> BP));
			CRealClass rRes(RCR_RAW, INTERNAL_TYPE( ((s64(g) << BP) / (s64(CRealClass(a).g))) ));
			rRes.f = f / a;
			return rRes;
		}

		CRealClass& operator +=(float a) { return *this = *this + CRealClass(a); return *this; }
		CRealClass& operator -=(float a) { return *this = *this - CRealClass(a); return *this; }
		CRealClass& operator *=(float a) { return *this = *this * CRealClass(a); return *this; }
		CRealClass& operator /=(float a) { return *this = *this / CRealClass(a); return *this; }	

		bool operator ==(float a) const { return g == CRealClass(a).g; }
		bool operator !=(float a) const { return g != CRealClass(a).g; }
		bool operator <=(float a) const { return g <= CRealClass(a).g; }
		bool operator >=(float a) const { return g >= CRealClass(a).g; }
		bool operator  <(float a) const { return g  < CRealClass(a).g; }
		bool operator  >(float a) const { return g  > CRealClass(a).g; }
		#endif

		#ifdef DOUBLE_SUPPORT
		operator double() { return g * (double)dSTEP(); }
		CRealClass(double a) : g( int(a / (double)dSTEP()) ) {}
		CRealClass& operator =(double a) { g= CRealClass(a).g; return *this; }
		
		CRealClass operator +(double a) const { return CRealClass(RCR_RAW, g + CRealClass(a).g); }
		CRealClass operator -(double a) const { return CRealClass(RCR_RAW, g - CRealClass(a).g); }
		
		/*
		CRealClass& operator +=(double a) { return *this = *this + a; return *this; }
		CRealClass& operator -=(double a) { return *this = *this - a; return *this; }
		CRealClass& operator *=(double a) { return *this = *this * a; return *this; }
		CRealClass& operator /=(double a) { return *this = *this / a; return *this; }
		*/
		
		bool operator ==(double a) const { return g == CRealClass(a).g; }
		bool operator !=(double a) const { return g != CRealClass(a).g; }
		bool operator <=(double a) const { return g <= CRealClass(a).g; }
		bool operator >=(double a) const { return g >= CRealClass(a).g; }
		bool operator  <(double a) const { return g  < CRealClass(a).g; }
		bool operator  >(double a) const { return g  > CRealClass(a).g; }
		#endif

		
		INTERNAL_TYPE data()
		{
			return(g);
		}

		INTERNAL_TYPE* pdata()
		{
			return(&g);
		}	
};
// -------------------------------------------------------------------------------------------
inline CRealClass operator /(unsigned int a, const CRealClass& b) { return CRealClass(a)/b; }// DMC
inline CRealClass operator /(const CRealClass& a, unsigned int b) { return a/CRealClass(b); }// DMC

inline CRealClass operator *(unsigned int a, const CRealClass& b) { return CRealClass(a)*b; }// DMC
inline CRealClass operator *(const CRealClass& a, unsigned int b) { return a*CRealClass(b); }// DMC

inline CRealClass operator *(int a, const CRealClass& b) { return CRealClass(a)*b; }// DMC
inline CRealClass operator *(const CRealClass& a, int b) { return a*CRealClass(b); }// DMC

inline int& operator +=(int& a, const CRealClass& b) { a = (CRealClass)a + b; return a; }
inline int& operator -=(int& a, const CRealClass& b) { a = (CRealClass)a - b; return a; }
inline int& operator *=(int& a, const CRealClass& b) { a = (CRealClass)a * b; return a; }
inline int& operator /=(int& a, const CRealClass& b) { a = (CRealClass)a / b; return a; }

inline unsigned int& operator +=(unsigned int& a, const CRealClass& b) { a = (CRealClass)a + b; return a; }
inline unsigned int& operator -=(unsigned int& a, const CRealClass& b) { a = (CRealClass)a - b; return a; }
inline unsigned int& operator *=(unsigned int& a, const CRealClass& b) { a = (CRealClass)a * b; return a; }
inline unsigned int& operator /=(unsigned int& a, const CRealClass& b) { a = (CRealClass)a / b; return a; }

inline long& operator +=(long& a, const CRealClass& b) { a = (CRealClass)a + b; return a; }
inline long& operator -=(long& a, const CRealClass& b) { a = (CRealClass)a - b; return a; }
inline long& operator *=(long& a, const CRealClass& b) { a = (CRealClass)a * b; return a; }
inline long& operator /=(long& a, const CRealClass& b) { a = (CRealClass)a / b; return a; }


// -------------------------------------------------------------------------------------------
#ifdef FLOAT_SUPPORT

	inline bool operator ==(float a, const CRealClass& b) { return CRealClass(a) == b; }
	inline bool operator !=(float a, const CRealClass& b) { return CRealClass(a) != b; }
	inline bool operator <=(float a, const CRealClass& b) { return CRealClass(a) <= b; }
	inline bool operator >=(float a, const CRealClass& b) { return CRealClass(a) >= b; }
	inline bool operator  <(float a, const CRealClass& b) { return CRealClass(a)  < b; }
	inline bool operator  >(float a, const CRealClass& b) { return CRealClass(a)  > b; }

	inline CRealClass operator +(float a, const CRealClass& b) { return CRealClass(a)+b; }
	inline CRealClass operator -(float a, const CRealClass& b) { return CRealClass(a)-b; }
	inline CRealClass operator *(float a, const CRealClass& b) { return CRealClass(a)*b; }
	inline CRealClass operator /(float a, const CRealClass& b) { return CRealClass(a)/b; }

	inline float& operator +=(float& a, const CRealClass& b) { a = a + b; return a; }
	inline float& operator -=(float& a, const CRealClass& b) { a = a - b; return a; }
	inline float& operator *=(float& a, const CRealClass& b) { a = a * b; return a; }
	inline float& operator /=(float& a, const CRealClass& b) { a = a / b; return a; }

#endif
// -------------------------------------------------------------------------------------------


// -------------------------------------------------------------------------------------------
#ifdef DOUBLE_SUPPORT

	inline bool operator ==(double a, const CRealClass& b) { return CRealClass(a) == b; }
	inline bool operator !=(double a, const CRealClass& b) { return CRealClass(a) != b; }
	inline bool operator <=(double a, const CRealClass& b) { return CRealClass(a) <= b; }
	inline bool operator >=(double a, const CRealClass& b) { return CRealClass(a) >= b; }
	inline bool operator  <(double a, const CRealClass& b) { return CRealClass(a)  < b; }
	inline bool operator  >(double a, const CRealClass& b) { return CRealClass(a)  > b; }

	/*
	inline double& operator +=(double& a, const CRealClass& b) { a = a + b; return a; }
	inline double& operator -=(double& a, const CRealClass& b) { a = a - b; return a; }
	inline double& operator *=(double& a, const CRealClass& b) { a = a * b; return a; }
	inline double& operator /=(double& a, const CRealClass& b) { a = a / b; return a; }
	*/

	inline CRealClass operator +(double a, const CRealClass& b) { return CRealClass(a)+b; }
	inline CRealClass operator -(double a, const CRealClass& b) { return CRealClass(a)-b; }
	inline CRealClass operator *(double a, const CRealClass& b) { return CRealClass(a)*b; }
	inline CRealClass operator /(double a, const CRealClass& b) { return CRealClass(a)/b; }

#endif

// ----------------------------------------------------------------------------
// Predefined Constants
// ----------------------------------------------------------------------------
const CRealClass _0r   = 0;
const CRealClass _025r = 0.25f;
const CRealClass _05r  = 0.5f;
const CRealClass _075r = 0.75f;
const CRealClass _1r   = 1.0f;

const CRealClass _2r   = 2*_1r;
const CRealClass _3r   = 3*_1r;
const CRealClass _4r   = 4*_1r;
const CRealClass _7r   = 7*_1r;
const CRealClass _8r   = 8*_1r;
const CRealClass _15r  = 15*_1r;
const CRealClass _16r  = 16*_1r;
const CRealClass _31r  = 31*_1r;
const CRealClass _32r  = 32*_1r;
const CRealClass _63r  = 63*_1r;
const CRealClass _64r  = 64*_1r;
const CRealClass _127r = 127*_1r;
const CRealClass _128r = 128*_1r;
const CRealClass _255r = 255*_1r;
const CRealClass _256r = 256*_1r;
const CRealClass _1o255r = _1r / _255r;

const CRealClass _5r   = 5*_1r;
const CRealClass _6r   = 6*_1r;
const CRealClass _9r   = 9*_1r;

const CRealClass _10r  = 10*_1r;
const CRealClass _100r = 100*_1r;
const CRealClass _1000r= 1000*_1r;
const CRealClass _01r  = _1r / _10r;
const CRealClass _001r = _1r / _100r;
const CRealClass _0001r= _1r / _1000r;
	
const CRealClass _INFr  = CRealClass(RCR_RAW,0x7fffffff);
const CRealClass _pINFr = CRealClass(RCR_RAW,0x7fffffff);
const CRealClass _nINFr = CRealClass(RCR_RAW,0x80000000);

const CRealClass _PIr  = 3.1415926536f;
const CRealClass _PI2r = _PIr/_2r;
const CRealClass _2PIr = _2r*_PIr;
const CRealClass _3PI2r= (_3r*_PIr)/_2r;

const CRealClass _PI4r = _PIr/_4r;
const CRealClass _PI8r = _PIr/_8r;

// -------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------
