// ----------------------------------------------------------------------------
/*! \file CFECinematicMgr
 *  \brief The class responsible of management and execution of cinematic scenes.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#include "CFECinematicMgr.h"
#include "game/script/CFEScriptMgr.h"
#include "support/system/CFESystem.h"
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
	m_oScriptMgr.Exec(NULL);

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
	if (m_poCinematic->m_oPlanes[_uiPlane] == NULL)
	{
		#ifdef _DEBUG
		CFESystem::Error("CFECinematicMgr: plane %03d is NULL.\n"
					     "      Look at the log for more info\n", _uiPlane);
		#endif
		return;
	}

	m_uiCurPlane = _uiPlane;
	m_oScriptMgr.Exec( m_poCinematic->m_oPlanes[m_uiCurPlane]->poGetScript() );

	#ifdef _DEBUG
	CFESystem::Print("CFECinematicMgr::SetPlane %03d, %s\n",m_uiCurPlane,m_poCinematic->m_oPlanes[m_uiCurPlane]->m_sPlaneName.szString());
	#endif
}
// -----------------------------------------------------------------------------
FEBool CFECinematicMgr::bNextPlane()
{
	if (m_poCinematic == NULL) return(false);

	if ((m_uiCurPlane+1) < m_poCinematic->m_oPlanes.size())
	{
		SetPlane(m_uiCurPlane + 1);
		return(true);
	}
	else
		return(false);
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::Update(FEReal _rDeltaT)
{
	if (bPlaneFinished()) return;
	m_oScriptMgr.bUpdate(_rDeltaT);
}
// -----------------------------------------------------------------------------
FEBool CFECinematicMgr::bPlaneFinished()
{
	return( m_oScriptMgr.bFinished() );
}
// -----------------------------------------------------------------------------
void CFECinematicMgr::FinishAction()
{
	//
	m_oScriptMgr.FinishAction();
}
// -----------------------------------------------------------------------------
