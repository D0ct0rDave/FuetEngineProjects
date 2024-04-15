// ----------------------------------------------------------------------------
/*! \class CFEFontLoader
 *  \brief A class to load CFEFonts
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEFontLoaderH
#define CFEFontLoaderH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEString.h"
#include "CFEFont.h"
// ----------------------------------------------------------------------------
class CFEFontLoader
{
    public:
            /// Loads an sprite from a given file    
            static CFEFont* poLoad(const CFEString& _sFilename);

    protected:

            /// Loads a font in a FNT file.
            static CFEFont* poLoadFNTFile(const CFEString& _sFilename, FEHandler _hMat);

            /// Loads a font in a FON file.
            static CFEFont* poLoadFONFile(const CFEString& _sFilename, FEHandler _hMat);

            /// Build a basic font around a given material.
            static CFEFont* poBuildBasicFont(FEHandler _hMat);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

