//-----------------------------------------------------------------------------
/*! \class CGameLoop
 *  \brief The Game Class all the game life and logic.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CGameH
#define CGameH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
// GAME STATES
//-----------------------------------------------------------------------------
enum
{
	GS_NONE,
	GS_INIT,
	GS_HUD_PRESENT,
	GS_READY,
	GS_GO,
	GS_LOOP,
	GS_PLAYER_DEAD,
	GS_FADE_OUT,
	GS_FADE_IN,
	GS_GAME_OVER,

	GS_BOSS_DEAD,
	GS_STAGE_CLEAR,
	GS_STAGE_FINISH,

	GS_NEXT_STAGE,

	GS_END_GAME,
	GS_FINISH
};
//-----------------------------------------------------------------------------
class CGame
{
	public:

		/// 
		static void Init(CFERenderer* _poRnd,CFEInput* _poInput);

		/// 
		static void Finish();

		/// 
		static void Update(FEReal _rDeltaT);

		/// 
		static void Render();

		/// Specific code to perform when the game enters to a given state.
		static void OnEnterState(uint _uiState);
		
		/// Specific code to perform when the game exits from a given state.
		static void OnExitState(uint _uiState);
		
		/// Retrieve the previous state of the game.
		static uint uiGetPrevState()
		{
			return(m_uiState);
		}

		/// Retrieve the state of the game.
		static uint uiGetState()
		{
			return(m_uiState);
		}

		/// Force the game to a given state: Should not be called directly unless strictly necessary.
		static void SetState(uint _uiState)
		{
			m_uiPrevState = m_uiState;
			m_uiState = _uiState;
		}

		/// Changes the game to a given state, performing Exit and Enter events.
		static void ChangeState(uint _uiState)
		{
			OnExitState(m_uiState);
			OnEnterState(_uiState);

			SetState(_uiState);
		}

	protected:
		static void ProcessMessages();
		
		static void InitGameFlow();
		
		static void FinishGameFlow();
		
		static uint	m_uiState;
		static uint	m_uiPrevState;
		static FEReal m_rStateTime;
		static CFERenderer* m_poRnd;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
