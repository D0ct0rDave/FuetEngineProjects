// -----------------------------------------------------------------------------
/*! \class CEntityFactory
 *  \brief This class creates game entities on demand.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// -----------------------------------------------------------------------------
#include "CEntityFactory.h"
#include "Camera/CCamera.h"
#include "Player/CPlayer.h"
#include "Enemy/CChinoCudeiro.h"
#include "Enemy/CDiscChinoCudeiro.h"
#include "Enemy/CSimpleEnemy.h"
#include "Enemy/CThrowerEnemy.h"
#include "Enemy/CEnanoSaltarin.h"
#include "Enemy/CThrownObject.h"
#include "Level/CEyeFollower.h"
// -----------------------------------------------------------------------------
CEntity* CEntityFactory::poCreate(const CFEString& _sEntity)
{
        // ----------------------------
        // Camera
        // ----------------------------
         if (_sEntity |= "Camera")
            return( new CCamera() );

        // ----------------------------
        // Player
        // ----------------------------
    else if (_sEntity |= "Player")
            return( new CPlayer() );
        
        // ----------------------------
        // Enemies
        // ----------------------------
    else if (_sEntity |= "ChinoCudeiro")
            return( new CChinoCudeiro() );

    else if (_sEntity |= "SimpleEnemy")
		return( new CSimpleEnemy() );

	else if (_sEntity |= "Thrower")
		return( new CThrowerEnemy() );

	else if (_sEntity |= "EnanoSaltarin")
		return( new CEnanoSaltarin() );

        // ----------------------------
        // Level entities
        // ----------------------------
    else if (_sEntity |= "EyeFollower")
            return( new CEyeFollower() );
        
        // ...
    
    return(NULL);
}
// -----------------------------------------------------------------------------
