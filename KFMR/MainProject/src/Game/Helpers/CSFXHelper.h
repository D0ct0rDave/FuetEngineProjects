//-----------------------------------------------------------------------------
/*! \class CSFXHelper
 *  \brief Helper class to load and spawn game sfx
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CSFXHelperH
#define CSFXHelperH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
#include "Game/Entity/CEntity.h"
//-----------------------------------------------------------------------------
#include "GameSFX.h"
const uint MAX_SFXSET_SIZE = 4;
//-----------------------------------------------------------------------------
class CSFXHelper
{
	public:

		/// 
		static void Init(EGameSFX _eSFXs[]);

		/// 
		static void Finish();

		/// 
		static void Reset();

        /// Creates and returns a particle system.
		static FEHandler hSpawnPSys(EGameSFX _eSFX);

        /// Creates a selfhandled particle system
		static void SpawnPSys(EGameSFX _eSFX,const CFEVect2& _oPos,FEReal _rDepth,FEReal _rTime);

        /// Spawns one particle at a given position.
		static void SpawnParticle(EGameSFX _eSFX,const CFEVect2& _oPos,FEReal _rDepth = _0r);

        /// Plays a follower FX.
		static void SpawnFollowerFX(EGameSFX _eSFX,CEntity* _poEnt,const CFEVect2& _oOffset);

		/// Performs an update step over the active sfx.
        static void Update(FEReal _rDeltaT);

    protected:
    
        
        /// List of loaded FX.
        typedef struct
        {
            uint            m_uiNumSFX;
            CFEParticleSys* m_poSFX[MAX_SFXSET_SIZE];

        }TSFXData;

        static TSFXData     m_oSFXDataTable[SFX_NUM_SFXS];

        /// List of active FX
        typedef struct TSFXInfo{
            FEHandler   m_hSFX;
            CEntity*    m_poEnt;
            CFEVect2    m_oOfs;
        };

        static CFEArray<TSFXInfo> m_oActiveFX;
        
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
