//-----------------------------------------------------------------------------
#include "HUDActions.h"
//-----------------------------------------------------------------------------
THUDActionInfo HUD_ACTIONS[HA_NUM_HUD_ACTIONS] =
{
    {"",""}, // HA_NONE,

	{"HUDPresentation1","Energy-Group"},    // HA_SHOW_PLAYER_HUD,
	{"",""}, // HA_HIDE_PLAYER_HUD,

	{"EnemyHUDShows","Enemy-Group"},        // HA_SHOW_ENEMY_HUD,
	{"EnemyHUDHides","Enemy-Group"},        // HA_HIDE_ENEMY_HUD,
	
	{"ReadyMsg_Show","ReadyMsg"},           // HA_SHOW_READY,
	{"GoMsg_Show","GoMsg"},                 // HA_SHOW_GO,
	{"GameOverMsg_Show","GameOverMsg"},     // HA_SHOW_GAMEOVER,
	
	{"CorinillaIn_Show","CortinillaFX"},    // HA_SHOW_CORTINILLAIN,
	{"CorinillaOut_Show","CortinillaFX"},   // HA_SHOW_CORTINILLAOUT,

	{"StageClearShows","StageClearMsg"}, // HA_SHOW_STAGE_CLEAR,
	{"StageClearHides","StageClearMsg"}, // HA_HIDE_STAGE_CLEAR,


	{"ShowVersusPlayer","Versus_Player"}, // HA_SHOW_VERSUS_PLAYER,
	{"ShowVersusBoss","Versus_Boss"},   // HA_SHOW_VERSUS_BOSS,
	{"HideVersusPlayer","Versus_Player"}, // HA_HIDE_VERSUS_PLAYER,
	{"HideVersusBoss","Versus_Boss"}, // HA_HIDE_VERSUS_BOSS,

	// HA_NUM_HUD_ACTIONS
};
//-----------------------------------------------------------------------------
