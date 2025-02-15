// ----------------------------------------------------------------------------
/*! \file CFECinematicMgr
 *  \brief The class responsible of management and execution of cinematic scenes.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFECinematicMgr.h"
#include "game/script/CFEScriptMgr.h"
// -----------------------------------------------------------------------------
CFECinematic*	CFECinematicMgr::m_poCinematic = NULL;
uint			CFECinematicMgr::m_uiCurPlane;
CFEScriptMgr	CFECinematicMgr::m_oScriptMgr;
// -----------------------------------------------------------------------------
void CFECinematicMgr::Init()
{
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::Finish()
{
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::Reset()
{
	SetPlane(0);
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::SetCinematic(CFECinematic *_poCinematic)
{
	m_poCinematic = _poCinematic;

	// Set first plane
	Restart();
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::Restart()
{
	SetPlane(0);
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::SetPlane(uint _uiPlane)
{
	if (m_poCinematic == NULL) return;
	if (_uiPlane >= m_poCinematic->m_oPlanes.size()) return;

	//
	m_uiCurPlane = _uiPlane;
	m_oScriptMgr.Exec( m_poCinematic->m_oPlanes[m_uiCurPlane]->m_poScript );
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::NextPlane()
{
	if (m_poCinematic == NULL) return;

	if (m_uiCurPlane < m_poCinematic->m_oPlanes.size())
		SetPlane(m_uiCurPlane + 1);
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::Update(FEReal _rDeltaT)
{
	if (bPlaneFinished()) return;
	m_oScriptMgr.bUpdate(_rDeltaT);
}
// -----------------------------------------------------------------------------
bool CFECinematicMgr::bPlaneFinished()
{
	return( m_oScriptMgr.bFinished() );
}
// -----------------------------------------------------------------------------
