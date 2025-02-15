//-----------------------------------------------------------------------------
#ifndef GameSoundsH
#define GameSoundsH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
enum EGameSounds
{
    SND_NONE,

    SND_GAME_INTRO_N,           // normal game intro (vs ultratomb intro)
    SND_GAME_OUTRO_N,

	SND_PLAYER_ATTACK,
	SND_PLAYER_JUMP_ATTACK,
	SND_PLAYER_PAIN,
	SND_PLAYER_HIT,
	SND_PLAYER_DEATH,
	
	SND_ENEMY_ATTACK,
	SND_ENEMY_PAIN,
	SND_ENEMY_HIT,
	SND_ENEMY_DEATH,
	
	SND_THROWN,
	SND_OBJECT_HIT,

	SND_CHINOCUDEIRO_ATTACK,
	
	SND_NUM_SOUNDS
};
//-----------------------------------------------------------------------------
typedef struct TSoundDefs{
    char*   m_sName;
    bool    m_bLoop;
}TSoundDefs;

extern TSoundDefs GAME_SOUNDS[SND_NUM_SOUNDS];
//-----------------------------------------------------------------------------
#endif // GameSoundsH
//-----------------------------------------------------------------------------
