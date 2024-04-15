// ----------------------------------------------------------------------------
/*! \class CFEMapRenderer
 *  \brief class to renders FuetEngine maps.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMapRendererH
#define CFEMapRendererH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
class CFEMap;
class CFERenderer;
// ----------------------------------------------------------------------------
class CFEMapRenderer
{
    public:

        /// Renders the given map using the provided renderer.
        static void Render(CFEMap *_poMap,CFERenderer *_poRenderer);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
