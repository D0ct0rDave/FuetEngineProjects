// ----------------------------------------------------------------------------
/*! \class CFEHUDEventMgr
 *  \brief A class to manage the logic of event handling.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDEventMgr.h"
//-----------------------------------------------------------------------------
FEHUDEventHandler CFEHUDEventMgr::m_pHandlerFunc = NULL;
//-----------------------------------------------------------------------------
void CFEHUDEventMgr::TriggerEvent(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj)
{
	if (m_pHandlerFunc != NULL)
	{
		m_pHandlerFunc(_sEventName,_sEventValue,_poObj);
	}
}
//-----------------------------------------------------------------------------
void CFEHUDEventMgr::RegisterEventHandler(FEHUDEventHandler _pHandlerFunc)
{
	m_pHandlerFunc = _pHandlerFunc;
}
//-----------------------------------------------------------------------------
