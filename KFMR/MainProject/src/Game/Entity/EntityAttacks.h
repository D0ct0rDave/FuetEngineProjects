//-----------------------------------------------------------------------------
/*! \class EntityAttacks
 *  \brief Enums the different types of Entity attacks.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef EntityAttacksH
#define EntityAttacksH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
//-----------------------------------------------------------------------------
typedef enum EEntityAttack
{
    EA_NONE,

    EA_RIGHT_PUNCH,
    EA_RIGHT_KICK,
    EA_RIGHT_CROUCH_KICK,
    EA_RIGHT_CROUCH_PUNCH,
    EA_RIGHT_JUMP_KICK,
    
    EA_LEFT_PUNCH,
    EA_LEFT_KICK,
    EA_LEFT_CROUCH_KICK,
    EA_LEFT_CROUCH_PUNCH,
    EA_LEFT_JUMP_KICK,
    
    EA_MATAMOSCAZOS,
    EA_OBJECT_KICK,
    
    EA_DOPEFISH_ATTACK,
    
    EA_NUM_ENTITY_ATTACKS
};
//-----------------------------------------------------------------------------
typedef struct TEntityAttack // Esta struct se podría usar para todos los ataques de todas las entidades
{
    EEntityAttack   m_eType;
    FEReal          m_rDamage;

}TEntityAttack;
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
