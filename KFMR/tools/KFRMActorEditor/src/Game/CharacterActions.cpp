//-----------------------------------------------------------------------------
#include "CharacterActions.h"

CFEString ACTION_NAMES[ACTION_NUM_ACTIONS] = 
{
    "",                     //        ACTION_NONE,

    // Common character actions / shared between player and other entities...
	"walk",                 //        ACTION_ACTOR_WALK,

	"idle",                 //        ACTION_ACTOR_IDLE,
	"kick",                 //        ACTION_ACTOR_KICK,
	"punch",                //        ACTION_ACTOR_PUNCH,

	"crouch_idle",          //        ACTION_ACTOR_CROUCH_IDLE,
	"crouch_kick",          //        ACTION_ACTOR_CROUCH_KICK,
	"crouch_punch",         //        ACTION_ACTOR_CROUCH_PUNCH,

	"jump_punch",           //        ACTION_ACTOR_JUMP_PUNCH,
	"jump_kick",            //        ACTION_ACTOR_JUMP_KICK,
	
	"pain",                 //        ACTION_ACTOR_PAIN,
	"death",                //        ACTION_ACTOR_DEATH,
	
	"enemy_death",          //        ACTION_ENEMY_DEATH,

	"jump",                 //        ACTION_ACTOR_JUMP,
	"jump_up",              //        ACTION_ACTOR_JUMP_UP,
	"jump_down",            //        ACTION_ACTOR_JUMP_DOWN,
    
    "grab",                 //          ACTION_ACTOR_GRAB
    "grab_crouch",          //          ACTION_ACTOR_GRAB_CROUCH

// Specific to the thrower enemy
    "throw_high",           //        ACTION_ACTOR_THROW_HIGH,
    "throw_low",            //        ACTION_ACTOR_THROW_LOW,

    // Specific Chino Cudeiro actions.
	"cc_walk",              //        ACTION_CHINO_CUDEIRO_WALK,
    "cc_reverse_walk",      //        ACTION_CHINO_CUDEIRO_RWALK,
    "cc_idle",              //        ACTION_CHINO_CUDEIRO_IDLE,
    "cc_attack",            //        ACTION_CHINO_CUDEIRO_ATTACK,

    "cc_sit",               //        ACTION_CHINO_CUDEIRO_SIT,
    "cc_wake",              //        ACTION_CHINO_CUDEIRO_WAKE,
    "cc_pain",              //        ACTION_CHINO_CUDEIRO_PAIN,
    "cc_death",             //        ACTION_CHINO_CUDEIRO_DEATH
    
	"df_idle",              //        ACTION_DOPEFISH_IDLE,
	"df_jump_up",           //        ACTION_DOPEFISH_JUMP_UP,
	"df_jump_down",         //        ACTION_DOPEFISH_JUMP_DOWN,

    /// Total number of defined actions.
        //        ACTION_NUM_ACTIONS
};
//-----------------------------------------------------------------------------
