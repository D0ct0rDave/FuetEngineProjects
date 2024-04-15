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
#include "CFEHUDGroup.h"
// ----------------------------------------------------------------------------
CFEHUDGroup::~CFEHUDGroup()
{
	for (uint o=0;o<m_oObjs.size();o++)
		delete m_oObjs[o];

	m_oObjs.clear();
}
//-----------------------------------------------------------------------------
