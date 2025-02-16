// ----------------------------------------------------------------------------
/*! \class CFEHUDInstMgrLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
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
#include "CFEHUDElemLocator.h"
#include "types/CFESingleton.h"
//-----------------------------------------------------------------------------
DECLARE_SINGLETON(__FILE__DATA)
{
    public:
        CFEArray<FEHandler> m_oInstances;
};
#define INSTANCEDDATA __FILE__DATA::I()
//-----------------------------------------------------------------------------
FEHandler CFEHUDInstMgr::hGetInstance(const CFEString& _sHUDModel)
{
    CFEHUD* poHUD = CFEHUDLoader::poLoad(_sHUDModel);

    CFEHUDInstancer oHUDInstancer;
    CFEHUD* poHUDInstance = oHUDInstancer.poCreateInstance(poHUD);

    INSTANCEDDATA->m_oInstances.push_back(poHUDInstance);
    
    return ((FEHandler)poHUDInstance);
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::Update(FEReal _rDeltaT)
{
    for (uint i=0;i<INSTANCEDDATA->m_oInstances.size();i++)
        CFEHUDUpdater::Update(INSTANCEDDATA->m_oInstances[i],_rDeltaT);
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::Render(FEHandler _hHUDInstance,CFERenderer* _poRenderer)
{
    CFEHUDRenderer oRenderer;
    oRenderer.Render(_hHUDInstance,_poRenderer);
}
//-----------------------------------------------------------------------------
CFEHUDElementAction* CFEHUDInstMgr::poGetElementAction(FEHandler _hHUDInstance,const CFEString& _sActionName)
{
	CFEHUDElemLocator oElemLocator;
	return( oElemLocator.poLocateElemAction((CFEHUD*)_hHUDInstance,_sActionName) );

	/*
	CFEHUD* poHUD = (CFEHUD*)_hHUDInstance;	

	for (uint e=0;e<poHUD->uiNumElements();e++)
	{
		CFEHUDElement* poElem = poHUD->poGetElement(e);
		if ((poElem!=NULL) && (poElem->sGetName() |= _sElement))
		{
			for (uint a=0;a<poElem->uiNumActions();a++)
			{
				CFEHUDElementAction* poAction = poElem->poGetAction(a);

				if ((poAction !=NULL) && (poAction->sGetName() |= _sActionName))
					return(poAction);
			}

			return(NULL);
		}
	}

	return(NULL);
	*/
}
//-----------------------------------------------------------------------------
CFEHUDObject* CFEHUDInstMgr::poGetObject(FEHandler _hHUDInstance,const CFEString& _sObjectName)
{
	CFEHUDElemLocator oElemLocator;
	return( oElemLocator.poLocateHUDObject((CFEHUD*)_hHUDInstance,_sObjectName) );

	/*
	CFEHUD* poHUD = (CFEHUD*)_hHUDInstance;	

	for (uint e=0;e<poHUD->uiNumElements();e++)
	{
		CFEHUDElement* poElem = poHUD->poGetElement(e);
		if ((poElem!=NULL) && (poElem->sGetName() |= _sElement))
		{
			for (uint l=0;l<poElem->uiNumLayers();l++)
			{
				CFEHUDObject* poAction = poElem->poGetAction(l);

				if ((poAction !=NULL) && (poAction->sGetName() |= _sObjectName))
					return(poAction);
			}

			return(NULL);
		}
	}

	return(NULL);
	*/
}
//-----------------------------------------------------------------------------
void CFEHUDInstMgr::SetActionTime(CFEHUDElementAction* _poAction,FEReal _rActionTime)
{
	CFEHUDUpdater::SetTime(_rActionTime);
	CFEHUDUpdater::Process(_poAction);
}
//-----------------------------------------------------------------------------

