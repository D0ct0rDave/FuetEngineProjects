//-----------------------------------------------------------------------------
#include "CActorTest.h"

#include "Game/CGameGlobals.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CAnimHelper.h"
#include "Game/Helpers/CGroundMgr.h"

#include <FuetEngine.h>

//-----------------------------------------------------------------------------
// Player actions
//-----------------------------------------------------------------------------
const FEReal LEFT_DIR_MULT  = 1;
const FEReal RIGHT_DIR_MULT = -1;
//-----------------------------------------------------------------------------
const uint FACE_ANIM_ATTACK = 0;
const uint FACE_ANIM_DEATH  = 3;
const uint FACE_ANIM_IDLE   = 4;
const uint FACE_ANIM_PAIN   = 5;
//-----------------------------------------------------------------------------
CActorTest::CActorTest()
{
	m_rDirMult = 1;
}
//-----------------------------------------------------------------------------
CActorTest::~CActorTest()
{
}
///-----------------------------------------------------------------------------
void CActorTest::Init(CFEInput* _poInput,const CFEVect2& _oPos)
{
    /*
    // Create the player actor.
    CAnimHelper::CreateActor("player");

	// Retrieve a valid instance of the newly crated actor.
	m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("player");
    */


    /*
    // Create the simpleenemy actor.
    m_hAnimInst = CAnimHelper::hGetReskinedActorInstance("simpleenemy");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    */

    // Create the thrower actor.
    /*
    m_hAnimInst = CAnimHelper::hGetReskinedActorInstance("thrower");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
    */
    
/*
    // Create the discipulo del chinocudeiro actor.
    CAnimHelper::CreateActor("dis_chinocudeiro");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("dis_chinocudeiro");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);
*/
    
    // Create the discipulo del chinocudeiro actor.
    CAnimHelper::CreateActor("enano_saltarin");        
    m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("enano_saltarin");
    SetActorFace( (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode( CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst),"face") );
    poGetActorFace()->SetAction(ENEMY_IDLE_FACE);

    // Create the small dopefish actor.
    // FEHandler hHnd = CFESpriteInstMgr::hGetInstance("data/actors/small_dopefish/small_dopefish");
    // CFESpriteInstMgr::Enable(hHnd);

    /*  
    // Create the chinocudeiro actor.
    CAnimHelper::CreateActor("chinocudeiro");

	// Retrieve a valid instance of the newly crated actor.
	m_hAnimInst = CFESkelAnimInstMgr::hGetInstance("chinocudeiro");

	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetAction(m_hAnimInst,m_uiAction);
	CFESkelAnimInstMgr::SetColor(m_hAnimInst,CFEColor(1,0,0,1));
	CFESkelAnimNode* poNode = CFESkelAnimInstMgr::poGetInstancedNode(m_hAnimInst);
	CFESkelAnimSprite* poSpr = (CFESkelAnimSprite*)CFESkelAnimNodeLocator::poLocateNode(poNode,"rhand");
	if (poSpr != NULL) poSpr->SetAction(1);
    */

    CFESkelAnimInstMgr::SetDepth(m_hAnimInst,0.5f);

	// Set input
	m_poInput = _poInput;
	
	m_uiAction = 0;
	m_oPos = _oPos;
	Reset();
}
//-----------------------------------------------------------------------------
void CActorTest::OnEnterState(uint _uiState)
{
}


void CActorTest::OnExitState(uint _uiState, uint _uiNewState)
{
}
//-----------------------------------------------------------------------------
void CActorTest::Spawn(const CFEVect2& _oPos,bool _bLeft)
{
	m_oPos   = _oPos;
	m_rDirMult = LEFT_DIR_MULT; // _bLeft?LEFT_DIR_MULT:RIGHT_DIR_MULT;

	CFESkelAnimInstMgr::Enable(m_hAnimInst);
	CFESkelAnimInstMgr::SetPos(m_hAnimInst,m_oPos);
	// CFESkelAnimInstMgr::SetScale(m_hAnimInst,CFEVect2(m_rDirMult*0.5,0.5));
	// CFESkelAnimInstMgr::SetAngle(m_hAnimInst,_PI2r_);

	SetEnergy(1);

	Enable();
	CEntityMgr::iAddEntity(this);
	ChangeState(PS_SPAWNING);	
}
///-----------------------------------------------------------------------------
void CActorTest::Update(FEReal _rDeltaT)
{
	m_rStateTime -= _rDeltaT;

	// Set proper height
    CFEVect2 oNewPos( oGetPos().x,CGroundMgr::rGetHeight( oGetPos().x) );
	SetPos( oNewPos );

    // Manage states
    if (m_poInput->bDown(IB_LEFT))
	{
		// Change Anim
		m_uiAction--;
		CFESkelAnimInstMgr::SetAction(m_hAnimInst,m_uiAction);
	}
else if (m_poInput->bDown(IB_RIGHT))
	{
		// Change Anim
		m_uiAction++;
		CFESkelAnimInstMgr::SetAction(m_hAnimInst,m_uiAction);
	}

	else if (m_poInput->bDown(IB_A))
	{
		// Replay Anim
		CFESkelAnimInstMgr::SetAction(m_hAnimInst,m_uiAction);
	}
}
//-----------------------------------------------------------------------------

