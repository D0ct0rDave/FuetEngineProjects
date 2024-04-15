// -----------------------------------------------------------------------------
/*! \class CFEScriptLoader
 *  \brief A class to load scripts.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFEScriptLoaderH
#define CFEScriptLoaderH
// -----------------------------------------------------------------------------
#include "CFEScript.h"
#include "Action/CFEActionLoader.h"
// -----------------------------------------------------------------------------
class CFEScriptLoader
{
	public:
	
		/// Loads a script.
		static CFEScript* poLoad(const CFEString& _sFilename);

		/// Sets the action loader to be used by the script loader.
		static void SetActionLoader(CFEActionLoader* _poActionLoader)
		{
			m_poActionLoader = _poActionLoader;
		}
		
		/// Retrieves the current action loader used by the script loader.
		static CFEActionLoader* poGetActionLoader()
		{
			return(m_poActionLoader);
		}

	protected:

		// holds the action loader to be used by the cinematic scene loader.
		static CFEActionLoader* m_poActionLoader;
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
