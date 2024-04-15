// -----------------------------------------------------------------------------
/*! \class CFECinematicLoader
 *  \brief A class to load cinematic scenes.
 *  \author David Márquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#ifndef CFECinematicLoaderH
#define CFECinematicLoaderH
// -----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "support/config/CFEConfigFile.h"
// -----------------------------------------------------------------------------
class CFECinematic;
class CFECinematicPlane;
// -----------------------------------------------------------------------------
class CFECinematicLoader
{
	public:

		/// Loads a cinematic scene.
		static CFECinematic* poLoad(const CFEString& _sFilename);

	protected:

		// Load a plane of a cinematic scene.
		static CFECinematicPlane* poLoadPlane(const CFEConfigFile& _oCfg,const CFEString& _sPrefix,const CFEString& _sWorkingDir);
};
// -----------------------------------------------------------------------------
#endif
// -----------------------------------------------------------------------------
