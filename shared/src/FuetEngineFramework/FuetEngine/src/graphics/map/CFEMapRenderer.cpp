// ----------------------------------------------------------------------------
/*! \class CFEMapRenderer
 *  \brief class to renders FuetEngine maps.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEMap.h"
#include "CFEMapRenderer.h"
#include "Support/Math/CFEMathIntersect.h"
#include "Graphics/Sprite/CFESpriteRenderer.h"
#include "Support/Graphics/CFERenderer.h"
// ----------------------------------------------------------------------------
static CFEVect2 gsoCamPos;
static FEReal	gsrCamZoom;
// ----------------------------------------------------------------------------
void CFEMapRenderer::SetCameraPos(FEReal _rX,FEReal _rY)
{
	gsoCamPos.x = _rX;
	gsoCamPos.y = _rY;
}
// ----------------------------------------------------------------------------
void CFEMapRenderer::SetZoom(FEReal _rZoom)
{
	gsrCamZoom = _rZoom;
}
// ----------------------------------------------------------------------------
void CFEMapRenderer::Render(CFEMap *_poMap,CFERenderer *_poRenderer)
{
	CFEVect2 oCamPos = _poRenderer->oGetCameraTranslation();
	CFEVect2 oCamArea;
	oCamArea.x = _poRenderer->uiGetScreenVWidth() / _poRenderer->rGetCameraZoom();
	oCamArea.y = _poRenderer->uiGetScreenVHeight() / _poRenderer->rGetCameraZoom();

	    for (uint uiLayer=0;uiLayer<_poMap->m_poLayers.size();uiLayer++)
	    {
	        CFEMapLayer* poLayer = _poMap->m_poLayers[uiLayer];
		    if ((poLayer==NULL) || (! poLayer->m_bVisible)) continue;

			#pragma message(__FUNCTION__"/"__FILE__)
        	#pragma message("#################")
			#pragma message("Implement automatic movement of a map layer.")
			#pragma message("#################")

		    CFEVect2 oLayerCamPos = (poLayer->m_oParallaxFact * oCamPos) /* + poLayer->m_oSpeed * GlobalTime */ ;
		    _poRenderer->TranslateCamera(oLayerCamPos.x,oLayerCamPos.y);

			for (uint uiSector=0;uiSector<poLayer->m_poSectors.size();uiSector++)
			{
				CFEMapSector* poSector = poLayer->m_poSectors[uiSector];
				if ((poSector==NULL) /*|| (! poSector->m_bVisible)*/) continue;
				
				// check if the sector is visible from the current camera position
				#if 1

					CFERect oCamRect;
					oCamRect.m_oIni.x = oLayerCamPos.x;
					oCamRect.m_oIni.y = oLayerCamPos.y;
					oCamRect.m_oEnd.x = oLayerCamPos.x + oCamArea.x;
					oCamRect.m_oEnd.y = oLayerCamPos.y + oCamArea.y;
					
					if (CFEMath::bOverlap(oCamRect,poSector->m_oBV))
					{
						for (uint uiElem=0;uiElem < poSector->m_oElements.size();uiElem++)
						{
							CFEMapElement* poElem = &poSector->m_oElements[uiElem];
							// if (! poElem->m_bVisible) continue;

							CFESpriteRenderer::Render(_poRenderer,(CFESpriteInst*)poElem->m_hSprInst,poElem->m_oPos,poLayer->m_rDepth,poElem->m_oScale,poElem->m_rAngle,poElem->m_oColor);
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