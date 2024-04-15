// ----------------------------------------------------------------------------
/*! \class CFESkelAnimSprite
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimSprite.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
CFESkelAnimSprite::~CFESkelAnimSprite()
{
	CFESpriteInstMgr::I()->ReleaseInstance(m_hSpriteInst);
}
//-----------------------------------------------------------------------------
void CFESkelAnimSprite::SetAction(uint _uiAction)
{
	CFESpriteInstMgr::I()->SetAction(m_hSpriteInst,_uiAction);
}
//-----------------------------------------------------------------------------
uint CFESkelAnimSprite::uiGetAction()
{
	return (CFESpriteInstMgr::I()->uiGetAction(m_hSpriteInst) );
}

//-----------------------------------------------------------------------------
