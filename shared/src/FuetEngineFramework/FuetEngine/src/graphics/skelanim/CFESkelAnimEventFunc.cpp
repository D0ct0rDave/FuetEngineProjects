// ----------------------------------------------------------------------------
/*! \class CFESkelAnimEventFunc
 *  \brief A class to store an event function.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimEventFunc.h"
#include "CFESkelAnimEventMgr.h"
//-----------------------------------------------------------------------------
void CFESkelAnimEventFunc::TriggerEvent(const CFEEvent& _oEvent)
{
	// CALL event handler
	if (m_pHandlerFunc==NULL)
		CFESkelAnimEventMgr::TriggerEvent(_oEvent.m_sEventName,_oEvent.m_sEventValue,m_poNode,m_pParam);
	else
		m_pHandlerFunc(_oEvent.m_sEventName,_oEvent.m_sEventValue,m_poNode,m_pParam);
}
//-----------------------------------------------------------------------------
#ifdef _DEBUG
static void test()
{
	CFESkelAnimEventFunc oEF;
}
#endif
//-----------------------------------------------------------------------------
