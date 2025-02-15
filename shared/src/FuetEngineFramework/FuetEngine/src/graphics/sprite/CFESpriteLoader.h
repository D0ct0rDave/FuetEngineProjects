// ----------------------------------------------------------------------------
/*! \class CFESpriteLoader
 *  \brief A class to load sprites.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESpriteLoaderH
#define CFESpriteLoaderH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFESprite.h"
// ----------------------------------------------------------------------------
class CFESpriteLoader
{
    public:
            /// Loads an sprite from a given file
            static CFESprite* poLoad(const CFEString& _sFilename);

    protected:
            /// Build a basic sprite around a given material
            static CFESprite* poBuildBasicSprite(FEHandler _hTex,const CFEString& _sSpriteName);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
