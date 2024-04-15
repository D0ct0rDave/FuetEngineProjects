// ----------------------------------------------------------------------------
/*! \class CFEEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFEHUDEventFunc.h"
#include "CFEHUDEventMgr.h"
//-----------------------------------------------------------------------------
void CFEHUDEventFunc::TriggerEvent(const CFEEvent& _oEvent)
{
	typedef void (*FEHUDEventHandler)(const CFEString& _sEventName,const CFEString& _sEventValue,CFEHUDObject* _poObj,FEPointer _pParam);

	// CALL event handler
	if (m_pHandlerFunc== NULL)
		CFEHUDEventMgr::TriggerEvent(_oEvent.m_sEventName,_oEvent.m_sEventValue,m_poObj,m_pParam);
	else
		m_pHandlerFunc(_oEvent.m_sEventName,_oEvent.m_sEventValue,m_poObj,m_pParam);
}
//-----------------------------------------------------------------------------
#ifdef _DEBUG
static void test()
{
	CFEHUDEventFunc oEF;
}
#endif
//-----------------------------------------------------------------------------
