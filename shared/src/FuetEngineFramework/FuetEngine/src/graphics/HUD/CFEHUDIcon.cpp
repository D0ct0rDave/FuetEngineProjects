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
CFEHUDIcon::~CFEHUDIcon()
{
	if (m_hSpriteInst != NULL)
		CFESpriteInstMgr::ReleaseInstance(m_hSpriteInst);
}
//-----------------------------------------------------------------------------
void CFEHUDIcon::SetCurAction(int _iAction)
{
	if ((_iAction != -1) && (_iAction != m_iCurAction))
	{
		m_iCurAction = _iAction;
		CFESpriteInstMgr::SetAction(m_hSpriteInst,_iAction);
	}
}
//-----------------------------------------------------------------------------
