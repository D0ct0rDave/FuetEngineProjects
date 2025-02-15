// -----------------------------------------------------------------------------
/*! \class CFECinematicLoader
 *  \brief A class to load cinematic scenes.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 1999-2009
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CFECinematicLoader.h"
#include "CFECinematic.h"
#include "game/script/CFEScriptLoader.h"
#include "support/misc/CFEStringUtils.h"
// -----------------------------------------------------------------------------
CFECinematicPlane* CFECinematicLoader::poLoadPlane(const CFEConfigFile& _oCfg,const CFEString& _sPrefix,const CFEString& _sWorkingDir)
{	
	CFECinematicPlane* poPlane  = new CFECinematicPlane;

		// Load the script associated with this cinematic plane.
		CFEString sScriptFilename = _oCfg.sGetString(_sPrefix + ".Script","");
		if (sScriptFilename != "")
			poPlane->m_poScript = CFEScriptLoader::poLoad(_sWorkingDir + CFEString("/") + sScriptFilename);

	return(poPlane);
}
// -----------------------------------------------------------------------------
CFECinematic* CFECinematicLoader::poLoad(const CFEString& _sFilename)
{
    CFEString sFilename = _sFilename + ".cin";
    CFEString sWorkingDir = CFEStringUtils::sGetPath(_sFilename);

	CFEConfigFile oCfg(sFilename);
	if (! oCfg.bInitialized()) return(NULL);

	CFECinematic* poScene = new CFECinematic;

			// Load planes
			uint uiNumPlanes = oCfg.iGetInteger("Cinematic.NumPlanes",0);
			for (uint i=0;i<uiNumPlanes;i++)
			{
				CFEString sPrefix = CFEString("Cinematic.Plane") + CFEString(i);
				CFECinematicPlane* poPlane = poLoadPlane(oCfg,sPrefix,sWorkingDir);
				poScene->m_oPlanes.push_back(poPlane);
			}

	return(poScene);
}
// -----------------------------------------------------------------------------
