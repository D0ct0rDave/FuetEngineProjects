// -----------------------------------------------------------------------------
/*! \class
 *  \brief
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CGameMsgMgr.h"
// -----------------------------------------------------------------------------
CFEArray<CGameMessage> CGameMsgMgr::m_oMsgStack;
// -----------------------------------------------------------------------------
void CGameMsgMgr::SendMsg(unsigned int _uiMsgID,CEntity* _poSrcEntity,FEPointer _pData)
{
	CGameMessage oMsg(_uiMsgID,_poSrcEntity,_pData);
	m_oMsgStack.push_back(oMsg);
}
// -----------------------------------------------------------------------------
CGameMessage CGameMsgMgr::oPeekMessage()
{
	CGameMessage oMsg = m_oMsgStack[0];
	m_oMsgStack.Delete(0);

	return(oMsg);
}
// -----------------------------------------------------------------------------
CGameMessage& CGameMsgMgr::oWatchMessage()
{
	return (m_oMsgStack[0]);
}
// -----------------------------------------------------------------------------
bool CGameMsgMgr::bPendingMessages()
{
	return (m_oMsgStack.size() > 0);
}
// -----------------------------------------------------------------------------
