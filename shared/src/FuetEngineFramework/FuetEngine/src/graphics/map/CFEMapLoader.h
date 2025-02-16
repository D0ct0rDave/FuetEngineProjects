// ----------------------------------------------------------------------------
/*! \class FEEnums
 *  \brief A class to load a FE map.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEMapLoaderH
#define CFEMapLoaderH
// ----------------------------------------------------------------------------
#include "Types/CFEString.h"
#include "Support/Config/CFEConfigFile.h"
#include "CFEMap.h"
// ----------------------------------------------------------------------------
class CFEMapLoader
{
    public:
            /// Loads a map from a given file    
            static CFEMap* poLoad(const CFEString& _sFilename);

    protected:

            /// Loads a sector
            static CFEMapSector* poLoadSector(const CFEConfigFile& _oCfg,const CFEString& _sSector,const CFEArray<CFEString>& _oSpriteList);

            /// Loads a layer
            static CFEMapLayer* poLoadLayer(const CFEConfigFile& _oCfg,const CFEString& _sLayer,const CFEArray<CFEString>& _oSpriteList);

            /// Loads an element
            static CFEMapElement* poLoadElement(const CFEConfigFile& _oCfg,const CFEString& _sElement,const CFEArray<CFEString>& _oSpriteList);

			// Loads the sprite set used in this map.
			static CFEArray<CFEString>* poLoadSpriteSet(const CFEConfigFile& _oCfg,const CFEString& _sWorkingDir);
};

// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------

