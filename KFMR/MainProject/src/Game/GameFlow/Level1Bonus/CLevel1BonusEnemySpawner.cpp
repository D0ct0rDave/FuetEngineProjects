//-----------------------------------------------------------------------------
#include "CLevel1BonusEnemySpawner.h"
#include "Level1BonusGlobals.h"

#include "Game/CGameGlobals.h"

#include "Game/Entity/CEntityMgr.h"
#include "Game/Entity/Enemy/CEnemy.h"

#include <stdlib.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
const uint   SPAWN_INIT_DELAY	= 3;		// Wait these seconds until start the challenge
const FEReal SPAWN_STEP_DELAY	= 1.0f;	// Threshold to launch the objects
const FEReal SPAWN_PERC_STEP	= 40;		// % to spawn one object every step
const FEReal SPAWN_PERC_GOOD	= 15;		

const uint	 THROWN_OBJ_LOW_POS		= 240;
const uint	 THROWN_OBJ_HIGH_POS	= 350;

const uint	 THROWN_OBJ_LEFT_POS	= 100;
const uint	 THROWN_OBJ_RIGHT_POS	= 740;
//-----------------------------------------------------------------------------
CLevel1BonusEnemySpawner::CLevel1BonusEnemySpawner(uint _uiSkill)
{
    // Store the skill we're playing
    m_uiSkill = _uiSkill;
	
    // Create a pool for the good objects
	for(uint i = 0; i < MAX_THROWN_OBJS; i++)
	{
		CLevel1BonusThrownObject* poObj = new CLevel1BonusThrownObject;
		poObj->Init(1, i);
		m_oPoolGoodObjs.iAdd(poObj);
	}
    // Create a pool for the bad objects
	for(uint i = 0; i < MAX_THROWN_OBJS; i++)
	{
		CLevel1BonusThrownObject* poObj = new CLevel1BonusThrownObject;
		poObj->Init(0, i);
		m_oPoolBadObjs.iAdd(poObj);
	}

	Reset();
}
//-----------------------------------------------------------------------------
CLevel1BonusEnemySpawner::~CLevel1BonusEnemySpawner()
{
}
//-----------------------------------------------------------------------------
void CLevel1BonusEnemySpawner::Reset()
{
    m_rTime = 0;
	m_bStarted = false;
	m_uiLivingEnts = 0;

	for(uint i = 0; i < MAX_THROWN_OBJS; i++)
	{
		CLevel1BonusThrownObject* poObj = m_oPoolGoodObjs.poGet(i);
	    if (poObj != NULL)
		    poObj->Reset(); 
		
		poObj = m_oPoolBadObjs.poGet(i);
	    if (poObj != NULL)
		    poObj->Reset();
	}
    m_oPoolGoodObjs.ReleaseAll();
    m_oPoolBadObjs.ReleaseAll();
}
//-----------------------------------------------------------------------------
void CLevel1BonusEnemySpawner::SpawnObject(bool _bGood, bool _bLeft, bool _bUp)
{
	m_uiLivingEnts++;

	// Get object.
	CThrownObject* poObj;
	

	if(_bGood)
	{
		poObj = m_oPoolGoodObjs.poGetResource();
	}
	else
	{
		poObj = m_oPoolBadObjs.poGetResource();
	}

	// Throw object.
	CFEVect2 oPos;

	if(_bLeft)
	{
		oPos.x = THROWN_OBJ_LEFT_POS;
	}
	else
	{
		oPos.x = THROWN_OBJ_RIGHT_POS;
	}
	if(_bUp)
	{
		oPos.y = THROWN_OBJ_LOW_POS;
	}
	else
	{
		oPos.y = THROWN_OBJ_HIGH_POS;
	}
	poObj->Spawn(oPos, _bLeft);
}
//-----------------------------------------------------------------------------
void CLevel1BonusEnemySpawner::Update(FEReal _rDeltaT)
{
	m_rTime += _rDeltaT;

	if(m_uiLivingEnts >= MAX_THROWN_OBJS) return;

	if(!m_bStarted)		// Initial time to wait
	{
		if(m_rTime > SPAWN_INIT_DELAY)
		{
			m_bStarted = true;
			m_rTime = 0;
		}
	}
	else
	{
		// Each step we have one % to spawn one object
		if(m_rTime > SPAWN_STEP_DELAY && CFEMath::iRand(0, 100) < SPAWN_PERC_STEP)
		{
			m_rTime = 0;
			
			// Bad object
			if(CFEMath::iRand(0, 100) > SPAWN_PERC_GOOD)
			{
				SpawnObject(false, CFEMath::iRand(0,1), CFEMath::iRand(0,1));
			}
			else	// Good object
			{
				SpawnObject(true, CFEMath::iRand(0,1), CFEMath::iRand(0,1));
			}
		}
	}
}
//-----------------------------------------------------------------------------
void CLevel1BonusEnemySpawner::NotifyDeath(CEntity* _poEnt)
{
	m_uiLivingEnts--;
	m_oPoolGoodObjs.Release((CLevel1BonusThrownObject*)_poEnt);
	m_oPoolBadObjs.Release((CLevel1BonusThrownObject*)_poEnt);
}
//-----------------------------------------------------------------------------
