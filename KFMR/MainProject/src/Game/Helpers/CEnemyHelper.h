//-----------------------------------------------------------------------------
/*! \class CEnemyHelper
 *  \brief Helper class to perform specific processes over enemies.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CEnemyHelperH
#define CEnemyHelperH
//-----------------------------------------------------------------------------
#include "Game/Entity/Enemy/CEnemy.h"
//-----------------------------------------------------------------------------
class CEnemyHelper
{
	public:

        /// Prepares an enemy to start a generic dying process.
        static void GenericDie(CEnemy* _poEnemy,TEntityAttack* _poReceivedAttack);

        /// Performs a generic die update step over the given enemy.
        static void GenericDieUpdate(FEReal _rDeltaT,CEnemy* _poEnemy);
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
