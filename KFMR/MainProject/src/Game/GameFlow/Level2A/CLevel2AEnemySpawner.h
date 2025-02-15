//-----------------------------------------------------------------------------
/*! \class CLevelEnemySpawner
 *  \brief The class that will spawn enemies in the level 0.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CLevel2AEnemySpawnerH
#define CLevel2AEnemySpawnerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"

#include "Game/GameFlow/CEnemySpawner.h"
//-----------------------------------------------------------------------------
class CSimpleEnemy;
//-----------------------------------------------------------------------------
class CLevel2AEnemySpawner : public CEnemySpawner
{
	public:

		///
		CLevel2AEnemySpawner(uint _uiSkill = 0);
		
		///
		~CLevel2AEnemySpawner();
		
		/// 
		virtual void Update(FEReal _rDeltaT);

		virtual void NotifyDeath(CEntity* _poEnt);
		
		/// 
		virtual void Reset();
		
	protected:
	
		void SpawnEnemy(uint _uiType,bool _bLeft);
		
		FEReal	m_rRightSideSpawnTime;
		uint	m_uiRightSideSpawns;
		uint	m_uiRightSideSpawnTemplate;

		FEReal  m_rLeftSideSpawnTime;
		uint    m_uiLeftSideSpawns;
		uint    m_uiLeftSideSpawnTemplate;

		uint    m_uiSkill;

        /// Entities living at a given moment in time.
        uint    m_uiLivingEnts;
        CEntity* m_poThrowers[2];

		CFEResourceQueue<CEntity>* m_oPool;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
