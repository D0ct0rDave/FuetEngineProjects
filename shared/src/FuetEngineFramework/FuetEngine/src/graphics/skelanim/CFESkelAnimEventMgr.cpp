// ----------------------------------------------------------------------------
/*! \class CFESkelAnimEventMgr
 *  \brief A class to manage the logic of event handling.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFESkelAnimEventMgr.h"
//-----------------------------------------------------------------------------
FESkelAnimEventHandler CFESkelAnimEventMgr::m_pHandlerFunc = NULL;
//-----------------------------------------------------------------------------
void CFESkelAnimEventMgr::TriggerEvent(const CFEString& _sEventName,const CFEString& _sEventValue,CFESkelAnimNode* _poNode,FEPointer _pParam)
{
	if (m_pHandlerFunc != NULL)
	{
		m_pHandlerFunc(_sEventName,_sEventValue,_poNode,_pParam);
	}
}
//-----------------------------------------------------------------------------
void CFESkelAnimEventMgr::RegisterEventHandler(FESkelAnimEventHandler _pHandlerFunc)
{
	m_pHandlerFunc = _pHandlerFunc;
}
//-----------------------------------------------------------------------------
