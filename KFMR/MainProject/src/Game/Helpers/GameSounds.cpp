//-----------------------------------------------------------------------------
#include "GameSounds.h"

TSoundDefs GAME_SOUNDS[SND_NUM_SOUNDS] = 
{
    {"",false},

    {"game/game_intro_n",false},        // SND_GAME_INTRO_N
    {"game/game_outro_n",false},        // SND_GAME_OUTRO_N

    {"player/attack",false},            // PLAYER ATTACK
	{"player/jump",false},              // PLAYER JUMP
	{"player/pain",false},              // PLAYER PAIN
    {"player/hit",false},               // PLAYER HIT
    {"player/death",false},             // PLAYER DEATH

	{"player/attack",false},            // SND_ENEMY_ATTACK,
	{"player/pain",false},              // SND_ENEMY_PAIN,
	{"player/hit",false},               // SND_ENEMY_HIT,
	{"enemy/simple/death",false},       // SND_ENEMY_DEATH,

	{"enemy/thrower/thrown",true},      // SND_THROWN,
	{"player/hit",false},               // SND_OBJECT_HIT,

    {"enemy/chinocudeiro/latigo",false},// SND_CHINOCUDEIRO_ATTACK,
    
    // SND_NUM_SOUNDS
};
//-----------------------------------------------------------------------------
