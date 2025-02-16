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
#include "CFEParticleSys.h"
#include "CFEParticleSysMgr.h"
#include "CFEParticleSysLoader.h"
// ----------------------------------------------------------------------------
CFEParticleSys* CFEParticleSysMgr::poLoadResource(const CFEString& _sFilename)
{
    return( CFEParticleSysLoader::poLoad(_sFilename) );
}
// ----------------------------------------------------------------------------
