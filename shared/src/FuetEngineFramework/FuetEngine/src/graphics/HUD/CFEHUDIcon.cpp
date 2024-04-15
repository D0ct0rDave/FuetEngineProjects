// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDIcon.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
//-----------------------------------------------------------------------------
CFEHUDIcon::~CFEHUDIcon()
{
	if (m_hSpriteInst != NULL)
		CFESpriteInstMgr::I()->ReleaseInstance(m_hSpriteInst);
}
//-----------------------------------------------------------------------------
