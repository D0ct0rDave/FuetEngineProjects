// ----------------------------------------------------------------------------
/*! \class CFEScreenUtils
 *  \brief Class to perform common screen utilities.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEScreenUtilsH
#define CFEScreenUtilsH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "Types/CFEColor.h"
// ----------------------------------------------------------------------------
class CFERenderer;
// ----------------------------------------------------------------------------
class CFEScreenUtils
{
    public:

        /// Renders a full screen quad using the given material.
        static void FSRender(CFERenderer* _poRenderer,FEHandler _hMat,FEReal _rDepth = _0r);

        /// Renders a full screen quad using the given material and color.
        static void FSRender(CFERenderer* _poRenderer,FEHandler _hMat,const CFEColor& _oColor,FEReal _rDepth = _0r);

        /// Renders a full screen quad using the given material, color and separate alpha value.
        static void FSRender(CFERenderer* _poRenderer,FEHandler _hMat,const CFEColor& _oColor,FEReal _rAlpha,FEReal _rDepth);
};
// ----------------------------------------------------------------------------
#endif // CFEScreenUtilsH
// ----------------------------------------------------------------------------

