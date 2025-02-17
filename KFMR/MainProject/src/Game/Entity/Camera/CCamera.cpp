//-----------------------------------------------------------------------------
/*! \class CCamera
 *  \brief The class that manages the game camera.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CCamera.h"
#include "Game/CGameMsgMgr.h"
#include "Game/GameMessages.h"
#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/CEntityRegistry.h"
//-----------------------------------------------------------------------------
const FEReal SPIN_FREQ = 3.0f;
const FEReal OBJ_SPEED = 600.0f;

// Object damage
static FEReal OBJECT_DAMAGE = 0.1f;
//-----------------------------------------------------------------------------
CCamera::CCamera() : m_bLocked(false), m_poRnd(NULL)
{
	CEntityRegistry::uiAddEntity(this,"camera");
}
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{
	CEntityRegistry::DeleteEntity(this);
}
//-----------------------------------------------------------------------------
void CCamera::Init(CFERenderer* _poRnd)
{
	m_poRnd = _poRnd;
	Disable();
}
//-----------------------------------------------------------------------------
void CCamera::Reset()
{
	Disable();
}
//-----------------------------------------------------------------------------
void CCamera::Spawn(const CFEVect2& _oPos)
{
	m_oPos = _oPos;
	Enable();
	CEntityMgr::iAddEntity(this);
}
//-----------------------------------------------------------------------------
void CCamera::Update(FEReal _rDeltaT)
{
    if (m_bLocked) return;

	// Control camera limits.
	FEReal rCamWidth = _05r * m_poRnd->uiGetScreenVWidth() /CAMERA_ZOOM;
	FEReal rCamHeight= m_poRnd->uiGetScreenVHeight()/CAMERA_ZOOM;

	FEReal rMinX = goGameGlobals.m_poMap->m_oBV.m_oIni.x;
	FEReal rMaxX = goGameGlobals.m_poMap->m_oBV.m_oEnd.x - 2*rCamWidth;

	m_oPos = goGameGlobals.m_poPlayer->oGetPos();
	m_oPos.x -= goGameGlobals.m_rScreenMargin;
	
	if (rMinX < rMaxX)
	{
		if (m_oPos.x < rMinX) m_oPos.x = rMinX;
		if (m_oPos.y < _0r) m_oPos.y = _0r;
		if ((m_oPos.y + rCamHeight) > goGameGlobals.m_poMap->m_oBV.m_oEnd.y) m_oPos.y = goGameGlobals.m_poMap->m_oBV.m_oEnd.y - rCamHeight;
	}
	
	m_oPos.y = _0r;// (goGameGlobals.m_poMap->m_oBV.m_oEnd.y+goGameGlobals.m_poMap->m_oBV.m_oIni.y)*0.5 - 300;
};
//-----------------------------------------------------------------------------
void CCamera::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	
}
//-----------------------------------------------------------------------------
