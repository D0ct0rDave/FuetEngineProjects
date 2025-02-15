// -----------------------------------------------------------------------------
/*! \class CEntityMgr
 *  \brief A class to manage all the entities of the system.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CHUDActorNode.h"
// ----------------------------------------------------------------------------
void CHUDActorNode::Render(CFEHUDVisitor* _poVisitor)
{
    CFERenderer* poRenderer = ((CFEHUDRenderer*)_poVisitor)->poGetRenderer();
    
    /// Renders a given sprite instance with the specified features.
    CFEVect2 oPos = m_poNode->oGetPos() + oGetPos();
    FEReal rAngle = m_poNode->rGetAngle() + rGetAngle();
    CFEVect2 oScale = m_poNode->oGetScale() * oGetScale();
    CFEColor oColor = m_poNode->oGetColor() * oGetColor();
    
    CFESkelAnimRenderer::Render(poRenderer,m_poNode,oPos,_0r,oScale,rAngle,oColor);
}
//-----------------------------------------------------------------------------
