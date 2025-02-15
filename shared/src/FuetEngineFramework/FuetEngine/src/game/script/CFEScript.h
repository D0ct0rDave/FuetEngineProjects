// -----------------------------------------------------------------------------
/*! \class CFEScript
 *  \brief A class to store a list of actions, the script.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEScriptH
#define CFEScriptH
// -----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEArray.h"
#include "game/script/action/CFEAction.h"
// -----------------------------------------------------------------------------
class CFEScript
{
	public:

		/// Default constructor for the class
		CFEScript()
		{
		}

		/// Adds a action to the script.
		void AddAction(CFEAction* _poAction)
		{
			m_poActionList.push_back(_poAction);
		}
		
		/// Retrieves the total number of actions in the script.
		uint uiNumActions()
		{
			return(m_poActionList.size());			
		}
		
		/// Retrieves the action linked to the given index.
		CFEAction* poGetAction(uint _uiAction)
		{
			if (_uiAction >= uiNumActions()) return(NULL);
			 // Check(_uiAction<uiNumActions(),"Invalid action number" );
			return( m_poActionList[_uiAction] );
		}

	protected:
		
		// The array that stores all the actuios.
		CFEArray<CFEAction*> m_poActionList;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
