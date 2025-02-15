//-----------------------------------------------------------------------------
/*! \class CLivingEntity
 *  \brief A class to derive from for entities with states
 *  \author David M&aacute;rquez de la Cruz
 *  \version 1.5
 *  \date 2010
 *  \par Copyright (c) 1999 David M&aacute;rquez de la Cruz
 *  \par License KFRM
 */
//-----------------------------------------------------------------------------
#ifndef CLivingEntityH
#define CLivingEntityH
//-----------------------------------------------------------------------------
#include <FuetEngine.h>
#include "CEntity.h"
//-----------------------------------------------------------------------------
class CLivingEntity : public CEntity
{
	public:

		CLivingEntity();
		~CLivingEntity();
		
		// All the entities have a maximum of 1.0 (100%) of energy.
		// They have a energy or damage multiplier which controls how hard are they.

		/// Set's the entity's energy
		void SetEnergy(FEReal _rEnergy)
		{
			m_rEnergy = _rEnergy;

			if (m_rEnergy>_1r)
				m_rEnergy = _1r;
			else 
				if (m_rEnergy<_0r)
					m_rEnergy = _0r;
		}

		/// Retrieves the living entity's energy
		FEReal rGetEnergy()
		{
			return(m_rEnergy);
		}

		/// Increments the entity's energy by the given ammount.
		void IncEnergy(FEReal _rAmount)
		{
			SetEnergy(m_rEnergy+_rAmount);
		}

		/// Decrements the entity's energy by the given ammount.
		void DecEnergy(FEReal _rAmount)
		{
			SetEnergy(m_rEnergy-_rAmount);
		}

		/// Accept procedure for processing functionality over this entity
		virtual void Accept(CGameVisitor* _poVisitor)
		{
			_poVisitor->Visit(this);
		}

	private:

		FEReal		m_rEnergy;

};
//-----------------------------------------------------------------------------
#endif
//-----------------------------------------------------------------------------
