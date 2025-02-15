// ----------------------------------------------------------------------------
/*! \class FEBasicTypes
 *  \brief Basic Types used along the FuetEngine
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef FEBasicTypesH
#define FEBasicTypesH
// ----------------------------------------------------------------------------
// Base types
// ----------------------------------------------------------------------------
/*
typedef int                 FInt;
typedef unsigned int        FNat;
typedef FEReal              FReal;
typedef unsigned char       FByte;
typedef unsigned char       FChar;
typedef bool                FBool;

typedef std::basic_string<char> CFString;
*/

typedef unsigned char*      FEPointer;
typedef void*               FEHandler;
typedef unsigned int        uint;

// ----------------------------------------------------------------------------
#if defined(REAL_CLASS)

    // Some systems may prefer to define their own real class. For instance systems
    // with fast fixed point arithmetic. See template class to define
    #include "CRealClass_Generic.h"

    // typedef CRealClass FEReal;
    #define FEReal CRealClass

#else

	typedef float FEReal;

	const FEReal _0r   = 0.0f;
	const FEReal _025r = 0.25f;
	const FEReal _05r  = 0.5f;
	const FEReal _075r = 0.75f;
	const FEReal _1r   = 1.0f;
	const FEReal _2r   = 2.0f;

	const FEReal _3r   = 3.0f;
	const FEReal _4r   = 4.0f;
	const FEReal _7r   = 7.0f;
	const FEReal _8r   = 8.0f;
	const FEReal _15r  = 15.0f;
	const FEReal _16r  = 16.0f;
	const FEReal _31r  = 31.0f;
	const FEReal _32r  = 32.0f;
	const FEReal _63r  = 63.0f;
	const FEReal _64r  = 64.0f;
	const FEReal _127r = 127.0f;
	const FEReal _128r = 128.0f;
	const FEReal _255r = 255.0f;
	const FEReal _256r = 256.0f;
	const FEReal _1o255r = _1r / _255r;

	const FEReal _5r   = 5.0f;
	const FEReal _6r   = 6.0f;
	const FEReal _9r   = 9.0f;

	const FEReal _10r  = 10.0f;
	const FEReal _100r = 100.0f;
	const FEReal _1000r= 1000.0f;
	const FEReal _01r  = _1r / _10r;
	const FEReal _001r = _1r / _100r;
	const FEReal _0001r= _1r / _1000r;

	const FEReal _INFr = 1e6f;
	const FEReal _pINFr = _INFr;
	const FEReal _nINFr = -_INFr;

	const FEReal _PIr  = 3.1415926536f;
	const FEReal _PI2r = _PIr/_2r;
	const FEReal _2PIr = _2r*_PIr;
	const FEReal _3PI2r= (_3r*_PIr)/_2r;

	const FEReal _PI4r = _PIr/_4r;
	const FEReal _PI8r = _PIr/_8r;

#endif

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
