// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMap.h"

/// Retrieves the layer associated with the given name.
CFEMapLayer* CFEMap::poGetLayer(const CFEString& _sLayerName)
{
	for (uint uiLayer=0;uiLayer<m_poLayers.size();uiLayer++)
		if (m_poLayers[uiLayer]->m_sName |= _sLayerName)
			return(m_poLayers[uiLayer]);
	
	return(NULL);
}
// ----------------------------------------------------------------------------