// -----------------------------------------------------------------------------
/*! \class CGameMsgMgr
 *  \brief A message facility to ease communication between entities and the game modes
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CGameMsgMgrH
#define CGameMsgMgrH
// -----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CEntity.h"
// -----------------------------------------------------------------------------
class CGameMessage
{
	public:

    CGameMessage() : m_uiMsgID(0), m_poEnt(NULL), m_pData(NULL)
    {
    }
    
    CGameMessage(uint _uiMsgID,CEntity* _poEnt,FEPointer _pData)    
    {
		m_uiMsgID = _uiMsgID;
        m_poEnt = _poEnt;
        m_pData = _pData;
	}

    uint			m_uiMsgID;
	CEntity*	m_poEnt;
    FEPointer		m_pData;
};
// -----------------------------------------------------------------------------
class CGameMsgMgr
{
	public:

		/// Sends a message from the source entity to the receiver.
		static void SendMsg(uint _uiMsgID,CEntity* _poSrcEntity,FEPointer _pData = NULL);

		/// Pick a message from the stack.
		static CGameMessage oPeekMessage();

		/// Retrieve the top most message of the stack, but do not take it out.
		static CGameMessage& oWatchMessage();

		/// Whether there are pending messages in the stack or not
		static bool bPendingMessages();

	protected:

		static CFEArray<CGameMessage> m_oMsgStack;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
