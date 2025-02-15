//-----------------------------------------------------------------------------
/*! \class CPlayer
 *  \brief The class that handles all the player procedures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CPlayerH
#define CPlayerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/CKFRMActor.h"
#include "Game/Helpers/CharacterActions.h"
#include "Game/Entity/EntityAttacks.h"
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
	PS_DEAD		= 8,	
	PS_JUMP		= 9
};
//-----------------------------------------------------------------------------
class CPlayer : public CKFRMActor
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
		virtual void OnExitState(uint _uiState, uint _uiNewState);

		/// Retrieves whether the player is currently attacking or not
		bool bAttacking()
		{
			return( uiGetState() == PS_ATTACK || (uiGetState() == PS_JUMP && m_bJumpAttack));
		}

		/// Retrieves whether the player is currently dead or not
		bool bDead()
		{
			return( uiGetState() == PS_DEAD );
		}

		/// Retrieves whether the player is currently crouch or not, even if attacking.
		bool bCrouch();
		bool bJumping();

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

		/// This function is implemented for convenience. Just to launch animations.
		/// a different kind of player can override this functuion to launch other
		/// animations different from the standar ones. For instance, a bonus player.
		virtual void PlayAction(ECharacterAction _eAction);

		// Returns bad objects taked
		uint uiGetBadObjects() { return m_uiBonusBad; };

protected:
		void Attack();
		
		CFEInput*	    m_poInput;		
		FEReal		    m_rSpeed;

		FEReal		    m_rLeftLimit;
		FEReal		    m_rRightLimit;	

		bool		    m_bJumpAttack;
		FEReal		    m_rJumpSpeed;
		FEReal		    m_rJumpHeight;
		FEReal		    m_rJumpInertia;
		bool		    m_bGoingUp;

		FEReal		    m_rTimerInertia;

		// Bonus
		uint		    m_uiBonusLife;
		uint			m_uiBonusOk;
		uint			m_uiBonusBad;

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
