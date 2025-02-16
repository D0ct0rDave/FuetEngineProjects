// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDIcon.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
void CFEHUDIcon::SetAction(uint _uiAction)
{
	m_uiAction = _uiAction;
	CFESpriteInstMgr::SetAction(m_hSpriteInst,_uiAction);
}
//-----------------------------------------------------------------------------
