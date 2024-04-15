// ----------------------------------------------------------------------------
/*! \class CFEHUDActionTime
 *  \brief Helper class to retrieve the time of an element action.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "support/misc/CFEStringUtils.h"
#include "graphics/font/CFEFontMgr.h"
#include "graphics/sprite/CFESpriteInstMgr.h"
#include "types/CFEKFBFuncUtils.h"

#include "CFEHUDLoader.h"
#include "CFEHUDAction.h"
#include "CFEHUDElement.h"
#include "CFEHUDObject.h"
#include "CFEHUDActionTime.h"
//-----------------------------------------------------------------------------
#define GENERAL_ACTION_TIME_CHECK()\
		if ((eWrapMode != KFBFWM_LOOP) && (eWrapMode != KFBFWM_PINGPONG))\
		{\
			 if ((rTime>=_0r)&& (rActionTime < rTime))\
				rActionTime = rTime;\
		}\
		else\
		{\
			return(-_1r);\
		}

FEReal CFEHUDActionTime::rGetActionTime(CFEHUDElementAction* _poAction)
{
	if (_poAction->uiNumActions() == 0) return(_0r);
	FEReal rActionTime = -_1r;

    for (uint i=0;i<_poAction->uiNumActions();i++)
    {
		CFEHUDObjectAction* poObjAction = _poAction->poGetAction(i);
		EFEKFBFuncWrapMode eWrapMode;
		FEReal rTime;

		eWrapMode = poObjAction->m_rXFunc.eGetWrapMode();
		rTime = poObjAction->m_rXFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_rYFunc.eGetWrapMode();
		rTime = poObjAction->m_rYFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_oScaleFunc.eGetWrapMode();
		rTime = poObjAction->m_oScaleFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_rAngleFunc.eGetWrapMode();
		rTime = poObjAction->m_rAngleFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_rDepthFunc.eGetWrapMode();
		rTime = poObjAction->m_rDepthFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_oColorFunc.eGetWrapMode();
		rTime = poObjAction->m_oColorFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_iActionFunc.eGetWrapMode();
		rTime = poObjAction->m_iActionFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_bVisFunc.eGetWrapMode();
		rTime = poObjAction->m_bVisFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();

		eWrapMode = poObjAction->m_oEventFunc.eGetWrapMode();
		rTime = poObjAction->m_oEventFunc.rGetMaxLimit();
		GENERAL_ACTION_TIME_CHECK();
	}

    return(rActionTime);
}
//-----------------------------------------------------------------------------
FEReal CFEHUDActionTime::rGetMaxActionTime(CFEHUDElementAction* _poAction)
{
	if (_poAction->uiNumActions() == 0) return(_0r);

	FEReal rMaxActionTime = -_1r;
    for (uint i=0;i<_poAction->uiNumActions();i++)
    {
		CFEHUDObjectAction* poObjAction = _poAction->poGetAction(i);
		FEReal rMaxTime;

		rMaxTime = poObjAction->m_rXFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_rYFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_oScaleFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_oColorFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_rAngleFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_rDepthFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_iActionFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_bVisFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;

		rMaxTime = poObjAction->m_oEventFunc.rGetMaxLimit();
		if ((rMaxTime>=_0r) && (rMaxActionTime < rMaxTime)) rMaxActionTime = rMaxTime;
    }

    return(rMaxActionTime);
}
//-----------------------------------------------------------------------------
