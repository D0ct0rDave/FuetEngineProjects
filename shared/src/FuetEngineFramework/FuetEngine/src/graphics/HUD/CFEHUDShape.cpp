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
#include "CFEHUDShape.h"
#include "graphics/mesh/CFEMeshInstMgr.h"
//-----------------------------------------------------------------------------
CFEHUDShape::~CFEHUDShape()
{
	if (m_hMeshInst != NULL)
	{
		CFEMeshInstMgr::I()->ReleaseInstance(m_hMeshInst);
	}
}
//-----------------------------------------------------------------------------
