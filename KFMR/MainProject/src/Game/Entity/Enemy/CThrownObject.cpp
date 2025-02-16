//-----------------------------------------------------------------------------
/*! \class CThrownObject
 *  \brief The class that manages objects thrown by ThrowerEnemy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CThrownObject.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"
#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityMgr.h"
#include "Game/Helpers/CSoundHelper.h"
//-----------------------------------------------------------------------------
const FEReal  SPIN_FREQ = 1.50f;
const FEReal OBJECT_DAMAGE = 0.1f;
//-----------------------------------------------------------------------------
TEntityAttack CThrownObject::m_oAttackInfo;
//-----------------------------------------------------------------------------
CThrownObject::CThrownObject()
{
    m_oAttackInfo.m_rDamage = OBJECT_DAMAGE;
	m_oAttackInfo.m_eType   = EA_OBJECT_KICK;
	OBJ_SPEED = 600.0f;	// Default speed
}
//-----------------------------------------------------------------------------
CThrownObject::~CThrownObject()
{
}
//-----------------------------------------------------------------------------
void CThrownObject::Init()
{
	m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/actors/thrower/objects");
	Reset();
}
//-----------------------------------------------------------------------------
void CThrownObject::Reset()
{
	CFESpriteInstMgr::Disable(m_hAnimInst);
	Disable();
}
//-----------------------------------------------------------------------------
void CThrownObject::SetSpeed(FEReal _rNewSpeed)
{
	OBJ_SPEED = _rNewSpeed;
}
//-----------------------------------------------------------------------------
void CThrownObject::Spawn(const CFEVect2& _oPos,bool _bLeft)
{
	m_oPos = _oPos;

	CFESpriteInstMgr::Enable(m_hAnimInst);
	CFESpriteInstMgr::SetPos(m_hAnimInst,_oPos);
	CFESpriteInstMgr::SetScale(m_hAnimInst,0.85f);
	CFESpriteInstMgr::SetAngle(m_hAnimInst,0);
	CFESpriteInstMgr::SetAction(m_hAnimInst,CFEMath::uiRand(2));

	m_rDir = _bLeft?1:-1;

	m_hSnd = CSoundHelper::hPlay(SND_THROWN);

	Enable();

	CEntityMgr::iAddEntity(this);
}
//-----------------------------------------------------------------------------
void CThrownObject::Kill()
{
	CSoundHelper::Stop(m_hSnd);
	CEntityMgr::DeleteEntity(this);	
	Reset();
	//CGameMsgMgr::SendMsg(GMSG_ENEMY_DIED,this,0);
}
//-----------------------------------------------------------------------------
void CThrownObject::Update(FEReal _rDeltaT)
{
	// if object exits screen then kill it.
	// else move in the given direction

	// si la direccion es izquierda, y la posicion del objeto es menor que la del lado izquierdo de la pantalla, kill it!
	if (
		   ((m_rDir<_0r) && (m_oPos.x < goGameGlobals.m_poCamera->oGetPos().x))
		|| ((m_rDir>_0r) && (m_oPos.x > (goGameGlobals.m_poCamera->oGetPos().x + 2*goGameGlobals.m_rScreenMargin)))
		)
	{
		Kill();
	}
	else
	{
		m_oPos.x += OBJ_SPEED * _rDeltaT * m_rDir;
		CFESpriteInstMgr::SetPos(m_hAnimInst,m_oPos);

		// spin the object
		CFESpriteInstMgr::SetAngle( m_hAnimInst, CFESpriteInstMgr::rGetAngle(m_hAnimInst) + SPIN_FREQ * _2PIr_ * _rDeltaT * m_rDir);

		// Instruct to test collisions for this object.
		CGameMsgMgr::SendMsg(GMSG_ENEMY_OBJ,this,(FEPointer)&m_oAttackInfo);
	}
};
//-----------------------------------------------------------------------------
void CThrownObject::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	switch (_uiMessage)
	{
		case GMSG_KILL_ENTITY:
		{
 			Kill();
		}
		break;
	}
}
//-----------------------------------------------------------------------------
