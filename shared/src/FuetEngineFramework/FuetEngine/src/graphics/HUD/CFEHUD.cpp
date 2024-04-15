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
#include "CFEHUD.h"
#include "CFEHUDElement.h"
//-----------------------------------------------------------------------------
CFEHUD::~CFEHUD()
{
	for (uint e=0;e<m_oElements.size();e++)
		delete m_oElements[e];

	m_oElements.clear();	
}
//-----------------------------------------------------------------------------
