// -----------------------------------------------------------------------------
/*! \class CFECinematic
 *  \brief A class to manage scripts.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFECinematic.h"
#include "game/script/CFEScript.h"
// -----------------------------------------------------------------------------
CFECinematicPlane::~CFECinematicPlane()
{
	if (m_poScript != NULL)
		delete m_poScript;
}
// -----------------------------------------------------------------------------
CFECinematic::CFECinematic()
{
}
// -----------------------------------------------------------------------------
CFECinematic::~CFECinematic()
{
	// delete the planes
	for (uint i=0;i<m_oPlanes.size();i++)
		delete m_oPlanes[i];

	m_oPlanes.clear();
}
// -----------------------------------------------------------------------------
