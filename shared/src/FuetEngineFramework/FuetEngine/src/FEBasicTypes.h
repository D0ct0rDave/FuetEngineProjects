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

    #include "CRealClass.h"
    // typedef CRealClass FEReal;
    #define FEReal CRealClass

#else

	typedef float FEReal;

	const FEReal _0r   = 0.0f;
	const FEReal _05r  = 0.5f;
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

	const FEReal _INFr = 1e6f;

	const FEReal _PIr_  = 3.1415926536f;
	const FEReal _PI2r_ = _PIr_/_2r;
	const FEReal _2PIr_ = _2r*_PIr_;

#endif
// ----------------------------------------------------------------------------
#include <vector>

template <typename T>
class CFEArray : public std::vector<T>
{
    public:

        /// Deletes the element in the array specified by the given index.
        void Delete(uint _uiElement)
        {
            this->erase( this->begin() + _uiElement);
        }
};


// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
