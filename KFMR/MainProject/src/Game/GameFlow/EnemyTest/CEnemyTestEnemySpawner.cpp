//-----------------------------------------------------------------------------
#include "CEnemyTestEnemySpawner.h"
// #include "Level1AGlobals.h"

#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityMgr.h"

#include "Game/Entity/Enemy/CEnemy.h"
#include "Game/Entity/Enemy/CSimpleEnemy.h"
#include "Game/Entity/Enemy/CThrowerEnemy.h"
#include "Game/Entity/Enemy/CChinoCudeiro.h"
#include "Game/Entity/Enemy/CDiscChinoCudeiro.h"
#include "Game/Entity/Enemy/CDopeFish.h"
#include "Game/Entity/Enemy/CEnanoSaltarin.h"

#include <stdlib.h>
#include <stdio.h>

#define ENEMY_ENTITY CDopefish
//-----------------------------------------------------------------------------
// Skill dependant values
//-----------------------------------------------------------------------------
const FEReal SKILL_FACTS[MAX_SKILLS] = {2.0f,1.0f,0.8f,0.7f};
const uint MAX_LIVING_ENTS[MAX_SKILLS] = {3,5,7,8}; 
//-----------------------------------------------------------------------------
const FEReal INITIAL_SPAWN_TIME = 4.0f;
static FEReal INITIAL_SPAWN_DISTANCE = 500.0f;
//-----------------------------------------------------------------------------
CEnemyTestEnemySpawner::CEnemyTestEnemySpawner(uint _uiSkill)
{
    // Store the skill we're playing
    m_uiSkill = _uiSkill;
	
    // Create a pool array for the enemies
    m_poEnemy = new ENEMY_ENTITY;
    m_poEnemy->Init(m_uiSkill);
    
    m_uiLivingEnts = 0;

    INITIAL_SPAWN_DISTANCE	= 2*goGameGlobals.m_rScreenMargin;
	Reset();
}
//-----------------------------------------------------------------------------
CEnemyTestEnemySpawner::~CEnemyTestEnemySpawner()
{
	delete m_poEnemy;
}
//-----------------------------------------------------------------------------
void CEnemyTestEnemySpawner::Reset()
{
    m_poEnemy->Reset();
    m_rTime = 0;
}
//-----------------------------------------------------------------------------
void CEnemyTestEnemySpawner::SpawnEnemy(uint _uiType,bool _bLeft)
{
	CFEVect2 oPos = goGameGlobals.m_poPlayer->oGetPos();
	if (_bLeft) oPos.x -= INITIAL_SPAWN_DISTANCE; else oPos.x += INITIAL_SPAWN_DISTANCE;

	CEntity* poEnemy = (CEntity*) m_poEnemy;
	if (poEnemy)
	{
		poEnemy->Enable();
		poEnemy->Spawn(oPos);
		
        m_uiLivingEnts++;
	}	
}
//-----------------------------------------------------------------------------
void CEnemyTestEnemySpawner::Update(FEReal _rDeltaT)
{
    if (m_uiLivingEnts == 0)
    {
        SpawnEnemy(0,false);
    }
}
//-----------------------------------------------------------------------------
void CEnemyTestEnemySpawner::NotifyDeath(CEntity* _poEnt)
{
	m_uiLivingEnts--;
}
//-----------------------------------------------------------------------------
