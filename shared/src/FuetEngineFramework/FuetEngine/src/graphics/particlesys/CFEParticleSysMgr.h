// ----------------------------------------------------------------------------
/*! \class CFEParticleSysMgr
 *  \brief Particle System Manager. Manages the different particle system definition structures.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEParticleSysMgrH
#define CFEParticleSysMgrH
// ----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEResourceMgr.h"
// ----------------------------------------------------------------------------
class CFEParticleSys;
// ----------------------------------------------------------------------------
DECLARE_RESOURCE_MANAGER(CFEParticleSysMgr,CFEParticleSys)
{
	friend CFEResourceMgr<CFEParticleSysMgr,CFEParticleSys>;

    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        static CFEParticleSys* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        static void InvalidateResource(CFEParticleSys* _poRes);
};
// ----------------------------------------------------------------------------
#endif
// ----------------------------------------------------------------------------
