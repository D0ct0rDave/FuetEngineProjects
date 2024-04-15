// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief Enums shared among the FuetEngine and the application
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
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
/// Retrieves the position relative to the given layer, taking into account loop flags and parallax factor.
CFEVect2 CFEMap::oGetRelLayerPos(uint _uiLayerIdx,const CFEVect2& _oPos)
{
	if (_uiLayerIdx >= m_poLayers.size()) return(CFEVect2::ZERO());
	CFEMapLayer* poLayer = m_poLayers[_uiLayerIdx];
	if (poLayer == NULL) return(CFEVect2::ZERO());

    CFEVect2 oRelPos = (poLayer->m_oParallaxFact * _oPos) /* + poLayer->m_oSpeed * GlobalTime */ ;
    
    // Take into account loop flags	    
    // process X
    if ((poLayer->m_uiLoopFlags & MAPLAYER_LOOP_X) && (oRelPos.x>=poLayer->m_rLoopStartX))
    {
		//
		FEReal rRange   = poLayer->m_rLoopEndX - poLayer->m_rLoopStartX;
		oRelPos.x -= poLayer->m_rLoopStartX;
		oRelPos.x  = poLayer->m_rLoopStartX + CFEMath::rMod(oRelPos.x,rRange); 
    }

    // process Y
    if ((poLayer->m_uiLoopFlags & MAPLAYER_LOOP_Y) && (oRelPos.y>=poLayer->m_rLoopStartY))
    {
		//
		FEReal rRange   = poLayer->m_rLoopEndY - poLayer->m_rLoopStartY;
		oRelPos.y -= poLayer->m_rLoopStartY;
		oRelPos.y  = poLayer->m_rLoopStartY + CFEMath::rMod(oRelPos.y,rRange); 
    }

    return(oRelPos);
}
// ----------------------------------------------------------------------------
