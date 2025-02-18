//-----------------------------------------------------------------------------
#include "CGame.h"
#include "Game/Helpers/CHUDMgr.h"
#include "CGameMsgMgr.h"
#include "GameMessages.h"
#include "CGameGlobals.h"

#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CInteractionMgr.h"
#include "Game/Helpers/CSFXHelper.h"

#include "Game/GameFlow/CGameFlow.h"
#include "Game/GameFlow/CGameFlowFactory.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/Player/CPlayer.h"
#include "Game/Entity/Player/CActorTest.h"
#include "Game/Entity/Camera/CCamera.h"
#include "Game/Map/CMapMgr.h"

#include <stdlib.h>
#include <stdio.h>
#include "Profile/CProfileMgr.h"
//-----------------------------------------------------------------------------
uint CGame::m_uiState		= GS_NONE;
uint CGame::m_uiPrevState	= GS_NONE;
FEReal CGame::m_rStateTime  = 1.0f;
CFERenderer* CGame::m_poRnd = NULL;
//-----------------------------------------------------------------------------
const FEReal STARTING_FADE_IN_TIME = 1.5f;
const FEReal END_FADE_OUT_TIME = 2.5f;
const FEReal PLAYER_DEAD_TIME = 4.0f;

static int m_iPlayerLives = 2;
static CGameFlow* m_poGameFlow = NULL;
//-----------------------------------------------------------------------------
void CGame::Init(CFERenderer* _poRnd,CFEInput* _poInput)
{
    goGameGlobals.m_poCurProfile = &CProfileMgr::m_oSlots[0];
    
	m_poRnd = _poRnd;

	ChangeState(GS_INIT);
	goGameGlobals.m_oLogger.Print("------> Game initialized: %.1f seconds\n",(float)goGameGlobals.m_oChrono.rGetElapsedTime());
}
//-----------------------------------------------------------------------------
void CGame::Finish()
{
    FinishGameFlow();
}
//-----------------------------------------------------------------------------
// Process the messages in the message manager queue.
//-----------------------------------------------------------------------------
void CGame::ProcessMessages()
{
	while (CGameMsgMgr::bPendingMessages())
	{
		CGameMessage oMsg = CGameMsgMgr::oPeekMessage();

		switch (oMsg.m_uiMsgID)
		{
			// It's time to present the HUD.
			case GMSG_START_GAME_LOOP:
			{
				ChangeState(GS_LOOP);
			}
			break;

			case GMSG_PLAYER_ATTACK:
			{
				// Update game collision
				CInteractionMgr::PlayerAttack(*(TEntityAttack*)oMsg.m_pData);
			}
			break;

			case GMSG_PLAYER_DAMAGED:
			{
				CHUDMgr::SetPlayerEnergy( *(FEReal*)oMsg.m_pData );
			}
			break;

			case GMSG_ENEMY_OBJ:
			{
				CInteractionMgr::ObjectCollided(oMsg.m_poEnt,*(TEntityAttack*)oMsg.m_pData);				
			}
			break;

			case GMSG_ENEMY_ATTACK:
			{
				// Update game collision
				CInteractionMgr::EnemyAttack(oMsg.m_poEnt,*(TEntityAttack*)oMsg.m_pData);				
			}
			break;

			case GMSG_ENEMY_DIED:
			{
				// Notify the death of a children to its parent.
				m_poGameFlow->poGetEnemySpawner()->NotifyDeath(oMsg.m_poEnt);
			}
			break;

			case GMSG_BOSS_STARTS:
			{
				CHUDMgr::Play(HA_SHOW_ENEMY_HUD);
				CHUDMgr::SetBossEnergy( 1 );
			}
			break;

			case GMSG_BOSS_DAMAGED:
			{
				// Notify the death of a children to its parent.
				CHUDMgr::SetBossEnergy( *(FEReal*)oMsg.m_pData );
			}
			break;

			case GMSG_BOSS_DIED:
			{
				// Notify the death of a children to its parent.
				m_poGameFlow->poGetEnemySpawner()->NotifyDeath(oMsg.m_poEnt);	
				m_poGameFlow->ReceiveMessage(GMSG_BOSS_DIED);
				//ChangeState(GS_BOSS_DEAD);				
			}
			break;
		}
	}
}
//-----------------------------------------------------------------------------
void CGame::InitGameFlow()
{
    m_poGameFlow = CGameFlowFactory::poCreate( goGameGlobals.m_poCurProfile->m_uiCurLevel, goGameGlobals.m_poCurProfile->m_uiCurSkill);

	CHUDMgr::SetPlayerEnergy(1);
	CHUDMgr::SetPlayerLives(m_iPlayerLives);

	m_poGameFlow->Reset();
	m_poGameFlow->Start();
}
//-----------------------------------------------------------------------------
void CGame::FinishGameFlow()
{
    CEntityMgr::Reset();
    
    CAnimHelper::Finish();
    CSoundHelper::Finish();
    CHUDMgr::Finish();
    CMapMgr::Finish();

    CFESkelAnimMgr::Clear();
    CFESkelAnimInstMgr::Reset();

    CFESpriteMgr::Clear();
    CFESpriteInstMgr::Reset();

    CFEParticleMgr::Reset();
    CFEParticleSysMgr::Clear();
    CFEParticleSysInstMgr::Reset();

    if (goGameGlobals.m_poMap != NULL)
        delete goGameGlobals.m_poMap;
    goGameGlobals.m_poMap = NULL;

    if (goGameGlobals.m_poCamera != NULL)
        delete goGameGlobals.m_poCamera;
    goGameGlobals.m_poCamera = NULL;
    
    if (goGameGlobals.m_poPlayer != NULL)
        delete goGameGlobals.m_poPlayer;
    goGameGlobals.m_poPlayer = NULL;

    if ((m_poGameFlow!=NULL) && (m_poGameFlow->poGetEnemySpawner()!= NULL))
        delete m_poGameFlow->poGetEnemySpawner();

    if (m_poGameFlow != NULL)
	    delete m_poGameFlow;

    m_poGameFlow = NULL;
}
//-----------------------------------------------------------------------------
void CGame::Update(FEReal _rDeltaT)
{
	m_rStateTime -= _rDeltaT;

	// Update the game flow
	if (! m_poGameFlow->bFinished())
	    m_poGameFlow->Update(_rDeltaT);

	// Handle game messages.
	ProcessMessages();

	/// Update game SFX helper.
	CSFXHelper::Update(_rDeltaT);

    // Update HUD Manager
	CHUDMgr::Update(_rDeltaT);
    
	// Update cinematic manager
	CFECinematicMgr::Update(_rDeltaT);
	
	// Update map manager
	CMapMgr::Update(_rDeltaT);
	
	// State specific process
	switch (m_uiState)
	{
		case GS_NONE:
		break;

		case GS_INIT:
		{
			/*
			if (! CFEScreenFX::bPlaying())
			{
	
			}
			*/
		}
		break;

		case GS_HUD_PRESENT:
		{
			/*
			if (!CHUDMgr::bPlayingAction(HA_SHOW_PLAYER_HUD))
			{
				ChangeState(GS_READY);
			}
			*/
		}
		break;

		case GS_READY:
		{
		    /*
			if (!CHUDMgr::bPlayingAction(HA_SHOW_READY))
			{
				ChangeState(GS_GO);
			}
			*/			
		}
		break;

		case GS_GO:
		{
		    /*
			if (!CHUDMgr::bPlayingAction(HA_SHOW_GO))
			{
				ChangeState(GS_LOOP);
			}
			*/			
		}
		break;

		case GS_LOOP:
		{
			if (goGameGlobals.m_poPlayer->bDead())
			{
				ChangeState(GS_PLAYER_DEAD);
			}

       else if (m_poGameFlow->bFinished())
			{
			    ChangeState(GS_STAGE_FINISH);
			}
		}
		break;

		case GS_PLAYER_DEAD:
		{
			if (m_rStateTime<=_0r)
			{
				// Change state
				if (m_iPlayerLives>=0)
				{
					ChangeState(GS_FADE_IN);
				}
				else
				{
					ChangeState(GS_GAME_OVER);
				}
			}
		}
		break;

		case GS_FADE_OUT:
		{
			if (!CHUDMgr::bPlayingAction(HA_SHOW_CORTINILLAOUT))
				ChangeState(GS_READY);
		}
		break;

		case GS_FADE_IN:
		{
			if (!CHUDMgr::bPlayingAction(HA_SHOW_CORTINILLAIN))
				ChangeState(GS_FADE_OUT);
		}
		break;

		case GS_GAME_OVER:
		{
			if (!CHUDMgr::bPlayingAction(HA_SHOW_GAMEOVER))
				ChangeState(GS_END_GAME);
		}
		break;
		
		case GS_BOSS_DEAD:		
		{
			if (!CHUDMgr::bPlayingAction(HA_HIDE_ENEMY_HUD))
				ChangeState(GS_STAGE_CLEAR);		
		}
		break;

		case GS_STAGE_CLEAR:
		{
			if (!CHUDMgr::bPlayingAction(HA_SHOW_STAGE_CLEAR))
				ChangeState(GS_STAGE_FINISH);
		}		
		break;
		
		case GS_STAGE_FINISH:
		{
			// if (!CHUDMgr::bPlayingAction(HA_HIDE_STAGE_CLEAR))
			ChangeState(GS_NEXT_STAGE);
		}
		break;

		case GS_END_GAME:
		{
			if (! CFEScreenFX::bPlaying())
			{
				ChangeState(GS_FINISH);
			}
		}
		break;

		case GS_NEXT_STAGE:
		{
			if (! CFEScreenFX::bPlaying())
			{
			    FinishGameFlow();

    			// Next level
	    		if (goGameGlobals.m_poCurProfile->m_uiCurLevel >= 0x1a)
                {
                    goGameGlobals.m_poCurProfile->m_uiCurLevel++;

                    if ((goGameGlobals.m_poCurProfile->m_uiCurLevel & 0x0f) > 0x0c)
					{
						// (0x10 = next level) + (first sublevel = 0x0a)
                        goGameGlobals.m_poCurProfile->m_uiCurLevel = (goGameGlobals.m_poCurProfile->m_uiCurLevel & 0xf0) + 0x10 + 0x0a; 
						
						// Change this if more than 1 level available
						if ((goGameGlobals.m_poCurProfile->m_uiCurLevel & 0xf0) > 0x10)
						{
							goGameGlobals.m_poCurProfile->m_uiCurLevel = 0x1a;
						}
					}
                }

	    		// Automatic Save of profile data.
	    		CProfileMgr::Save();

				ChangeState(GS_INIT);
			}
		}
		break;

		case GS_FINISH:
		{
			
		}
		break;
	}
}
//-----------------------------------------------------------------------------
#ifdef _DEBUG
#define CASE(STATE)\
    case STATE:\
    goGameGlobals.m_oLogger.Print(" Entering %s\n",#STATE);
#else
    #define CASE(STATE)\
        case STATE:
#endif

void CGame::OnEnterState(uint _uiState)
{
	switch (_uiState)
	{
		CASE(GS_NONE)
		break;

		CASE(GS_INIT)
		{
            InitGameFlow();

	        CFEScreenFX::SetBackground(NULL,CFEColor(0,0,0,1));
	        CFEScreenFX::FadeOut(STARTING_FADE_IN_TIME);
	        CFESoundMixer::FadeIn(STARTING_FADE_IN_TIME);
		}
		break;

        /*
		/CASE(GS_HUD_PRESENT)
		{
			goGameGlobals.m_poPlayer->Disable();
			CHUDMgr::Play(HA_SHOW_PLAYER_HUD);
		}
		break;

		CASE(GS_READY)
		{
			goGameGlobals.m_poPlayer->Disable();
			CHUDMgr::Play(HA_SHOW_READY);
		}
		break;

		CASE(GS_GO)
		{
			goGameGlobals.m_poPlayer->Enable();
			CHUDMgr::Play(HA_SHOW_GO);
		}
		break;
        */

		CASE(GS_LOOP)
		goGameGlobals.m_poPlayer->Enable();
		break;
		
		CASE(GS_PLAYER_DEAD)
		{
			m_rStateTime = PLAYER_DEAD_TIME;
			m_iPlayerLives--;
		}
		break;
		
		CASE(GS_FADE_OUT)
		{
			// While screen is black we can safely do all this stuff
			CHUDMgr::Reset();
			CHUDMgr::SetPlayerLives( m_iPlayerLives );			

			m_poGameFlow->Reset();
			m_poGameFlow->Restart();

			CHUDMgr::Play(HA_SHOW_CORTINILLAOUT);
		}
		break;

		CASE(GS_FADE_IN)
		{
			CHUDMgr::Play(HA_SHOW_CORTINILLAIN);
		}
		break;

		CASE(GS_GAME_OVER)
		{
			CHUDMgr::Play(HA_SHOW_GAMEOVER);
		}
		break;

		CASE(GS_BOSS_DEAD)
		{
			CHUDMgr::Play(HA_HIDE_ENEMY_HUD);
		}
		break;

		CASE(GS_STAGE_CLEAR)
		{
			CHUDMgr::Play(HA_SHOW_STAGE_CLEAR);
		}
		break;
		
		CASE(GS_STAGE_FINISH)
		{
			// CHUDMgr::Play(HA_HIDE_STAGE_CLEAR);
		}
		break;

		CASE(GS_NEXT_STAGE)
		{
		}
		break;

		CASE(GS_END_GAME)
		{
			CFEScreenFX::SetBackground(NULL,CFEColor(0,0,0,1));
			CFEScreenFX::FadeIn(0.000001f);
			CFEScreenFX::Update(1);
		}
		break;
		
		CASE(GS_FINISH)
		break;
	}
};
//-----------------------------------------------------------------------------
void CGame::OnExitState(uint _uiState)
{
}
//-----------------------------------------------------------------------------
void CGame::Render()
{
    if (m_poGameFlow != NULL)
        m_poGameFlow->Render();
}
//-----------------------------------------------------------------------------
