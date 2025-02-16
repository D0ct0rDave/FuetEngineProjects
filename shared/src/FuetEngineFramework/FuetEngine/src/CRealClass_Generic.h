// -------------------------------------------------------------------------------------------
#ifndef CRealClassGenericH
#define CRealClassGenericH
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

// For private construction via guts, or for some platform to hardcode construct.
enum CRealClassRAW { RCR_RAW };

class CRealClass {

	private:

		int	g; // the guts

		const static int BP= 12;  // how many low bits are right of Binary Point
		const static int BP2= BP*2;  // how many low bits are right of Binary Point
		const static int BPhalf= BP/2;  // how many low bits are right of Binary Point

		#if defined(DOUBLE_SUPPORT)
		static double STEP() { return 1.0 / (1<<BP); }  // smallest step we can represent

		#elif defined (FLOAT_SUPPORT)
		static float STEP() { return 1.0f / (1<<BP); }  // smallest step we can represent
		#endif

	public:

		CRealClass(CRealClassRAW, int guts) : g(guts) {}

		static CRealClass PInf() { return CRealClass(RCR_RAW,0x7fffffff); }; // max value representable
		static CRealClass NInf() { return CRealClass(RCR_RAW,0xffffffff); }; // min value representable

		CRealClass() : g(0) {}
		CRealClass(const CRealClass& a) : g( a.g ) {}

		CRealClass(int a) : g( a << BP ) {}
		CRealClass(unsigned int a) : g( a << BP ) {}
		CRealClass(long a) : g( a << BP ) {}

		CRealClass& operator =(const CRealClass& a) { g= a.g; return *this; }
		
		CRealClass& operator =(int a) { g= CRealClass(a).g; return *this; }
		CRealClass& operator =(unsigned int a) { g= CRealClass(a).g; return *this; }
		CRealClass& operator =(long a) { g= CRealClass(a).g; return *this; }
		
		operator int() { return g>>BP; }
		operator unsigned int() { return g>>BP; }
		operator long() { return g>>BP; }

		CRealClass operator +() const { return CRealClass(RCR_RAW,g); }
		CRealClass operator -() const { return CRealClass(RCR_RAW,-g); }

		CRealClass operator +(const CRealClass& a) const { return CRealClass(RCR_RAW, g + a.g); }
		CRealClass operator -(const CRealClass& a) const { return CRealClass(RCR_RAW, g - a.g); }
	#if 1
		// more acurate, using long long
		CRealClass operator *(const CRealClass& a) const { return CRealClass(RCR_RAW,  (int)( ((long long)g * (long long)a.g ) >> BP)); }
	#else
		// faster, but with only half as many bits right of binary point
		CRealClass operator *(const CRealClass& a) const { return CRealClass(RCR_RAW, (g>>BPhalf) * (a.g>>BPhalf) ); }
	#endif
		CRealClass operator /(const CRealClass& a) const { return CRealClass(RCR_RAW, int( (((long long)g << BP2) / (long long)(a.g)) >> BP) ); }



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
		CRealClass(float a) : g( int(a / (float)STEP()) ) {}	
		CRealClass& operator =(float a) { g= CRealClass(a).g; return *this; }
		operator float() { return g * (float)STEP(); }
		operator const float() const { return g * (float)STEP(); }

		CRealClass operator +(float a) const { return CRealClass(RCR_RAW, g + CRealClass(a).g); }
		CRealClass operator -(float a) const { return CRealClass(RCR_RAW, g - CRealClass(a).g); }
		CRealClass operator *(float a) const { return CRealClass(RCR_RAW, (g>>BPhalf) * (CRealClass(a).g>>BPhalf) ); }
		CRealClass operator /(float a) const { return CRealClass(RCR_RAW, int( (((long long)g << BP2) / (long long)(CRealClass(a).g)) >> BP) ); }

		CRealClass& operator +=(float a) { return *this = *this + a; return *this; }
		CRealClass& operator -=(float a) { return *this = *this - a; return *this; }
		CRealClass& operator *=(float a) { return *this = *this * a; return *this; }
		CRealClass& operator /=(float a) { return *this = *this / a; return *this; }	

		bool operator ==(float a) const { return g == CRealClass(a).g; }
		bool operator !=(float a) const { return g != CRealClass(a).g; }
		bool operator <=(float a) const { return g <= CRealClass(a).g; }
		bool operator >=(float a) const { return g >= CRealClass(a).g; }
		bool operator  <(float a) const { return g  < CRealClass(a).g; }
		bool operator  >(float a) const { return g  > CRealClass(a).g; }
		#endif

		#ifdef DOUBLE_SUPPORT
		operator double() { return g * (double)STEP(); }
		CRealClass(double a) : g( int(a / (double)STEP()) ) {}
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
		
		int data()
		{
			return(g);
		}

		int* pdata()
		{
			return(&g);
		}	
};
// -------------------------------------------------------------------------------------------
const CRealClass _0r = 0;
const CRealClass _05r = 0.5f;
const CRealClass _1r = 1;
const CRealClass _2r = 2;
const CRealClass _3r  = 3;
const CRealClass _4r  = 4;
const CRealClass _7r  = 7;
const CRealClass _8r  = 8;
const CRealClass _15r  = 15;
const CRealClass _16r  = 16;
const CRealClass _31r  = 31;
const CRealClass _32r  = 32;
const CRealClass _63r  = 63;
const CRealClass _64r  = 64;
const CRealClass _127r  = 127;
const CRealClass _128r  = 128;
const CRealClass _255r = 255;
const CRealClass _256r = 256;

const CRealClass _INFr = CRealClass::PInf();

const CRealClass _PI_  = 3.1415926536f;
const CRealClass _PI2_ = _PI_/_2r;
const CRealClass _2PI_  = _2r*_PI_;
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

// -------------------------------------------------------------------------------------------
#endif
// -------------------------------------------------------------------------------------------


