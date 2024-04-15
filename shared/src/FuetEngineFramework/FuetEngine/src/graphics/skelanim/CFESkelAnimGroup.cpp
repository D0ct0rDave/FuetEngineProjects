// ----------------------------------------------------------------------------
/*! \class CFESkelAnimGroup
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimGroup.h"
//-----------------------------------------------------------------------------
CFESkelAnimGroup::~CFESkelAnimGroup()
{
	for (uint i=0;i<m_oChild.size();i++)
		delete m_oChild[i];
}
//-----------------------------------------------------------------------------
