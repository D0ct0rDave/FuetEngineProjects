// ----------------------------------------------------------------------------
/*! \class CGameVisitor
 *  \brief A generic class to perform any kind of process over all the entities of the game.
 *  \brief modified to provide basic behaviour to every new created class.
 *  \brief Every entity created should have a visit entry here.
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
class CDiscChinoCudeiro;
class CSmallDopefish;
class CLevel1BonusThrownObject;
class CDopefish;
class CEnanoSaltarin;
//-----------------------------------------------------------------------------
class CGameVisitor
{
    public:
        /// 
        virtual void Visit(CEntity* _poEnt)
        {
            // nothing to do!
        };

        /// 
        virtual void Visit(CPlayer* _poEnt)
        {
            Visit((CEntity*)_poEnt);
        }

        /// 
        virtual void Visit(CSimpleEnemy* _poEnt)
        {
            Visit((CEntity*)_poEnt);
        }

        /// 
        virtual void Visit(CThrowerEnemy* _poEnt)
        {
            Visit((CEntity*)_poEnt);
        }

        /// 
        virtual void Visit(CThrownObject* _poEnt)
        {
            Visit((CEntity*)_poEnt);
        }

        /// 
        virtual void Visit(CChinoCudeiro* _poEnt)
        {
            Visit((CEntity*)_poEnt);
        }

        /// 
        virtual void Visit(CLevel1BonusThrownObject* _poEnt)
        {
            Visit((CEntity*)_poEnt);
		}

		/// 
		virtual void Visit(CDiscChinoCudeiro* _poEnt)
		{
			Visit((CEntity*)_poEnt);
		}

		/// 
		virtual void Visit(CSmallDopefish* _poEnt)
		{
			Visit((CEntity*)_poEnt);
		}

		/// 
		virtual void Visit(CDopefish* _poEnt)
		{
			Visit((CEntity*)_poEnt);
		}

		/// 
		virtual void Visit(CEnanoSaltarin* _poEnt)
		{
			Visit((CEntity*)_poEnt);
		}
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------

