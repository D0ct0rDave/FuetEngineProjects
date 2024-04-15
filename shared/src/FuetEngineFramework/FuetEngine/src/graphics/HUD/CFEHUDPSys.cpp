// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M�rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M�rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDPSys.h"
#include "core/CFECore.h"

#include "graphics/particlesys/CFEParticleSysInstMgr.h"
//-----------------------------------------------------------------------------
CFEHUDPSys::~CFEHUDPSys()
{
	if (m_hPSysInst != NULL)
	{
		CFEParticleSysInstMgr::I()->ReleaseInstance(m_hPSysInst);
	}
}
//-----------------------------------------------------------------------------
