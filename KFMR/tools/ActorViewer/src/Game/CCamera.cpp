//-----------------------------------------------------------------------------
/*! \class CCamera
 *  \brief The class that manages the game camera.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CCamera.h"
#include "Game/CGameGlobals.h"
//-----------------------------------------------------------------------------
const FEReal SPIN_FREQ = 3;
const FEReal OBJ_SPEED = 600;

// Object damage
static FEReal OBJECT_DAMAGE = 0.1f;
//-----------------------------------------------------------------------------
CCamera::CCamera()
{
}
//-----------------------------------------------------------------------------
CCamera::~CCamera()
{
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
}
//-----------------------------------------------------------------------------
void CCamera::Update(FEReal _rDeltaT)
{

	// Control camera limits.
	FEReal rCamWidth = 0.5f * m_poRnd->uiGetScreenVWidth() /CAMERA_ZOOM;
	FEReal rCamHeight= m_poRnd->uiGetScreenVHeight()/CAMERA_ZOOM;

	FEReal rMinX = 0; // goGameGlobals.m_poMap->m_oBV.m_oIni.x;
	FEReal rMaxX = 2000; // goGameGlobals.m_poMap->m_oBV.m_oEnd.x - 2.0f*rCamWidth;

	m_oPos = goGameGlobals.m_poPlayer->oGetPos();
	m_oPos.x -= goGameGlobals.m_rScreenMargin;

	if (m_oPos.x < rMinX) m_oPos.x = rMinX;
	if (m_oPos.y < 0.0f) m_oPos.y = 0.0f;
	// if ((m_oPos.y + rCamHeight) > goGameGlobals.m_poMap->m_oBV.m_oEnd.y) m_oPos.y = goGameGlobals.m_poMap->m_oBV.m_oEnd.y - rCamHeight;
	m_oPos.y = 0.0f;// (goGameGlobals.m_poMap->m_oBV.m_oEnd.y+goGameGlobals.m_poMap->m_oBV.m_oIni.y)*0.5f - 300.0f;
};
//-----------------------------------------------------------------------------
void CCamera::ReceiveMessage(uint _uiMessage,CEntity* _poSrcEntity,FEPointer _pData)
{
	
}
//-----------------------------------------------------------------------------
