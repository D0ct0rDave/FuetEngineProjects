// ----------------------------------------------------------------------------
/*! \class CFESkelAnimLoader
 *  \brief A class to load FuetEngine HUD layouts.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFESkelAnimLoaderH
#define CFESkelAnimLoaderH
//-----------------------------------------------------------------------------
#include "types/CFEString.h"
#include "support/config/CFEConfigFile.h"
//-----------------------------------------------------------------------------
class CFESkelAnim;
class CFESkelAnimNode;
class CFESkelAnimActionSet;
class CFESkelAnimAction;
class CFESkelAnimNodeAction;
//-----------------------------------------------------------------------------
class CFESkelAnimLoader
{
    public:

        /// Loads a skeleton animation from a given file    
        static CFESkelAnim* poLoad(const CFEString& _sFilename);
        static CFESkelAnimNode* poLoadNode(const CFEString& _sFilename);        
        static CFESkelAnimAction* poLoadAction(const CFEString& _sFilename, CFESkelAnim* _poAnim);
        
        /// Loads a skeleton node from a given config file
        static CFESkelAnimNode* poLoadNode(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile);

        /// This function should be called after linking a new anim node to a skel anim object.
        static void BuildActionNodeTable(CFESkelAnim* _poAnim);

    protected:

		static int iFindNode(CFESkelAnim* _poAnim,const CFEString& _sName);

		static CFESkelAnimActionSet* poLoadActionSet(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim);

		static CFESkelAnimAction* poLoadAction(const CFEString& _sPrefix,const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim);

        static CFESkelAnimNodeAction* poLoadNodeAction(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile, CFESkelAnim* _poAnim);

        static void LoadNode(const CFEString& _sPrefix, const CFEConfigFile& _oConfigFile,CFESkelAnimNode* _poNode);
        
        static CFEString m_sWorkingDir;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
