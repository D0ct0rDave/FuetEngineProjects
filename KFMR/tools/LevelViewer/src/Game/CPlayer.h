//-----------------------------------------------------------------------------
/*! \class CPlayer
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CPlayerH
#define CPlayerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"
#include "Game/CActor.h"
//-----------------------------------------------------------------------------
typedef enum EPlayerStates
{
	PS_NONE		= 0,
	PS_SPAWNING = 1,
	PS_IDLE		= 2,
	PS_WALK		= 3,
	PS_ATTACK	= 4,	
	PS_CROUCH	= 5,
	PS_PAIN		= 6,
	PS_DYING	= 7,	
	PS_DEAD		= 8
};
//-----------------------------------------------------------------------------
class CPlayer : public CActor
{
	public:

		CPlayer();
		~CPlayer();

		/// Initializes the player.
		virtual void Init(CFEInput* _poInput,const CFEVect2& _oPos);

		/// Spawns the player in the map in left or right direction.
		virtual void Spawn(const CFEVect2& _oPos,bool _bLeft);

		/// Spawns the player in the map in left or right direction.
		virtual void Spawn(const CFEVect2& _oPos)
		{
			Spawn(_oPos,false);
		}

		/// Think function in case it's needed
		virtual void Update(FEReal _rDeltaT);

		/// Specific code to perform when the entity enters to a given state.
		virtual void OnEnterState(uint _uiState);

		/// Specific code to perform when the entity exits from a given state.
		virtual void OnExitState(uint _uiState);

		/// Retrieves whether the player is currently attacking or not
		bool bAttacking()
		{
			return( uiGetState() == PS_ATTACK );
		}

		/// Retrieves whether the player is currently dead or not
		bool bDead()
		{
			return( uiGetState() == PS_DEAD );
		}

		/// Retrieves whether the player is currently crouch or not, even if attacking.
		bool bCrouch();

        /// If someone wants to communicate with this entity...
        virtual void ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData);

		/// Retrieves the direction the player is facing to...
		FEReal rGetDir()
		{
			return(m_rDirMult);
		}
	
		/// Sets the limits which the player can move.
		void SetActionLimits(FEReal _rLeftLimit,FEReal _rRightLimit)
		{
			m_rLeftLimit =  _rLeftLimit;
			m_rRightLimit = _rRightLimit;
		}

		/// Resets the entity to a given initial state		
		virtual void Reset();

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}


protected:
		void Attack();
		
		CFEInput*	m_poInput;		
		FEReal		m_rSpeed;
		FEReal		m_rDamage;

		FEReal		m_rLeftLimit;
		FEReal		m_rRightLimit;		
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
