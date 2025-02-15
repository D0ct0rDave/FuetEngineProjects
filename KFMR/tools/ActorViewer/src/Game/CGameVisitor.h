// ----------------------------------------------------------------------------
/*! \class CGameVisitor
 *  \brief A generic class to perform any kind of process over all the entities of the game
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.0
 *  \date 2009
 *  \par Copyright (c) 2009 David M&aacute;rquez de la Cruz
 *  \par FuetEngine License
 */
// ----------------------------------------------------------------------------
#ifndef CGameVisitorH
#define CGameVisitorH
//-----------------------------------------------------------------------------
class CEntity;
class CLivingEntity;
class CPlayer;
class CEnemy;
class CSimpleEnemy;
class CThrowerEnemy;
class CThrownObject;
class CChinoCudeiro;
//-----------------------------------------------------------------------------
class CGameVisitor
{
    public:
        /// 
        virtual void Visit(CEntity* _poEnt) = 0;
        /// 
        virtual void Visit(CPlayer* _poEnt) = 0;
        /// 
        virtual void Visit(CSimpleEnemy* _poEnt) = 0;
        /// 
        virtual void Visit(CThrowerEnemy* _poEnt) = 0;
        /// 
        virtual void Visit(CThrownObject* _poEnt) = 0;
        /// 
        virtual void Visit(CChinoCudeiro* _poEnt) = 0;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
