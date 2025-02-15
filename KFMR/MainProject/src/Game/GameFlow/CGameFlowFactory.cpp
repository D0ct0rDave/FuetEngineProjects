//-----------------------------------------------------------------------------
/*! \class CGameFlowFactory
 *  \brief This class is responsible of the creation of a game flow object.
 *  \brief This file is specific of the specific game.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#include "CGameFlowFactory.h"
//-----------------------------------------------------------------------------
// Game flows
//-----------------------------------------------------------------------------
#include "Game/GameFlow/Level1A/CLevel1AGameFlow.h"
#include "Game/GameFlow/Level1B/CLevel1BGameFlow.h"
#include "Game/GameFlow/Level1Bonus/CLevel1BonusGameFlow.h"
#include "Game/GameFlow/Level2A/CLevel2AGameFlow.h"
#include "Game/GameFlow/Level2B/CLevel2BGameFlow.h"
#include "Game/GameFlow/Level2Bonus/CLevel2BonusGameFlow.h"

#include "Game/GameFlow/Test/CTestGameFlow.h"
#include "Game/GameFlow/EnemyTest/CEnemyTestGameFlow.h"
//-----------------------------------------------------------------------------
CGameFlow* CGameFlowFactory::poCreate(uint _uiLevel,uint _uiSkill)
{
	switch (_uiLevel)
	{
		default:
	    return( new CTestGameFlow(_uiSkill) );
	    break;

	    case 0:
        return (new CEnemyTestGameFlow(_uiSkill) );    
	    break;

		case 0x1A:
		return( new CLevel1AGameFlow(_uiSkill) );
		break;

		case 0x1B:
		return( new CLevel1BGameFlow(_uiSkill) );
		break;

		case 0x1C:
		return (new CLevel1BonusGameFlow(_uiSkill) );
		break;

		case 0x2A:
		return (new CLevel2AGameFlow(_uiSkill) );
		break;

		case 0x2B:
		return (new CLevel2BGameFlow(_uiSkill) );
		break;

		case 0x2C:
		return (new CLevel2BonusGameFlow(_uiSkill) );
		break;
    }

	return(NULL);
}
//-----------------------------------------------------------------------------
