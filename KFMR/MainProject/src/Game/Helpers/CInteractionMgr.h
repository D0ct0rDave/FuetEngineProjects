//-----------------------------------------------------------------------------
/*! \class CInteractionMgr
 *  \brief The class reponsible of handling entities interactions.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CInteractionMgrH
#define CInteractionMgrH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/Entity/EntityAttacks.h"
//-----------------------------------------------------------------------------
class CEntity;
//-----------------------------------------------------------------------------
class CInteractionMgr
{
	public:

		/// 
		static void Init();

		/// 
		static void Finish();

		/// Performs a player attack check against the active enemies in the scene.
		static void PlayerAttack(const TEntityAttack& _oAttack);

		/// Tests this enemy attack against the player and performs actions.
		static void EnemyAttack(CEntity* _poEnt,const TEntityAttack& _oAttack);

		/// Tests if the given object collides against the player and performs actions.
		static void ObjectCollided(CEntity* _poEnt,const TEntityAttack& _oAttack);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
