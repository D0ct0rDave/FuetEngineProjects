//-----------------------------------------------------------------------------
#include "CLevel2BonusThrownObject.h"

#include <FuetEngine.h>
#include "Game/CGameGlobals.h"
#include "Game/GameMessages.h"
#include "Game/CGameMsgMgr.h"
#include "Game/Entity/CEntityMgr.h"
//-----------------------------------------------------------------------------
const uint BONUS_THROWN_OBJ_SPEED = 300;

#define MAX_THROWN_SOUNDS	2
static FEHandler SNDH_THROWN[MAX_THROWN_SOUNDS];
//-----------------------------------------------------------------------------
CLevel2BonusThrownObject::CLevel2BonusThrownObject()
{
	SetSpeed(BONUS_THROWN_OBJ_SPEED);
}
//-----------------------------------------------------------------------------
void CLevel2BonusThrownObject::Init(bool _bIsGood, int _iIndex)
{
	m_bIsGood = _bIsGood;

	SNDH_THROWN[0] = CFESoundMgr::hLoad("data/sound/fx/enemy/thrower/thrown_1");
	SNDH_THROWN[1] = CFESoundMgr::hLoad("data/sound/fx/enemy/thrower/thrown_2");
	CFESoundMgr::bSetSoundProperty(SNDH_THROWN[0],"Loop",(FEPointer)true);
	CFESoundMgr::bSetSoundProperty(SNDH_THROWN[1],"Loop",(FEPointer)true);

	if(m_bIsGood)
	{
		// Lo hago con un switch ya que no sé qué funciones de string va a tener la DS y para ahorrarnos problemas al portaje
		switch(_iIndex%6)
		{
			case 0:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_0");
			break;
			case 1:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_1");
			break;
			case 2:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_2");
			break;
			case 3:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_3");
			break;
			case 4:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_4");
			break;
			case 5:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/good_5");
			break;
		}
	}
	else
	{
		switch(_iIndex%6)
		{
			case 0:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_0");
			break;
			case 1:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_1");
			break;
			case 2:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_2");
			break;
			case 3:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_3");
			break;
			case 4:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_4");
			break;
			case 5:
				m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/objects/bad_5");
			break;
		}
	}

	Reset();
}
//-----------------------------------------------------------------------------
void CLevel2BonusThrownObject::Update(FEReal _rDeltaT)
{
    CThrownObject::Update(_rDeltaT);
    
    // Do not spin good objects.
    if (m_bIsGood)
		CFESpriteInstMgr::SetAngle( m_hAnimInst, _0r);
}
//-----------------------------------------------------------------------------
void CLevel2BonusThrownObject::Kill()
{
	CFESoundPlayer::Stop(m_hSnd);
	CEntityMgr::DeleteEntity(this);	
	Reset();
	CGameMsgMgr::SendMsg(GMSG_ENEMY_DIED,this,0);
}
//-----------------------------------------------------------------------------