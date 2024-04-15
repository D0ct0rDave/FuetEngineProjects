// ----------------------------------------------------------------------------
/*! \class CFEParticleSysLoader
 *  \brief A class to load Particle System definitions.
 *  \author David Márquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David Márquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEParticleSysLoaderH
#define CFEParticleSysLoaderH
//-----------------------------------------------------------------------------
#include "CFEParticleSys.h"
//-----------------------------------------------------------------------------
class CFEParticleSysLoader
{
	public:
		static CFEParticleSys* poLoad(const CFEString& _sFilename);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
