// ----------------------------------------------------------------------------
/*! \class CFEHUDInstMgrLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDInstMgr.h"
#include "CFEHUDLoader.h"
#include "CFEHUDInstancer.h"
#include "CFEHUDUpdater.h"
#include "CFEHUDRenderer.h"

#include "CFEHUD.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"

#include "CFEHUDElemLocator.h"
#include "CFEHUDRectGen.h"
#include "CFEHUDMaxDepthGetter.h"

#include "types/CFESingleton.h"
#include "types/CFERect.h"

#include "Support/Math/CFEMathIntersect.h"
#include "Support/Graphics/CFERenderer.h"
//-----------------------------------------------------------------------------
static FEReal m_rLastDeltaT = _0r;
//-----------------------------------------------------------------------------
/*
DECLARE_SINGLETON(__FILE__DATA)
{
    public:
        CFEArray<FEHandler> m_oInstances;
};
#define INSTANCEDDATA __FILE__DATA::I()
*/
class CFEHUDMgrInst
{
	public:
		CFEHUD*				m_poHUD;
		CFEArray<CFERect>	m_poElemRects;
};
//-----------------------------------------------------------------------------
FEHandler CFEHUDInstMgr::hGetInstance(const CFEString& _sHUDModel)
{
	/*
    CFEHUD* poHUD = CFEHUDLoader::poLoad(_sHUDModel);

    CFEHUDInstancer oHUDInstancer;
    CFEHUD* poHUDInstance = oHUDInstancer.poCreateInstance(poHUD);

    // INSTANCEDDATA->m_oInstances.push_back(poHUDInstance);
    
    return ((FEHandler)poHUDInstance);
    */

	CFEHUD* poHUD = CFEHUDLoader::poLoad(_sHUDModel);
	if (poHUD == NULL) return(NULL);

	return( hGetInstance(poHUD) );
}
//-----------------------------------------------------------------------------
FEHandler CFEHUDInstMgr::hGetInstance(CFEHUD* _poHUD)
{
	CFEHUDMgrInst* poInst = new CFEHUDMgrInst;
	poInst->m_poHUD = _poHUD;

	for (uint i=0;i<_poHUD->uiNumElements();i++)
	{
		CFERect oR = CFEHUDRectGen::oGetRect(NULL, _poHUD->poGetElement(i)->poGetLayer(0) );
		FEReal  rMaxDepth = CFEHUDMaxDepthGetter::rGetMaxDepth( _poHUD->poGetElement(i)->poGetLayer(0) );
		if (rMaxDepth > _1r)
		{
			FEReal rXExtent = oR.rWidth()*_05r*rMaxDepth;
			FEReal rYExtent = oR.rHeight()*_05r*rMaxDepth;

			oR.m_oIni.x -= rXExtent;
			oR.m_oIni.y -= rYExtent;
			oR.m_oEnd.x += rXExtent;
			oR.m_oEnd.y += rYExtent;
		}

		poInst->m_poElemRects.push_back(oR);
	}

    return ( (FEHandler)poInst );
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::ReleaseInstance(FEHandler _hHUDInstance)
{
	if (_hHUDInstance == NULL) return;

	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;
	delete poInst->m_poHUD;
	poInst->m_poHUD = NULL;
	delete poInst;
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::Update(FEReal _rDeltaT)
{
    /*
    for (uint i=0;i<INSTANCEDDATA->m_oInstances.size();i++)
        CFEHUDUpdater::Update(INSTANCEDDATA->m_oInstances[i],_rDeltaT);
	*/
	m_rLastDeltaT = _rDeltaT;
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::Update(FEHandler _hHUDInstance,FEReal _rDeltaT)
{
	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;
	CFEHUDUpdater::Update(poInst->m_poHUD,_rDeltaT);
	m_rLastDeltaT = _rDeltaT;
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer)
{
	if (_hHUDInstance == NULL) return;

	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;

	CFEVect2 oCamPos = _poRenderer->oGetCameraTranslation();

	CFEVect2 oViewportArea;
	oViewportArea.x = _poRenderer->uiGetScreenVWidth() / _poRenderer->rGetCameraZoom();
	oViewportArea.y = _poRenderer->uiGetScreenVHeight() / _poRenderer->rGetCameraZoom();

	// MARGIN prevents rects touching the viewport to be rendered
	const FEReal MARGIN = _0001r;

	CFERect oViewRect;
	oViewRect.m_oIni.x = _poRenderer->oGetViewTranslation().x + MARGIN;
	oViewRect.m_oIni.y = _poRenderer->oGetViewTranslation().y + MARGIN;
	oViewRect.m_oEnd.x = oViewRect.m_oIni.x + oViewportArea.x - MARGIN;
	oViewRect.m_oEnd.y = oViewRect.m_oIni.y + oViewportArea.y - MARGIN;

	oViewRect.m_oIni += oCamPos; 
	oViewRect.m_oEnd += oCamPos;

	for (uint e=0;e<poInst->m_poHUD->uiNumElements();e++)
	{
	    CFEHUDElement* poElem = poInst->m_poHUD->poGetElement(e);
		if ((poElem==NULL) || (poElem->uiNumLayers() == 0) || (! poElem->poGetLayer(0)->bIsVisible()) ) continue;

		// check if the element is visible from the current camera position
		if (CFEMath::bOverlap(oViewRect,poInst->m_poElemRects[e]))
			CFEHUDRenderer::Render(poElem,_poRenderer,m_rLastDeltaT);	
	}
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDInstMgr::poGetElementAction(FEHandler _hHUDInstance,const CFEString& _sActionName)
{
	if (_hHUDInstance == NULL) return(NULL);

	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;
	CFEHUDElemLocator oElemLocator;
	return( oElemLocator.poLocateElemAction(poInst->m_poHUD,_sActionName) );
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDInstMgr::poGetObject(FEHandler _hHUDInstance,const CFEString& _sObjectName)
{
	if (_hHUDInstance == NULL) return(NULL);

	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;
	CFEHUDElemLocator oElemLocator;

	return( oElemLocator.poLocateHUDObject(poInst->m_poHUD,_sObjectName) );
}
//-----------------------------------------------------------------------------
CFEHUD* CFEHUDInstMgr::poGetHUD(FEHandler _hHUDInstance)
{
	if (_hHUDInstance == NULL) return(NULL);

	CFEHUDMgrInst* poInst = (CFEHUDMgrInst*)_hHUDInstance;
	return (poInst->m_poHUD);
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::SetActionTime(CFEHUDElementAction* _poAction,FEReal _rActionTime)
{
	if (_rActionTime<_0r) return;
	CFEHUDUpdater::Process(_poAction,_rActionTime);
}
//-----------------------------------------------------------------------------

