//-----------------------------------------------------------------------------
#include "FuetEngineExt.h"
#include "CAnimHelper.h"
//-----------------------------------------------------------------------------
static int ANM_WALK			=1;
static int ANM_CROUCH_IDLE	=5;
static int ANM_IDLE			=4;

#define MAX_JUMP_KICK_ANIMS 1
static int ANM_JUMP_KICK[MAX_JUMP_KICK_ANIMS];

// #define MAX_JUMP_PUNCHES 1
// static int ANM_JUMP_PUNCH[MAX_JUMP_PUNCHES];

#define MAX_KICK_ANIMS 1
static int ANM_KICK[MAX_KICK_ANIMS];

#define MAX_PUNCH_ANIMS 1
static int ANM_PUNCH[MAX_PUNCH_ANIMS];

#define MAX_CROUCH_KICK_ANIMS 1
static int ANM_CROUCH_KICK[MAX_CROUCH_KICK_ANIMS];

#define MAX_CROUCH_PUNCH_ANIMS 1
static int ANM_CROUCH_PUNCH[MAX_CROUCH_PUNCH_ANIMS];

#define MAX_PAIN_ANIMS 1
static int ANM_PAIN[MAX_PAIN_ANIMS];

#define MAX_DEATH_ANIMS 1
static int ANM_DEATH[MAX_DEATH_ANIMS];

//-----------------------------------------------------------------------------
void CAnimHelper::Init(uint _uiLevel)
{
	// Load anims
	FEHandler hAnimInst = CFESkelAnimInstMgr::hGetInstance("data/actors/actor/actor");
	CFESkelAnim* poAnim = CFESkelAnimInstMgr::poGetAnim(hAnimInst);

	ANM_IDLE			= poAnim->poGetAnimActionSet()->iGetActionIdx("idle");
	ANM_WALK			= poAnim->poGetAnimActionSet()->iGetActionIdx("walk");

	ANM_KICK[0]			= poAnim->poGetAnimActionSet()->iGetActionIdx("kick1");
	// ANM_KICK[1]			= poAnim->poGetAnimActionSet()->iGetActionIdx("kick3");
	// ANM_KICK[2]			= poAnim->poGetAnimActionSet()->iGetActionIdx("kick5");

	ANM_PUNCH[0]		= poAnim->poGetAnimActionSet()->iGetActionIdx("punch2");
	// ANM_PUNCH[1]		= poAnim->poGetAnimActionSet()->iGetActionIdx("punch4");
	// ANM_PUNCH[2]		= poAnim->poGetAnimActionSet()->iGetActionIdx("punch6");

	ANM_JUMP_KICK[0]	= poAnim->poGetAnimActionSet()->iGetActionIdx("jump_kick1");
	// ANM_JUMP_KICK[1]	= poAnim->poGetAnimActionSet()->iGetActionIdx("jump_kick3");

	ANM_CROUCH_IDLE		= poAnim->poGetAnimActionSet()->iGetActionIdx("crouch_idle");

	ANM_CROUCH_KICK[0]	= poAnim->poGetAnimActionSet()->iGetActionIdx("crouch_kick1");
	// ANM_CROUCH_KICK[1]	= poAnim->poGetAnimActionSet()->iGetActionIdx("crouch_kick2");

	ANM_CROUCH_PUNCH[0]	= poAnim->poGetAnimActionSet()->iGetActionIdx("crouch_punch1");
	// ANM_CROUCH_PUNCH[1]	= poAnim->poGetAnimActionSet()->iGetActionIdx("crouch_punch3");

	// ANM_PAIN[0]			= poAnim->poGetAnimActionSet()->iGetActionIdx("pain1");
	// ANM_PAIN[1]			= poAnim->poGetAnimActionSet()->iGetActionIdx("pain2");

	// ANM_DEATH[0]			= poAnim->poGetAnimActionSet()->iGetActionIdx("death1");
}
//-----------------------------------------------------------------------------
void CAnimHelper::Finish()
{
}
//-----------------------------------------------------------------------------
void CAnimHelper::Play(FEHandler _hActorInst,EGameAnim _eAnim)
{
	switch (_eAnim)
	{
		case ANIM_ACTOR_WALK:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_WALK);
		break;
	
		case ANIM_ACTOR_IDLE:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_IDLE);
		break;
		
		case ANIM_ACTOR_KICK:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_KICK[CFEMath::iRand(0,MAX_KICK_ANIMS-1)]);
		break;
		
		case ANIM_ACTOR_PUNCH:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_PUNCH[CFEMath::iRand(0,MAX_PUNCH_ANIMS-1)]);
		break;

		case ANIM_ACTOR_CROUCH_IDLE:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_CROUCH_IDLE);
		break;
		
		case ANIM_ACTOR_CROUCH_KICK:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_CROUCH_KICK[CFEMath::iRand(0,MAX_CROUCH_KICK_ANIMS-1)]);
		break;
		
		case ANIM_ACTOR_CROUCH_PUNCH:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_CROUCH_PUNCH[CFEMath::iRand(0,MAX_CROUCH_PUNCH_ANIMS-1)]);
		break;

		case ANIM_ACTOR_JUMP_KICK:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_JUMP_KICK[CFEMath::iRand(0,MAX_JUMP_KICK_ANIMS-1)]);
		break;

		case ANIM_ACTOR_PAIN:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_PAIN[CFEMath::iRand(0,MAX_PAIN_ANIMS-1)]);
		break;

		case ANIM_ACTOR_DEATH:
		CFESkelAnimInstMgr::SetAction(_hActorInst,ANM_DEATH[CFEMath::iRand(0,MAX_DEATH_ANIMS-1)]);
		break;
	};
}
///-----------------------------------------------------------------------------
CFESkelAnimSprite* CAnimHelper::poSubstNode(FEHandler _hAnimInst,const CFEString& _sNodeName,const CFEString& _sSprite,uint _uiAction)
{
	CFESkelAnimNodeLocator oLocator;
	CFESkelAnimSprite* poNode = (CFESkelAnimSprite*)oLocator.poLocateSkelAnimNode(CFESkelAnimInstMgr::poGetInstancedNode(_hAnimInst),_sNodeName);
	if (poNode != NULL)
	{
		FEHandler hInst = CFESpriteInstMgr::hGetInstance(_sSprite);
		if (hInst != NULL)
		{
			CFESpriteInstMgr::ManageRender(hInst,false);
			CFESpriteInstMgr::Enable(hInst);

			poNode->SetSprite( hInst );
			poNode->SetAction(_uiAction);
		}
	}

	return(poNode);
}
//-----------------------------------------------------------------------------
