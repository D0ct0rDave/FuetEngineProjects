//-----------------------------------------------------------------------------
/*! \class CEyeFollower
 *  \brief The class that manages objects thrown by ThrowerEnemy.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CEyeFollower.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"
#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityMgr.h"
#include "Game/Helpers/CSoundHelper.h"
#include "Game/Helpers/CSFXHelper.h"
//-----------------------------------------------------------------------------
const uint      RAISE_THRESHOLD = 175;
const uint      FALL_THRESHOLD  = 250;
const int       RAISEFALL_HEIGHT = 100;
const FEReal    RAISEFALL_TIME  = 0.35;
//-----------------------------------------------------------------------------
typedef enum EEyeFollowerStates
{
	EF_NONE		= 0,
	EF_RAISE    = 1,
	EF_IDLE     = 2,
	EF_FALL     = 3,
	EF_SLEEPING
};
//-----------------------------------------------------------------------------
CEyeFollower::CEyeFollower()
{
    Init();
}
//-----------------------------------------------------------------------------
CEyeFollower::~CEyeFollower()
{
}
//-----------------------------------------------------------------------------
void CEyeFollower::Init()
{
	m_hAnimInst	= CFESpriteInstMgr::hGetInstance("data/entities/mapache");
	m_hEye[0]   = CFESpriteInstMgr::hGetInstance("data/entities/mapache_eye");
	m_hEye[1]   = CFESpriteInstMgr::hGetInstance("data/entities/mapache_eye");
	Reset();
}
//-----------------------------------------------------------------------------
void CEyeFollower::Reset()
{
	CFESpriteInstMgr::Disable(m_hAnimInst);
	Disable();
}
//-----------------------------------------------------------------------------
void CEyeFollower::Spawn(const CFEVect2& _oPos)
{
	CFESpriteInstMgr::Enable(m_hAnimInst);
	CFESpriteInstMgr::SetAngle(m_hAnimInst,0);
	CFESpriteInstMgr::SetDepth(m_hAnimInst,m_rDepth);

    for (uint i=0;i<2;i++)
    {
        CFESpriteInstMgr::Enable(m_hEye[i]);
        CFESpriteInstMgr::SetDepth(m_hEye[i],m_rDepth - 0.01);
    }

	Enable();
	CEntityMgr::iAddEntity(this);
     
    // Set the initial position of the entity.
    m_oPos = _oPos;

    // Sets the height of the entity
    m_rCurHeight = RAISEFALL_HEIGHT;
	SetHeight(m_rCurHeight);

	ChangeState(EF_SLEEPING);
}
//-----------------------------------------------------------------------------
void CEyeFollower::SetHeight(FEReal _rHeight)
{
    CFEVect2 oPos = m_oPos;
    oPos.y += _rHeight;

	CFESpriteInstMgr::SetPos(m_hAnimInst,oPos);
    for (uint i=0;i<2;i++)
    {
        CFEVect2 oEyePos;
        oEyePos.x = oPos.x + (FEReal(2*i) - 1.0)*12.0f;
        oEyePos.y = oPos.y - 32;

        CFESpriteInstMgr::SetPos(m_hEye[i], oEyePos);
    }
}
//-----------------------------------------------------------------------------
void CEyeFollower::Kill()
{
    CEntityMgr::DeleteEntity(this);	
	Reset();
}
//-----------------------------------------------------------------------------
void CEyeFollower::Update(FEReal _rDeltaT)
{
    m_rStateTime -= _rDeltaT;

	switch (uiGetState())
	{
	    case EF_RAISE:
	    {
	        if (m_rStateTime>_0r)
	        {
	            FEReal rFact = (m_rStateTime/RAISEFALL_TIME);
	            FEReal rH = CFEMath::rLerp(0,RAISEFALL_HEIGHT,rFact*rFact);
	            SetHeight(rH);
	        }
	        else
	            ChangeState(EF_IDLE);
	    }
	    break;
	    
	    case EF_IDLE:
	    {
            if (CFEMath::rAbs(goGameGlobals.m_poPlayer->oGetPos().x - m_oPos.x) >= FALL_THRESHOLD)
	        {
	            ChangeState(EF_FALL);
	        }
            else
            {
                // spin the object
	            for (uint i=0;i<2;i++)
	            {
	                CFEVect2 oDir = CFESpriteInstMgr::oGetPos(m_hEye[i]) - goGameGlobals.m_poPlayer->oGetPos();
	                oDir.Normalize();
	                FEReal rAngle = oDir.x * _PI2r;

	                CFESpriteInstMgr::SetAngle( m_hEye[i], rAngle);
	            }
            }
	    }
	    break;

	    case EF_FALL:
	    {
	        if (m_rStateTime>_0r)
	        {
	            FEReal rFact = (m_rStateTime/RAISEFALL_TIME);
	            FEReal rH = CFEMath::rLerp(RAISEFALL_HEIGHT,0,rFact*rFact);

	            SetHeight(rH);
	        }
	        else
	            ChangeState(EF_SLEEPING);
	    }
	    break;

	    case EF_SLEEPING:
	    {
	        if (CFEMath::rAbs(goGameGlobals.m_poPlayer->oGetPos().x - m_oPos.x) <= RAISE_THRESHOLD)
	        {
	            ChangeState(EF_RAISE);
	        }
	    }
	    break;	    
    }
};
//-----------------------------------------------------------------------------
void CEyeFollower::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
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
void CEyeFollower::OnEnterState(uint _uiState)
{
    switch (_uiState)
	{
	    case EF_RAISE:
	    {
	        m_rStateTime = RAISEFALL_TIME;
	    }
	    break;

	    case EF_IDLE:
	    {
	    }
	    break;

	    case EF_FALL:
	    {
	        CSFXHelper::SpawnParticle(SFX_CINECT_DOWN,CFEVect2(m_oPos.x,m_oPos.y-25),m_rDepth);
            m_rStateTime = RAISEFALL_TIME;
	    }
	    break;
    }    
}
//-----------------------------------------------------------------------------
void CEyeFollower::OnExitState(uint _uiState, uint _uiNewState)
{

}
//-----------------------------------------------------------------------------
