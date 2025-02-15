//-----------------------------------------------------------------------------
#include "FuetEngineExt.h"

#include "CSoundHelper.h"
//-----------------------------------------------------------------------------
#define MAX_PAIN_SOUNDS		4
static FEHandler SNDH_PAIN[MAX_PAIN_SOUNDS];

#define MAX_ATTACK_SOUNDS	5
static FEHandler SNDH_ATTACK[MAX_ATTACK_SOUNDS];

#define MAX_JUMP_SOUNDS		7
static FEHandler SNDH_JUMP[MAX_JUMP_SOUNDS];

#define MAX_HIT_SOUNDS		9
static FEHandler SNDH_HIT[MAX_HIT_SOUNDS];

#define MAX_DEATH_SOUNDS	3
static FEHandler SNDH_DEATH[MAX_DEATH_SOUNDS];
//-----------------------------------------------------------------------------
#define MAX_ENEMY_DEATH_SOUNDS	6
static FEHandler SNDH_ENEMY_DEATH[MAX_ENEMY_DEATH_SOUNDS];

#define MAX_CHINOCUDEIRO_ATTACK_SOUNDS	4
static FEHandler SNDH_CHINOCUDEIRO_ATTACK[MAX_CHINOCUDEIRO_ATTACK_SOUNDS];
//-----------------------------------------------------------------------------
void LoadSoundSerie(FEHandler* _phSerieHandlerArray,const CFEString& _sFilenameRoot,uint _uiSerieSize)
{
	for (uint i=0;i<_uiSerieSize;i++)
		_phSerieHandlerArray[i] = CFESoundMgr::hLoad(_sFilenameRoot + CFEString("_") + CFEString(i+1) );
}
//-----------------------------------------------------------------------------
void CSoundHelper::Init(uint _uiLevel)
{
	// Load sounds
	// LoadSoundSerie(SNDH_PAIN,"data/sound/fx/player/pain",4);
	SNDH_PAIN[0] = CFESoundMgr::hLoad("data/sound/fx/player/pain_1");
	SNDH_PAIN[1] = CFESoundMgr::hLoad("data/sound/fx/player/pain_2");
	SNDH_PAIN[2] = CFESoundMgr::hLoad("data/sound/fx/player/pain_3");
	SNDH_PAIN[3] = CFESoundMgr::hLoad("data/sound/fx/player/pain_4");

	// LoadSoundSerie(SNDH_ATTACK,"data/sound/fx/player/attack",5);
	SNDH_ATTACK[0] = CFESoundMgr::hLoad("data/sound/fx/player/attack_1");
	SNDH_ATTACK[1] = CFESoundMgr::hLoad("data/sound/fx/player/attack_2");
	SNDH_ATTACK[2] = CFESoundMgr::hLoad("data/sound/fx/player/attack_3");
	SNDH_ATTACK[3] = CFESoundMgr::hLoad("data/sound/fx/player/attack_4");
	SNDH_ATTACK[4] = CFESoundMgr::hLoad("data/sound/fx/player/attack_5");

	// LoadSoundSerie(SNDH_JUMP,"data/sound/fx/player/jump",7);
	SNDH_JUMP[0] = CFESoundMgr::hLoad("data/sound/fx/player/jump_1");
	SNDH_JUMP[1] = CFESoundMgr::hLoad("data/sound/fx/player/jump_2");
	SNDH_JUMP[2] = CFESoundMgr::hLoad("data/sound/fx/player/jump_3");
	SNDH_JUMP[3] = CFESoundMgr::hLoad("data/sound/fx/player/jump_4");
	SNDH_JUMP[4] = CFESoundMgr::hLoad("data/sound/fx/player/jump_5");
	SNDH_JUMP[5] = CFESoundMgr::hLoad("data/sound/fx/player/jump_6");
	SNDH_JUMP[6] = CFESoundMgr::hLoad("data/sound/fx/player/jump_7");

	// LoadSoundSerie(SNDH_HIT,"data/sound/fx/player/hit",9);
	SNDH_HIT[0] = CFESoundMgr::hLoad("data/sound/fx/player/hit_1");
	SNDH_HIT[1] = CFESoundMgr::hLoad("data/sound/fx/player/hit_2");
	SNDH_HIT[2] = CFESoundMgr::hLoad("data/sound/fx/player/hit_3");
	SNDH_HIT[3] = CFESoundMgr::hLoad("data/sound/fx/player/hit_4");
	SNDH_HIT[4] = CFESoundMgr::hLoad("data/sound/fx/player/hit_5");
	SNDH_HIT[5] = CFESoundMgr::hLoad("data/sound/fx/player/hit_6");
	SNDH_HIT[6] = CFESoundMgr::hLoad("data/sound/fx/player/hit_7");
	SNDH_HIT[7] = CFESoundMgr::hLoad("data/sound/fx/player/hit_8");
	SNDH_HIT[7] = CFESoundMgr::hLoad("data/sound/fx/player/hit_9");

	// LoadSoundSerie(SNDH_HIT,"data/sound/fx/player/hit",9);
	SNDH_DEATH[0] = CFESoundMgr::hLoad("data/sound/fx/player/death_2");
	SNDH_DEATH[1] = CFESoundMgr::hLoad("data/sound/fx/player/death_3");
	SNDH_DEATH[2] = CFESoundMgr::hLoad("data/sound/fx/player/death_4");

	// LoadSoundSerie(SNDH_ENEMY_DEATH,"data/sound/fx/player/enemy/death",6);
	SNDH_ENEMY_DEATH[0] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_1");
	SNDH_ENEMY_DEATH[1] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_2");
	SNDH_ENEMY_DEATH[2] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_3");
	SNDH_ENEMY_DEATH[3] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_4");
	SNDH_ENEMY_DEATH[4] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_5");
	SNDH_ENEMY_DEATH[5] = CFESoundMgr::hLoad("data/sound/fx/enemy/simple/death_6");
	
	// LoadSoundSerie(SNDH_CHINOCUDEIRO_ATTACK,"data/sound/fx/player/enemy/chinocudeiro/latigo",4);
	SNDH_CHINOCUDEIRO_ATTACK[0] = CFESoundMgr::hLoad("data/sound/fx/enemy/chinocudeiro/latigo_1");
	SNDH_CHINOCUDEIRO_ATTACK[1] = CFESoundMgr::hLoad("data/sound/fx/enemy/chinocudeiro/latigo_2");
	SNDH_CHINOCUDEIRO_ATTACK[2] = CFESoundMgr::hLoad("data/sound/fx/enemy/chinocudeiro/latigo_3");
	SNDH_CHINOCUDEIRO_ATTACK[3] = CFESoundMgr::hLoad("data/sound/fx/enemy/chinocudeiro/latigo_4");
}
//-----------------------------------------------------------------------------
void CSoundHelper::Finish()
{
}
//-----------------------------------------------------------------------------
void CSoundHelper::Play(EGameSound _eSound, FEReal _rVol, FEReal _rPan,bool _bLoop)
{
	switch (_eSound)
	{
		case SND_PLAYER_ATTACK:
		CFESoundPlayer::Play(SNDH_ATTACK[CFEMath::iRand(0,MAX_ATTACK_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;

		case SND_PLAYER_JUMP_ATTACK:
		CFESoundPlayer::Play(SNDH_JUMP[CFEMath::iRand(0,MAX_JUMP_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;
		
		case SND_PLAYER_PAIN:		
		CFESoundPlayer::Play(SNDH_PAIN[CFEMath::iRand(0,MAX_PAIN_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;
		
		
		case SND_PLAYER_HIT:
		case SND_ENEMY_HIT:
		case SND_OBJECT_HIT:
		CFESoundPlayer::Play(SNDH_HIT[CFEMath::iRand(0,MAX_HIT_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;

		case SND_PLAYER_DEATH:
		CFESoundPlayer::Play(SNDH_DEATH[CFEMath::iRand(0,MAX_DEATH_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;

		case SND_ENEMY_PAIN:
		case SND_ENEMY_DEATH:
		CFESoundPlayer::Play(SNDH_ENEMY_DEATH[CFEMath::iRand(0,MAX_ENEMY_DEATH_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;
		
		case SND_CHINOCUDEIRO_ATTACK:
		CFESoundPlayer::Play(SNDH_CHINOCUDEIRO_ATTACK[CFEMath::iRand(0,MAX_CHINOCUDEIRO_ATTACK_SOUNDS-1)],SML_FX,_rVol,_rPan);
		break;
	};
}
//-----------------------------------------------------------------------------
