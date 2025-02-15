//-----------------------------------------------------------------------------
#ifndef HUDActionsH
#define HUDActionsH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
// -----------------------------------------------------------------------------
typedef enum EHUDAction
{
	HA_NONE,

	HA_SHOW_PLAYER_HUD,
	HA_HIDE_PLAYER_HUD,

	HA_SHOW_ENEMY_HUD,
	HA_HIDE_ENEMY_HUD,
	
	HA_SHOW_READY,
	HA_SHOW_GO,
	HA_SHOW_GAMEOVER,
	
	HA_SHOW_CORTINILLAIN,
	HA_SHOW_CORTINILLAOUT,
	
	HA_SHOW_STAGE_CLEAR,
	HA_HIDE_STAGE_CLEAR,

	HA_SHOW_VERSUS_PLAYER,
	HA_SHOW_VERSUS_BOSS,
	HA_HIDE_VERSUS_PLAYER,
	HA_HIDE_VERSUS_BOSS,
	
	HA_NUM_HUD_ACTIONS
};
//-----------------------------------------------------------------------------
typedef struct THUDActionInfo
{
    CFEString m_sActionName;
    CFEString m_sGroupName;

}THUDActionInfo;

extern THUDActionInfo HUD_ACTIONS[HA_NUM_HUD_ACTIONS];
//-----------------------------------------------------------------------------
#endif // HUDActionsH
//-----------------------------------------------------------------------------
