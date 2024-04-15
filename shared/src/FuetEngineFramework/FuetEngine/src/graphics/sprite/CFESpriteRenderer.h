// ----------------------------------------------------------------------------
/*! \class CFESpriteRenderer
 *  \brief Class to render sprite instances.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESpriteRendererH
#define CFESpriteRendererH
// ----------------------------------------------------------------------------
class CFESpriteInst;
class CFESpriteAction;
class CFERenderer;
// ----------------------------------------------------------------------------
class CFESpriteRenderer
{
    public:
            /// Renders a given sprite instance with the specified features.
            static void Render(CFERenderer *_poRenderer,CFESpriteInst* _poInstance,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor);
			
			/// Retrieves the geometry of the current frame of a given sprite instance.
			static void GetSpriteGeometry(CFESpriteInst* _poInstance,const CFEVect2& _oPos,const CFEVect2& _oScale,FEReal _rAngle,CFEVect2* _poVX);

};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
