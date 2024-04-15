// ----------------------------------------------------------------------------
/*! \class CFEMeshRenderer
 *  \brief Class to render Mesh instances.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMeshRendererH
#define CFEMeshRendererH
// ----------------------------------------------------------------------------
class CFEMeshInst;
class CFEMeshAction;
class CFERenderer;
// ----------------------------------------------------------------------------
class CFEMeshRenderer
{
    public:
            /// Renders a given Mesh instance with the specified features.
            static void Render(CFERenderer *_poRenderer,CFEMeshInst* _poInstance,const CFEVect2& _oPos,FEReal _rDepth,const CFEVect2& _oScale,FEReal _rAngle,const CFEColor& _oColor);

};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
