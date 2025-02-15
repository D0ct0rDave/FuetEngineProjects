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
#ifndef CFECinematicMgrH
#define CFECinematicMgrH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "CFECinematic.h"
#include "game/script/CFEScriptMgr.h"
// -----------------------------------------------------------------------------
class CFECinematicMgr
{
	public:

		static void Init();
		static void Finish();
		static void Reset();

		static void Update(FEReal _rDeltaT);

		static void SetCinematic(CFECinematic* _poCinematic);
		static void SetPlane(uint _uiPlane);
		static void Restart();
		static void NextPlane();
		static bool bPlaneFinished();

	public:
		static CFECinematic*	m_poCinematic;
		static uint				m_uiCurPlane;

	protected:

		static CFEScriptMgr		m_oScriptMgr;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
