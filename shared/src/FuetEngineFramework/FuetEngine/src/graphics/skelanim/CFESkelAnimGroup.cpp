// ----------------------------------------------------------------------------
/*! \class CFESkelAnimGroup
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimGroup.h"
//-----------------------------------------------------------------------------
CFESkelAnimGroup::~CFESkelAnimGroup()
{
	for (uint i=0;i<m_oNodes.size();i++)
		delete m_oNodes[i];
}
//-----------------------------------------------------------------------------
