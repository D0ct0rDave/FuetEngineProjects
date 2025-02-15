// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimRendererH
#define CFESkelAnimRendererH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEColor.h"
//-----------------------------------------------------------------------------
class CFERenderer;
class CFESkelAnimNode;
//-----------------------------------------------------------------------------
class CFESkelAnimRenderer
{
	public:

        /// Renders a given sprite instance with the specified features.
        static void Render(CFERenderer *_poRenderer,CFESkelAnimNode* _poNode,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
