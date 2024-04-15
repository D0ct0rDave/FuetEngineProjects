// ----------------------------------------------------------------------------
/*! \class CFEConfigFile
 *  \brief A class for generic configuration settings reading
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDElement.h"
#include "CFEHUDAction.h"
#include "CFEHUDObject.h"
//-----------------------------------------------------------------------------
CFEHUDElement::~CFEHUDElement()
{
	for (uint l=0;l<m_oLayers.size();l++)
		delete m_oLayers[l];

	m_oLayers.clear();

	for (uint a=0;a<m_oActions.size();a++)
		delete m_oActions[a];

	m_oActions.clear();
}
//-----------------------------------------------------------------------------
