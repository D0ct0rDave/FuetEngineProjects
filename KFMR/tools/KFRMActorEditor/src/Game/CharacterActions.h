//-----------------------------------------------------------------------------
#ifndef CharacterActionH
#define CharacterActionH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
enum ECharacterAction
{
    ACTION_NONE,

    // Common character actions / shared between player and other entities...
	ACTION_ACTOR_WALK,

	ACTION_ACTOR_IDLE,
	ACTION_ACTOR_KICK,
	ACTION_ACTOR_PUNCH,

	ACTION_ACTOR_CROUCH_IDLE,
	ACTION_ACTOR_CROUCH_KICK,
	ACTION_ACTOR_CROUCH_PUNCH,

	ACTION_ACTOR_JUMP_PUNCH,
	ACTION_ACTOR_JUMP_KICK,

	ACTION_ACTOR_PAIN,
	ACTION_ACTOR_DEATH,
	
	ACTION_ENEMY_DEATH,

	ACTION_ACTOR_JUMP,
	ACTION_ACTOR_JUMP_UP,
	ACTION_ACTOR_JUMP_DOWN,
	
	// specific to level1 bonus
	ACTION_ACTOR_GRAB,
	ACTION_ACTOR_GRAB_CROUCH,

    // Specific to the thrower enemy
    ACTION_ACTOR_THROW_HIGH,
    ACTION_ACTOR_THROW_LOW,

    // Specific Chino Cudeiro actions.
	ACTION_CHINO_CUDEIRO_WALK,
    ACTION_CHINO_CUDEIRO_RWALK,
    ACTION_CHINO_CUDEIRO_IDLE,
    ACTION_CHINO_CUDEIRO_ATTACK,

    ACTION_CHINO_CUDEIRO_SIT,
    ACTION_CHINO_CUDEIRO_WAKE,
    ACTION_CHINO_CUDEIRO_PAIN,
    ACTION_CHINO_CUDEIRO_DEATH,

    // Specific Dopefish actions.
	ACTION_DOPEFISH_IDLE,
	ACTION_DOPEFISH_JUMP_UP,
	ACTION_DOPEFISH_JUMP_DOWN,
    
    /// Total number of defined actions.
    ACTION_NUM_ACTIONS
};
//-----------------------------------------------------------------------------
extern CFEString ACTION_NAMES[ACTION_NUM_ACTIONS];
//-----------------------------------------------------------------------------
#endif // CharacterActionH
//-----------------------------------------------------------------------------
