//-----------------------------------------------------------------------------
/*! \class GameEntity
 *  \brief Enums of all the entities in the game
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef GameEntitiesH
#define GameEntitiesH
//-----------------------------------------------------------------------------
typedef enum EGameEntity
{
    GE_UNDEFINED,
    
    // Camera
    GE_CAMERA,
    
    // The Player
    GE_PLAYER,
    
    // Enemies
    GE_CHINO_CUDEIRO,
    GE_SIMPLE_ENEMY,
    GE_THROWER,
    GE_THROWN_OBJECT,    
    GE_DISC_CHINO_CUDEIRO,
    GE_SMALL_DOPEFISH,
    GE_ENANO_SALTARIN,
    GE_DOPEFISH,
    
    // Level Entities
    GE_EYE_FOLLOWER,
    
    // 
    GE_NUM_ENTITIES
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
