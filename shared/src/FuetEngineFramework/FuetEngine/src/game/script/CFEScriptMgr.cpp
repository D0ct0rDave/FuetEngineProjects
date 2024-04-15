// -----------------------------------------------------------------------------
/*! \class CFEScriptMgr
 *  \brief A class to manage scripts.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#if defined(DEVELOPMENT) && defined(WIN32)
// #include <typeinfo>
#include "support/system/CFESystem.h"
#endif

#include "CFEScriptMgr.h"
// -----------------------------------------------------------------------------
/// Prepares the execution of the given script.
void CFEScriptMgr::Exec(CFEScript* _poScript)
{
	m_poScript			= _poScript;

	if (m_poScript == NULL)
		m_poCurrentAction = NULL;
	else
		SetAction(0);
}
// -----------------------------------------------------------------------------
void CFEScriptMgr::SetAction(uint _uiAction)
{
	m_uiCurrentAction	= _uiAction;
	m_poCurrentAction	= m_poScript->poGetAction(m_uiCurrentAction);

	if (m_poCurrentAction != NULL)	
	{
		m_poCurrentAction->Reset();

		#if defined(DEVELOPMENT) && defined(WIN32)
		CFESystem::Print("SCRIPTING ENGINE: Executing Action %04d, %s\n", _uiAction, m_poCurrentAction->sGetActionSignature() );
		#endif
	}
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
