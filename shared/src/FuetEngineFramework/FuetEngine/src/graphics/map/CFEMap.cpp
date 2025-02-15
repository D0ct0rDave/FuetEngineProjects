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
// ----------------------------------------------------------------------------
CFEMapSector::~CFEMapSector()
{
	m_oElements.clear();
}
// ----------------------------------------------------------------------------
CFEMapLayer::~CFEMapLayer()
{
	for (uint i=0;i<m_poSectors.size();i++)
	{
		delete m_poSectors[i];
	}

	m_poSectors.clear();
}
// ----------------------------------------------------------------------------
CFEMap::~CFEMap()
{
	for (uint i=0;i<m_poLayers.size();i++)
	{
		delete m_poLayers[i];
	}

	m_poLayers.clear();
}
// ----------------------------------------------------------------------------
/// Retrieves the layer associated with the given name.
CFEMapLayer* CFEMap::poGetLayer(const CFEString& _sLayerName)
{
	for (uint uiLayer=0;uiLayer<m_poLayers.size();uiLayer++)
		if (m_poLayers[uiLayer]->sGetName() |= _sLayerName)
			return(m_poLayers[uiLayer]);
	
	return(NULL);
}
// ----------------------------------------------------------------------------