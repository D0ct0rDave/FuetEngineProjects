// ----------------------------------------------------------------------------
/*! \class CFESkelAnimSprite
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimSprite.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
void CFESkelAnimSprite::SetAction(uint _uiAction)
{
	CFESpriteInstMgr::SetAction(m_hSpriteInst,_uiAction);
}
//-----------------------------------------------------------------------------
uint CFESkelAnimSprite::uiGetAction()
{
	return (CFESpriteInstMgr::uiGetAction(m_hSpriteInst) );
}

//-----------------------------------------------------------------------------
