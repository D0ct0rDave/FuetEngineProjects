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
#ifndef CLevel2BonusEnemySpawnerH
#define CLevel2BonusEnemySpawnerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "FuetEngineExt.h"

#include "Game/GameFlow/Level1Bonus/CLevel1BonusThrownObject.h"
#include "Game/GameFlow/CEnemySpawner.h"
#include "Game/Helpers/CFERandomResourceQueue.h"
//-----------------------------------------------------------------------------
class CSimpleEnemy;
//-----------------------------------------------------------------------------
const uint MAX_THROWN_OBJS		= 6;		// Max number of the thrown objects
//-----------------------------------------------------------------------------
class CLevel2BonusEnemySpawner : public CEnemySpawner
{
	public:

		///
		CLevel2BonusEnemySpawner(uint _uiSkill = 0);
		
		///
		~CLevel2BonusEnemySpawner();
		
		/// 
		virtual void Update(FEReal _rDeltaT);

		virtual void NotifyDeath(CEntity* _poEnt);
		
		/// 
		virtual void Reset();
		
	protected:
	
		void SpawnObject(bool _bGood, bool _bLeft, bool _bUp);

		uint    m_uiSkill;

		bool	m_bStarted;

        /// Entities living at a given moment in time.
        uint    m_uiLivingEnts;

		CFERandomResourceQueue<CLevel1BonusThrownObject> m_oPoolGoodObjs;
		CFERandomResourceQueue<CLevel1BonusThrownObject> m_oPoolBadObjs;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
