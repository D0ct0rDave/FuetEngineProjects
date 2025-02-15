// ----------------------------------------------------------------------------
/*! \class CFEMapRenderer
 *  \brief class to renders FuetEngine maps.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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

        /// Sets the current camera location in the map
        static void SetCameraPos(FEReal _rX,FEReal _rY);

        /// Sets the current camera zoom.
        static void SetZoom(FEReal _rZoom);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
