//-----------------------------------------------------------------------------
#include "CLevel1AEnemySpawner.h"
#include "Level1AGlobals.h"

#include "Game/CGameGlobals.h"
#include "Game/Entity/CEntityMgr.h"

#include "Game/Entity/Enemy/CEnemy.h"
#include "Game/Entity/Enemy/CSimpleEnemy.h"
#include "Game/Entity/Enemy/CThrowerEnemy.h"

#include <stdlib.h>
#include <stdio.h>
//-----------------------------------------------------------------------------
const uint MAX_ENEMY_TYPES = 2;
const uint MAX_ENEMIES[MAX_ENEMY_TYPES] = {16,3};
//-----------------------------------------------------------------------------
const uint MAX_SPAWN_TEMPLATES = 2;
const uint MAX_SPAWN_TEMPLATE_PERIODS = 4;

const FEReal SPAWN_TIMES_TEMPLATE[MAX_SPAWN_TEMPLATES][MAX_SPAWN_TEMPLATE_PERIODS] =
{
	{1.0f,4.0f,1.0f,4.0f},
	{1.0f,1.0f,1.0f,4.0f}
};
//-----------------------------------------------------------------------------
// Skill dependant values
//-----------------------------------------------------------------------------
const FEReal SKILL_FACTS[MAX_SKILLS] = {2.0f,1.0f,0.8f,0.7f};
const uint MAX_LIVING_ENTS[MAX_SKILLS] = {3,5,7,8}; 
//-----------------------------------------------------------------------------
const FEReal INITIAL_SPAWN_TIME = 4.0f;
static FEReal INITIAL_SPAWN_DISTANCE = 500.0f;
//-----------------------------------------------------------------------------
CLevel1AEnemySpawner::CLevel1AEnemySpawner(uint _uiSkill)
{
    // Store the skill we're playing
    m_uiSkill = _uiSkill;
	
    // Create a pool array for the enemies
    m_oPool = new CFEResourceQueue<CEntity>[MAX_ENEMY_TYPES];

    // for every enemy type ...    
    for (uint j=0;j<MAX_ENEMY_TYPES;j++)
    {
	    m_oPool[j].Init( MAX_ENEMIES[j] );

        for (uint i=0;i<MAX_ENEMIES[j];i++)
        {
            CEnemy* poEnemy = NULL;

            switch (j)
            {
                // Simple enemy.
                case 0:
                {
	                poEnemy = new CSimpleEnemy;
                }
                break;

                // Thrower.     
                case 1:
	            {
	                poEnemy = new CThrowerEnemy;     
	            }
                break;
            }

            if (poEnemy != NULL)
            {
				poEnemy->Init(m_uiSkill);
                m_oPool[j].iAdd(poEnemy);
            }
        }
    }

    INITIAL_SPAWN_DISTANCE	= 2*goGameGlobals.m_rScreenMargin;
	Reset();
}
//-----------------------------------------------------------------------------
CLevel1AEnemySpawner::~CLevel1AEnemySpawner()
{
    for (uint j=0;j<MAX_ENEMY_TYPES;j++)
    {
    	for (uint i=0;i<MAX_ENEMIES[j];i++)
		    delete m_oPool[j].poGet(i);
    
	    m_oPool[j].Finish();
	}
	
	delete m_oPool;
}
//-----------------------------------------------------------------------------
void CLevel1AEnemySpawner::Reset()
{
    for (uint j=0;j<MAX_ENEMY_TYPES;j++)
    {
    	for (uint i=0;i<MAX_ENEMIES[j];i++)
    	{
    	    CEntity* poEnemy = m_oPool[j].poGet(i);

		    if (poEnemy != NULL)
			    poEnemy->Reset();
    	}

	    m_oPool[j].ReleaseAll();
	}
	
	m_rRightSideSpawnTime = INITIAL_SPAWN_TIME * SKILL_FACTS[m_uiSkill];
	m_uiRightSideSpawns = 0;
	m_uiRightSideSpawnTemplate = CFEMath::iRand(0,MAX_SPAWN_TEMPLATES-1);

	m_rLeftSideSpawnTime = INITIAL_SPAWN_TIME * SKILL_FACTS[m_uiSkill];
	m_uiLeftSideSpawns = 0;
	m_uiLeftSideSpawnTemplate = CFEMath::iRand(0,MAX_SPAWN_TEMPLATES-1);

    m_uiLivingEnts = 0;
    m_poThrowers[0] = NULL;
    m_poThrowers[1] = NULL;

    m_rTime = 0;
}
//-----------------------------------------------------------------------------
void CLevel1AEnemySpawner::SpawnEnemy(uint _uiType,bool _bLeft)
{
	CFEVect2 oPos = goGameGlobals.m_poPlayer->oGetPos();
	if (_bLeft) oPos.x -= INITIAL_SPAWN_DISTANCE; else oPos.x += INITIAL_SPAWN_DISTANCE;

	CEntity* poEnemy = (CEntity*) m_oPool[_uiType].poGetResource();
	if (poEnemy)
	{
		poEnemy->Enable();
		poEnemy->Spawn(oPos);
		
	    // Store Throwers
    	if (_uiType == 1)
	    	m_poThrowers[_bLeft?1:0] = poEnemy;

        m_uiLivingEnts++;
        
        if (_bLeft)
            m_uiLeftSideSpawns++;
        else
            m_uiRightSideSpawns++;
	}	

}
//-----------------------------------------------------------------------------
void CLevel1AEnemySpawner::Update(FEReal _rDeltaT)
{
	/*
	if (m_uiLivingEnts >= 1) return;
	SpawnEnemy(2,false);
	m_uiLivingEnts = 1;
	return;
	*/
	m_rTime += _rDeltaT;

	// Do not allow more than X living enemies at the same time.
    if (m_uiLivingEnts >= MAX_LIVING_ENTS[m_uiSkill]) return;

	// only allow spawning on the right if not at the begining of the level, and there is no thrower at the right
	if ((goGameGlobals.m_poCamera->oGetPos().x < RIGHT_SPAWN_LIMIT) && (m_poThrowers[0]==NULL))
	{
		m_rRightSideSpawnTime -= _rDeltaT;
		if (m_rRightSideSpawnTime<=_0r)
		{
			// spawn entity on the right
			if (((m_uiRightSideSpawns+1) % 12) == 0)
			    SpawnEnemy(1,false);
            else
                SpawnEnemy(0,false);

			// program next spawn
			m_rRightSideSpawnTime = SPAWN_TIMES_TEMPLATE[m_uiRightSideSpawnTemplate][m_uiRightSideSpawns % MAX_SPAWN_TEMPLATE_PERIODS] * SKILL_FACTS[m_uiSkill];
		}
	}

	// only allow spawning on the left if not at the end of the level, and there is no thrower at the left
	if (CFEMath::bBetween(LEFT_SPAWN_LIMIT,TOTAL_SPAWN_LIMIT,goGameGlobals.m_poCamera->oGetPos().x) && (m_poThrowers[1]==NULL))
	{
		m_rLeftSideSpawnTime -= _rDeltaT;
		if (m_rLeftSideSpawnTime<=_0r)
		{
	        // spawn entity on the left
			if (((m_uiLeftSideSpawns+1) % 12) == 0)
			    SpawnEnemy(1,true);
            else
                SpawnEnemy(0,true);

			// program next spawn
            m_rLeftSideSpawnTime = SPAWN_TIMES_TEMPLATE[m_uiLeftSideSpawnTemplate][m_uiLeftSideSpawns % MAX_SPAWN_TEMPLATE_PERIODS]* SKILL_FACTS[m_uiSkill];
		}
	}
}
//-----------------------------------------------------------------------------
void CLevel1AEnemySpawner::NotifyDeath(CEntity* _poEnt)
{
    switch (_poEnt->eGetType() )
	{
	    case GE_THROWER:
	    {
	        m_oPool[1].Release(_poEnt);

		    if (_poEnt == m_poThrowers[0])
			    m_poThrowers[0] = NULL;
		    else
			    m_poThrowers[1] = NULL;
        };
        break;

	    case GE_SIMPLE_ENEMY:
	    {
	        m_oPool[0].Release(_poEnt);
        };
        break;
	}

	m_uiLivingEnts--;
}
//-----------------------------------------------------------------------------
