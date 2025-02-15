//-----------------------------------------------------------------------------
/*! \class GameMessages
 *  \brief Describe all the messages used by the game
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef GameMessagesH
#define GameMessagesH
//-----------------------------------------------------------------------------
enum
{
	GMSG_NONE,
	
	GMSG_PRESENT_HUD,
	GMSG_START_GAME_LOOP,
	
	GMSG_PLAYER_ATTACK,
	GMSG_PLAYER_DAMAGED,
	
	GMSG_ENEMY_ATTACK,
	//GMSG_ENEMY_ATTACK_DOWN,
	//GMSG_ENEMY_ATTACK_UP,
	GMSG_ENEMY_DIED,	
	GMSG_ENEMY_OBJ,
	
	GMSG_DAMAGE_ENTITY,
	GMSG_KILL_ENTITY,
		
	GMSG_BOSS_STARTS,
	GMSG_BOSS_DAMAGED,
	GMSG_BOSS_DIED,

	GMSG_GET_BONUS_OBJ,
	GMSG_GET_BAD_BONUS_OBJ,
	
	GMSG_NUM_MSGS
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
