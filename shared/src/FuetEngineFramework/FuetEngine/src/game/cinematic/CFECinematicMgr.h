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

		///
		static void Init();

		/// Frees resources
		static void Finish();

		/// Resets to the initial state.
		static void Reset();

		/// Performs an update step in the Cinematic Manager.
		static void Update(FEReal _rDeltaT);

		/// Sets the cinematic to be played.
		static void SetCinematic(CFECinematic* _poCinematic);

		/// Retrieves the current cinematic.
		static CFECinematic* poGetCinematic()
		{
			return(m_poCinematic);
		}

		/// Sets the cinematic plane to the given one.
		static void SetPlane(uint _uiPlane);

		/// Retrieves the plane index being played.
		static uint uiGetPlane()
		{
			return(m_uiCurPlane);
		}

		/// Restarts
		static void Restart();

		/// Goes to the next plane in the cinematic sequence. Returns false if there is no other plane to play.
		static FEBool bNextPlane();

		/// Tells whether the current playing plane have finished or not.
		static FEBool bPlaneFinished();

		/// Finishes the current action being played.
		static void FinishAction();

	protected:

		static CFECinematic*	m_poCinematic;
		static uint				m_uiCurPlane;
		static CFEScriptMgr		m_oScriptMgr;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
