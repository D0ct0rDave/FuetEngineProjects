//-----------------------------------------------------------------------------
#include "CEnemyExiter.h"
#include "Game/Entity/CEntityMgr.h"

#include "Game/Entity/Enemy/CSimpleEnemy.h"
#include "Game/Entity/Enemy/CThrowerEnemy.h"
// #include "Game/GameFlow/Common/CThrownObject.h"
// #include "Game/GameFlow/Level0/CChinoCudeiro.h"
// #include "Game/Player/CPlayer.h"
//-----------------------------------------------------------------------------
void CEnemyExiter::DoExit()
{
	CEnemyExiter oEE;
	for (uint e=0;e<MAX_ENTITIES;e++)
	{
		CEntity* poEnt = CEntityMgr::poGetEntity(e);

		if (poEnt != NULL)
		    poEnt->Accept(&oEE);
	}
}
//-----------------------------------------------------------------------------
void CEnemyExiter::Visit(CSimpleEnemy* _poEnt)
{
    _poEnt->ChangeState(SES_EXIT);
}
//-----------------------------------------------------------------------------
void CEnemyExiter::Visit(CThrowerEnemy* _poEnt)
{
    _poEnt->ChangeState(TES_EXIT);
}
//-----------------------------------------------------------------------------
