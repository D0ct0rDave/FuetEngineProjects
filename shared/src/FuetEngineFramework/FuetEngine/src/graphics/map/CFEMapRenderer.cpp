// ----------------------------------------------------------------------------
/*! \class CFEMapRenderer
 *  \brief class to renders FuetEngine maps.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMap.h"
#include "CFEMapRenderer.h"
#include "Support/Math/CFEMathIntersect.h"
#include "Graphics/Sprite/CFESpriteInstMgr.h"
#include "Graphics/Sprite/CFESpriteRenderer.h"
#include "Support/Graphics/CFERenderer.h"
// ----------------------------------------------------------------------------
void CFEMapRenderer::Render(CFEMap *_poMap,CFERenderer *_poRenderer)
{	
	CFEVect2 oCamPos = _poRenderer->oGetCameraTranslation();
	CFEVect2 oCamArea;
	oCamArea.x = _poRenderer->uiGetScreenVWidth() / _poRenderer->rGetCameraZoom();
	oCamArea.y = _poRenderer->uiGetScreenVHeight() / _poRenderer->rGetCameraZoom();
    FEReal rDeltaT = _poRenderer->rGetDeltaT();

	    for (uint uiLayer=0;uiLayer<_poMap->m_poLayers.size();uiLayer++)
	    {
	        CFEMapLayer* poLayer = _poMap->m_poLayers[uiLayer];
		    if ((poLayer==NULL) || (! poLayer->m_bVisible)) continue;

			// #pragma message(__FUNCTION__"/"__FILE__)
        	#pragma message("#################")
			#pragma message("Implement automatic movement of a map layer.")
			#pragma message("#################")

			// Camera position after parallax effect is applied
		    CFEVect2 oLayerCamPos = _poMap->oGetRelLayerPos(uiLayer,oCamPos); 


			// process the layer as seen by the layer camera.    
		    _poRenderer->TranslateCamera(oLayerCamPos.x,oLayerCamPos.y);
			for (uint uiSector=0;uiSector<poLayer->m_poSectors.size();uiSector++)
			{
				CFEMapSector* poSector = poLayer->m_poSectors[uiSector];
				if ((poSector==NULL) /*|| (! poSector->m_bVisible)*/) continue;
				
				// check if the sector is visible from the current camera position
				#if 1

					CFERect oViewRect;
					oViewRect.m_oIni.x = oLayerCamPos.x + _poRenderer->oGetViewTranslation().x;
					oViewRect.m_oIni.y = oLayerCamPos.y + _poRenderer->oGetViewTranslation().y;
					oViewRect.m_oEnd.x = oViewRect.m_oIni.x + oCamArea.x;
					oViewRect.m_oEnd.y = oViewRect.m_oIni.y + oCamArea.y;
					
					if (CFEMath::bOverlap(oViewRect,poSector->m_oBV))
					{
						for (uint uiElem=0;uiElem < poSector->m_oElements.size();uiElem++)
						{
							CFEMapElement* poElem = &poSector->m_oElements[uiElem];
							if (! poElem->m_bVisible) continue;
							
							CFEColor oColor;
							oColor = poElem->m_oColor * poLayer->m_oColor;

							CFESpriteInstMgr::I()->Update(poElem->m_hSprInst,rDeltaT);
							CFESpriteRenderer::Render(_poRenderer,(CFESpriteInst*)poElem->m_hSprInst,poElem->m_oPos,poLayer->m_rDepth + (0.00999f - poElem->m_rDepth),poElem->m_oScale,poElem->m_rAngle,oColor);
						}
					}

				#else

					CFERect oCamRect;
					oCamRect.m_oIni.x = oLayerCamPos.x - oCamArea.x;;
					oCamRect.m_oIni.y = oLayerCamPos.y - oCamArea.y;
					oCamRect.m_oEnd.x = oLayerCamPos.x + 2.0*oCamArea.x;
					oCamRect.m_oEnd.y = oLayerCamPos.y + 2.0*oCamArea.y;

					for (uint uiElem=0;uiElem < poSector->m_oElements.size();uiElem++)
					{
						CFEMapElement* poElem = &poSector->m_oElements[uiElem];
						if (! poElem->m_bVisible) continue;
						
						if ( 
								CFEMath::bBetween(oCamRect.m_oIni.x,oCamRect.m_oEnd.x,poElem->m_oPos.x)
						     && CFEMath::bBetween(oCamRect.m_oIni.y,oCamRect.m_oEnd.y,poElem->m_oPos.y)
								)
							CFESpriteRenderer::Render(_poRenderer,(CFESpriteInst*)poElem->m_hSprInst,poElem->m_oPos,poElem->m_oScale,poElem->m_rAngle,poElem->m_oColor);
					}

				#endif
			}
	    }

	_poRenderer->TranslateCamera(oCamPos.x,oCamPos.y);
}
// ----------------------------------------------------------------------------
