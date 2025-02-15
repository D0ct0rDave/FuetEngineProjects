// -----------------------------------------------------------------------------
/*! \class CFEScriptMgr
 *  \brief A class to manage scripts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEScriptMgrH
#define CFEScriptMgrH
// -----------------------------------------------------------------------------
#include "CFEScript.h"
// -----------------------------------------------------------------------------
class CFEScriptMgr
{
	public:
	
		/// Prepares the execution of the given script.
		void Exec(CFEScript* _poScript);

		/// Updates the execution of the current script. Returns false if the current script has finished.
		bool bUpdate(FEReal _rDeltaT);

		/// Returns true if the execution of the script has finished.
		bool bFinished();

	protected:
		
		///
		void SetAction(uint _uiAction);
	
		CFEScript*	m_poScript;
		CFEAction*	m_poCurrentAction;
		uint		m_uiCurrentAction;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
