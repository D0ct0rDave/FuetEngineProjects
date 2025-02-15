//-----------------------------------------------------------------------------
/*! \class CAttackChecker
 *  \brief The class reponsible of checking entity attacks. This class is specific to every game, so 
 *  \brief one should re-implement this in every different project.
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CAttackCheckerH
#define CAttackCheckerH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "Game/CGameVisitor.h"
#include "Game/Entity/EntityAttacks.h"
//-----------------------------------------------------------------------------
class CAttackChecker : public CGameVisitor
{
	public:

		// Sets the damage to be inflicted to a possible attacked entity.
		void SetAttack(const TEntityAttack& _oAttack)
		{
			m_oAttack = _oAttack;
		}

		// Retrieves if an entity was damaged by the given entity or not.
		void Check(CEntity* _poEnt);

	public:
        /// 
        virtual void Visit(CEntity* _poEnt);
        /// 
        virtual void Visit(CPlayer* _poEnt);
        /// 
        virtual void Visit(CSimpleEnemy* _poEnt);
        /// 
        virtual void Visit(CThrowerEnemy* _poEnt);
        /// 
        virtual void Visit(CLevel1BonusThrownObject* _poEnt);
        /// 
        virtual void Visit(CThrownObject* _poEnt);
        /// 
		virtual void Visit(CChinoCudeiro* _poEnt);
		/// 
		virtual void Visit(CDiscChinoCudeiro* _poEnt);
		/// 
		virtual void Visit(CSmallDopefish* _poEnt);	
		/// 
		virtual void Visit(CDopefish* _poEnt);	
		/// 
		virtual void Visit(CEnanoSaltarin* _poEnt);	

	private:
		void DefaultAttack	 (CPlayer* _poPlayer, CEntity* _poEnemy);
		void CCDiscipleAttack(CPlayer* _poPlayer, CDiscChinoCudeiro* _poEnemy);
		void DopefishAttack(CPlayer* _poPlayer, CDopefish* _poEnemy);

	protected:

		TEntityAttack   m_oAttack;
};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
