//-----------------------------------------------------------------------------
/*! \class CCamera
 *  \brief The class that manages the game camera.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CCameraH
#define CCameraH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CEntity.h"
//-----------------------------------------------------------------------------
class CCamera : public CEntity
{
	public:

		CCamera();
		~CCamera();

		/// Initializes the camera entity.
		void Init(CFERenderer* _poRnd);

		/// Spawns the camera entity.
		virtual void Spawn(const CFEVect2& _oPos);

		// 
		virtual void Reset();

		// 
		virtual void Update(FEReal _rDeltaT);

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData);

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}
	
		/// Locks the camera to the current position. The camera will not move following the player.
		void Lock()
		{
			m_bLocked = true;
		}

		/// Unlocks the camera from the current position. The camera will move following the player.
		void Unlock()
		{
			m_bLocked = false;
		}

		/// Unlocks the camera from the current position. The camera will move following the player.
		bool bLocked()
		{
			return( m_bLocked );
		}


	protected:
        
        bool            m_bLocked;		
		CFERenderer*	m_poRnd;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
