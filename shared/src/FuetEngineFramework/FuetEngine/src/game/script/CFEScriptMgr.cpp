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
#include "CFEScriptMgr.h"
// -----------------------------------------------------------------------------
/// Prepares the execution of the given script.
void CFEScriptMgr::Exec(CFEScript* _poScript)
{
	m_poScript			= _poScript;
	SetAction(0);
}
// -----------------------------------------------------------------------------
void CFEScriptMgr::SetAction(uint _uiAction)
{
	m_uiCurrentAction	= _uiAction;
	m_poCurrentAction	= m_poScript->poGetAction(m_uiCurrentAction);

	if (m_poCurrentAction != NULL)	
		m_poCurrentAction->Reset();
}
// -----------------------------------------------------------------------------
/// Updates the execution of the current script
FEBool CFEScriptMgr::bUpdate(FEReal _rDeltaT)
{
	// if there is no action, the script execution has finished.
	if (m_poCurrentAction == NULL) return(false);
	
	// Update the current action.
	while ((m_poCurrentAction!=NULL) && (m_poCurrentAction->bUpdate(_rDeltaT) == false))
	{
		// next action
		SetAction(m_uiCurrentAction+1);	
	}

	return(m_poCurrentAction != NULL);
}
// -----------------------------------------------------------------------------
void CFEScriptMgr::FinishAction()
{
	if (m_poCurrentAction != NULL)	
		m_poCurrentAction->ForceFinish();
}
// -----------------------------------------------------------------------------
/// Returns true if the execution of the script has finished.
FEBool CFEScriptMgr::bFinished()
{
	return(m_poCurrentAction == NULL);
}
// -----------------------------------------------------------------------------
