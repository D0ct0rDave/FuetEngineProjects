// ----------------------------------------------------------------------------
/*! \class CFEHUDLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEHUDLoaderH
#define CFEHUDLoaderH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "support/config/CFEConfigFile.h"
//-----------------------------------------------------------------------------
class CFEHUD;
class CFEHUDObject;
class CFEHUDElementAction;
class CFEHUDObjectAction;
class CFEHUDElement;
//-----------------------------------------------------------------------------
class CFEHUDLoader
{
    public:

        /// Loads a HUD from a given file    
        static CFEHUD* poLoad(const CFEString& _sFilename);

        /// Loads a HUD element from disk
        static CFEHUDElement* poLoadElement(const CFEString& _sFilename);

        /// Loads a HUD element actions from disk
        static void LoadElementActions(const CFEString& _sFilename,CFEHUDElement* _poElem);

        /// Loads a HUD object from disk
        static CFEHUDObject* poLoadObject(const CFEString& _sFilename);

    protected:

        static CFEHUDElement* poLoadElement(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile);

        static CFEHUDObject* poLoadObject(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile);

        static void LoadElementActions(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile,CFEHUDElement* _poElem);

		static CFEHUDElementAction* poLoadAction(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem);

        static CFEHUDObjectAction* poLoadObjAction(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile, CFEHUDElement* _poElem);

        static void LoadCommonObjectProperties(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile,CFEHUDObject* _poObj);
        
        static CFEString m_sWorkingDir;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
