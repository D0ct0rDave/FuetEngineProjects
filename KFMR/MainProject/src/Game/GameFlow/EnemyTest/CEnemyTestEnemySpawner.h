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
#ifndef CEnemyTestEnemySpawnerH
#define CEnemyTestEnemySpawnerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"

#include "Game/GameFlow/CEnemySpawner.h"
//-----------------------------------------------------------------------------
class CEnemy;
//-----------------------------------------------------------------------------
class CEnemyTestEnemySpawner : public CEnemySpawner
{
	public:

		///
		CEnemyTestEnemySpawner(uint _uiSkill = 0);
		
		///
		~CEnemyTestEnemySpawner();
		
		/// 
		virtual void Update(FEReal _rDeltaT);

		virtual void NotifyDeath(CEntity* _poEnt);
		
		/// 
		virtual void Reset();
		
	protected:
	
		void SpawnEnemy(uint _uiType,bool _bLeft);
		uint m_uiSkill;
		uint m_uiLivingEnts;
		CEnemy* m_poEnemy;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
