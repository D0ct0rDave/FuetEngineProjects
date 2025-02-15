//-----------------------------------------------------------------------------
/*! \class CEntity
 *  \brief A class to derive from for entities with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEntityH
#define CEntityH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CStatedObject.h"
#include "CGameVisitor.h"
//-----------------------------------------------------------------------------
class CEntity : public CStatedObject
{
	public:

		CEntity();
		~CEntity();

		/// Respawns the entity at the given position.
		virtual void Spawn(const CFEVect2& _oPos) {};

		/// Think function in case it's needed.
		virtual void Update(FEReal _rDeltaT) {};

		/// Sets the position of the entity.
		virtual void SetPos(const CFEVect2& _oPos)
		{
			m_oPos = _oPos;
		}

		/// Retrieve the position of the entity.
		CFEVect2& oGetPos()
		{
			return(m_oPos);
		}

		/// Enables (or disables) the entity.
		void Enable(bool _bEnable = true)
		{
			m_bEnabled = _bEnable;
		}

		/// Enables (or disables) the entity.
		void Disable()
		{
			m_bEnabled = false;
		}

		/// Tells whether the entity is enabled or not.
		bool bEnabled()
		{
			return(m_bEnabled);
		}

		/// Resets the entity to a given initial state.
		virtual void Reset(){};
	
        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData) {};

		/// Accept procedure for processing functionality over entities
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	protected:
		FEReal		m_rStateTime;
		CFEVect2	m_oPos;
	
	private:

		bool		m_bEnabled;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
