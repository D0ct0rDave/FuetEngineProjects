// ----------------------------------------------------------------------------
/*! \class CFEParticleMgr
 *  \brief Particle system clases and definitions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CFEParticleMgrH
#define CFEParticleMgrH
//-----------------------------------------------------------------------------
#include "FEBasicTypes.h"
#include "types/CFEVect2.h"
#include "types/CFEResourceQueue.h"
#include "types/CFEResourceMgr.h"
#include "CFEParticleSys.h"
//-----------------------------------------------------------------------------
class CFERenderer;
class CFEParticle;
//-----------------------------------------------------------------------------
/*
DECLARE_RESOURCE_MANAGER(CFEParticleMgr,CFEParticle)
{
	friend CFEResourceMgr<CFEParticleMgr,CFEParticle>;

    protected:

        /// OVERRIDED: Implementation of the poLoadResource function.
        static CFEParticle* poLoadResource(const CFEString& _sFilename);

        /// OVERRIDED: Implementation of the InvalidateResource function.
        static void InvalidateResource(CFEParticle* _poRes);
        
        /// Creates the given number of particles.
        static void CreateParticles(uint _uiNumParticles, const CFEVect2& _oPos, FEReal _rDepth, CFEParticleSys* _poPS);

        /// Updates the living particles in the system.
        static void Update(FEReal _rDeltaT);

        /// Renders the living particles in the system.
        static void Render(CFERenderer* _poRenderer);
    
        /// Invalidates all living particles.
        static void Reset();       
};
*/

class CFEParticleMgr
{
    public:

        /// Main initialiation procedure.
        static void Init(uint _uiMaxParticles = 256);
        
        /// Main finalization procedure.
        static void Finish();

        /// Creates the given number of particles.
        static void CreateParticles(uint _uiNumParticles, const CFEVect2& _oPos, const FEReal& _rDepth, CFEParticleSys* _poPS);

        /// Updates the living particles in the system.
        static void Update(const FEReal& _rDeltaT);

        /// Renders the living particles in the system.
        static void Render(CFERenderer* _poRenderer);
    
        /// Invalidates all living particles.
        static void Reset();       
};

//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
